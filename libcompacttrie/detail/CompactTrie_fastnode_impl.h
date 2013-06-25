#include <bit.h>

template<class _N>
FastNode<_N>::FastNode(node_type* head)
{
  _empty = false;
  _cm = head->data.loc.cm;
  bind(head->data.loc.chunk_ptr);
}

template<class _N>
void 
FastNode<_N>::bind(char* ptr)
{
  assert(!_empty);
  if (ptr == NULL) {
    _empty = true;
    return;
  }
  typename value_type::value_type v = value_type::fromMem(ptr);
  typename value_type::value_type vv = value_type::value(v);
  _hasData = (vv & 0x4) != 0; 
  _charsSize = vv >> 3;
  ptr += value_type::size(v);
  if ((vv & 1) == 0) {
      _zero = *reinterpret_cast<char**>(ptr);
      ptr += sizeof(void*); 
  } else {
      typename value_type::value_type zv = value_type::fromMem(ptr);
      _zero = reinterpret_cast<char*>(_cm->global_address(value_type::value(zv)));
      ptr += value_type::size(zv); 
  }
  if ((vv & 2) == 0) {
      _one = *reinterpret_cast<char**>(ptr);
      ptr += sizeof(void*); 
  } else {
      typename value_type::value_type zv = value_type::fromMem(ptr);
      _one = reinterpret_cast<char*>(_cm->global_address(value_type::value(zv)));
      ptr += value_type::size(zv); 
  }
  _chars = reinterpret_cast<char_type*>(ptr);
}


template<class _N>
void
FastNode<_N>::goNextNode(char_type c)
{
  if (asm_bt(c, _charsSize & (sizeof(char_type)*8)-1)) {
    bind(_one);
  } else {
    bind(_zero);
  }
}
