#ifndef COMPACTTRIE_NODE_H_
#define COMPACTTRIE_NODE_H_

#include <VInt.h>

template<class _N>
class NodeFactory;

typedef enum {
  npt_global = 0,
  npt_chunk = 1,
} NodePointerType;

template <class Char, class Value, class ChunkManager, class BasePoinerType = size_t>
class Node {
  public:
    typedef Node<Char,Value,ChunkManager, BasePoinerType>  this_type;
    typedef VInt<BasePoinerType>                           value_type;
    typedef typename value_type::value_type                size_type;
    typedef Char                                           char_type;
    typedef Value                                          data_type;
    typedef ChunkManager                                   chunk_manager_type;

    Node(chunk_manager_type* cm);
    void store();
    bool hasData();
    NodePointerType getType();
    bool isDirty();
    size_type getCharsLength();
    size_type getBranchBit();
    this_type* getZero();
    this_type* getOne();
    this_type* getNextNode(char_type c);
    this_type* getNextSiblingNode(char_type c);
    this_type* getNextCharNode();
    this_type* setSubtree(this_type* node);
    const char_type* getChars();
    data_type* getData();
    this_type* getParent();
    void setParent(this_type* parent, bool isOne);
    void setBranchBit(size_type bit);
    void setZero(this_type* node);
    void setOne(this_type* node);
    void setData(data_type* data);
    void setChars(const char_type* chars, size_type size, size_type bitBranch);
    void setChars(const char_type* chars, size_type size);
    void eraseData();
    void eraseZero();
    void eraseOne();
    void replace(this_type* node);
    bool isConnectDirty();

    template <class C, class V, class CM, class B>
    friend std::ostream& operator<<(std::ostream& out, Node<C,V,CM, B>& _this);
    void printNode();
  public:
    class Data;
    
    Node(this_type* parent, bool isOne, typename chunk_manager_type::pointer pointer);
    Node(this_type* parent, bool isOne, void* pointer);
    Node(this_type* node);
    ~Node();

    void clear(chunk_manager_type* cm);
    void bind(void* chunk);
    void bind(Data& d, void* chunk);
    void store(void* chunk);
    void destroy();
    void disconnect();
    size_type need_chunk_size();
    size_type getNodeSize();
    size_type getFlags();
    void updateParent();

    Data data;
    
    friend class NodeFactory<this_type>;
};

#include <detail/CompactTrie_node_impl.h>

#endif /* COMPACTTRIE_NODE_H_ */
