#include <Debug.h>

template<class C, class BT>
bool
CompactTrieSet<C,BT>::find(const char_type* key, size_type len)
{
  bool ret =  CompactTrieBase<C,char,BT>::find(key, len);
  CompactTrieBase<C,char,BT>::factory->clearTail();
  return ret;
}

template<class C, class BT>
void
CompactTrieSet<C,BT>::insert(const char_type* key, size_type len)
{
  CompactTrieBase<C,char,BT>::insert(key, len, 1); 
  CompactTrieBase<C,char,BT>::factory->clearTail();
}

template<class C, class BT>
void
CompactTrieSet<C,BT>::erase(const char_type* key, size_type len)
{
  CompactTrieBase<C,char,BT>::erase(key, len); 
  CompactTrieBase<C,char,BT>::factory->clearTail();
}

template<class C, class BT>
typename CompactTrieSet<C,BT>::iterator
CompactTrieSet<C,BT>::list(const char_type* key, size_type len)
{
  CompactTrieBase<C,char,BT>::list(key, len); 
  CompactTrieBase<C,char,BT>::factory->clearTail();
}

template<class C, class BT>
typename CompactTrieSet<C,BT>::iterator
CompactTrieSet<C,BT>::begin()
{
  return CompactTrieBase<C,char,BT>::begin();
}

template<class C, class BT>
typename CompactTrieSet<C,BT>::iterator
CompactTrieSet<C,BT>::end()
{
  return CompactTrieBase<C,char,BT>::end();
}
