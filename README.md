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

通过 C++ `new` 表达式分配内存，主要有以下两个问题：

* 每分配一个元素，最终都需要调用一次 `malloc`；
* 每个元素除自身大小所占内存之外，`malloc` 还会为其地址上下各分配一个 `cookie`（共计两个，在 64 位机器上，占 `8 * 2 = 16` 字节）。

通过自定义内存管理机制，希望可以减少 `malloc` 调用次数，同时避免过多 `cookie` 浪费空间。

## Per-class allocator 1

重载 `operator new / delete`：

```C++
void *Screen::operator new(size_t size) {
    Screen *p;
    // 当 linked list 为空，申请 chunk 大小内存
    if (!freeStore) {
        size_t chunk = screenChunk * size;
        freeStore = p = reinterpret_cast<Screen *>(new char[chunk]);
        // 将整块内存分片，当作 linked list 串接在一起
        for (; p != &freeStore[screenChunk - 1]; ++p) {
            p->next = p + 1;
        }
        p->next = nullptr;
    }
    p = freeStore;
    freeStore = freeStore->next;
    return p;
}

void Screen::operator delete(void *p) {
    // 将 delete object 插回 free list 前端
    (static_cast<Screen *>(p))->next = freeStore;
    freeStore = static_cast<Screen *>(p);
}
```

测试：

```C++
// 初始化 allocator test 1 指针与 chunk 大小
Screen *Screen::freeStore = nullptr;
const int Screen::screenChunk = 24;

// per-class allocator - test 1
void Screen_test() {
    cout << sizeof(Screen) << endl;
    size_t const N = 100;
    Screen *p[N];

    for (int i = 0; i < N; ++i) {

        p[i] = new Screen(i);   // 调用重载 operator new
//        p[i] = ::new Screen(i); // 调用全局 operator new
    }

    for (int i = 0; i < 10; ++i) {
        cout << p[i] << endl;
    }

    for (auto &i: p) {
        delete i;// 调用重载 operator delete
//        ::delete i; // 调用全局 operator delete
    }
}
```

```bash
# 调用重载 operator new / delete
D:\FileD\Workspace_Local\Memory-Allocator\cmake-build-debug\Memory_Allocator.exe
16
0xf915d0
0xf915e0
0xf915f0
0xf91600
0xf91610
0xf91620
0xf91630
0xf91640
0xf91650
0xf91660

# 调用全局 ::operator new / delete
D:\FileD\Workspace_Local\Memory-Allocator\cmake-build-debug\Memory_Allocator.exe
16
0xfc6f80
0xfc6fa0
0xfc15d0
0xfc15f0
0xfc1610
0xfc1630
0xfc1650
0xfc1670
0xfc1690
0xfc16b0
```

由测试结果可知，调用重载 `operator new / delete` 数组相邻元素间隔 16，而调用全局间隔 32（16 + 8 + 8，两个 cookie）。

该方案不足为：

* 每个对象都包含一个 `next` 指针，需要占用 8 个字节；
* `operator delete` 没有 `free` 其占用的内存，而是将其插入到了空闲链表的头部。

## Per-class allocator 2

重载 `operator new / delete`：

```C++
void *Airplane::operator new(size_t size) {
    if (size != sizeof(Airplane)) return ::operator new(size);

    Airplane *p = headOfFreeList;
    // 当 p 非空，直接移动指针，否则申请 block 大小内存
    if (p) headOfFreeList = p->next;
    else {
        auto *newBlock = static_cast<Airplane *> (::operator new(BLOCK_SIZE * sizeof(Airplane)));
        // 内存切片
        for (int i = 1; i < BLOCK_SIZE - 1; ++i) {
            newBlock[i].next = &newBlock[i + 1];
        }
        newBlock[BLOCK_SIZE - 1].next = nullptr; // 结束 list
        p = newBlock;
        headOfFreeList = &newBlock[1];
    }
    return p;
}

void Airplane::operator delete(void *deadObject, size_t size) {
    if (deadObject == nullptr) return;
    if (size != sizeof(Airplane)) {
        ::operator delete(deadObject);
        return;
    }

    // 同第一版，将收回的指针插入到头部
    auto *carcass = static_cast<Airplane*>(deadObject);
    carcass->next = headOfFreeList;
    headOfFreeList = carcass;
}
```

测试：

```C++
// per-class allocator - test 2
void Airplane_test() {
    cout << sizeof(Airplane) << endl;
    size_t const N = 100;
    Airplane *p[N];

    for (auto & i : p) {
        i = ::new Airplane;
    }

    for (int i = 0; i < 10; ++i) {
        cout << p[i] << endl;
    }

    for (auto & i : p) {
        ::delete i;
    }
}
```

```bash
D:\FileD\Workspace_Local\Memory-Allocator\cmake-build-debug\Memory_Allocator.exe
8
0x642490
0x642498
0x6424a0
0x6424a8
0x6424b0
0x6424b8
0x6424c0
0x6424c8
0x6424d0
0x6424d8
```

由测试结果可知，调用重载 `operator new / delete` 数组相邻元素间隔 8（存在内存对齐），去掉了 cookie 部分。

## Static allocator

## Macro for static allocator

## 参考资料

* 侯捷 C++ 内存管理
