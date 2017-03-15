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

* Naming conventions
    * functions, identifiers and source files are `snake_case`
    * Types are `CamelCase`
    * Constants (outside of a function) are `ALL_CAPS`

* Style
    * Const correctness is encouraged (if a function can be const it should probably be const)
    * `constexpr` > inline function > template magic > preprocessor macros (last resort)
    * The only real use case for macros is the use of the stringify operator    

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

Short version:

1. git clone the repo
2. Add the repo to Team Explorer
3. Doubleclick repo
4. Create a new solution with an empty project (uncheck Create directory for solution, unless you prefer that of course)
5. (Optional) Add a filter for each code subdirectory to both header/source files
6. Add the header/source files to the project
7. Go to Project Properties
8. C/C++ -> Precompiled Header. Set "Precompiled Header" to "Create (/Yc)"
9. C/C++ -> Precompiled Header. Set "Precompiled Header File" to "stdafx.h"
10. VC++ Directories. Add the include folder to "Include Directories"
11. VC++ Directories. Add the repective library subfolder to "Library Directories"
12. Linker -> Input. Add the following to "Additional Dependencies"
    glew32s.lib
    lua53.lib
    SDL2.lib
    SDL2_image.lib
    SDL2_ttf.lib
    SDL2main.lib
    SDL2test.lib
    opengl32.lib
    glu32.lib
13. Linker -> System. Set "SubSystem" to "Console (/SUBSYSTEM:CONSOLE)".
14. Build the project
15. Copy the DLL files from the respective library subfolder to Debug/ (where the .exe file was created).
 
Step by step:

1. git clone the repository.
2. File -> New -> Repository
3. Click "Add" above the textbox in the now opened Team Explorer window.
3. Click "..." on the right of the textbox.
4. Point it to the root of the repository.
5. Click the "Add" Button below the textbox.
6. Doubleclick the newly added entry.
7. Under "Solution" in the Team Explorer window, click "New..."
8. Choose "Empty Project"
9. Make sure "Create directory for solution" is unchecked
10. Give it a name and click "OK"
11. In Solution Explorer, rightclick "Header Files" choose Add -> New Filter. Name the filter "ini".
12. Rightclick the newly created filter and choose Add -> Existing Item...
13. Navigate to ini/, select all the *.h files and click "Add"
14. Rightclick "Header Files" and choose Add -> Existing Item...
15. Navigate to the root of the repository, select all *.h files and click "Add"
16. Rigthclick "Source Files" and choose Add -> New Filter. Name the filter "ini".
17. Rightclick the newly created filter and choose Add -> Existing Item...
18. Navigate to ini/, select all the *.cpp files and click "Add"
19. Rightclick "Source Files" and choose Add -> Existing Item...
20. Navigate to the root of the repository, select all *.cpp files and click "Add"
21. In the Solution Explorer, rightclick the Project (named whatever you entered earlier when creating the solution) and choose Properties
22. Make sure the "Platform" on the top right dropdown is set to Win32
23. On the left pane of the newly opened "Property Pages" window, expand the C/C++.
24. Choose "Precompiled Header" under C/C++.
25. On the right pane, for "Precompiled Header" choose "Create (/Yc)" from the dropdown
26. Put "stdafx.h" (without "") for "Precompiled Header File"
27. On the left pane, choose "VC++ Directories"
28. On the right pane open the dropdown of "Include Directories" and click "<Edit...>"
29. In the newly opened "Include Directories" window, click the leftmost button of the top right button row ("New Line")
30. Now click the "..." button on the right side of the opened textbox.
31. Navigate to include/ (in the repository folder) and click "Select Folder"
32. Click "OK"
33. On the right pane open the dropdown of "Library Directories" and click "<Edit...>"
34. Now click the "..." button on the right side of the opened textbox.
35. Navigate to lib/x86/ (in the repository folder) and click "Select Folder"
36. Click "OK"
37. On the left pane, choose "Debugging"
38. On the right pane, open the dropdown for "Working Directory" and choose "<Edit...>"
39. Set the textbox to ".."
40. Click "OK"
41. In the left pane, expand "Linker"
42. Under Linker, choose "Input"
43. On the right pane, open the dropdown for "Additional Dependencies"
44. Choose "<Edit...>"
45. Add the following to the top text field 
    glew32s.lib
    lua53.lib
    SDL2.lib
    SDL2_image.lib
    SDL2_ttf.lib
    SDL2main.lib
    SDL2test.lib
    opengl32.lib
    glu32.lib
46. Click "OK"
47. On the left pane, under "Linker" choose "System"
48. On the right pane, open the drop-down for "SubSystem"
49. Choose "Console (/SUBSYSTEM:CONSOLE)"
50. Click "OK" to close the Property Pages window.
51. In the menubar choose Build -> Rebuild Solution
52. In Explorer (the Windows one), go to lib/x86/ and copy all *.dll files to <ProjectName>/Debug/ (<ProjectName> being whatever you named the project)
53. Choose Debug -> Start Debugging
