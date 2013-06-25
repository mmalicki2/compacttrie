#include <detail/CompactTrie_fastnode.h>
#include <cstring>

template<class C, class V, class BT>
bool 
CompactTrieBase<C,V,BT>::find(const char_type* key, size_type len)
{
  if (head == NULL) {
    return false;
  }
  if (head->getCharsLength() > len) {
    return false;
  }
  FastNode<node_type> node(head); 
  size_type posBegin = 0;
  size_type posEnd = node.getCharsLength();
  do { 
    if (!asm_memcmp(&key[posBegin], node.getChars(), posEnd-posBegin)) {
      return false;
    }
    if (posEnd < len) {
      node.goNextNode(key[posEnd]);
      if (node.isEmpty()) {
        return false;
      }
      posBegin = posEnd;
      posEnd += node.getCharsLength();
    } else {
      assert (posEnd == len);
      return node.hasData();
    }
  } while (posEnd <= len);
  
  return false;
}

