# Project libSearch

## Overview
This project is a C++ library for searching and manipulating lists. It provides functionalities for creating, building, and manipulating lists with various search algorithms.

## Features
- **List Creation**: The library allows users to create lists with custom nodes.
- **List Building**: Users can build lists with specified search criteria and modes.
- **Search Algorithms**: The library implements search algorithms for finding specific elements in lists.
- **Memory Management**: Memory allocation and deallocation are handled within the library for efficient resource usage.
- **File I/O**: The library supports reading data from files to build lists.

## Getting Started
To use the library in your project, follow these steps:
1. Clone or download the repository.
2. Include the necessary header files (`list.h`, `module.h`, `stdafx.h`) in your project.
3. Link the library files (`libSearch.cpp`, `dllmain.cpp`, `stdafx.cpp`) with your project.
4. Use the provided functions and structures to manipulate lists according to your requirements.

## Usage
Here's a basic example of how to use the library:

```cpp
#include "list.h"
#include "module.h"
#include "stdafx.h"

int main() {
    // Create a new list node
    struct list *node = create_node("needle", "haystack", "type", 1);
    
    // Insert the node into a list
    insert_node(&tree_node, node, free_node);

    // Perform searches on the list
    int result = bld_match(tree_node, "needle");

    // Print the list
    print_list(tree_node);

    // Free memory allocated for the list
    free_list(tree_node, free_node);
    
    return 0;
}
```

## Contributing
Contributions to the project are welcome. Feel free to open issues for bug fixes, feature requests, or any other improvements you'd like to suggest. Pull requests are also appreciated.

## License
This project is licensed under the BSD 3 License. See the `LICENSE` file for details.
