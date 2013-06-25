template<class C, class V, class BT>
void 
CompactTrieBase<C,V,BT>::merge(node_type* node)
{
  assert(node);
  assert(node->getParent());
  node_type* newNode =  factory->createNode();
  node_type* mParent = node->getParent();
  char_type buf[node->getCharsLength() + mParent->getCharsLength()];
  std::copy(mParent->getChars(), mParent->getChars() + mParent->getCharsLength(), buf);
  std::copy(node->getChars(), node->getChars() + node->getCharsLength(), buf+mParent->getCharsLength());
  newNode->setSubtree(node);
  newNode->replace(mParent);
  assert(node->getCharsLength() + mParent->getCharsLength() == sizeof(buf)/sizeof(*buf));
  newNode->setChars(buf, node->getCharsLength() + mParent->getCharsLength(), node->getBranchBit());
  storeNodes(newNode);
  mParent->destroy();
  if (mParent->getParent() == NULL) {
    head = factory->setHead(newNode);
  }
}

template<class C, class V, class BT>
void 
CompactTrieBase<C,V,BT>::erase(const char_type* key, size_type len)
{
  if (head == NULL) {
    return;
  }
  if (head->getCharsLength() > len) {
    return;
  }
  node_type* node = head; 
  size_type posBegin = 0;
  size_type posEnd = node->getCharsLength();
  do {
    const char_type* chars = node->getChars();
    for (int i = posBegin; i < posEnd; ++i) {
      if (key[i] != chars[i-posBegin]) {
        return;
      }
    }
    if (posEnd < len) {
      node = node->getNextNode(key[posEnd]);
      if (node == NULL) {
        return;
      }
      posBegin = posEnd;
      posEnd += node->getCharsLength();
    } else {
      assert (posEnd == len);
      if (node->hasData()) {
        node->eraseData();
        while (node->getOne() == NULL && node->getZero() == NULL) {
          node->disconnect();
          node = node->getParent();
          if (node == NULL) break;
          if (node->hasData()) break;
        }
        if (node == NULL) {
          head = NULL;
          break;
        }
        if ((node->getOne() != NULL && node->getZero() == NULL) && !node->hasData()) {
            merge(node->getOne());
        } else if ((node->getOne() == NULL && node->getZero() != NULL) && !node->hasData()) {
            merge(node->getZero());
        } else {
          assert(node->hasData() || (node->getOne() != NULL && node->getZero() != NULL));
          storeNodes(node);
        }
        break;
      } else {
        break;
      }
    }
  } while (posEnd <= len);
}

