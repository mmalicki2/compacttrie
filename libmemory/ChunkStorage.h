#ifndef CHUNKSTORAGE_H
#define CHUNKSTORAGE_H

#include <cstdint>
#include <cstring>

template<class _Index = uint64_t>
class ChunkStorage {
  public:
    typedef _Index    pointer;
    typedef size_t    size_type;

    ChunkStorage(size_t type_size);
    
    size_type size() const;
    void* global_address(pointer p);
    pointer address(void *x) const;
    pointer allocate();
    void deallocate(pointer p);
    size_type max_size() const;
    void clear();
    size_t unused_size() const;
  private: 
    class Data;

    Data data;
};

#include <ChunkStorage_impl.h>

#endif /* CHUNKSTORAGE_H */
