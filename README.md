# htoop

Compile your html!! C++ <3 HTML. A declarative HTML & CSS builder DSL in C++ that supports modular components, template interpolation, and direct DOM mutations.

---

## Build

### - Windows
```bash
g++ -o demo demo.cpp -lws2_32
.\demo.exe
```

### - Linux
```bash
# To run the CLI compilation demo:
g++ -O3 -Wall -Wextra -std=c++17 -o main main.cpp && ./main

# To run the interactive HTTP Server demo:
g++ -O3 -Wall -Wextra -std=c++17 -o demo demo.cpp -pthread && ./demo
```

---

## Features Showcase

### 1. Form Builder Elements
Declare form components using structured helper tags:
```cpp
Form({
    Label("Name:", "name-input"),
    Input("text", "user_name", "Jane Doe").set_id("name-input"),
    Label("Bio:", "bio"),
    TextArea("bio", "Write something...").set_id("bio"),
    Button("Submit").add_class("btn")
})
```

### 2. Conditional Rendering (`.append_if`)
Conditionally chain content dynamically in layout streams:
```cpp
Div().append_if(is_admin, P("Admin Control Panel"))
```

### 3. Template Variable Interpolation
Substitute placeholder parameters recursively inside text or attributes:
```cpp
Node page = Div({ P("Welcome back, {{username}}!") });
page.interpolate({{"username", "Jane"}});
```

### 4. DOM Tree Querying & Mutating
Query components programmatically using standard CSS classes or IDs:
```cpp
Node* header = doc.find_by_id("header");
if (header) {
    header->add_class("highlighted");
}

std::vector<Node*> inputs = doc.find_by_class("input-field");
```

---

## Roadmaps & Progress
- [x] implement attributes
- [x] implement other tags
- [x] improve the architecture (Move semantics & standard exceptions)
- [x] implement the server
- [x] remove the intermediate step (html less)
- [x] implement Node search: `find_by_id`/`find_by_class`
- [x] implement conditional rendering `append_if`
- [x] variable interpolation: replace `{{key}}` with value
- [x] form element helpers
