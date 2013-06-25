#include <detail/CompactTrie_alg_find_impl.h>
#include <detail/CompactTrie_alg_insert_impl.h>
#include <detail/CompactTrie_alg_erase_impl.h>
#include <detail/CompactTrie_alg_list_impl.h>

#include <detail/CompactTrie_nodefactory.h>
#include <detail/CompactTrie_node.h>
#include <detail/CompactTrie_iterator.h>

#include <algorithm>

template<class C, class V, class BT>
CompactTrieBase<C,V,BT>::CompactTrieBase() 
{
  factory = new NodeFactory<node_type>();
  head = NULL;
}

template<class C, class V, class BT>
typename CompactTrieBase<C,V,BT>::iterator 
CompactTrieBase<C,V,BT>::begin()
{
  return list("", 0);
}

template<class C, class V, class BT>
typename CompactTrieBase<C,V,BT>::iterator 
CompactTrieBase<C,V,BT>::end()
{
  return iterator();
}

template<class C, class V, class BT>
void 
CompactTrieBase<C,V,BT>::storeNodes(node_type* node)
{
  assert(node != NULL);
  while (node != NULL && node->isDirty()) {
    node->store();
    if (node->getParent() == NULL) {
      if (node != head) {
        head = factory->setHead(node);
      }
      break;
    }
    node = node->getParent();
  }
}

template<class C, class V, class BT>
void 
CompactTrieBase<C,V,BT>::storeNodes(node_type* n1, node_type* n2)
{
  assert(n1 != NULL && n2 != NULL);
  assert(n1->getParent() == n2->getParent());
  assert(n1->getParent() != NULL);
  n1->store();
  n2->store();
  storeNodes(n1->getParent());
}

template<class C, class V, class BT>
std::ostream&  
operator<<(std::ostream& out, const CompactTrieBase<C,V,BT>& _this)
{
  //out << "head: " << SShead_size << std::endl;
  out << "npt_global: " <<  SSglobal << std::endl;
  //out << "nodes count: " << _this.factory->mem->size() << std::endl;
  //out << "npt_chunk :" << _this.factory->cm.size() << std::endl;
  
  if (_this.head != NULL) {
    out << *_this.head  << std::endl;
  } else {
    out << "NULL" << std::endl;
  }
  return out;
}
