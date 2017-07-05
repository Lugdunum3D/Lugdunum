---
title: C++ Code Guidelines
---

# Header files

Each `.cpp` file should have an associated `.hpp` file.
Place the definitions for templates and inline functions in separated `.inl` files, alongside the corresponding header file in which it is included.

If applicable, include the `.inl` file inside the namespaces of the header file, as to not repeat these namespace in the inline file and include it at the end of the header file, just before the namespaces' closing brackets.

## Self-contained Headers

Header should be "self-contained", i.e. they must include all their dependencies, and the user should not have to worry about them.

## Headers Guards

To protect headers against double inclusion, headers must start with:

```cpp
#pragma once
```

## Forward Declarations

Preferably avoid forward declarations when possible, include the necessary files when possible, but do not feel restrained by this rule.

## Inline Functions

Inline functions should be implemented in `.inl` files.
Simple getters and setters should be inlined, as well as other short functions (usually less than 10 lines).

Typically, do not inline functions with loops, switch statements and others (unless if, in the common case, the loop or switch statement is never executed), as in this case, inlining the function might not be cost-effective.

**Example:**

lug/System/Logger/Logger.hpp:
```cpp
#pragma once

// ...

namespace lug {
namespace System {
namespace Logger {

class LUG_SYSTEM_API Logger {
    // ...
    
    template<typename T, typename... Args>
    void debug(const T& fmt, Args&&... args);

    // ...
};

#include <lug/System/Logger/Logger.inl>

} // Logger
} // System
} // lug
```

Corresponding inline file in lug/System/Logger/Logger.inl:
```cpp
// No namespace opened here

template<typename T, typename... Args>
inline void Logger::debug(const T& fmt, Args&&... args) {
    // ...
}

// No namespace closed here either
```

## Names and Order of Includes

Inclusion should happen in this order, each section separated by a new line and sorted in alphabetic order:

1. Related header (in a `.cpp` file, this is the corresponding `.hpp` header)
2. C library headers
3. C++ library headers
4. Other libraries' headers
5. Project headers

All of a project's header files should be listed as descendants of the project's source directory without use of UNIX directory shortcuts `.` (the current directory) or `..` (the parent directory).
These headers should be included as "system" headers, with angle brackets instead of double quotes, because it looks better in our opinion. Deal with it ;)

**Example:**

```cpp
#include <lug/System/Logger/Logger.hpp>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <memory>
#include <set>
#include <string>

#include <vulkan.h>

#include <lug/System/Logger/Handler.hpp>
#include <lug/System/Logger/Message.hpp>
```

You should include all the headers that define the symbols you rely upon, except in the unusual case of forward declaration. If you rely on symbols from `bar.hpp`, don't rely on the fact that you included `foo.hpp` which (currently) includes `bar.hpp`: include `bar.hpp` yourself, unless `foo.hpp` explicitly demonstrates its intent to provide you the symbols of `bar.hpp`. However, any includes present in the related header do not need to be included again in the related `.cpp` (i.e., `foo.cpp` can rely on `foo.hpp`'s includes).

# Scoping

## Namespaces

All namespaces should be terminated by a comment after the end bracket specifying the name of the corresponding namespace. A namespace doesn't imply another level of indentation, see below for an example.

Namespaces should be used also in the `.cpp` files to avoid repetition.

**Example:**

```cpp
namespace lug {
namespace System {
namespace Logger {

class Logger {
    // ...
};

} // Logger
} // System
} // lug
```

Do not declare anything in the namespace `std`, and do not use inline namespace, except for very, very specific use-cases.

*using-directive* and *namespace-aliases* are prohibited in header files, only use them in `.cpp` files or in some particular cases in internal-only namespaces.

## Unnamed Namespaces and Static Variables

Use of `static` variables and *unnamed namespaces* is encouraged in `.cpp` files for all code that does not need to be referenced elsewhere. Do not use that in header files.

## Nonmember, Static Member and Global Functions

Do not use global functions, always put them in a namespace. Do not use class as a namespace for some functions, use a namespace for that.

Static methods should generally be closely related to instances of the class or the class's static data.

## Local Variables

Do not separate variable declaration from its initialization.

```cpp
int x = 40; // Good
int y;

y = 2;      // Bad (initialization separated from declaration)
```

Declare variables in the lowerest scope and as close as possible of the first use.

## Static and Global Variables

Prefer POD (plain old data) when using static and global variables (except some very particular cases, one example would be the global Logger object, which is a static member of lug::System::Logger::Logger).

Preferably, do not use static and global variables at all.

# Classes

## Constructors

Where applicable, initialize members in the class definition (in the `.hpp` file).

**Example:**

```cpp
// ...

namespace lug {
namespace Graphics {

// ...

class LUG_GRAPHICS_API Camera : public Node {
    // ...

protected:
    Scene* _scene{nullptr};
    RenderQueue _renderQueue;
    RenderView* _renderView{nullptr};

    float _fov{45.0f};
    float _near{0.1f};
    float _far{100.0f};

private:
    // ...

    Math::Mat4x4f _projMatrix{Math::Mat4x4f::identity()};
    Math::Mat4x4f _viewMatrix{Math::Mat4x4f::identity()};

    bool _needUpdateProj{true};
    bool _needUpdateView{true};
};

} // Graphics
} // lug
```

## Implicit conversions and User defined conversions

Do not define implicit conversions, use the `explicit` keyword for conversion operators and single-argument-constructors.

Even with the `explicit` keyword, only use user defined conversions when it's meaningfull in some particular cases. In Lugdunum, we use them to convert types from our Vulkan abstraction to native Vulkan types.

**Example:**

```cpp
// ...

namespace lug {
namespace Graphics {
namespace Vulkan {

class Device {
public:
    // ...

    explicit operator VkDevice() const {
        // ...
    }
    
    // ...
};

} // Vulkan
} // Graphics
} // lug
```
    

## Copyable and Movable Types

All classes should define a default move and copy constructor and a default move and copy assignment operator using `= default`. If the move/copy operations are not useful for your class, you should disable them with `= delete`.

```cpp
namespace lug {
namespace Graphics {
namespace Vulkan {

class LUG_GRAPHICS_API Camera final : public lug::Graphics::Camera {
public:
    Camera(const std::string& name);

    Camera(const Camera&) = delete;
    Camera(Camera&&) = default;

    Camera& operator=(const Camera&) = delete;
    Camera& operator=(Camera&&) = default;
};

} // Vulkan
} // Graphics
} // lug
```


## Structs vs. Classes

`struct` are only for passing "inactive" data or Plain Old Data. They don't have constructors, destructors, functions. Everything else is a `class`. 

## Inheritance and multiple Inheritance

All methods should be private, except for methods that need to be accessed in subclasses which have to be protected.
When a method need to be override, define it as virtual in the base class and use the key word `override`.
If no class inherited from the sublclass override the method, the key word `final` must be used. The key word `final` must also be used for the inheritance itself, if no class inherit from the subclass.
Make your base class destructor virtual;

```cpp
namespace lug {
namespace Graphics {

class LUG_GRAPHICS_API Light : public MovableObject {
public:
    virtual ~Light();
    virtual void* getData(uint32_t& size) = 0;
    // Virtual destructor and method because overridden in PointLight

    // ...
};

class LUG_GRAPHICS_API PointLight final : public Light {
    // Use final here because no class inherit from PointLight

public:
    ~PointLight() override final;
    void* getData(uint32_t& size) override final;
    // Use final here because no subclass of PointLight will override getData() or the destructor
    // Also use override to overide getData and Light destructor

    // ...
};

} // Graphics
} // lug
```

Multiple inheritance is discouraged and is only allowed if all base classes are interfaces or if the base classes are abstract classes (but discouraged). The diamond inheritance is disallowed.

## Interfaces

All methods must be pure virtual (ends with `= 0`).
The interface must declare a virtual destructor.

```cpp
class Foo {
public:
    virtual ~Foo() = default;
    // virtual destructor

    virtual method() = 0;
    // Pure virtual method
};

```

## Operator Overloading

Do not abuse of operator overloading, only use them if their meaning is obvious.
The operators should be defined in the same namespace and class as the type it overloads, except from the binary operators (taking two parameters) that is encouraged to be declared in a non-member function (however in the same namespace as the class)


## Declaration Order

The declaration order in a class should be `public:`, followed by `protected:`, then `private:`. The methods and data members should have different blocks.
Each declaration block should be separated with a blank line. It's encouraged to separated with a blank line inside the blocks too for each methods/properties that can be grouped.

In each declaration block, the order in the following:
- Using declarations, typedefs and enums
- Constants (static const)
- Constuctors and assignment operators
- Destructors
- Methods, including static methods
- Data properties

# Functions

## Parameter Ordering

When a function takes inputs and outputs (i.e. an output pointer), it should list inputs first, then outputs. 

Inputs are usually values or `const` references, whereas inputs and input/output should be pointers to non-`const` variables. 

## Write Short Functions

It seems obvious, but prefer dividing large functions in "atomic" functions, dedicated to one specific task. As a rule of thumb, a function is considered as large when it has more than 50 lines.

Short functions improve code maintainability and readability.

## Reference Arguments

All parameters passed by reference must be labeled const.

**Example:**
```cpp
void foo(const string &in, std::string *out);
```

If you want to pass a `null`-able value as a parameter, you can use a raw pointer instead.

## Function Overloading

Function overloading can add complexity to the code and make it less readable. You can use function overlading but ask yourself first if there is not a better, more readable option available.

## Default Arguments

Default arguments are allowed on non-virtual functions when the default is guaranteed to always have the same value accross possible overloaded functions. For the same reasons detailed function overloading, be careful when using default arguments.

## Trailing Return Type Syntax

C++11 introduced a new syntax for function return types.

Old:
```cpp
int foo(int x);
```

New (C++11 only):
```cpp
auto foo(int x) -> int;
```

The difference is that in the new syntax, the type is declared in the function's scope.

Do continue to use the older style of function declaration where the return type goes before the function name. Use the new trailing-return-type form only in cases where it's required (such as lambdas).


# Other

## Ownership and Smart Pointers

Object ownership is represented by a `std::unique_ptr` or a `std::shared_ptr`, a.k.a. smart pointers. Consider that you never have ownership on raw pointers, so you must never free or delete a raw pointer.

Do not use shared ownership without a very good reason to back it up.

Never use `std::auto_ptr`. Instead, use `std::unique_ptr`.

# Others C++ Features

## Rvalue References

Use rvalues only in constructors and movement operators/constructors, or to do perfect forwarding (with `std::forward`).

## Friends

Use of `friend` is not strictly forbidden but you should avoid it as possible. You can use friend if it allows to remove a public access that is only used by the friended class.

Friends should be defined in the same file as much as possible.

## Exceptions

Use exceptions sparsely, only when another option such as return status/code or asserts is not available.

## Run-Time Type Information (RTTI)

Avoid over using Run Time Type Information (RTTI).

Using the type of an object at run-time is in general a problem of architecture. And it's also hard to maintain if you have decision trees or switch statements scattered throughout the code which all need to be updated when making changes.

## Casting

Even if the C++\-style cast syntax (with `static_cast<>`) is more verbose, always use it over old C-style casts.

* Use brace initialization to convert arithmetic types (e.g. int64{x}). This is the safest approach because code will not compile if conversion can result in information loss. The syntax is also concise.
* Use `static_cast` as the equivalent of a C-style cast that does value conversion, when you need to explicitly up-cast a pointer from a class to its superclass, or when you need to explicitly cast a pointer from a superclass to a subclass. In this last case, you must be sure your object is actually an instance of the subclass.
* Use `const_cast` to remove the const qualifier, avoid using it too frequently.
* Use `reinterpret_cast` to do unsafe conversions of pointer types to and from integer and other pointer types. Use this only if you know what you are doing and you understand the aliasing issues.

## Streams

Use streams only when they actually are the best tool for the job. Stream formatting and performance is not that good so think of the available alternatives when using streams.

Do not use `std::cout` or `std::cerr` for logging purpose, use [`System::Logger::Logger`](#lug::System::Logger::Logger) instead, which supports custom types, and other useful features such as easy-to-use formatting and cross-platform handlers/sinks.

Overload `<<` as a streaming operator for your type only if it represents a value and writes a human readable representation of that value. Do not expose implementation details in the output of `<<`. Such overloaded types are de-facto supported by Lugdunum's logger.

## Preincrement and Predecrement

Always use the prefixed form.

## Use of const

Always use `const` where applicable, and use `constexpr` when you are defining true constants, i.e. fixed at compile time. When writing code, put the `const` keyword before the type:

```cpp
const int* foo;
```

## Integer Types

Always use fixed-size integer types from `<cstdint>` such as `int32_t`, `int16_t`, `uint32_t`, etc. instead of `unsigned`, `long`, `unsigned int`, etc.

When applicable, always use `size_t` or `ptrdiff_t` to hint at the actual purpose of the variable.

## Preprocessor Macros

Avoid preprocessor macros, prefer `constexpr` values, inline functions, or even lambdas.

X macros are a special case and are not as much discouraged, but do weigh the advantages of the code lightness versus the readability disadvantage induced by X macros. X macros are very hard to read for inexperienced programmers, can quickly become too complicated and can really hurt the maintanability of the codebase. Be smart, and keep them simple!

## 0 and nullptr/NULL

Use `0` for integers, `0.0` for reals, `nullptr` (do *not* use `NULL`) for pointers, and `'\0'` for chars.

## sizeof

Prefer `sizeof(varname)` to `sizeof(type)` as it improves code maintainability.

**Example:**
```cpp
SomeType data;
memset(&data, 0, sizeof(data));     // Good
memset(&data, 0, sizeof(SomeType)); // Bad
```

## auto

Use `auto` to avoid type names that are noisy, obvious, or unimportant - cases where the type doesn't aid in clarity for the reader. Continue to use manifest type declarations when it helps readability. However, do not use a `auto` variable with initializer lists.

Only use auto on local variables.

## Braced Initializer List

Prefer using Braced Initializer List where possible.

## Lambda expressions

Use lambda expressions when appropriate, e.g. to pass a short comparaison function to an std algorithm.

Always use explicit captures by specifying which variables do you want, and make sure that the lifetime of the variable is longer than the lifetime of the lambda when capturing by reference or capturing a pointer.

Keep unnamed lambdas short and without a lot of captures.

Specify the return type of the lambda explicitly only when it is not obvious to the readers.

## Template metaprogramming

Think twice before using template metaprogramming, prefer a simpler technique if possible.

## std::hash

Do not define specializations of `std::hash`, as writing hash functions is difficult and error-prone, even for experts. Due to the high risk of ending up with a broken hash function, it has been decided to forbid specializing `std::hash` for your types.

## C++14

Always use C++14 libraries and features if possible, but keep it compatible with all the project supported compilers.

## Nonstandard Extensions

Only use standard extensions, exeptionnally where at least widely used and available on all the project supported compilers. Be smart and don't introduce non-maintainable code in the codebase ;)

# Naming

## File and Folder Names

File names must match the class defined inside and the file must be placed in a directory structure matching the namespace for the class.

Therefore, filenames should be in UpperCamelCase and shoud not contain separators such as spaces, dashes or underscores.

Header files must head with the `.hpp` extension, inline header files must hend with `.inl` and must be placed alongside the classic header files, and finally, source files must end with the `.cpp` extension.

Source files must be placed in the `./src/` folder, whereas header and inline source files must be located in the `./include/` folder.

**Example:** `Foo::Bar::MyClass` should have the following directory structure:

```dirtree
.
|-- src
|   |-- Foo
|   |   +-- Bar
|   |       |-- MyClass.hpp
|   |       +-- MyClass.inl
|   +-- ...
+--include
    |-- Foo
    |   +-- Bar
    |       +-- MyClass.cpp
    +-- ...
```


## Type Names

Type names should be in UpperCamelCase (with no underscores): `MyClass`.
This applies for classes, structs, type aliases, enums and type template parameters.

## Variable Names

Variable names should be in lowerCamelCase (with no underscores): `myVariable`.

Private and protected class members should start with an underscore.

**Example:**
```cpp
class Foo {
// ...
public:
    int barPublic;
    
protected;
    int _barProtected;
    
private:
    int _barPrivate;
};
```


## Constant Names

Refer to [Variable Names](#variable-names) above.

## Function Names

Function names shoul be written the same way as variable names, in lowerCamelCase, with no underscores: `myFunction()`.

When there is an acronym, it should be capitalized: write `sendUDP()`, not `sendUdp()`.

## Namespace Names

Nested namespaces should be in UpperCamelCase and the top-level namespace should be in lowerCamelCase, with no underscores, e.g.: `lug::Graphics`.

Do not use nested namespaces that would match top-level namespaces:
```cpp
namespace lug {
namespace std { // Bad
    // ...
}
}
```

## Enumerator Names

Refer to [Variable Names](#variable-names) above.

## Macro Names

Macro names should be written in upper case with underscore between words: `MY_MACRO`.
Keep in mind that macros are not recommanded (See [Preprocessor Macros](#preprocessor-macros)).


# Comments

## Comment Style

Use `//` for single-line comments and `/* */` for multiline comments outside of function blocks. Small blocks of multiline text can be written as mutliple `//` lines, see an example [in the implementation section](#implementation-comments).

A comment should always start with an upper case letter, and there should be a space after the opening comment syntax.

**Example:**
```cpp
//comment                                       // Bad

/* This is single-line a comment */             // Bad

// This is a single-line comment                // Good

/** 
 * This is a multiline
 * comment, that spans three lines of
 * text.
 */                                             // Good
 
// This is also a small
// mutliline comment, but this is allowed       // Good
```



## Class Comments

Each class should be described with a block preceding the class declaration, in accordance with the Doxygen format (with `@`, not `\`, i.e. `@brief` instead of `\brief`).

**Example:**
```cpp
/**
 * @brief      Class for camera.
 *
 *             This class represents a Camera in the 3D engine. A scene can be attached
 *             to a Camera. Only one scene can be attached to a Camera.
 *             A Camera can be attached to only one Render::View.
 */
 class LUG_GRAPHICS_API Camera : public Node {
     // ...
 }
```

## Function Comments

Same as classes, function declarations should be preceded with a block defining the function purpose, params, and return values. The block is also in accordance with the Doxygen format (with `@`, not `\`, i.e. `@brief` instead of `\brief`).

**Example:**
```cpp
class LUG_GRAPHICS_API Graphics {
    /**
     * @brief      Initializes the application with the informations filled in @p initInfo structure.
     *
     * @param[in]  initInfo  The initialize information.
     *
     * @return     @p true if the initialization was successful.
     */
    bool init(const InitInfo& initInfo);
}
```

## Variable Comments

### Data member

Comments of members of struct, union, class, or enum should be written after the variable declaration, with `///<` (in accordance to the Doxygen format) instead of the usual comment syntax.

**Example:**
```cpp
struct foo {
    int bar; ///< This is a data member comment.
};
```

The comment should describe, in less than one line, the purpose of the data. This comment will be present in the generated API documentation.

However, no comment is needed if the type and name of the data member are self explanatory.

### Global variables

The comment style of global variables is the same as single-line comments, described in [Comment Style](#comment-style).

## Implementation Comments

If a block is tricky or too complicated to understand it by reading the code, a comment can be written before it.

At Lugdunum, we prefer well written and readable code over over-commented, unreadable blocs of code.

For example, you should not comment trivial operations.

**Example:**
```cpp
for (std::size_t i = 0; i < renderQueue.getLightsNb(); ++i) {

    // Blend constants are used as dst blend factor
    // Now the depth buffer is filled, we can set the blend constants to 1 to enable blending
    if (i == 1) {
        const float blendConstants[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        vkCmdSetBlendConstants(static_cast<VkCommandBuffer>(cmdBuffer), blendConstants);
    }
}
```

**Trivial code:**
```
// Increment i                // Bad
i += 1;
```

The same applies for single lines, however, if you feel like you have to comment everything, maybe you should rethink your code first ;)

**Example:**
```cpp
// All the lights pipelines have the same renderPass
API::RenderPass* renderPass = _pipelines[Light::Light::Type::Directional]->getRenderPass();
```

## Punctuation, Spelling and Grammar

Comments should have good punctuation, spelling and grammar, like narrative texts.

Comments can sometimes be less formal, like for short comment describing a data member.

## TODO Comments

You should generally add a TODO comment before any code that is incomplete or needs review and or particular attention. This allows temporary quircks and hacks to be grouped and easily searched (e.g. in an IDE) in order to be correctly addressed before any merging is done to a definitive branch or version.

The name of the person who added such comment should appear inside parenthesis, right after the TODO. As such, the person resonsible for the comment remains easily tracked and also accountable for the TODO.

**Example:**
```cpp
// TODO(saveman71): replace opening file with something more global
std::ifstream shaderCode(file, std::ios::binary);
```

# Formatting

## Line Length

A line should not be more than 120 characters. This greately code enhance readability and prevents editor auto-wrapping that usually isn't smart enough to split the line(s) at the right position(s).

## Non-ASCII Characters

Although Non-ASCII characters should be rare, because applications should be localized externally, they must use `u8` prefix to ensure that the string literal uses UTF-8 encoding: `u8"äôéè"`.

Don't use `char16_t` or `char32_t` because they are not for UTF-8 character storage. Don't use the Windows type `wchar_t`, unless you are working with the Windows API in implementation specific files, or regions delimited by preprocessor directives.

## Spaces vs. Tabs

Never use tabs, only use spaces.
Indentation is only with 4 spaces, so configure your editor to correctly indent with 4 spaces.

## Function Declarations and Definitions

The return type, function name and parameters should be on the same line.

**Example:**
```cpp
void Node::lookAt(const Math::Vec3f& targetPosition) {
    // ...
}
```

If the line is longer than the [maximum line length](#line-length), you should write each parameter on one, separated line. The last parameter has to contain the closing parenthesis and the opening bracket of the function's scope.

**Example:**
```cpp
void Node::lookAt(
    const Math::Vec3f& targetPosition, // 4 spaces indent
    const Math::Vec3f& localDirectionVector, 
    const Math::Vec3f& up,
    TransformSpace space) {
    // ...
}
```

## Lambda Expressions

Lambda expressions are to be formatted the same way as functions.
There is no space between the capture mode and the variable captured.

**Example:**
```cpp
auto toUpper = [&foo](char c) {
    return static_cast<char>(toupper(c));
};
```

## Function Calls

Splitting arguments in function calls should respect the same rules as in [function declarations](#function-declarations-and-definitions).

**Example:**
```cpp
void main(int ac, char* av[]) {
    // ...
    callFooWhichIsALongFunctionAndTakesManyArguments(
        andNo,
        your,
        functionNames,
        should,
        reallyNot,
        beThatLong);
}
```


## Braced Initializer List Format

Splitting arguments in braced initializer lists should respect the same rules as in [function declarations](#function-declarations-and-definitions) except that the closing curly brace should be on his own line.
There is no space after the opening and the closing curly braces `{}`.

Examples:

```cpp
VkDescriptorSetLayoutCreateInfo createInfo{
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
    createInfo.pNext = nullptr,
    createInfo.flags = 0,
    createInfo.bindingCount = bindingCount,
    createInfo.pBindings = layoutBindings
};

lug::Graphics::Vulkan::Image::Extent extent = {width, height};
```

## Conditionals

The conditions should have no spaces after the opening parenthesis `(` and before the closing parenthesis `)`, and there should be one space between the condition keyword and the opening parenthesis `(`.
The `else` keyword should be on the same line as the closing bracket `}` of the previous condition.

**Example:**
```cpp
if (condition) {
    // 4 spaces indent
    // ... 
} else if (condition) {
    // ...
} else {
    // ...
}
```

For short single-line conditions, it's OK to put the condition on one line, but only if it improves readability:

```cpp
if (!condition) return false; // Ok
```

If the short single-line condition is not on one line, you must wrap the body with curly braces:
```cpp
if (!condition)    // Bad
    return false;

if (!condition) {  // Good
    return false;
}
```
## Loops and Switch Statements

As for conditions, you must always wrap the body for loops statements with curly braces, even if it's only one line long.

**Example:**

```cpp
for (uint32_t i = 0; i < 5; ++i)    // Bad
    std::cout << i << std::endl;
    
for (uint32_t i = 0; i < 5; ++i) {  // Good
    std::cout << i << std::endl;
}
```

Switch brackets `{}` follow the same rules as function brackets.
You should not use brackets `{}` around `case` keyword.

**Example:**
```cpp
switch (enumVal) {
    case VK_SUCCESS: // 4 spaces indent
        return "Success"; // 8 spaces indent
    case VK_NOT_READY:
        return "A fence or query has not yet completed";
}
```

It's OK to put case on the same line if it enhances readability. However, all the `case`s should one line long, as to keep consistency among each `switch` statement.

**Example:**
```cpp
// Bad
switch (type) {
    case Light::Type::Directional:    return std::make_unique<Light::Directional>(name);
    case Light::Type::Point:
        return std::make_unique<Light::Point>(name);
    case Light::Type::Spot:           return std::make_unique<Light::Spot>(name);
}

// Good
switch (type) {
    case Light::Type::Directional:    return std::make_unique<Light::Directional>(name);
    case Light::Type::Point:          return std::make_unique<Light::Point>(name);
    case Light::Type::Spot:           return std::make_unique<Light::Spot>(name);
}
```

## Pointer and Reference Expressions

When declaring a pointer, the `*` should be placed on the type, i.e. there is no space before the `*` or `&`.

**Example:**
```cpp
int foo;
int* bar; // Good
int * x; // Bad

bar = &foo;
```

## Boolean Expressions

Spaces around boolean operators are obligatory.
If a boolean expression is longer than the [maximum line length](#line-length), you should write each expression on separate lines, with the boolean operators at the end of each lines.

**Example:**
```cpp
if (!_pipelines[Light::Type::Directional] ||
    !_pipelines[Light::Type::Point] || // 4 spaces indent
    !_pipelines[Light::Type::Spot]) {
    // ...
}
```

## Return Values

The use of parenthesis around the return value is disallowed:
```cpp
return (5); // Bad
return 5; // Good
```

The only exception is for complex expressions:
```cpp
return (longExpressionA &&
        longExpressionB);
```
## Variable and Array Initialization

Prefer using `{}` than `()`.
There is no spaces around and inside the `{}` or `()`.

**Example:**
```cpp
int foo(5); // Good
int foo{5}; // Better
```

## Preprocessor Directives

Preprocessor directives follow a separate intentation scheme:
* Each preprocessor directive starts with a `#` on the first character of the line
* Nested conditions should have their content indented with one and only space per indent level.
* Again, preprocessor directive are not dependent on the indentation of the code they currently are located, and in the same way, indentation of code located inside preprocessor directives should not be disturbed.

A good way to remember this is that final, preprocessed code, should have the correct indentation. 

```cpp
int main(int ac, char* av[]) {
    uint16_t foo = 21;

#if defined(MACRO_A)
    if (ac > 2) {
        // Code run only if macro A
        foo += 21;
# if defined(MACRO_B)
        // Code run only if macro A and macro B
        foo -= 42;
# endif
    }
#endif

    return foo;
}
```

## Constructor Initializer Lists

If the constructor line is longer than the [maximum line length](#line-length), all the code after `:` should be written to a new line.

**Example:**
```cpp
Camera::Camera(const std::string& name) : Node(name) { // Short constructor
    // ...
}

Image::Image(VkImage image, const Device* device, const Extent& extent, bool swapchainImage, VkImageAspectFlags aspect) :
    _image(image), _device(device), _swapchainImage(swapchainImage), _aspect(aspect), _extent(extent) { // Long constructor, 4 spaces
    // ...           
}
```


## Vertical Whitespace

Try to limit the use of blank lines, but you can use them sparsely to split logically independant code sections and help readability.
Each file should end with a new line (`\n`).

# Conclusion

This style guideline is quite complete, but still missing some details. If you find an edge-case that this guideline does not cover, feel free to report any issue or contribute to this guideline.

As a general rule of thumb, your code should be the most readable possible, and it is always possible to flex some rules, if it makes your code better.

Good luck, have fun coding with us!