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

    Node(std::string tag, std::string text) {
	this->tag = tag;
        this->append(text);
    }

    // self closing
    static Node self(std::string tag) {
	return Node(tag, true);
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
        Node text_node("");
        text_node.text = text;
        children.push_back(text_node);
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

}

#endif
/* ------------------------------------------
#ifndef HTML_HPP
#define HTML_HPP

#include <iostream>
#include <string>
#include <vector>

namespace htoop {
    class Node {
    private:
	std::vector<Node> children;
	std::string text;
	std::string tag;
    public:
	Node(std::string tag) {
	    this->tag = tag;
	}

	Node(std::string tag, std::string text) {
	    this->tag = tag;
	    this->append(text);
	}

	std::string to_string(); 
	void append(const Node &node);
	void append(std::string text);
    };

    //======Implementation=======

    // render
    std::string Node::to_string() {
	std::string block;
	if (tag == "text") {
	    block.append(text + "\n");
	} else {
	    block.append("<" + tag + ">\n");
	    for (int i = 0; i < children.size(); ++i) {
		block.append(children[i].to_string());
	    }
	    block.append("</" + tag + ">\n");
	}
	return block;
    }

    // append whole node
    void Node::append(const Node &node) {
	children.push_back(node);
    }

    // append text as node
    void Node::append(std::string text) {
	Node node = Node("text");
	node.text = text;
	children.push_back(node);
    }
}
#endif
*/
