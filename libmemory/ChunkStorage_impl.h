#include <cassert>
#include <iterator>
#include <bit.h>
template<class _Index = size_t>
class ChunkStorage<_Index>::Data {
  public:
    Data(size_t item_size_) {
      item_size = item_size_;
      size = 0;
      mem[0] = new char[sizeof(pointer)];
      *reinterpret_cast<pointer*>(&mem[0][0]) = 0;
      for (int i = 1; i < sizeof(pointer)*8+1; ++i) {  
        mem[i] = NULL;
      }
    }
    ~Data() {
      for (int i = 0; i < sizeof(pointer)*8+1; ++i) {  
        delete[] mem[i];
      }
    }
    void clear() {
      size = 0;
      *reinterpret_cast<pointer*>(&mem[0][0]) = 0;
    }


    size_type size;
    size_t item_size;
    char* mem[sizeof(pointer)*8+1]; 
};

template<class _Index>
ChunkStorage<_Index>::ChunkStorage(size_t type_size) : data(type_size) 
{
  if (type_size < sizeof(pointer)) {
    abort();
  }
}

template<class _Index>
typename ChunkStorage<_Index>::size_type 
ChunkStorage<_Index>::size() const
{
  return data.size;
}

template<class _Index>
void* 
ChunkStorage<_Index>::global_address(pointer p)
{
  if (p == 0) {
    return NULL;
  }
  pointer bit;
  asm_bsr(bit, p);
  assert(bit < sizeof(data.mem));
  assert(bit > 0);
  assert(data.mem[bit] != NULL);
  asm_btr(p, bit);
  assert(data.item_size*p < data.item_size*(1<<bit));
  return reinterpret_cast<void*>(&data.mem[bit][data.item_size*p]); 
}

template<class _Index>
typename ChunkStorage<_Index>::pointer 
ChunkStorage<_Index>::address(void* x) const
{
  static_assert(sizeof(size_type) <= sizeof(void*), "size_type nie miesci void*");
  for (size_t i = 1; data.mem[i]; ++i) {
    size_type dis = (reinterpret_cast<size_type>(x) - reinterpret_cast<size_type>(data.mem[i]))/data.item_size;
    if (dis >= 0 && dis < 1 << i) {
      asm_bts(dis,i);
      return dis;
    }
  }
  abort();
}

template<class _Index>
typename ChunkStorage<_Index>::pointer
ChunkStorage<_Index>::allocate()
{
  pointer ret = *reinterpret_cast<pointer*>(&data.mem[0][0]);
  if (ret == 0) {
    ret = data.size+2; //zerowy bit dla zwolnionych
    pointer bit;
    asm_bsr(bit, ret);
    if (data.mem[bit] == NULL) {
      data.mem[bit] = new char[data.item_size*(1 << bit)];
    }
  } else {;
    *reinterpret_cast<pointer*>(&data.mem[0][0]) = *reinterpret_cast<pointer*>(global_address(ret));
  }

  data.size++;
  assert(ret > 1);
  return ret; 
}

template<class _Index>
void
ChunkStorage<_Index>::deallocate(pointer p)
{
  if (p == 0) return;
  *reinterpret_cast<pointer*>(global_address(p)) = *reinterpret_cast<pointer*>(&data.mem[0][0]);
  *reinterpret_cast<pointer*>(&data.mem[0][0]) = p;
  data.size--;
}

template<class _Index>
typename ChunkStorage<_Index>::size_type 
ChunkStorage<_Index>::max_size() const
{
  return data.type_size > 0 ? (1 << data.type_size*8) - 2 : 0;
}

template<class _Index>
void 
ChunkStorage<_Index>::clear()
{
  data.clear();
}

template<class _Index>
size_t 
ChunkStorage<_Index>::unused_size() const
{
  size_t s = 1;
  for (int i = 1; i < sizeof(pointer)*8+1; ++i) {  
    if (data.mem[i] != NULL) {
      s += (1 << i); 
    } else {
      break;
    }
  }
  return s - data.size;
}
