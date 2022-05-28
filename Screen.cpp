//
// Author: Yulan Zhou
//

#include "Screen.h"

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
