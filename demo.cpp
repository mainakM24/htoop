#define _WIN32_WINNT 0x0A00
#include <string>
#include "./include/httplib.h"
#include "htoop.hpp"

using namespace htoop;

int main(void)
{
    Node style("style");
    style.append(R"(
    body {
	font-family: monospace;
	font-size: 20px;
	background: #181818;
	color: white;
	margin: 0;
	padding: 10px;
    }
    .header {
	color: #38bdf8;
	text-align: center;
	font-size: 40px;
    }
    h1 {
	text-align: center;
	color: #38bdf8;
    }
    )");

    Node home("html");
    Node head("head");
    Node body("body");

    Node title("title");
    title.append("HTOOP");

    head.append(title);
    head.append(style);

    Node header("div");
    header.attr("class", "header");
    header.append("HTOOP");

    Node hero("h1");
    hero.append("HTOOP: Web in C++");

    Node ul("ul");
    ul.append(Node("li").append("Features"));
    ul.append(Node("li").append("HTML-less Web"));
    ul.append(Node("li").append("Custom HTML Builder"));
    ul.append(Node("li").append("Self-closing tags"));
    ul.append(Node("li").append("Attribute system"));

    body.append(header);
    body.append(hero);
    body.append(ul);

    home.append(head);
    home.append(body);

    httplib::Server svr;
    svr.Get("/", [&home](const httplib::Request &, httplib::Response &res) {
	res.set_content(home.to_string(), "text/html");
    });

    std::cout << "Listening on port 8080\n";
    svr.listen("0.0.0.0", 8080);

    return 0;
}
