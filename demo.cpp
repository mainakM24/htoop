#define _WIN32_WINNT 0x0A00
#include <string>
#include <iostream>
#include "./include/httplib.h"
#include "htoop.hpp"

using namespace htoop;

std::string get_styles() {
    Stylesheet css;
    css.select("body")
	.set("font-family", "system-ui, sans-serif")
	.set("font-size", "16px")
	.set("background", "#0f172a")
	.set("color", "#f8fafc")
	.set("margin", "0")
	.set("padding", "40px");

    css.select(".container")
	.set("max-width", "600px")
	.set("margin", "0 auto")
	.set("background", "#1e293b")
	.set("padding", "30px")
	.set("border-radius", "12px")
	.set("box-shadow", "0 4px 6px -1px rgb(0 0 0 / 0.1)");

    css.select("h1, h2")
	.set("color", "#38bdf8")
	.set("margin-top", "0");

    css.select(".form-group")
	.set("margin-bottom", "20px");

    css.select(".notice")
	.set("background", "#0f172a")
	.set("border-left", "4px solid #38bdf8")
	.set("padding", "12px")
	.set("margin-bottom", "20px")
	.set("border-radius", "4px");

    css.select("label")
	.set("display", "block")
	.set("margin-bottom", "8px")
	.set("font-weight", "600")
	.set("color", "#94a3b8");

    css.select("input, select, textarea")
	.set("width", "100%")
	.set("padding", "10px")
	.set("background", "#0f172a")
	.set("border", "1px solid #334155")
	.set("border-radius", "6px")
	.set("color", "#f8fafc")
	.set("box-sizing", "border-box");

    css.select(".btn")
	.set("background", "#38bdf8")
	.set("color", "#0f172a")
	.set("border", "none")
	.set("padding", "12px 24px")
	.set("font-weight", "bold")
	.set("border-radius", "6px")
	.set("cursor", "pointer")
	.set("width", "100%");

    css.select(".footer")
	.set("margin-top", "30px")
	.set("text-align", "center")
	.set("font-size", "14px")
	.set("color", "#64748b");

    css.select("a")
	.set("color", "#38bdf8")
	.set("text-decoration", "none");

    return css.to_string();
}

Node CreateFormPage(const std::string& username, bool is_first_time) {
    Node page = Html({
	Head({
	    Title("HTOOP Form Server"),
	    Style(get_styles())
	}),
	Body({
	    Div({
		H1("Welcome to HTOOP!"),
		P("Hi {{name}}, compile your HTML using modern C++ components."),

		// Note: The conditional greeting notice will be appended programmatically using find_by_id!

		H2("Submit Feedback"),
		Form({
		    Div({
			Label("Name:", "name-input"),
			Input("text", "user_name", "{{name}}").set_id("name-input")
		    }).add_class("form-group"),

		    Div({
			Label("Choose Topic:", "topic"),
			Select("topic", {
			    Option("bugs", "Bug report"),
			    Option("docs", "Documentation improvement"),
			    Option("love", "Just sending love ❤️")
			}).set_id("topic")
		    }).add_class("form-group"),

		    Div({
			Label("Message:", "msg"),
			TextArea("msg", "Tell us what you think!").set_id("msg")
		    }).add_class("form-group"),

		    Button("Send Feedback").add_class("btn")
		}).attr("method", "POST").attr("action", "/feedback"),

		Div({
		    A("HTOOP Github", "https://github.com/mainakM24/htoop")
		}).add_class("footer")
	    }).add_class("container").set_id("main-container")
	})
    });

    // Programmatically append the notice if first-time user
    if (is_first_time) {
	if (Node* main_container = page.find_by_id("main-container")) {
	    main_container->append(P("✨ Thank you for checking out HTOOP for the first time!").add_class("notice"));
	}
    }

    // Interpolate variables recursively
    page.interpolate({{"name", username}});

    return page;
}

int main(void) {
    httplib::Server svr;

    svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
	Node home = CreateFormPage("Guest Developer", true);
	res.set_content(home.to_string(), "text/html");
    });

    svr.Post("/feedback", [](const httplib::Request& req, httplib::Response& res) {
	std::string submitted_name = req.get_param_value("user_name");
	if (submitted_name.empty()) {
	    submitted_name = "Anonymous";
	}
	Node thank_you = Html({
	    Head({
		Title("Thank You!"),
		Style(get_styles())
	    }),
	    Body({
		Div({
		    H1("Thank You!"),
		    P("We received your submission, " + escape(submitted_name) + "!"),
		    A("Go Back", "/")
		}).add_class("container")
	    })
	});
	res.set_content(thank_you.to_string(), "text/html");
    });

    std::cout << "Listening on port 8080. Open http://localhost:8080/ in your browser.\n";
    svr.listen("0.0.0.0", 8080);
    return 0;
}
