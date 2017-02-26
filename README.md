# MGC

To contribute place your .dll files at the project scope.

Remember to #include "stddef.h" in all your files as first line, its the precompiled header.

### msys2/mingw64 guide for using clion/cmake on windows
1. get the msys2 installer from http://www.msys2.org/
2. install (most likely x64)
3. install libraries (run these commands in `C:\msys64\usr\bin\bash.exe` )
    1. `pacman -Su`
    2. `pacman -S git mingw-w64-x86_64-toolchain mingw64/mingw-w64-x86_64-SDL2 mingw64/mingw-w64-x86_64-SDL2_mixer mingw64 /mingw-w64-x86_64-SDL2_image mingw64/mingw-w64-x86_64-SDL2_ttf mingw64/mingw-w64-x86_64-SDL2_net mingw64/mingw-w64-x 86_64-cmake make mingw64/mingw-w64-x86_64-lua mingw64/mingw-w64-x86_64-glew`
3. Settings >> Build, Execution, Deployment >> Toolchains
   - set MinGW Home to `C:\msys64\mingw64`
4. Done

