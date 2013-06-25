template<class _N>    
typename NodeFactory<_N>::node_type* 
NodeFactory<_N>::createHead()
{
  head = new node_type(&cm);
  head->data.loc.factory = this;
  return head;
}
   
template<class _N>
typename NodeFactory<_N>::node_type* 
NodeFactory<_N>::setHead(node_type* node)
{
  assert(node != NULL);
  if (head != node) {
    node_type* newHead = new node_type(&cm); 
    *newHead = *node;
    newHead->data.loc.factory = this;
    if (head != NULL) {
      clearTail();
      delete head;
    }
    head = newHead;
    head->data.loc.parent = NULL;
    head->data.loc.one = NULL;
    head->data.loc.zero = NULL;
  } else {
    clearTail();
  }
  return head;
}

template<class _N>
NodeFactory<_N>::NodeFactory()
{
  head = NULL;
  mem = new mem_type(sizeof(node_type));
};

template<class _N>
NodeFactory<_N>::~NodeFactory() 
{
  delete head;
  delete mem;
};

template<class _N>    
typename NodeFactory<_N>::node_type* 
NodeFactory<_N>::createNode()
{
  void* ptr = mem->global_address(mem->allocate());
  node_type* n = new(ptr) node_type(&cm);
  n->data.loc.factory = this;
  return n;
}

template<class _N>    
typename NodeFactory<_N>::node_type* 
NodeFactory<_N>::createNode(node_type* parent, bool isOne, typename node_type::chunk_manager_type::pointer pointer)
{
  void* ptr = mem->global_address(mem->allocate());
  node_type* n = new(ptr) node_type(parent, isOne, pointer);
  n->data.loc.factory = this;
  return n;
}

template<class _N>    
typename NodeFactory<_N>::node_type* 
NodeFactory<_N>::createNode(node_type* parent, bool isOne, void* pointer)
{
  void* ptr = mem->global_address(mem->allocate());
  node_type* n = new(ptr) node_type(parent, isOne, pointer);
  n->data.loc.factory = this;
  return n;
}

template<class _N>    
void
NodeFactory<_N>::destroyNode(node_type* node)
{
  mem->deallocate(mem->address(reinterpret_cast<void*>(node)));
}

template<class _N>
void 
NodeFactory<_N>::clearTail()
{
  head->data.loc.one = NULL;
  head->data.loc.zero = NULL;
  mem->clear();
}

