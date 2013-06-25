#ifndef VINT_H_
#define VINT_H_

#include <cassert>
#include <cmath>

#include <bit.h>
#include <iostream>

template<class T>
class VInt {
  public:
  typedef T value_type;
  typedef size_t size_type;

  static const int bitsize_count = std::ceil(std::log(sizeof(value_type)));
  static const value_type bitsize_mask = (1 << bitsize_count) - 1;

  static size_type size(value_type item)
  {
    return (item & bitsize_mask) + 1;
  }

  static value_type value(value_type item) 
  {
    return item >> bitsize_count;
  }

  static value_type fromInt(uint64_t value) 
  {
    if (value == 0) {
      return 0x00;
    } else {
      uint64_t bit;
      asm_bsr(bit, value);
      int size = (bit+1 + bitsize_count + 7) >> 3;
      assert(size <= sizeof(value_type));
      assert(size >= 1);
      return (value << bitsize_count) | (size-1);
    }
  }
#if 0
  static char* toMem(char* ptr, value_type item)
  {
    size_type s = size(item);
    for (size_type i = 0; i<s; ++i) {
      *((unsigned char*)(ptr+i)) = ((unsigned char)item) & 0xFF;
      item >>= 8;
    }
    return ptr+s;
  }

  static value_type fromMem(char* ptr)
  {
    size_type s =  ((size_type)((unsigned char)*ptr & bitsize_mask)) + 1;
    value_type ret = 0;
    for (size_type i = s; i > 0; --i) {
      ret = ret << 8;
      ret |= *((unsigned char*)ptr+i-1);
    }
    assert(size(ret) == s);
    return ret;
  }
#endif
#if 1
  static char* toMem(char* ptr, const value_type item)
  {
    size_type s = size(item);
    *((value_type*)ptr) &= ~(((value_type)1<<(8*s))-1);
    *((value_type*)ptr) |=  item;
    return ptr+s;
  }

  static value_type fromMem(char* ptr)
  {
    value_type m = *((value_type*)ptr);
    m &= ((value_type)1<<(8*((m & bitsize_mask)+1)))-1;
    return m;
  }
#endif
};

#endif /* VINT_H_ */
