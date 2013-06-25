#ifndef COMPACTTRIE_FASTNODE_H_
#define COMPACTTRIE_FASTNODE_H_

#include <detail/CompactTrie_node.h>

template<class _N>
class FastNode {
  public:
    typedef _N                                         node_type;
    typedef typename node_type::char_type              char_type;
    typedef typename node_type::size_type              size_type;
    typedef typename node_type::value_type             value_type;
    typedef typename node_type::chunk_manager_type     chunk_manager_type;

    FastNode(node_type* head);
    char_type* getChars() const { return _chars; }
    size_type getCharsLength() const { return _charsSize / (sizeof(char_type)*8); }
    void goNextNode(char_type c);
    bool hasData() const { return _hasData;}
    bool isEmpty() const { return _empty; }
  private:
    void bind(char* ptr);

    chunk_manager_type* _cm;
    char* _one;
    char* _zero;
    char_type* _chars;
    bool _hasData;
    bool _empty;
    size_type _charsSize;
};


#include <detail/CompactTrie_fastnode_impl.h>

#endif /* COMPACTTRIE_FASTNODE_H_ */
