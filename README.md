# macOS + VS Code C/C++ Lab Workspace Template

This template is designed for system-programming labs on macOS with VS Code. It supports:

- tiny one-file C exercises
- tiny one-file C++ exercises
- POSIX pthread C exercises
- medium multi-file C++ projects
- C++ async/future/thread exercises

The Makefiles build into `bin/app`, object files into `build/`, and dependency files into `build/*.d`.

## 1. One-time macOS setup

```bash
xcode-select --install
clang --version
make --version
lldb --version
```

Recommended VS Code extensions:

- `ms-vscode.cpptools` for C/C++ IntelliSense
- `vadimcn.vscode-lldb` for LLDB debugging on macOS

Optional:

```bash
brew install cmake ninja gcc llvm
```

You do not need CMake for these templates. Make is enough.

## 2. Create a new exercise/project

From this folder:

```bash
./tools/new-exercise.sh single-c lab04/ex01
./tools/new-exercise.sh single-cpp lab04/ex02
./tools/new-exercise.sh pthread-c lab04/ex03
./tools/new-exercise.sh multifile-cpp lab04/ex04
./tools/new-exercise.sh async-cpp lab05/ex01
```

This creates folders under `workspace/`.

## 3. Build and run

```bash
cd workspace/lab04/ex01
make
make run
make clean
make debug
make asan
```

## 4. VS Code workflow

Recommended: open the specific exercise folder, not the whole template root.

```bash
code workspace/lab04/ex01
```

Then use:

- `Terminal > Run Task > make: build`
- `Terminal > Run Task > make: run`
- `Run and Debug > Debug bin/app`

## 5. Makefile variables

You can override variables without editing the Makefile:

```bash
make CXXSTD=c++20
make MODE=release
make ARGS="input.bin output.bin" run
make SAN=thread asan
```

