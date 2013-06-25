#ifndef COMPACTTRIE_ITERATOR_H_
#define COMPACTTRIE_ITERATOR_H_

#include <vector>

template<class C, class V, class BT = size_t>
class  CompactTrieBase<C,V,BT>::Citerator {
  public:
    
    Citerator() : data(NULL) {};
    ~Citerator(); 
    Citerator(const char_type* key, size_type len, node_type* root);
    
    size_type getKeyLength();
    char_type* getKey();
    value_type getValue();
    Citerator& operator++();
    key_type operator*() {
      return key_type(getKey(), getKeyLength());
    }
    friend bool operator!=(const Citerator& v1, const Citerator&v2) {
      return v1.data != v2.data;
    }
    friend bool operator==(const Citerator& v1, const Citerator&v2) {
      return v1.data == v2.data;
    }

  private:
    struct data_type {
      node_type* root;
      node_type* node;
      std::vector<char_type> key;
    };
    data_type* data;
    
    void goToBeginNode(node_type* node = NULL);
};

#include <detail/CompactTrie_iterator_impl.h>

#endif /* COMPACTTRIE_ITERATOR_H_ */
