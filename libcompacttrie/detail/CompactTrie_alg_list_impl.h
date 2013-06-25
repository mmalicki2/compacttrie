template<class C, class V, class BT>
typename CompactTrieBase<C,V,BT>::iterator 
CompactTrieBase<C,V,BT>::list(const char_type* key, size_type len)
{
  if (head == NULL) {
    return iterator();
  }
  node_type* node = head; 
  size_type posBegin = 0;
  size_type posEnd = node->getCharsLength();
  do {
    posEnd = std::min(posEnd, len);
    const char_type* chars = node->getChars();
    for (int i = posBegin; i < posEnd; ++i) {
      if (key[i] != chars[i-posBegin]) {
        return iterator();
      }
    }
    if (posEnd < len) {
      node = node->getNextNode(key[posEnd]);
      if (node == NULL) {
        return iterator();
      }
      posBegin = posEnd;
      posEnd += node->getCharsLength();

    } else {
      assert (posEnd == len);
      return iterator(key, posBegin, node);
    }
  } while (posEnd <= len);
  
  return iterator();

}

