//
// Author: Yulan Zhou
//

#include <iostream>
#include "Screen.h"
#include "Airplane.h"

#pragma pack(4)

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

    for (auto & i : p) {
        i = new Airplane;
    }

    for (int i = 0; i < 10; ++i) {
        cout << p[i] << endl;
    }

    for (auto & i : p) {
        delete i;
    }
}


int main() {
    // per-class allocator test 1
    // Screen_test();

    // per-class allocator test 2
    Airplane_test();
}