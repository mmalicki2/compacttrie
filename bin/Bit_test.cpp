#include <iostream>

#include <bit.h>

#include <VInt.h>
using namespace std;

template<class T>
void asm_bt_test(T v) {
  for (char bit = 0; bit < sizeof(T)*8; ++bit) { 
    int ret = asm_bt(v, bit);
    cerr << "asm_bt(" << dec << (size_t)v << ", " << (int)bit << "): " << hex<<ret << endl;
  }
}

template<class T>
void asm_bsr_test(T v) {
  T ret;
  asm_bsr<T>(ret, v);
  cerr << "asm_bsr(" << dec <<(size_t)v << "): " << hex <<ret << endl;
}

template<class T>
void asm_bsf_test(T v) {
  T ret;
  asm_bsf<T>(ret, v);
  cerr << "asm_bsf(" << dec <<(size_t)v << "): " << hex <<ret << endl;
}

template<class T>
void asm_bts_test(T bit) {
  T v = 0;
  asm_bts(v, bit);
  cerr << "asm_bts(" << dec<< (size_t)bit << "): " << v << endl;
}

template<class T>
void asm_btr_test(T bit) {
  T v = -1;
  asm_btr(v, bit);
  cerr << "asm_btr(" << dec<< (size_t)bit << "): " << v << endl;
}

void asm_memcmp_test() {
  char s1[] = "blabla";
  char s2[] = "blafla";
  int ret = asm_memcmp(s1, s2, 6);
  cerr << ret << endl;
}

void Vint_test() {
  typedef VInt<size_t> vint;
  size_t mem[] = {(size_t)-1, (size_t)-1, (size_t)-1, (size_t)-1};
  
  typename vint::value_type v;
  typename vint::value_type v2;
  v = vint::fromInt(9);
  vint::toMem((char*)mem, v);
  cerr << hex<< mem[0] << " " << mem[1] << " " << mem[2] << " " << mem[3] << endl;
  v2 = vint::fromMem((char*)mem);
  cerr << ((v2 == v) ? "vint ok " : "vint err ") << v  << " - " << v2 << endl;
 }

int main(int argc, const char** argv) 
{
  asm_bt_test<char>(0x01);
  asm_bt_test<char>(0x02);
  asm_bt_test<char>(0x03);
  asm_bt_test<size_t>(0x8000000000);
  cerr << endl;
  asm_bsr_test<size_t>(0x01);
  asm_bsr_test<size_t>(0x04);
  asm_bsr_test<size_t>(0x05);
  asm_bsr_test<size_t>(0x8000000000);
  cerr << endl;
  asm_bsf_test<size_t>(0x01);
  asm_bsf_test<size_t>(0x04);
  asm_bsf_test<size_t>(0x05);

  cerr << endl;
  asm_bts_test<size_t>(0x00);
  asm_bts_test<size_t>(0x01);
  asm_bts_test<size_t>(39);
  
  cerr << endl;
  asm_btr_test<size_t>(0x00);
  asm_btr_test<size_t>(0x01);
  asm_btr_test<size_t>(39);

  asm_memcmp_test();
  Vint_test();
}

