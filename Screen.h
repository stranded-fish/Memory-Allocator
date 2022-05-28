//
// Author: Yulan Zhou
//

#ifndef MEMORY_ALLOCATOR_SCREEN_H
#define MEMORY_ALLOCATOR_SCREEN_H

#include <iostream>

class Screen {
public:
    explicit Screen(int x) : i(x) {};

    int get() const { return i; }

    void *operator new(size_t size);

    void operator delete(void *p);

private:
    Screen *next{};
    static Screen *freeStore;
    static const int screenChunk;
private:
    int i;
};

#endif //MEMORY_ALLOCATOR_SCREEN_H
