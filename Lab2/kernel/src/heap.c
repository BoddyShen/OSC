#include "heap.h"

extern char _heap_top;
extern char _heap_bottom;
static char *heap_top_ptr = &_heap_top;

void *simple_malloc(size_t size)
{
    void *ret = heap_top_ptr;

    // allocate memory size in 8-byte aligned chunks
    size = (size + 7) & ~7;

    if (heap_top_ptr + size > &_heap_bottom) {
        return NULL;
    }
    heap_top_ptr += size;
    return ret;
}
