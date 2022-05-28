//
// Author: Yulan Zhou
//

#ifndef MEMORY_ALLOCATOR_AIRPLANE_H
#define MEMORY_ALLOCATOR_AIRPLANE_H

#include <iostream>

class Airplane {
public:
    unsigned long getMiles() const { return rep.miles; }

    char getType() const { return rep.type; }

    void set(unsigned long m, char t) {
        rep.miles = m;
        rep.type = t;
    }

    void *operator new(size_t size);

    void operator delete(void *deadObject, size_t size);

private:
    // 数据
    struct AirplaneRep {
        unsigned long miles; // 4 字节
        char type;           // 1 字节
    };

    // 嵌入式指针
    union {
        AirplaneRep rep; // 针对使用中的 objects
        Airplane *next;  // 针对 free list 上的 object
    };
    static const int BLOCK_SIZE;     // 内存池大小：512 元素
    static Airplane *headOfFreeList;
};


#endif //MEMORY_ALLOCATOR_AIRPLANE_H
