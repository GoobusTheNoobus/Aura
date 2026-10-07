# Aura
Aura is an interpreter written for practice in C++. A good amount of code
is written at school. Please note that this program has more aura than it's 
author (@GoobusTheNoobus).

## Building
Aura uses CMake and Ninja to build. A C++20 compiler is also needed.
```
cmake -B <build-folder> -G Ninja -DCMAKE_BUILD_TYPE=<build-type>
cmake --build <build-folder>
```
This should build/link an executable into `<build-folder>/aura`. Replace 
`<build-folder>` with your own folder name. A good build folder name should
begin with `build`. For example, `build-debug` or `build-release`. Please 
make sure your build folder never makes it into the github repo (add your
build folder in the .gitignore).

**No LLM generated code is present in this repository.**
