#ifndef COMPACTTRIE_NODEFACTORY_H_
#define COMPACTTRIE_NODEFACTORY_H_

#include <ChunkStorage.h>

template<class _N>
class NodeFactory {
  public:
    typedef _N              node_type;
    typedef NodeFactory<_N> this_type;
    
    NodeFactory();
    ~NodeFactory();
    
    node_type* createHead();
    node_type* setHead(node_type* node);
    node_type* createNode();
    node_type* createNode(node_type* parent, bool isOne, typename node_type::chunk_manager_type::pointer pointer);
    node_type* createNode(node_type* parent, bool isOne, void* pointer);
    void destroyNode(node_type* node);
  
    void clearTail();
  
  private: 
    typedef ChunkStorage<size_t> mem_type;

    typename node_type::chunk_manager_type cm; 
    node_type* head;
    mem_type* mem;
};

#include <detail/CompactTrie_nodefactory_impl.h>

#endif /* COMPACTTRIE_NODEFACTORY_H_ */
