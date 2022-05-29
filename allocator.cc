//
// Author: Yulan Zhou
//

#include "allocator.h"

namespace my_allocator {
    void *allocator::allocate(size_t size) {
        obj *p;
        if (!freeStore) {
            size_t chunk = CHUNK * size;
            freeStore = p = static_cast<obj *>(malloc(chunk));
            // 内存切片
            for (int i = 0; i < (CHUNK - 1); ++i) {
                p->next = (obj *) ((char *) p + size);
                p = p->next;
            }
            p->next = nullptr; // 结束 list
        }
        p = freeStore;
        freeStore = freeStore->next;
        return p;
    }

    void allocator::deallocate(void *p, size_t) {
        // 将收回的指针插入到头部
        ((obj *) p)->next = freeStore;
        freeStore = (obj *) p;
    }
}
