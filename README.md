# The Shortest Path to Learn C++ (and A*): A Brief Recap

C++ has been (and will likely be for at least a bit) a central programming language in robotics. It is a powerful language that is flexible and (relatively) easy to write-in, while also being fast and efficient. This post
 comes to serve as a brief recap/summary/cheatsheet of some of the basics of C++ programming. We will be implementing the A* algorithm and discussing relevant C++ elements as we go.

From a high level, we will complete the following tasks today, and cover the following topics in the process:

1. Set up the directory structure for our planner project.
    * [Common directory structure in C++ projects.](#setting-up-common-directory-structure-in-c)
2. Create the main entrypoint for our program.
    * [The `main` function.](#the-main-function)
3. Set up a debugger in VSCode.
    * [Setting up a debugger in VSCode](#setting-up-a-debugger-in-vscode)
4. Define the `AStarPlanner` class and the `SearchState` struct.
    * Discussing: `class`, `struct`, `public`, `private`, `protected`, `virtual`, and `override`. 
    * [Classes, structs, and objects.](#classes-structs-and-objects)
5. Add member variables and functions to the `AStarPlanner` class and the `SearchState` struct. Discussing: the `plan`, `getSuccessors`, and `computeHeuristic` functions.
    * Discussing: passing by reference, default arguments, and operator overloading.   
    * [Functions](#functions)
6. Add `open` and `closed` lists to the `AStarPlanner` class.
    * Discussing: `std::vector`, `std::pair`, `std::deque`, `std::shared_ptr`, and `std::priority_queue`.
    * [The standard library: useful objects.](#the-standard-library-useful-objects)
7. [Fill in the logic for the A* algorithm and helper methods.](#implementing-a)


## Setting Up: Common Directory Structure in C++
    
When starting a new C++ project, it is common to have a directory structure like this:

```
.  
├── CMakeLists.txt
├── include
│   └── my_project
│       └── my_class_header_file.hpp
└── src
    ├── my_class_source_file.cpp
    └── main.cpp
```

The `include` directory is where we'll put our header files. Header files are used to declare classes, functions, and variables that are used across our program. In our case, we will have a header file for our A* algorithm, which we will call `astar.hpp`. The `src` directory is where we put our source files. These usually contain the implementation of the classes and functions declared in the header files. In our case, we will have a source file for our A* algorithm, which we will call `astar.cpp`. `main.cpp` is the entrypoint file for our program. Everything that will run will begin there (this will hopefully become clearer soon). Now, since C++ is a compiled language, we need to tell the compiler how to build our project and turn it into an executable binary which we can run. This is where the `CMakeLists.txt` file comes in. This file contains information for building the project.

To start out then, create the following directory structure:

```
.
├── CMakeLists.txt
├── include
│   └── shortest_path_to_cpp
│       └── astar.hpp
└── src
    ├── astar.cpp
    └── main.cpp
```

In what follows we will be keeping track of three files: `astar.hpp`, `astar.cpp`, and `main.cpp`.  They will start out empty and we will gradually fill them in.


---

<Details markdown="block">
  <summary>The current state of our code files.</summary>
  

```cpp
// astar.hpp
```

```cpp
// astar.cpp
```

```cpp
// main.cpp
```
  
</Details> 

---



## The `main` Function
C++ programs start executing at the `main` function. Whatever this function does is what the program will do! Nothing fancy here. Normally, this function will create objects, call functions, and do whatever else is needed to run the program. In our case we will construct a grid here, create an A* planner object, and call its `plan` function. 

The `main` function is the entry point of the program, and it has the following signature:
    
```cpp
#include <iostream>
int main(int argc, char** argv)
{
    // Your code here.
    std::cout << "Welcome to Planning with Friends!" << std::endl;
    return 0;
}
```

Here, the `argc` parameter is the number of arguments passed to the program, and `argv` is an array of strings containing the arguments. We won't be using these in this post, but they are useful when you want to pass arguments to your program from the command line. It's also possible to omit these parameters from the signature if you don't need them.

Let's create a simple example before we proceed. We'll also use this as an opportunity for setting up the build system with CMake. Start by creating all the `.cpp` and `.hpp` files in the directory structure we discussed earlier. Now, for the `CMakeLists.txt` file, add the following:

```cmake
cmake_minimum_required(VERSION 3.10)

project(shortest_path_to_cpp)

set(CMAKE_CXX_STANDARD 17)

include_directories(include/shortest_path_to_cpp)

add_executable(planner src/main.cpp src/astar.cpp)
```
To build the project, create a `build` directory in the root of the project and run the following commands:

```bash
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make
```
Change the `Debug` to `Release` if you want to build a release version of the program. This will create an executable called `planner` in the `build` directory. You can run it by running `./planner` in the terminal.


---

<Details markdown="block">
  <summary>The current state of our code files.</summary>
  

```cpp
// astar.hpp
```

```cpp
// astar.cpp
```

```cpp
// main.cpp
#include <iostream>
int main(int argc, char** argv)
{
    // Your code here.
    std::cout << "Welcome to Planning with Friends!" << std::endl;
    return 0;
}
```
  
</Details> 

---



## Setting Up a Debugger in VSCode
One crucial tool for developing in C++ is a debugger. A debugger allows you to pause your program at any point and inspect the values of variables, the call stack, and more. This is incredibly useful when you are trying to figure out why your program is not working as expected. One popular way to set up a debugger is through VSCode, and today we will do just that. It is of course possible to use other editors/IDEs (e.g., CLion), or debug directly from the terminal (e.g., with `gdb`), but we will focus on VSCode since it is free and easy to set up. On top of helping us with debugging code, VSCode can also handle build tasks for us.

There are two main JSON files involved in setting up the debugger and build in VSCode: `launch.json` and `tasks.json`. The `launch.json` file contains the configuration for the debugger and the `tasks.json` file contains the configuration for the build tasks. 
Let's create these files. The first step is to open your code in VSCode. To do that (via the terminal), navigate to the directory where your code is located and run `code .`. This will open the current directory in VSCode. Now, create a `.vscode` directory in the root of your project and create the `launch.json` and `tasks.json` files in it. This can be done automatically via the GUI as well. Populate the `launch.json` file with the following:

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Planner Debug",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/build/planner",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ],
            "preLaunchTask": "build"
        }
    ]
}
```

And populate the `tasks.json` file with the following:

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "build",
            "type": "shell",
            "command": "cmake",
            "args": [
                "--build",
                "${workspaceFolder}/build",
                "--config",
                "Debug"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": [
                "$gcc"
            ]
        }
    ]
}
```

In the VSCode GUI we can now go to the debug tab (ctrl + shift + D in Ubuntu) and find our `Planner Debug` configuration.
Clicking on the play button runs our program in debug mode  (make sure that your build files were created with the `Debug` CMake flag). Allow me to repeat -- if this does not work _make sure_ that your code was built in `Debug` mode. Delete your `build` directory and run `cmake .. -DCMAKE_BUILD_TYPE=Debug` again.

We can play around with the debugger: create some variables and add some breakpoints in `main.cpp` to see how it works.


---

<Details markdown="block">
  <summary>The current state of our code files.</summary>
  
  Let's also add a header guard to our `astar.hpp` file to prevent multiple inclusions of the file. This may become clearer later.

```cpp
// astar.hpp
#pragma once
```

```cpp
// astar.cpp
```

```cpp
// main.cpp
#include <iostream>
int main(int argc, char** argv)
{
    // Your code here.
    std::cout << "Welcome to Planning with Friends!" << std::endl;
    return 0;
}
```
  
</Details> 

---


## Classes, Structs, and Objects
Let's begin implementing our A* planning algorithm! In this section we'll start setting up the `AStarPlanner` class and the `SearchState` struct which we will us in our implementation and also discuss classes and structs in C++ more generally. 


### `class`es
A `class` is a user-defined data type that groups related data and functions together. Let's discuss a few features of classes and them put them to work in our A* planner implementation.

* **Member Variables**:
Member variables are variables that belong to a class. They are declared inside the class definition and can be accessed by any member function of the class. A member variable we care about in A* is the grid we are planning on. We will store this in the `AStarPlanner` class under the name `grid_`. The trailing underscore is a common naming convention for [`private`](#access-modifiers) member variables in C++.

* **Member Functions**:
Similar to member variables, member functions (also called class methods) are functions that are defined in the class and can access any member of the class. They are used to perform operations on the data stored in the class with potential extra inputs. Our A* planner will have a member function called `plan` that will take a start and goal position and plan a path between them.

* **Constructors and Destructors**:
Constructors are special member functions that are called when an object of a class is created. They are used to initialize the object's member variables. Destructors are are similar, and are called when an object is destroyed. They are used to clean up resources used by the object. C++ provides a default constructor and destructor if you don't define one yourself. We will store the planning grid in the `AStarPlanner` class in the constructor. We will do this in an initialization list, which is a list of member variables to initialize in the constructor. This is more efficient than initializing the variables in the constructor body.

* **Access Modifiers**:   Access modifiers are keywords that control the visibility of class members. There are three access modifiers in C++: `public`, `private`, and `protected`. Members declared as `public` can be accessed from outside the class, members declared as `private` can only be accessed from within the class, and members declared as `protected` can be accessed from within the class and by derived classes (see the next section for more on this).

* **Inheritance**:
Inheritance is a feature of C++ that allows you to create a new class that is based on an existing class. Inherited classes (also called derived classes) include all the public and protected members of the base class (also called the parent class). This allows you to reuse code and create a hierarchy of classes. In our implementation, we will create a `Planner` base class and then create a derived class for the A* planner. Per the definition of the base class, all derived-class planners will be required to implement a `plan` function.

#### A* Planner Class
Alright, less talk more do. Let's get going with the implementation. 
We'll use a class to represent a planning algorithm in our program, and a derived class for our A* planner. The parent class will be _abstract_, meaning that it will have at least one _pure virtual function_---a function with no implementation marked with (`=0`). This is often done to dictate the structure of derived classes and force them to implement certain functions. In our case, we will have a abstract `Planner` base class that will have the pure virtual function `plan`, The derived A* planner class will implement this function.

We will also add some private methods, `computeHeuristic`, `expand`, and `getSuccessors`, to help with the planning process. As a matter of convenience, we'll create aliases to some types (i.e., shorter names to types that are long to type out and used frequently). For example, we will create an alias to `std::pair<int, int>`, which we use to store robot position values, and simply call it `Position`. This will make our code more readable and easier to understand. We do this with the keyword `using` (or the older `typedef`).


```cpp
// astar.hpp
#include <vector>
#include <memory>

#pragma once
using Position = std::pair<int, int>;

class Planner
{
public:
    virtual void plan(const Position& start, const Position& goal) = 0;
};

class AStarPlanner : public Planner
{
private:
    std::vector<std::vector<int>> grid_;

    // TODO: computeHeuristic.
    // TODO: getSuccessors.
    // TODO: expand.
public:
    // Member functions.
    Planner(const std::vector<std::vector<int>>& grid) : grid_(grid) {}
    void plan(const Position& start, const Position& goal) override;

    // Member variables.
    // TODO: Open list, 
    // TODO: closed list,
    // ...
};
```

### `struct`s
A `struct` is a data type identical to a class, with the exception that its members are public by default (as opposed to private in classes). They are used to group related data together. In our implementation, we will use a `struct` to represent a state in the search.

```cpp
// astar.hpp
struct SearchState
{
    Position pos;
    int g;
    int h;
    int f;
    std::shared_ptr<SearchState> parent;
};
```


The state of our code now, with the `Planner` and `AStarPlanner` classes and the `SearchState` struct defined, is as follows:



---

<Details markdown="block">
  <summary>The current state of our code files.</summary>
  
  Let's also add a header guard to our `astar.hpp` file to prevent multiple inclusions of the file. This may become clearer later.

```cpp
// astar.hpp
#include <vector>
#include <memory>

#pragma once

using Position = std::pair<int, int>;

struct SearchState
{
    Position pos;
    int g;
    int h;
    int f;
    std::shared_ptr<SearchState> parent;
};

class Planner
{
public:
    virtual void plan(const Position& start, const Position& goal) = 0;
};

class AStarPlanner : public Planner
{
private:
    // Variables.
    std::vector<std::vector<int>> grid_;
    // TODO: open_;
    // TODO: closed_;
    
    // Methods.
    // TODO: computeHeuristic.
    // TODO: getSuccessors.
public:
    // Member functions.
    Planner(const std::vector<std::vector<int>>& grid);
    void plan(const Position& start, const Position& goal) override;

};
```

```cpp
// astar.cpp
# include <astar.hpp>

Planner::Planner(const std::vector<std::vector<int>>& grid) : grid_(grid) {}

void AStarPlanner::plan(const Position& start, const Position& goal)
{
    // Implement the A* algorithm here.
}
```

```cpp
// main.cpp
#include <iostream>
int main(int argc, char** argv)
{
    // Your code here.
    std::cout << "Welcome to Planning with Friends!" << std::endl;
    return 0;
}
```
  
</Details> 

---



## Functions
As we get ready to define the member functions of our A* algorithm, it's worth mentioning some features of functions in C++.

* **Overloading**: C++ allows you to define multiple functions with the same name but different parameters. This is called function overloading. It is convenient when you want to perform the same operation on different types of data.
    ```cpp
    int add(int a, int b)
    {
        return a + b;
    }

    float add(float a, float b)
    {
        return a + b;
    }
    ```
    If we wanted to compare the f-values of two `SearchState` objects, for example, we could overload the `<` operator in the struct itself.
    ```cpp
    bool operator<(const SearchState& other) const
    {
        return -f < -other.f;
    }
    ```
    We won't do it here today though since we will be working with pointers.

* **Default Arguments**: You can provide default values for function parameters. If a value is not provided when the function is called, the default value will be used.
    ```cpp
    int add(int a, int b = 0)
    {
        return a + b;
    }
    ```

* **Custom Comparators**: When using `std::priority_queue` with custom types, we need to provide a way to compare elements. We create a dedicated comparator struct that tells the priority queue how to order elements based on their `f` values.

    ```cpp
    struct SearchStatePtrComparator {
        bool operator()(const std::shared_ptr<SearchState>& lhs, const std::shared_ptr<SearchState>& rhs) const {
            // Returns true if lhs should be popped before rhs.
            return lhs->f > rhs->f;
        }
    };
    ```
    Note that the `const` keyword at the end of the function declaration means that the function does not modify the object it is called on. The "const reference" `other` is explained next.

* **Passing Arguments**: function arguments can be passed in a few ways: by value, by reference, and by pointer. 
    
    * **Pass by value**: The default is by value, which means that a copy of the object is passed to the function. 
        ```cpp
        void foo(std::vector<int> v)
        {
            // v is copied, and the copy is modified.
            v.push_back(1);
        }
        ```
    
    * **Pass by reference**: When you pass a variable by reference, you are passing the memory address of the variable instead of the value. This is more efficient than passing by value, as the entire object is not copied. 
        ```cpp
        void foo(std::vector<int>& v)
        {
            // v is modified directly.
            v.push_back(1);
        }
        ```
        A common use case for passing by reference is when you want to modify the object passed to the function. Or in other words, if you want to "return more than one value" from a function. An example for this is below, where both `a` and `b` are modified.
        ```cpp
        void assignValues(int& a, int& b)
        {
            a = 1;
            b = 2;
        }
        ```

        A version of passing by reference is is **passing by `const` reference**, which means that the object cannot be modified. This is useful when we want to use the information stored in some object without changing it.
        ```cpp
        void foo(const std::vector<int>& v)
        {
            // v cannot be modified.
            v.push_back(1); // This will not compile.
            int x = v[0]; // This is fine.
        }

        ```
    * **Pass by pointer**: A pointer is a variable that stores the memory address of another variable. When you pass a variable by pointer, you are passing the memory address of the variable. This is similar to passing by reference, but you need to dereference the pointer to access the object.
        ```cpp
        void foo(std::vector<int>* v)
        {
            // v is modified directly.
            v->push_back(1);
        }
        ```

Let's put this to work by adding the definitions (not implementation yet) of the various functions we will need in our A* planner. We have already done the `plan` function, so let us now continue with the `computeHeuristic` and `getSuccessors` functions. 

```cpp
// astar.hpp
...
    int computeHeuristic(Position a, Position b);
    std::vector<SearchStatePtr> getSuccessors(const SearchStatePtr& state);
...
```
### Objects
An object is an instance of a class or struct. In our code we'll create, for example, an instance of the `AStarPlanner` class and call its `plan` function. Let's set up the `main` function to create a grid and call the planner.

```cpp
int main()
{
    std::vector<std::vector<int>> grid = {
        0, 0, 0, 0, 0,
        0, 1, 1, 1, 0,
        0, 1, 0, 0, 0,
        0, 1, 0, 1, 0,
        0, 0, 0, 0, 0
    };

    AStarPlanner planner(grid);
    Position start = {0, 0};
    Position goal = {4, 4};
    planner.plan(start, goal);

    return 0;
}
```

It is also possible to directly create a pointer to an object using the `new` keyword or `std::make_shared` function. This is useful when you want to create an object that will outlive the current scope. In our case, we will create shared pointers to `SearchState` objects. We'll talk more about pointers in the next section.

```cpp
std::shared_ptr<SearchState> state = std::make_shared<SearchState>();
```

In fact, since we will be making use of this pointer quite a bit, we'll create an alias for it to make our code more readable. 

```cpp
using SearchStatePtr = std::shared_ptr<SearchState>;

// Comparator for search state pointer.
struct SearchStatePtrComparator {
    bool operator()(const SearchStatePtr& lhs, const SearchStatePtr& rhs) const {
        // Returns true if lhs should be popped before rhs.
        return lhs->f > rhs->f;
    }
};
```


---

<Details markdown="block">
  <summary>The current state of our code files.</summary>
  

```cpp
// astar.hpp
#include <vector>
#include <iostream>
#include <memory>
#include <queue>
#include <unordered_set>

using Position = std::pair<int, int>;
using Path = std::vector<Position>;

struct SearchState {
    int g;
    int h;
    int f;
    Position pos;
    std::shared_ptr<SearchState> parent;
};

using SearchStatePtr = std::shared_ptr<SearchState>;

// Comparator for search state pointer.
struct SearchStatePtrComparator {
    bool operator()(const SearchStatePtr& lhs, const SearchStatePtr& rhs) const {
        // Returns true if lhs should be popped before rhs.
        return lhs->f > rhs->f;
    }
};

class Planner {
public:
    virtual Path plan(Position start, Position goal)=0;
    
private:
};

class AStarPlanner : public Planner {
private:
    // Variables.
    std::vector<std::vector<int>> grid_;
    // TODO: open_;
    // TODO: closed_;
    
    // Methods.
    int computeHeuristic(Position a, Position b);
    std::vector<SearchStatePtr> getSuccessors(const SearchStatePtr& state);

public:
    AStarPlanner(const std::vector<std::vector<int>>& grid);
    ~AStarPlanner();
    Path plan(Position start, Position goal) override;
};
```

```cpp
// astar.cpp
#include <planner.hpp>
#include <functional>


AStarPlanner::AStarPlanner(const std::vector<std::vector<int>>& grid) : grid_(grid) {
    std::cout << "AStarPlanner constructor called." << std::endl;
}

AStarPlanner::~AStarPlanner() {
    std::cout << "AStarPlanner destructor called." << std::endl;
}

int AStarPlanner::computeHeuristic(Position a, Position b) {
}

std::vector<SearchStatePtr> AStarPlanner::getSuccessors(const SearchStatePtr& state) {
}


Path AStarPlanner::plan(Position start, Position goal) {
    std::cout << "Plan() called" << std::endl;
}
```

```cpp
// main.cpp
#include <planner.hpp>
#include <iostream>
#include <memory>
#include <queue>

int main(int argc, char** argv) {
    Position start = {0, 0};
    Position goal = {4, 4};
    std::vector<std::vector<int>> grid = {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 1, 0, 0}
    };

    auto planner = std::make_shared<AStarPlanner>(grid);
    Path path = planner->plan(start, goal);

    for (auto pos : path) {
        std::cout << "(" << pos.first << ", " << pos.second << ")" << std::endl;
    }

    return 0;
}

```

</Details>


## The Standard Library: Useful Objects
We are almost ready to implement the logic of the A* algorithm. We are only missing the OPEN and CLOSED lists. Conceptually, there are various data structures that we could use to achieve different goals. Between lists, sets, heaps, and trees, we have a lot of options. 

* **Lists** are like simple containers -- they hold elements nicely, but are not ideal if we want to repeatedly sort their elements or check if elements are present.

* **Sets** are unordered but have fast lookup times. These are often implemented as hash tables. 

* **Maps** are similar to sets, but they store key-value pairs. Getting the value associated with a key is done in constant time and checking if a key is in the map is also done in constant time.

* **Heaps** allow us to access the element with the highest priority in constant time and insert new elements in logarithmic time. 

* **Trees** allow for fast (but not constant time) search while preserving order.

This section will discuss the C++ counterparts for these data structures and how we can use them in our implementation. Specifically, we will cover the relevant objects from the C++ standard library 

* **`std::vector`**:
A vector is a dynamic array that can grow and shrink in size. It is conceptually similar to a list that can change its size at runtime. In our implementation, we have already used vectors to represent the grid and store the path. Another use for a `vector` can be the queue of a depth-first search, for example. Searching through a vector can be done with the `std::find` function, which returns an [iterator](#what-we-did-not-talk-about) to the element if it is found, or the end of the vector if it is not found.

    ```cpp
    std::vector<int> v = {1, 2, 3, 4, 5};
    if (std::find(v.begin(), v.end(), 3) != v.end()) {
        std::cout << "Element found!" << std::endl;
    }
    ```

* **`std::deque`**: A deque (double-ended queue) is a data structure that allows insertion and deletion at both ends. It is similar to a vector, but it is more efficient when elements are added or removed from the front of the container. This can be used as the queue in a breadth-first search, for example. Checking if an element is in a deque can be done in linear time.

    ```cpp
    std::deque<int> d = {1, 2, 3, 4, 5};
    if (std::find(d.begin(), d.end(), 3) != d.end()) {
        std::cout << "Element found!" << std::endl;
    }
    ```

    Adding and removing elements from the back and front of a deque is done like this:

    ```cpp
    d.push_back(6);
    d.push_front(0);
    
    d.pop_back();
    d.pop_front();
    ```


* **`std::pair`** A pair is a simple container that can hold two values. It is useful when you need to return two values from a function or store two values together. In our implementation, we use pairs to represent positions on the grid.

* **`std::unordered_set`**: An unordered set is a data structure that stores unique elements in no particular order. Under the hood, it is a hash table. If hashing custom elements, you will need to provide a hash function.
In our implementation, we will use an unordered set to store the closed list of states.
Checking if an element is in an unordered set can be done in constant time.

    ```cpp
    std::unordered_set<int> s = {1, 2, 3, 4, 5};
    if (s.find(3) != s.end()) {
        std::cout << "Element found!" << std::endl;
    }
    ```

* **`std::unordered_map`**: An unordered map is a data structure that stores key-value pairs in no particular order. If hashing custom elements, you will need to provide a hash function. A similar `find` method to the unordered set can be used to check if a key is in the map.


* **`std::priority_queue`**:
A priority queue is a data structure that stores elements in a sorted order. When elements are added to the queue, they are placed in the correct position based on their priority. The element with the highest priority is at the front of the queue. In our implementation, we will use a priority queue to store the open list of states to explore. The priority queue uses a custom comparator to order the states based on their `f` values.

    ```cpp
    std::priority_queue<SearchStatePtr, std::vector<SearchStatePtr>, SearchStatePtrComparator> open_list;
    ```

    We can access the element with the highest priority in constant time with the `top` method. We can add elements to the queue with the `push` method and remove elements with the `pop` method.

    ```cpp
    SearchStatePtr state = std::make_shared<SearchState>();
    state->f = 4;
    open_list.push(state);
    open_list.pop();
    ```

We have already seen some pointers in this post, but we will also introduce a few more objects from the standard library that are useful in this context.

* **Pointers and Smart Pointers**:
C++ has introduce, since C++11, a class of objects called smart pointers. These are objects that manage the memory of a pointer automatically. A `shared_ptr` is a smart pointer that can be shared among multiple objects. It keeps track of how many objects are pointing to the same memory location and deletes the memory when the last object is destroyed. In our implementation, we will use shared pointers to manage the memory of our search states. It is worth looking at a quick example of how shared pointers can be safer to work with than raw pointers.

    ```cpp
    // Using raw pointers
    void foo()
    {
        SearchState* state = new SearchState();
        // Do something with state
        delete state;
    }

    // Using shared pointers
    void bar()
    {
        std::shared_ptr<SearchState> state = std::make_shared<SearchState>();
        // Do something with state
    }
    ```
    In the first function, if we were to forget deleting the `state` object, we would have a memory leak. In the second function, the `state` object will be automatically deleted when it goes out of scope (i.e., when the function ends).

    For any pointer, the actual object it points to can be retrieved by *dereferencing* the pointer. This is done with the `*` operator. For example, to access the `f` value of a `SearchState` object pointed to by a shared pointer, we would do the following:

    ```cpp
    SearchStatePtr state = std::make_shared<SearchState>();
    state->f = 4;
    SearchState s = *state;
    ```

* **The `auto` Keyword**: The `auto` keyword is used to automatically deduce the type of a variable. This can be useful when the type of the variable is long or complex (but clear from context). In our implementation, we use `auto` to create a shared pointer to an `AStarPlanner` object.

    ```cpp
    auto planner = std::make_shared<AStarPlanner>(grid);
    ```
    This is equivalent to writing:
    ```cpp
    std::shared_ptr<AStarPlanner> planner = std::make_shared<AStarPlanner>(grid);
    ```

With this new and exciting knowledge, let's define the open and closed lists. We will also add a hash function to support hashing of `Position` objects. 

```cpp
// astar.hpp
...
#include <unordered_set>
#include <queue>

using Position = std::pair<int, int>;   

// Custom hash function for std::pair<int, int>
namespace std {
    template <>
    struct hash<std::pair<int, int>> {
        std::size_t operator()(const std::pair<int, int>& p) const {
            auto hash1 = std::hash<int>{}(p.first);
            auto hash2 = std::hash<int>{}(p.second);
            return hash1 ^ (hash2 << 1);
        }
    };
}

...
    std::priority_queue<SearchStatePtr, std::vector<SearchStatePtr>, SearchStatePtrComparator> open_;
    std::unordered_set<Position> closed_;
...
```


---

<Details markdown="block">
  <summary>The current state of our code files.</summary>

`astar.hpp` is done!

```cpp
// astar.hpp
#include <vector>
#include <iostream>
#include <memory>
#include <queue>
#include <unordered_set>

using Position = std::pair<int, int>;
using Path = std::vector<Position>;

// Custom hash function for std::pair<int, int>
namespace std {
    template <>
    struct hash<std::pair<int, int>> {
        std::size_t operator()(const std::pair<int, int>& p) const {
            auto hash1 = std::hash<int>{}(p.first);
            auto hash2 = std::hash<int>{}(p.second);
            return hash1 ^ (hash2 << 1);
        }
    };
}

struct SearchState {
    int g;
    int h;
    int f;
    Position pos;
    std::shared_ptr<SearchState> parent;
};

using SearchStatePtr = std::shared_ptr<SearchState>;

// Comparator for search state pointer.
struct SearchStatePtrComparator {
    bool operator()(const SearchStatePtr& lhs, const SearchStatePtr& rhs) const {
        // Returns true if lhs should be popped before rhs.
        return lhs->f > rhs->f;
    }
};

class Planner {
public:
    virtual Path plan(Position start, Position goal)=0;
    
private:
};

class AStarPlanner : public Planner {
private:
    // Variables.
    std::vector<std::vector<int>> grid_;
    std::priority_queue<SearchStatePtr, std::vector<SearchStatePtr>, SearchStatePtrComparator> open_;
    std::unordered_set<Position> closed_;
    
    // Methods.
    int computeHeuristic(Position a, Position b);
    std::vector<SearchStatePtr> getSuccessors(const SearchStatePtr& state);

public:
    AStarPlanner(const std::vector<std::vector<int>>& grid);
    ~AStarPlanner();
    Path plan(Position start, Position goal) override;
};
```

We still have some work on `astar.cpp`. This is its current state:
```cpp
// astar.cpp
#include <planner.hpp>
#include <functional>


AStarPlanner::AStarPlanner(const std::vector<std::vector<int>>& grid) : grid_(grid) {
    std::cout << "AStarPlanner constructor called." << std::endl;
}

AStarPlanner::~AStarPlanner() {
    std::cout << "AStarPlanner destructor called." << std::endl;
}

int AStarPlanner::computeHeuristic(Position a, Position b) {
}

std::vector<SearchStatePtr> AStarPlanner::getSuccessors(const SearchStatePtr& state) {
}


Path AStarPlanner::plan(Position start, Position goal) {
    std::cout << "Plan() called" << std::endl;
}
```

`main.cpp` is done!
```cpp
// main.cpp
#include <planner.hpp>
#include <iostream>
#include <memory>
#include <queue>

int main(int argc, char** argv) {
    Position start = {0, 0};
    Position goal = {4, 4};
    std::vector<std::vector<int>> grid = {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 1, 0, 0}
    };

    auto planner = std::make_shared<AStarPlanner>(grid);
    Path path = planner->plan(start, goal);

    for (auto pos : path) {
        std::cout << "(" << pos.first << ", " << pos.second << ")" << std::endl;
    }

    return 0;
}

```

</Details>

## A Note on `for` Loops
In C++, there are a few ways to iterate over a collection of elements. The most common way is to use a one of the following `for` loops:

* **Range-based `for` loop**: This is a simple and clean way to iterate over a collection of elements. It is especially useful when you want to iterate over all elements in a collection. The syntax is as follows:
    ```cpp
    std::vector<int> v = {1, 2, 3, 4, 5};
    for (int i : v) {
        std::cout << i << std::endl;
    }
    ```
    This will print each element of the vector `v` on a new line.

* **`for` loop with iterators**: An iterator is an object that points to an element in a collection. You can use iterators to access elements in a collection and iterate over them. The syntax for a `for` loop with iterators is as follows:
    ```cpp
    std::vector<int> v = {1, 2, 3, 4, 5};
    for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << std::endl;
    }
    ```

* **`for` loop with index**: If you need to access the index of an element in a collection, you can use a `for` loop with an index. The syntax is as follows:
    ```cpp
    std::vector<int> v = {1, 2, 3, 4, 5};
    for (int i = 0; i < v.size(); ++i) {
        std::cout << v[i] << std::endl;
    }
    ```

* **`for_each` algorithm**: The `std::for_each` algorithm is a standard library algorithm that applies a function to each element in a collection. The syntax is as follows:
    ```cpp
    std::vector<int> v = {1, 2, 3, 4, 5};
    std::for_each(v.begin(), v.end(), [](int i) {
        std::cout << i << std::endl;
    });
    ```


# Implementing A*

We have all of our building blocks in place, so we can turn our attention to implementing the remaining functions to get the A* algorithm going!

Let's start with the `computeHeuristic` function. This function will compute the manhattan distance between two points.

```cpp
int AStarPlanner::computeHeuristic(Position a, Position b) {
    // Manhattan distance.
    return abs(a.first - b.first) + abs(a.second - b.second);
}
```

Moving on, let's implement `getSuccessors`. This takes in a `SearchState` and returns all the possible `SearchState`s that can be reached from that in one edge transition. 

```cpp

std::vector<SearchStatePtr> AStarPlanner::getSuccessors(const SearchStatePtr& state) {
    std::vector<SearchStatePtr> successors;

    // Define the possible moves.
    std::vector<Position> moves = {
        {0, 1}, {0, -1}, {1, 0}, {-1, 0}
    };

    // For each move.
    for (auto move : moves) {
        // Compute the new position.
        Position pos_new = {state->pos.first + move.first, state->pos.second + move.second};

        // Check if the new position is within the grid.
        if (pos_new.first < 0 || pos_new.first >= grid_.size() || pos_new.second < 0 || pos_new.second >= grid_[0].size()) {
            continue;
        }

        // Check if the new position is an obstacle.
        if (grid_[pos_new.first][pos_new.second] == 1) {
            continue;
        }

        // Create the new state.
        auto state_new = std::make_shared<SearchState>();
        state_new->pos = pos_new;
        successors.push_back(state_new);
    }

    return successors;
}
```

Finally, let's implement the `plan` function. This is the core of the A* algorithm. 

```cpp

Path AStarPlanner::plan(Position start, Position goal) {
    std::cout << "Plan() called" << std::endl;
    // Create the start state.
    auto startState = std::make_shared<SearchState>();
    startState->g = 0;
    startState->h = computeHeuristic(start, goal);
    startState->f = startState->g + startState->h;
    startState->pos = start;
    startState->parent = nullptr;

    // Add the start state to the open list.
    open_.push(startState);

    // While the open list is not empty.
    while (!open_.empty()) {
        // Get the state with the minimum f value.
        auto currentState = open_.top();
        open_.pop();

        // Check if the current state is the goal state.
        if (currentState->pos == goal) {
            // Reconstruct the path.
            Path path;
            while (currentState != nullptr) {
                path.push_back(currentState->pos);
                currentState = currentState->parent;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        // Add the current state to the closed list.
        closed_.insert(currentState->pos);

        // Get the successors of the current state.
        auto successors = getSuccessors(currentState);

        // For each successor.
        for (auto successor : successors) {
            // If the successor is in the closed list, skip it.
            if (closed_.find(successor->pos) != closed_.end()) {
                continue;
            }

            // Compute the g, h, and f values.
            successor->g = currentState->g + 1;
            successor->h = computeHeuristic(successor->pos, goal);
            successor->f = successor->g + successor->h;
            successor->parent = currentState;

            // Add the successor to the open list.
            open_.push(successor);
        }
    }

    return {};

}
```


# What we did not talk about
* Templates
* Namespaces
* Exceptions
* Assertions
* `static` keyword
* `friend` functions
* `virtual` inheritance
* The Boost library
* The Standard Library: Algorithms
* Lambda functions
* And more and more :).

**Use of LLMs**: GitHub Copilot was active when writing this post, so, as always when using LLMs, there is a plagiarism concern. Please let me know if this text looks similar to anything previously published and I'll make sure to add the appropriate citations.
