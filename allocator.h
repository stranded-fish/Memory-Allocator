//
// Author: Yulan Zhou
//

#ifndef MEMORY_ALLOCATOR_ALLOCATOR_H
#define MEMORY_ALLOCATOR_ALLOCATOR_H

#include <iostream>

namespace my_allocator {
    class allocator {
    private:
        struct obj {
            struct obj* next; // 嵌入式指针
        };
    public:
        void *allocate(size_t);
        void deallocate(void*, size_t);
    private:
        obj *freeStore = nullptr;
        const int CHUNK = 5;
    };
}

#endif //MEMORY_ALLOCATOR_ALLOCATOR_H
