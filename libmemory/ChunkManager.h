#ifndef CHUNKMANAGER_H_
#define CHUNKMANAGER_H_

#include <cstring>
#include <cstdint>

const static size_t global_min_global_chunk_size = 4*sizeof(void*)+4*sizeof(size_t);

//P = okresla ilosc mozliwych kawałków i wilkosc najmniejszego kawałka
template<class P = size_t>
class ChunkManager {
  public:
    typedef size_t pointer;

    //static_assert<sizeof(size_t) >= sizeof(void*)>;
    
    ChunkManager(size_t max_chunk_size = global_min_global_chunk_size);
    void* global_address(pointer x) const;
    pointer allocate(size_t size);
    void deallocate(pointer p);
    size_t get_max_chunk_size() const;

    pointer null();
    bool isNULL(pointer p);
    size_t size() const;
    size_t getChunkSize(pointer p) const;

    size_t unused_size() const;

  private: 
    typedef P internal_pointer;
    class Data;

    Data data;
};

#include <ChunkManager_impl.h>


#endif /* CHUNKMANAGER_H_ */
