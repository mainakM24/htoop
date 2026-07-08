#ifndef HTML_HPP
#define HTML_HPP

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <algorithm>
#include <cctype>

namespace htoop {
    inline std::string escape(const std::string& text) {
	std::string result;
	for (char c : text) {
	    switch(c) {
	    case '&': result += "&amp;"; break;
	    case '<': result += "&lt;"; break;
	    case '>': result += "&gt;"; break;
	    case '"': result += "&quot;"; break;
	    default: result += c;
	    }
	}
	return result;
    }

    class Node {
    private:
	std::vector<Node> children;
	std::unordered_map<std::string, std::string> attributes;
	std::string text;
	std::string tag;
	bool self_closing = false;

	std::string indent(int level) const {
	    return std::string(level * 2, ' ');
	}

    public:
	// --- constructors ---
	explicit Node(std::string tag) : tag(std::move(tag)) {}

	Node(std::string tag, bool sc) : tag(std::move(tag)), self_closing(sc) {}

	// self closing builder helper
	static Node Self(std::string tag) {
	    return Node(std::move(tag), true);
	}

	// text block
	static Node Text(const std::string& t) {
	    Node node("");
	    node.text = escape(t);
	    return node;
	}

	// raw text (unescaped, e.g. for style/script tags)
	static Node RawText(std::string raw_html) {
	    Node node("");
	    node.text = std::move(raw_html);
	    return node;
	}

	// --- append ---
	Node& append(const Node& node) {
	    if (self_closing) {
		throw std::logic_error("ERROR: can't append to self-closing tag: " + tag);
	    }
	    children.push_back(node);
	    return *this;
	}

	Node& append(Node&& node) {
	    if (self_closing) {
		throw std::logic_error("ERROR: can't append to self-closing tag: " + tag);
	    }
	    children.push_back(std::move(node));
	    return *this;
	}

	Node& append(const std::string& t) {
	    if (self_closing) {
		throw std::logic_error("ERROR: can't append to self-closing tag: " + tag);
	    }
	    children.push_back(Node::Text(t));
	    return *this;
	}

	// --- conditional rendering ---
	Node& append_if(bool condition, const Node& node) {
	    if (condition) {
		return append(node);
	    }
	    return *this;
	}

	Node& append_if(bool condition, Node&& node) {
	    if (condition) {
		return append(std::move(node));
	    }
	    return *this;
	}

	Node& append_if(bool condition, const std::string& t) {
	    if (condition) {
		return append(t);
	    }
	    return *this;
	}

	// --- attributes ---
	Node& attr(const std::string& key, const std::string& value) {
	    attributes[key] = value;
	    return *this;
	}

	// set id
	inline Node& set_id(const std::string& id) {
	    attributes["id"] = id;
	    return *this;
	}

	// add class
	inline Node& add_class(const std::string& cls) {
	    std::string& curr = attributes["class"];
	    if (!curr.empty()) curr += " ";
	    curr += cls;
	    return *this;
	}

	// --- find / DOM query API ---
	Node* find_by_id(const std::string& id) {
	    auto it = attributes.find("id");
	    if (it != attributes.end() && it->second == id) {
		return this;
	    }
	    for (auto& child : children) {
		if (Node* found = child.find_by_id(id)) {
		    return found;
		}
	    }
	    return nullptr;
	}

	const Node* find_by_id(const std::string& id) const {
	    auto it = attributes.find("id");
	    if (it != attributes.end() && it->second == id) {
		return this;
	    }
	    for (const auto& child : children) {
		if (const Node* found = child.find_by_id(id)) {
		    return found;
		}
	    }
	    return nullptr;
	}

	std::vector<Node*> find_by_class(const std::string& class_name) {
	    std::vector<Node*> matches;
	    find_by_class_helper(class_name, matches);
	    return matches;
	}

	std::vector<const Node*> find_by_class(const std::string& class_name) const {
	    std::vector<const Node*> matches;
	    find_by_class_helper(class_name, matches);
	    return matches;
	}

    private:
	void find_by_class_helper(const std::string& class_name, std::vector<Node*>& matches) {
	    auto it = attributes.find("class");
	    if (it != attributes.end()) {
		const std::string& cls_val = it->second;
		size_t pos = 0;
		while ((pos = cls_val.find(class_name, pos)) != std::string::npos) {
		    bool start_ok = (pos == 0 || std::isspace(static_cast<unsigned char>(cls_val[pos - 1])));
		    bool end_ok = (pos + class_name.length() == cls_val.length() || 
				   std::isspace(static_cast<unsigned char>(cls_val[pos + class_name.length()])));
		    if (start_ok && end_ok) {
			matches.push_back(this);
			break;
		    }
		    pos += class_name.length();
		}
	    }
	    for (auto& child : children) {
		child.find_by_class_helper(class_name, matches);
	    }
	}

	void find_by_class_helper(const std::string& class_name, std::vector<const Node*>& matches) const {
	    auto it = attributes.find("class");
	    if (it != attributes.end()) {
		const std::string& cls_val = it->second;
		size_t pos = 0;
		while ((pos = cls_val.find(class_name, pos)) != std::string::npos) {
		    bool start_ok = (pos == 0 || std::isspace(static_cast<unsigned char>(cls_val[pos - 1])));
		    bool end_ok = (pos + class_name.length() == cls_val.length() || 
				   std::isspace(static_cast<unsigned char>(cls_val[pos + class_name.length()])));
		    if (start_ok && end_ok) {
			matches.push_back(this);
			break;
		    }
		    pos += class_name.length();
		}
	    }
	    for (const auto& child : children) {
		child.find_by_class_helper(class_name, matches);
	    }
	}

    public:
	// --- variable interpolation ---
	Node& interpolate(const std::unordered_map<std::string, std::string>& vars) {
	    if (!text.empty()) {
		for (const auto& [key, value] : vars) {
		    std::string placeholder = "{{" + key + "}}";
		    size_t pos = 0;
		    while ((pos = text.find(placeholder, pos)) != std::string::npos) {
			std::string escaped_val = escape(value);
			text.replace(pos, placeholder.length(), escaped_val);
			pos += escaped_val.length();
		    }
		}
	    }
	    for (auto& [k, v] : attributes) {
		for (const auto& [key, value] : vars) {
		    std::string placeholder = "{{" + key + "}}";
		    size_t pos = 0;
		    while ((pos = v.find(placeholder, pos)) != std::string::npos) {
			v.replace(pos, placeholder.length(), value);
			pos += value.length();
		    }
		}
	    }
	    for (auto& child : children) {
		child.interpolate(vars);
	    }
	    return *this;
	}

	// --- rendering ---
	std::string to_string(int level = 0) const {
	    std::string out;

	    // text nodes
	    if (tag.empty()) {
		return indent(level) + text + "\n";
	    }

	    if (self_closing) {
		out += indent(level) + "<" + tag;
		for (const auto& [k, v] : attributes) {
		    out += " " + k + "=\"" + v + "\"";
		}
		out += " />\n";
		return out;
	    }

	    out += indent(level) + "<" + tag;
	    for (const auto& [k, v] : attributes) {
		out += " " + k + "=\"" + v + "\"";
	    }
	    out += ">\n";

	    for (const auto& child : children) {
		out += child.to_string(level + 1);
	    }

	    out += indent(level) + "</" + tag + ">\n";
	    return out;
	}
    };

// --- Stylesheet and CSS ---
    class CSSRule {
    private:
	std::string selector;
	std::vector<std::pair<std::string, std::string>> properties;
    public:
	explicit CSSRule(std::string selector) : selector(std::move(selector)) {}

	CSSRule& set(const std::string& key, const std::string& value) {
	    properties.push_back({key, value});
	    return *this;
	}

	std::string to_string() const {
	    std::string out = selector + " {\n";
	    for (const auto& [k, v] : properties) {
		out += "  " + k + ": " + v + ";\n";
	    }
	    out += "}\n";
	    return out;
	}
    };

    class Stylesheet {
    private:
	std::vector<CSSRule> rules;
    public:
	CSSRule& select(const std::string& selector) {
	    rules.emplace_back(selector);
	    return rules.back();
	}

	std::string to_string() const {
	    std::string out;
	    for (const auto& rule : rules) {
		out += rule.to_string();
	    }
	    return out;
	}
    };


// --- HELPER FUNCTIONS ---

    inline Node create(std::string tag, std::vector<Node> children = {}) {
	Node n(std::move(tag));
	for (auto& c : children) {
	    n.append(std::move(c));
	}
	return n;
    }

    inline Node Html(std::vector<Node> children = {}) {
	return create("html", std::move(children));
    }

    inline Node Body(std::vector<Node> children = {}) {
	return create("body", std::move(children));
    }

    inline Node Head(std::vector<Node> children = {}) {
	return create("head", std::move(children));
    }

    inline Node Title(const std::string& title) {
	return create("title", { Node::Text(title) });
    }

    inline Node Script(std::vector<Node> children = {}) {
	return create("script", std::move(children));
    }

    inline Node Style(const std::string& css) {
	return create("style", { Node::RawText(css) });
    }

    inline Node Div(std::vector<Node> children = {}) {
	return create("div", std::move(children));
    }

    inline Node A(const std::string& text, const std::string& href) {
	return create("a", { Node::Text(text) }).attr("href", href);
    }

    inline Node Header(std::vector<Node> children = {}) {
	return create("header", std::move(children));
    }

    inline Node Footer(std::vector<Node> children = {}) {
	return create("footer", std::move(children));
    }

    inline Node Section(std::vector<Node> children = {}) {
	return create("section", std::move(children));
    }

    inline Node Main(std::vector<Node> children = {}) {
	return create("main", std::move(children));
    }

    inline Node H1(const std::string& text) {
	return create("h1", { Node::Text(text) });
    }

    inline Node H2(const std::string& text) {
	return create("h2", { Node::Text(text) });
    }

    inline Node H3(const std::string& text) {
	return create("h3", { Node::Text(text) });
    }

    inline Node H4(const std::string& text) {
	return create("h4", { Node::Text(text) });
    }

    inline Node P(const std::string& text) {
	return create("p", { Node::Text(text) });
    }

    inline Node UL(std::vector<Node> children = {}) {
	return create("ul", std::move(children));
    }

    inline Node LI(const std::string& text) {
	return create("li", { Node::Text(text) });
    }

    inline Node Button(const std::string& text) {
	return create("button", { Node::Text(text) });
    }

    inline Node IMG(const std::string& src) {
	return Node::Self("img").attr("src", src);
    }

    // --- FORM ELEMENTS ---
    inline Node Form(std::vector<Node> children = {}) {
	return create("form", std::move(children));
    }

    inline Node Input(const std::string& type, const std::string& name, const std::string& value = "") {
	Node input = Node::Self("input").attr("type", type).attr("name", name);
	if (!value.empty()) {
	    input.attr("value", value);
	}
	return input;
    }

    inline Node Label(const std::string& text, const std::string& for_id = "") {
	Node label = create("label", { Node::Text(text) });
	if (!for_id.empty()) {
	    label.attr("for", for_id);
	}
	return label;
    }

    inline Node TextArea(const std::string& name, const std::string& placeholder = "") {
	Node ta = create("textarea");
	ta.attr("name", name);
	if (!placeholder.empty()) {
	    ta.attr("placeholder", placeholder);
	}
	return ta;
    }

    inline Node Select(const std::string& name, std::vector<Node> options = {}) {
	return create("select", std::move(options)).attr("name", name);
    }

    inline Node Option(const std::string& value, const std::string& label_text) {
	return create("option", { Node::Text(label_text) }).attr("value", value);
    }
}

#endif
