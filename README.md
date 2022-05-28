# C++ Memory Allocator

本仓库主要为 **侯捷 C++ 内存管理** 课程所介绍的 allocator 的实现与学习。

目录：

- [C++ Memory Allocator](#c-memory-allocator)
  - [自定义内存管理的动机](#自定义内存管理的动机)
  - [Per-class allocator 1](#per-class-allocator-1)
  - [Per-class allocator 2](#per-class-allocator-2)
  - [Static allocator](#static-allocator)
  - [Macro for static allocator](#macro-for-static-allocator)
  - [参考资料](#参考资料)

## 自定义内存管理的动机

![C++ 应用程序内存分配途径](https://yulan-img-work.oss-cn-beijing.aliyuncs.com/img/202205282047088.png)

![数组大小](https://yulan-img-work.oss-cn-beijing.aliyuncs.com/img/202205282051202.png)

通过 C++ new 表达式分配内存，主要有以下两个问题：

* 每分配一个元素，最终都需要调用一次 `malloc`；
* 每个元素除自身大小所占内存之外，`malloc` 还会为其地址上下各分配一个 `cookie`（共计两个，在 64 位机器上，占 `8 * 2 = 16` 字节）。

通过自定义内存管理机制，希望可以减少 `malloc` 调用次数，同时避免过多 `cookie` 浪费空间。

## Per-class allocator 1

## Per-class allocator 2

## Static allocator

## Macro for static allocator

## 参考资料

* 侯捷 C++ 内存管理
