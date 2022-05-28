//
// Author: Yulan Zhou
//

#include "Airplane.h"

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
