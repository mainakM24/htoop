#define _WIN32_WINNT 0x0A00
#include <string>
#include "./include/httplib.h"
#include "htoop.hpp"

using namespace htoop;

Node get_styles() {
    Node style("style");
    Stylesheet css;

    css.select("body")
	.set("font-family", "monospace")
	.set("font-size", "20px")
	.set("background", "#181818")
	.set("color", "white")
	.set("margin", "0")
	.set("padding", "20px");

    css.select(".header")
	.set("color", "#38bdf8")
	.set("text-align", "center")
	.set("font-size", "40px");

    style.append(css.to_string());
    return style;
}

Node Header(const std::string &title) {
    return Div({
	    H1(title)
	}).attr("class", "header");
}

Node Features() {
    return Div({
	    H1("Features"),
		UL({
			LI("HTML-less Web"),
			LI("Component System"),
			LI("Custom HTML Builder"),
			LI("Supports Self-closing tags"),
			LI("Attribute system"),
			LI("CSS builder")
		    })
	});
}

Node Extra() {
    return Div({
	    Node("button").append("click me"),
	    IMG().attr("src", "https://picsum.photos/id/103/300/200")
	});
}

Node App() {
    return Div({
	    Header("HTOOP"),
	    H1("HTOOP: Web in C++").attr("class", "header"),
	    Features(),
	    Extra()
	});
}


int main(void)
{
    Node home("html");
    Node head("head");
    Node body("body");
    Node title("title");

    title.append("HTOOP");
    head.append(title);

    Node style = get_styles();
    head.append(style);

    body.append(App());

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

