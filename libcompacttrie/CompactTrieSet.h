#ifndef COMPACTTRIESET_H_
#define COMPACTTRIESET_H_

#include <string>
#include <ostream>

#include <detail/CompactTrie.h>

template<class C, class BT = size_t>
class  CompactTrieSet : protected CompactTrieBase<C,char,BT> { 
  public:
    typedef C                                               char_type;
    typedef std::basic_string<char_type>                    key_type;
    typedef BT                                              size_type;
    typedef CompactTrieBase<C,char,BT>                      base_type;
    typedef typename CompactTrieBase<C,char,BT>::Citerator  iterator;

    bool find(const char_type* key, size_type len);
    bool find(const std::basic_string<char_type> key) {
      return find(key.data(), key.length());
    }
    void insert(const char_type* key, size_type len);
    void insert(const std::basic_string<char_type>& key) {
      insert(key.data(), key.length());
    }
    void erase(const char_type* key, size_type len);
    void erase(const std::basic_string<char_type>& key) {
      erase(key.data(), key.length());
    }
    
    iterator list(const char_type* prefix, size_type len);
    iterator list(const std::basic_string<char_type>& prefix) {
      return list(prefix.data(), prefix.length());
    }
    
    iterator begin();
    iterator end();
    
    template<class Ct, class BTt>
    friend std::ostream& operator<<(std::ostream& out, const CompactTrieSet<Ct,BTt>&  _this) {
      return out << static_cast<base_type>(_this);
    }
};

#include <CompactTrieSet_impl.h>

#endif /* COMPACTTRIESET_H__ */
