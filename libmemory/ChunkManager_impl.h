#include <ChunkManager.h>
#include <ChunkStorage.h>

#include <algorithm>

template<class P>
class ChunkManager<P>::Data {
  private:
    Data() {};
  public:
    Data(size_t max_chunk_size_) {
      assert(max_chunk_size_ > 1);
      max_chunk_size = max_chunk_size_;
      asm_bsr(bits, max_chunk_size_);
      assert((1 << bits) == max_chunk_size_);
      storage = new storage_type*[max_chunk_size];
      for (size_t i = 0; i < max_chunk_size; ++i) {
        storage[i] = new storage_type(std::max(sizeof(internal_pointer), i+1));
      }
    }
    ~Data() {
      for (size_t i = 0; i < max_chunk_size; ++i) {
        delete storage[i];
      }
      delete[] storage;
    }
    
    typedef ChunkStorage<internal_pointer> storage_type;
    size_t max_chunk_size;
    size_t bits;
    storage_type** storage;
};

template<class P>
ChunkManager<P>::ChunkManager(size_t max_chunk_size) : data(max_chunk_size) 
{
  assert(data.bits > 0);
}

template<class P>
void* 
ChunkManager<P>::global_address(pointer x) const
{
  size_t hint_size = x & ((1 << data.bits) - 1);
  assert(hint_size < data.max_chunk_size);
  assert(hint_size >= 0);
  return data.storage[hint_size]->global_address(x >> data.bits);
}


template<class P>
typename ChunkManager<P>::pointer 
ChunkManager<P>::allocate(size_t size)
{
  assert(size > 0);
  assert(size <= data.max_chunk_size);
  pointer p = data.storage[size-1]->allocate();
  return (p <<  data.bits) | (size-1) ;
}

template<class P>
void
ChunkManager<P>::deallocate(pointer p)
{
  data.storage[p & ((1 << data.bits)-1)]->deallocate(p >> data.bits);
}

template<class P>
size_t 
ChunkManager<P>::get_max_chunk_size() const
{
  return data.max_chunk_size;
}

template<class P>
bool
ChunkManager<P>::isNULL(pointer p)
{
  assert(data.bits > 0);
  return (p >> data.bits) == 0;
}

template<class P>
typename ChunkManager<P>::pointer 
ChunkManager<P>::null() 
{
  return 0x0; 
}


template<class P>
size_t
ChunkManager<P>::size() const
{
  size_t s = 0;
  for (size_t i = 0; i < data.max_chunk_size; ++i) {
    s += (i+1)*data.storage[i]->size();
  }
  return s;
}

template<class P>
size_t
ChunkManager<P>::getChunkSize(pointer p) const
{
  return p & ((1 << data.bits)-1);
}

template<class P>
size_t
ChunkManager<P>::unused_size() const
{
  size_t s = 0;
  for (size_t i = 0; i < data.max_chunk_size; ++i) {
    s += (i+1)*data.storage[i]->unused_size();
  }
  return s;
}
