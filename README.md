# kirito
An experiment in cutting and connecting bits of list-like data-structures in C++ with ideas from Python

The name is kirito because slicing lists is cutting and connecting them, like kirito claims his name means. In addition, I envision this project becoming powerful, much like kirito, but never particularly likable, much like kirito.

I guess it should in theory support all list-like STL containers, and if you ask your compiler nicely, it should optimize down to something acceptable, and many errors in usage should be at compile time even if cryptic. I actually skimmed some compiled stuff with gcc and O3 and it looks pretty good. 

## Usage

The usage syntax is intended to remind one of Python list indexing syntax and how much better Python list indexing syntax is.

```
#include <kirito.h>  // single header, header only library

using kirito::I; // for Slice Indexing (copies stored type)
using kirito::V; // for View Indexing (is actually a pair of iterators)

// All your base container constructors and stuff work
kirito::FancyContainer<std::vector<int>> fancy = {1, 2, 3, 4, 5, 6, 7, 8, 9};

// Get a slice of a fancy container starting at 1, until 5, in steps of 2
auto slice = fancy[I(1)(5)(2)];

// You can leave out start/stop/step python style
slice = fancy[I()(5)];  // 0 until 5 in steps of 1
slice = fancy[I(2)()(3)];  // 2 until end in steps of 3

// Don't like copies? V does the same thing, but gives you views into the base container
auto view = fancy[V(1)(5)(2)];

// You can pass views into things that expect containers, but like that obviously incurs a copy
int somefunction(std::vector<int> notaview);
somefunction(view);
```

## Building Tests

Like there is only one test right now but I'm writing more I swear. It's cmake so,
```
mkdir build && cd build && cmake .. && make
```

## History

Someone I knew from Animania, the University of Michigan Anime Club, complained about indexing into containers in C++, and wanted something more like Python. I was looking for a quick project to try to write some C++17 code. Here we are today. 
