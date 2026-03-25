#define _WIN32_WINNT 0x0A00
#include <string>
#include "htoop.hpp"

using namespace htoop;

std::string get_styles() {
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
    return css.to_string();
}

Node Header(const std::string &title) {
    return Div({
	    H1(title)
	}).add_class("header")
	.set_id("hdr")
	.add_class("cont");
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
	    Button("Click Me"),
	    IMG("https://picsum.photos/id/103/300/200"),
	    A("Htoop: Github Link", "https://github.com/mainakM24/httop")
	});
}

Node App() {
    return Div({
	    Header("HTOOP"),
	    H1("HTOOP: Web in <C++>").attr("class", "header"),
	    Features(),
	    Extra()
	});
}


int main(void)
{
    Node home_page = Html({
	    Head({ Title("HTOOP"), Style(get_styles()) }),
	    Body({ App() })
	});

    std::cout << home_page.to_string();

    return 0;
}
