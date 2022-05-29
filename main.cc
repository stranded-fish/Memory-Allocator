//
// Author: Yulan Zhou
//

#include <iostream>
#include <vector>
#include <ext\pool_allocator.h>
#include "Screen.h"
#include "Airplane.h"
#include "allocator.h"

#pragma pack(8)

#define DECLARE_POOL_ALLOC() \
public:                      \
    void *operator new(size_t size) { return myAlloc.allocate(size); } \
    void operator delete(void *pdead, size_t size) { return myAlloc.deallocate(pdead, size); } \
protected:                   \
    static my_allocator::allocator myAlloc;

#define IMPLEMENT_POOL_ALLOC(class_name) my_allocator::allocator class_name::myAlloc;

using namespace std;

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

const int Airplane::BLOCK_SIZE = 512;
Airplane *Airplane::headOfFreeList = nullptr;

// per-class allocator - test 2
void Airplane_test() {
    cout << sizeof(Airplane) << endl;
    size_t const N = 100;
    Airplane *p[N];

    for (auto &i: p) {
        i = new Airplane;
    }

    for (int i = 0; i < 10; ++i) {
        cout << p[i] << endl;
    }

    for (auto &i: p) {
        delete i;
    }
}

class Foo {
public:
    long long L;
    string str;
    static my_allocator::allocator myAlloc;
public:
    Foo() : L(0), str("10") {}

    explicit Foo(long l) : L(l) {}

    void *operator new(size_t size) {
        return myAlloc.allocate(size);
    }

    void operator delete(void *pdead, size_t size) {
        return myAlloc.deallocate(pdead, size);
    }
};

my_allocator::allocator Foo::myAlloc;

// static allocator    test 3
void allocate_test() {
    const int N = 100;
    Foo *p[N];
    cout << "sizeof(Foo)=" << sizeof(Foo) << endl;
    for (int i = 0; i < 20; ++i) {
        p[i] = new Foo(i);
//        p[i] = ::new Foo(i);
        cout << p[i] << ' ' << p[i]->L << endl;
    }

    for (int i = 0; i < 20; ++i) {
        delete p[i];
//        ::delete p[i];
    }
}

class Goo {
DECLARE_POOL_ALLOC()
private:
    long long val1;
    int val2;
    int val3;
};

IMPLEMENT_POOL_ALLOC(Goo)

// macro for static allocator test 4
void macro_test() {
    const int N = 100;
    Goo *p[N];
    cout << sizeof(Goo) << endl;
    for (int i = 0; i < 10; ++i) {
        p[i] = new Goo;
//        p[i] = ::new Goo;
        cout << p[i] << endl;
    }
    for (int i  = 0; i < 10; ++i) {
        delete p[i];
//        ::delete p[i];
    }
}

int main() {
    // per-class allocator test 1
     Screen_test();

    // per-class allocator test 2
     Airplane_test();

    // static allocator    test 3
     allocate_test();

    // macro for static allocator test 4
    macro_test();
}