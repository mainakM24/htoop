#define _WIN32_WINNT 0x0A00
#include <string>
#include "./include/httplib.h"
#include "htoop.hpp"

int main(void)
{
    httplib::Server svr;
    htoop::Node root = htoop::Node("html");
    htoop::Node head = htoop::Node("head");
    htoop::Node body = htoop::Node("body");

    body.append(htoop::Node("h1", "This is generated from cpp"));
    body.append(htoop::Node("h2", "This is h2"));
    body.append("<ul>\
		<li> This is </li>\
		<li> Literal html </li>\
		</ul>");

    body.append("<h1 style=\"color: blue;\">CSS is not implemented yet!!</h1>");
    body.append(htoop::Node("button", "No html file needed"));
    root.append(body);

    svr.Get("/", [&root](const httplib::Request &, httplib::Response &res) {
	res.set_content(root.to_string(), "text/html");
    });

    std::cout << "Listening on port 8080\n";
    svr.listen("0.0.0.0", 8080);

    return 0;
}
