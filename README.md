# MGC

## Conribute
### Style guidelines
* File structure
    * Use `#pragma once` instead of include guards, every half decent compiler should support it
    * Add very frequent includes to `stdafx.h` with the appropriate `using` statements
    * Yes: `using std::vector;` No: ~~`using namespace std;`~~
    * You should include external libraries (angle brackets) before internal ones (quotes)
    * Every compilation unit should have an associated header by the same name
    * use the namespace `mgc` for classes directly dependent on the game and your own one for those that dont (This is somewhat vague and might have to be decided on a case by case basis)
    
* Memory and smart pointers
    * Try to use references. EXCEPT for reassigning, there its better to use a pointer:
    ```C++
    void add_one(int& i) { i += 1 } // DONT
    void add_one(int* i) { *i += 1 } // DO
    
    int i = 0;
    add_one(i); // DONT
    add_one(&i); // DO
    ```
    * Generally you should try to use sdl types and stack allocation first before touching the heap
    * Make sure that memory is owned by exactly one source
    * If you return a pointer **NEVER** expect the caller to delete it
    * Do not use smart pointers just because you are too lazy to call delete once (And dont come me with exception safety, all important cleanup is done by the game after a crash, everything else is done by any half decent OS)
    * Use them if the lifetime of an object cant possibly determined (although you should make sure to test all other options first), in those rare cases `shared_ptr` with its reference count has a valid use

* Getters and Setters
   * Don't write get/set pairs for private variables, make the variable public instead.
   * If the get/set pair has side effects, consider picking a verb instead like `set_name` -> `rename`

* Naming conventions
    * functions, identifiers and source files are `snake_case`
    * Types are `CamelCase`
    * Constants (outside of a function) are `ALL_CAPS`

* Style
    * Const correctness is encouraged (if a function can be const it should probably be const)
    * `constexpr` > inline function > template magic > preprocessor macros (last resort)
    * The only real use case for macros is the use of the stringify operator
 
* Namespaces vs Classes
   * Use a namespace for free floating functions, and classes for state + functions
   * Don't abuse classes as namespaces
   * If you feel like deleting the only constructor you are doing something wrong

### msys2/mingw64 guide for using clion/cmake on windows
1. get the msys2 installer from http://www.msys2.org/
2. install (most likely x64)
3. install libraries (run these commands in `C:\msys64\usr\bin\bash.exe` )
    1. `pacman -Su`
    2. `pacman -S git mingw-w64-x86_64-toolchain mingw64/mingw-w64-x86_64-SDL2 mingw64/mingw-w64-x86_64-SDL2_mixer mingw64 /mingw-w64-x86_64-SDL2_image mingw64/mingw-w64-x86_64-SDL2_ttf mingw64/mingw-w64-x86_64-SDL2_net mingw64/mingw-w64-x 86_64-cmake make mingw64/mingw-w64-x86_64-lua mingw64/mingw-w64-x86_64-glew`
3. Settings >> Build, Execution, Deployment >> Toolchains
   - set MinGW Home to `C:\msys64\mingw64`
4. Done

### Visual Studio Setup (Visual Studio Community 2015)

Open the project file, that's all you have to do!
