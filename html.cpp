#define _WIN32_WINNT 0x0A00
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "./include/httplib.h"


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
    void append(Node node);
    void append(std::string text);
};

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

void Node::append(Node node) {
    children.push_back(node);
}

void Node::append(std::string text) {
    Node node = Node("text");
    node.text = text;
    children.push_back(node);
}

int main(void)
{
    httplib::Server svr;
    Node root = Node("html");
    Node head = Node("head");
    Node body = Node("body");

    body.append(Node("h1", "This is generated from cpp"));
    body.append(Node("h2", "This is h2"));
    body.append("<ul>\
		<li> This is </li>\
		<li> Literal html </li>\
		</ul>");

    body.append("<h1 style=\"color: blue;\">CSS is not implemented yet!!</h1>");
    body.append(Node("button", "No html file needed"));
    root.append(body);

    svr.Get("/", [&root](const httplib::Request &, httplib::Response &res) {
	res.set_content(root.to_string(), "text/html");
    });

    std::cout << "Listening on port 8080\n";
    svr.listen("0.0.0.0", 8080);

    // std::ofstream file("index.html");
    // file << root.to_string();
    // file.close();

    return 0;
}
