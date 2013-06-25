template<class C, class V, class BT>
void 
CompactTrieBase<C,V,BT>::insert_inner(const char_type* key, size_type len, value_type value, node_type* node, size_type posBegin, size_type posBranch)
{
  node_type* parent = factory->createNode();
  node_type* zero = factory->createNode();
  node_type* one = factory->createNode(); 
  one->setChars(&key[posBranch], len-posBranch);
  one->setData(&value);
  zero->setSubtree(node);
  zero->setChars(&node->getChars()[posBranch-posBegin], node->getCharsLength()-(posBranch-posBegin), node->getBranchBit());
  size_type bitBranch;
  size_type c = (typename std::make_unsigned<char_type>::type)node->getChars()[posBranch-posBegin] ^ (typename std::make_unsigned<char_type>::type)key[posBranch]; //XXX:
  asm_bsr(bitBranch, c);
  if (! asm_bt(key[posBranch], bitBranch)) {
    std::swap(zero, one);
  }
  parent->replace(node);
  assert(posBranch-posBegin);
  parent->setChars(&key[posBegin], posBranch-posBegin, bitBranch);
  parent->setZero(zero);
  parent->setOne(one);
  if (node == head) {
    storeNodes(zero, one);
  } else {
    storeNodes(zero, one);
  }
}

template<class C, class V, class BT>
void 
CompactTrieBase<C,V,BT>::insert_char_branch(const char_type* key, size_type len, value_type value, node_type* node, size_type pos)
{
  size_type bitBranch;
  size_type c = (typename std::make_unsigned<char_type>::type) key[pos] ^ (typename std::make_unsigned<char_type>::type)node->getChars()[0];
  asm_bsr(bitBranch, c);
  assert(node->getCharsLength() != 0);
  
  node_type* dataNode = factory->createNode();
  dataNode->setChars(&key[pos], len-pos);
  dataNode->setData(&value);
  if (node->getParent() == NULL) {
    assert(pos == 0);
    node_type* newNode = factory->createNode();
    newNode->setBranchBit(bitBranch);
    if (asm_bt(key[pos], newNode->getBranchBit())) {
      newNode->setOne(dataNode);
      newNode->setZero(node);
    } else {
      newNode->setZero(dataNode);
      newNode->setOne(node);
    }
    storeNodes(dataNode); //XXX: 
    return;
  }

  
  while (node->getParent() != NULL && node->getParent()->getBranchBit() < bitBranch && node->getParent()->getCharsLength() == 0) {
    node = node->getParent();
    assert(node->getCharsLength() == 0);

  };
  node_type* parent = node->getParent();
  if (parent == NULL) {
    assert(pos == 0);
    node_type* newNode = factory->createNode();
    newNode->setBranchBit(bitBranch);
    if (asm_bt(key[pos], newNode->getBranchBit())) {
      newNode->setOne(dataNode);
      newNode->setZero(node);
    } else {
      newNode->setOne(node);
      newNode->setZero(dataNode);
    }
    storeNodes(dataNode);
  } else if (parent->getBranchBit() > bitBranch) {
    assert(node->getBranchBit() < bitBranch || node->getCharsLength() != 0);
    node_type* newNode = factory->createNode();
    newNode->setBranchBit(bitBranch);
    if (asm_bt(key[pos], newNode->getBranchBit())) {
      newNode->setOne(dataNode);
      newNode->setZero(node);
    } else {
      newNode->setOne(node);
      newNode->setZero(dataNode);
    }
    if (asm_bt(key[pos], parent->getBranchBit())) {
      parent->setOne(newNode);
    } else {
      parent->setZero(newNode);
    }
    storeNodes(dataNode);
  } else if (parent->getBranchBit() < bitBranch) {
    assert(node->getBranchBit() < bitBranch || node->getCharsLength() != 0);
    //XXX: jesli sasiad noda jest NULL to wystrczy przestawić branch bit, a w ten NULL wstawić nowe dane. 
    //  Fix: Zadbac o to by gdy sasiad jest NULL to ma on maksymalny branch bit -- nigdy nie wykona sie ponizszy kod
    //CDBG <<"\n" << parent << "->" << node << " " << parent->getBranchBit() << " " << node->getBranchBit() << " " << bitBranch  << std::endl;
    node_type* newNode = factory->createNode();
    newNode->setBranchBit(parent->getBranchBit());
    newNode->setZero(parent->getZero());
    newNode->setOne(parent->getOne());
    parent->setBranchBit(bitBranch);
    if (asm_bt(key[pos], parent->getBranchBit())) {
      parent->setOne(dataNode);
      parent->setZero(newNode);
    } else {
      parent->setZero(dataNode);
      parent->setOne(newNode);
    }
    storeNodes(newNode, dataNode);
  } else {
    assert(node->getBranchBit() < bitBranch || node->getCharsLength() != 0);
    if (asm_bt(key[pos], parent->getBranchBit())) {
      assert(parent->getOne() == NULL);
      parent->setOne(dataNode);
    } else {
      assert(parent->getZero() == NULL);
      parent->setZero(dataNode);
    }
    storeNodes(dataNode);
  }
}

template<class C, class V, class BT>
void
CompactTrieBase<C,V,BT>::insert_child(const char_type* key, size_type len, value_type value, node_type* parent_node, size_type pos)
{
  node_type* dataNode = factory->createNode();
  dataNode->setChars(&key[pos], len-pos);
  dataNode->setData(&value);
  if (asm_bt(key[pos], parent_node->getBranchBit())) {
    assert(parent_node->getOne() == NULL);
    parent_node->setOne(dataNode);
  } else {
    assert(parent_node->getZero() == NULL);
    parent_node->setZero(dataNode);
  }
  storeNodes(dataNode);
}

template<class C, class V, class BT>
void 
CompactTrieBase<C,V,BT>::insert(const char_type* key, size_type len, value_type value)
{
  if (head == NULL) {
    head = factory->createHead();
    head->setChars(key, len);
    head->setData(&value);
    head->store();
    return;
  }
  
  node_type* node = head; 
  size_type posBegin = 0;
  size_type posEnd = node->getCharsLength();
  while (posEnd <= len) {
    const char_type* chars = node->getChars();
    if (posBegin < posEnd) {
      if (key[posBegin] == chars[0]) {
        for (int i = posBegin+1; i < posEnd; ++i) {
          if (key[i] != chars[i-posBegin]) {
            insert_inner(key, len, value, node, posBegin, i);
            return;
          }
        }
      } else {
        insert_char_branch(key, len, value, node, posBegin);
        return;
      }
    }
    if (posEnd < len) {
      node_type* nnode = node->getNextNode(key[posEnd]);
      if (nnode == NULL) {
        nnode = node->getNextSiblingNode(key[posEnd]);
        if (nnode == NULL) {
          assert(node->getCharsLength() != 0);
          insert_child(key, len, value, node, posEnd);
          return;
        }
        if (nnode->getCharsLength() == 0) {
          nnode = nnode->getNextCharNode();
        }
        insert_char_branch(key, len, value, nnode, posEnd);
        return;
      }
      node = nnode;
      posBegin = posEnd;
      posEnd += node->getCharsLength();
    } else {
      assert (posEnd == len);
      node->setData(&value);
      storeNodes(node);
      return;
    }
  }
  
  assert(len < posEnd);
  const char_type* chars = node->getChars();
  if (key[posBegin] == chars[0]) {
    for (int i = posBegin+1; i < len; ++i) {
      if (key[i] != chars[i-posBegin]) {
        insert_inner(key, len, value, node, posBegin, i);
        return;
      }
    }
  } else {
    insert_char_branch(key, len, value, node, posBegin);
    return;
  }

  node_type* parent = factory->createNode();
  node_type* child = factory->createNode();
  parent->replace(node);
  parent->setData(&value);
  parent->setChars(&key[posBegin], len-posBegin);
  child->setSubtree(node);
  child->setChars(&chars[len-posBegin], posEnd-len, node->getBranchBit());
  if (asm_bt(chars[len-posBegin], parent->getBranchBit())) {
    assert(parent->getZero() == NULL);
    parent->setOne(child);
  } else {
    assert(parent->getOne() == NULL);
    parent->setZero(child);
  }
  storeNodes(child);
}

