#ifndef COMPACTTREE_H_
#define COMPACTTREE_H_

#include <ChunkManager.h>

template<class _N>
class NodeFactory;

template <class Char, class Value, class ChunkManager, class BasePoinerType>
class Node;

template<class C, class V, class BT = size_t>
class  CompactTrieBase {
  public:
    class Citerator;
    typedef C                               char_type;
    typedef V                               value_type;
    typedef BT                              size_type;
    typedef ChunkManager<size_t>            chunk_manager_type;
    typedef Node<C,V,chunk_manager_type,BT> node_type;
    typedef std::basic_string<char_type>    key_type;
    typedef Citerator                       iterator;

    node_type* head;
    NodeFactory<node_type>* factory;
    
    CompactTrieBase();
    bool find(const char_type* key, size_type len);
    void insert(const char_type* key, size_type len, value_type value);
    void erase(const char_type* key, size_type len);
    iterator list(const char_type* prefix, size_type len);
    iterator begin();
    iterator end();
  
  private:
    void merge(node_type* node);
    void insert_inner(const char_type* key, size_type len, value_type value, node_type* node, size_type posBegin, size_type posBranch);
    void insert_char_branch(const char_type* key, size_type len, value_type value, node_type* node, size_type pos);
    void insert_child(const char_type* key, size_type len, value_type value, node_type* parent_node, size_type pos);
    void storeNodes(node_type* node);
    void storeNodes(node_type* zero, node_type* one);
    
    template<class Ct, class Vt, class BTt>
    friend std::ostream& operator<<(std::ostream& out, const CompactTrieBase<Ct,Vt,BTt>&  _this);
};

#include <detail/CompactTrie_impl.h>

#endif /* COMPACTTREE_H_ */
