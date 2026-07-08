#define _WIN32_WINNT 0x0A00
#include <string>
#include <iostream>
#include <cassert>
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

    css.select(".btn")
	.set("background", "#38bdf8")
	.set("color", "black")
	.set("border", "none")
	.set("padding", "10px 20px")
	.set("cursor", "pointer");

    css.select("body > div")
	.set("border", "1px solid #333")
	.set("padding", "10px");

    return css.to_string();
}

int main(void) {
    // 1. Variable template setup
    std::unordered_map<std::string, std::string> vars = {
	{"username", "Jane Doe"},
	{"page_title", "Welcome to HTOOP"},
	{"action_url", "/submit-feedback"}
    };

    // 2. Build the DOM tree using form elements and conditional elements
    bool is_admin = true;
    bool show_notice = false;

    Node doc = Html({
	Head({
	    Title("{{page_title}}"),
	    Style(get_styles())
	}),
	Body({
	    Div({
		H1("HTOOP Templates & Forms"),
		P("Hello, {{username}}! Please fill out the form below.")
	    }).add_class("container").set_id("header-container"),

	    // Forms tags & Inputs helper
	    Form({
		Label("Feedback Category:", "cat-select"),
		Select("category", {
		    Option("bug", "Report a Bug"),
		    Option("feature", "Request a Feature"),
		    Option("other", "Other")
		}).set_id("cat-select"),

		Label("Your Message:", "msg-area"),
		TextArea("message", "Type your feedback here...").set_id("msg-area"),

		// Conditional rendering
		Button("Submit Feedback").add_class("btn").set_id("submit-btn")
	    }).attr("method", "POST").attr("action", "{{action_url}}").set_id("feedback-form"),

	    // Add some conditional rendering snippets
	    Div().add_class("container").set_id("conditional-block")
		.append_if(is_admin, Div({ P("Admin Settings Panel") }).add_class("admin-panel"))
		.append_if(show_notice, Div({ P("This notice should NOT render") }).add_class("notice-panel"))
	})
    });

    // 3. Perform Variable Interpolation
    doc.interpolate(vars);

    // 4. Test DOM query API
    std::cout << "--- Testing DOM Queries ---\n";
    Node* header_div = doc.find_by_id("header-container");
    if (header_div) {
	std::cout << "Found header-container!\n";
	// Programmatic mutation via DOM pointer:
	header_div->add_class("highlighted");
    }

    std::vector<Node*> containers = doc.find_by_class("container");
    std::cout << "Found " << containers.size() << " elements with class 'container'.\n";
    for (size_t i = 0; i < containers.size(); ++i) {
	containers[i]->attr("data-index", std::to_string(i));
    }

    std::cout << "\n--- Rendered HTML Output ---\n";
    std::cout << doc.to_string();

    return 0;
}
