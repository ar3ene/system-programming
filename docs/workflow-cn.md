# 推荐工作流

## 小题：单文件

用 `single-c` 或 `single-cpp`。只改 `src/main.c` 或 `src/main.cpp`。

```bash
./tools/new-exercise.sh single-c lab04/ex01
cd workspace/lab04/ex01
make run
```

传参数：

```bash
make ARGS="input.bin output.bin" run
```

## Pthreads 题

用 `pthread-c`。它已经有 mutex + pthread_create + join 示例。

```bash
./tools/new-exercise.sh pthread-c lab04/ex04
cd workspace/lab04/ex04
make run
```

## 多文件 C++ 工程

用 `multifile-cpp`。头文件放 `include/`，实现放 `src/`。

```bash
./tools/new-exercise.sh multifile-cpp project01
cd workspace/project01
make run
```

## C++ async/future/promise 题

用 `async-cpp`。

```bash
./tools/new-exercise.sh async-cpp lab05/ex02
cd workspace/lab05/ex02
make CXXSTD=c++20 run
```

## 常用命令

```bash
make              # 编译
make run          # 编译并运行
make ARGS="..." run
make debug        # 生成调试版 bin/app
make asan         # AddressSanitizer 编译并运行
make SAN=thread asan  # ThreadSanitizer；macOS 支持情况取决于 clang 版本
make release      # O2 release build
make clean
make print        # 查看 Makefile 检测到的源文件
```

## VS Code

建议每次只打开一个 exercise 文件夹：

```bash
code workspace/lab04/ex01
```

这样 VS Code 的 build/debug 都会默认针对当前 exercise 的 `Makefile`。

