#ifndef HTML_HPP
#define HTML_HPP

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

namespace htoop {
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
	// --- constructor ---
	Node(std::string tag) {
	    this->tag = tag;
	}

	Node(std::string tag, bool sc) {
	    this->tag = tag;
	    this->self_closing = sc;
	}

	// Not working properly
	// Node(std::string tag, std::string text) {
	//     this->tag = tag;
	//     this->append(text);
	// }

	// self closing
	static Node self(std::string tag) {
	    return Node(tag, true);
	}

	// text block
	static Node Text(const std::string& t) {
	    Node node("");
	    node.text = t;
	    return node;
	}

	// --- append ---
	Node& append(const Node& node) {
	    if (self_closing) {
		std::cerr << __FILE__ << ":";
		std::cerr << std::to_string(__LINE__) << ":" << __FUNCTION__ << "() => ";
		std::cerr << "ERROR: can't append to -> " << tag << std::endl;
		std::exit(1);
	    }
	    children.push_back(node);
	    return *this;
	}

	Node& append(const std::string& text) {
	    if (self_closing) {
		std::cerr << __FILE__ << ":";
		std::cerr << std::to_string(__LINE__) << ":" << __FUNCTION__ << "() => ";
		std::cerr << "ERROR: can't append to " << tag << std::endl;
		std::exit(1);
	    }
	    // Node text_node("");
	    // text_node.text = text;
	    children.push_back(Node::Text(text));
	    return *this;
	}

	// --- attributes ---
	Node& attr(const std::string& key, const std::string& value) {
	    attributes[key] = value;
	    return *this;
	}

	// --- render ---
	std::string to_string(int level = 0) const {
	    std::string out;

	    // text nodes
	    if (tag.empty()) {
		return indent(level) + text + "\n";
	    }

	    if (self_closing) {
		// opening tag
		out += indent(level) + "<" + tag;

		for (const auto& [k, v] : attributes) {
		    out += " " + k + "=\"" + v + "\"";
		}

		// closing tag
		out += " />\n";
		return out;
	    }

	    // opening tag
	    out += indent(level) + "<" + tag;

	    for (const auto& [k, v] : attributes) {
		out += " " + k + "=\"" + v + "\"";
	    }

	    out += ">\n";

	    // children
	    for (const auto& child : children) {
		out += child.to_string(level + 1);
	    }

	    // closing tag
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
	// selector => Ex: body/h1/.class
	CSSRule(std::string selector) {
	    this->selector = selector;
	}

	// props => Ex: background = white
	CSSRule& set(const std::string& key, const std::string& value) {
	    properties.push_back({key, value});
	    return *this;
	}

	// render
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
	// collection of css groups
	std::vector<CSSRule> rules;
    public:
	CSSRule& select(const std::string& selector) {
	    // creates CSSRule with selector arg
	    rules.emplace_back(selector);
	    return rules.back();
	}

	// render
	std::string to_string() const {
	    std::string out;
	    for (const auto& rule : rules) {
		out += rule.to_string();
	    }
	    return out;
	}
    };

// --- HELPER FUNCTIONS ---

    // Generic builder
    inline Node create(std::string tag, std::vector<Node> children = {}) {
	Node n(tag);
	for (auto& c : children) {
	    n.append(c);
	}
	return n;
    }

    inline Node Div(std::vector<Node> children = {}) {
	return create("div", children);
    }

    inline Node H1(const std::string& text) {
	return create("h1", { Node::Text(text) });
    }

    inline Node P(const std::string& text) {
	return create("p", { Node::Text(text) });
    }

    inline Node UL(std::vector<Node> children = {}) {
	return create("ul", children);
    }

    inline Node LI(const std::string& text) {
	return create("li", { Node::Text(text) });
    }

    inline Node IMG() {
	return Node::self("img");
    }
}

#endif
