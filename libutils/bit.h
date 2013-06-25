#ifndef BIT_H
#define BIT_H

#include <cstring>

template<class T>
inline void asm_bsr(T& res_bit, const T value) {
  asm (                 
    "bsr %[value],%[res_bit]"         
    : [res_bit] "=r" (res_bit)     
    : [value] "r" (value) 
    : "cc"
    );
}

template<class T>
inline void asm_bsf(T& res_bit, const T value) {
  asm (                 
    "bsf %[value],%[res_bit]"         
    : [res_bit] "=r" (res_bit)     
    : [value] "r" (value)       
    : "cc"
    );
}

#define asm_btr(value, bit) \
  asm (                 \
    "btr %1,%0"         \
    : "+r" (value)      \
    : "r" (bit)         \
    : "cc"              \
    )

#define asm_bts(value, bit) \
  asm (                 \
    "bts %1,%0"         \
    : "+r" (value)      \
    : "r" (bit)         \
    : "cc"              \
    )

inline void asm_bt(char& res, const size_t value, const size_t bit) {
  asm (                       
    "xor %[res],%[res] \n"               
    "bt %[bit],%[value] \n"        
    "setc %[res]"                 
    : [res] "+r" (res)              
    : [value] "r" (value), [bit] "r" (bit)  
    : "cc"
    );
}

inline char asm_bt(const size_t value, const size_t bit) {
  char res;
  asm_bt(res, value, bit);
  return res;
}

inline bool asm_memcmp(const void* src, const void* dest, size_t count) {
  if (count >= sizeof(size_t)) {
    return memcmp(src, dest, count) == 0;
  } else {
    const size_t mask = (((size_t)1) << (count*8)) - 1;
    return ( (*((size_t*)src) ^ *((size_t*)dest)) & mask) == 0;  
  }
}

#endif /* BIT_H */
