static const int DEFAULT_KEY_SIZE = 200;

template<class C, class V, class BT>
void
CompactTrieBase<C,V,BT>::Citerator::goToBeginNode(node_type* node)
{
  assert(data);
  size_t oldsize;
  if (node == NULL) {
    node = data->root;
  }
  while (!node->hasData()) {
    oldsize = data->key.size();
    data->key.resize(oldsize+node->getCharsLength());
    std::copy(node->getChars(), node->getChars()+node->getCharsLength(), data->key.data()+oldsize);
    if (node->getZero() != NULL) {
      node = node->getZero();
    } else if (node->getOne() != NULL) { 
      node = node->getOne();
    }
  }
  oldsize = data->key.size();
  data->key.resize(oldsize+node->getCharsLength());
  std::copy(node->getChars(), node->getChars()+node->getCharsLength(), data->key.data()+oldsize);
  data->node = node;
}

template<class C, class V, class BT>
CompactTrieBase<C,V,BT>::Citerator::Citerator(const char_type* key, size_type len, node_type* root) :data(NULL)
{
  assert(data==NULL);
  data = new data_type();
  NodeFactory<node_type>* factory = new NodeFactory<node_type>();
  data->root = factory->setHead(root);
  data->key.reserve(DEFAULT_KEY_SIZE);
  data->key.resize(len);
  std::copy(key, key+len, data->key.data());
  
  goToBeginNode();
}


template<class C, class V, class BT>
CompactTrieBase<C,V,BT>::Citerator::~Citerator() 
{ 
  if (data != NULL) {
    delete data->root->data.loc.factory;
    delete data; 
  }
}

template<class C, class V, class BT>
typename CompactTrieBase<C,V,BT>::size_type
CompactTrieBase<C,V,BT>::Citerator::getKeyLength()
{
  assert(data);
  return data->key.size();
}

template<class C, class V, class BT>
typename CompactTrieBase<C,V,BT>::char_type*
CompactTrieBase<C,V,BT>::Citerator::getKey()
{
  assert(data);
  return data->key.data();
}

template<class C, class V, class BT>
typename CompactTrieBase<C,V,BT>::value_type
CompactTrieBase<C,V,BT>::Citerator::getValue()
{
  assert(data);
  return *data->node->getData();
}

template<class C, class V, class BT>
typename CompactTrieBase<C,V,BT>::Citerator&
CompactTrieBase<C,V,BT>::Citerator::operator++()
{
  assert(data);
  assert(data->node);
  assert(data->node->hasData());
  node_type* node = data->node;

  if (node->getZero() != NULL) {
    goToBeginNode(node->getZero());
    return *this;
  }
  if (node->getOne() != NULL) {
    goToBeginNode(node->getOne());
    return *this;
  }
  if (node == data->root) {
    delete data;
    data = NULL;
    return *this;
  }

  while ((node->getParent()->getOne() == NULL) || (node->getParent()->getOne() == node)) {
    assert(data->key.size() - node->getCharsLength() >= 0);
    data->key.resize(data->key.size() - node->getCharsLength());
    node_type* nnode = node->getParent();
    data->root->data.loc.factory->destroyNode(node);
    node = nnode;
    if (node == data->root) {
      delete data;
      data = NULL;
      return *this;
    }
  }
  assert(data->key.size() - node->getCharsLength() >= 0);
  data->key.resize(data->key.size() - node->getCharsLength());
  node_type* nnode = node->getParent();
  data->root->data.loc.factory->destroyNode(node);
  node = nnode;
  goToBeginNode(node->getOne());   

  return *this;
}

