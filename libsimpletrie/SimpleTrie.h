#ifndef SIMPLETRIE_H_
#define SIMPLETRIE_H_
#include <string>
#include <map>
#include <utility>
#include <iostream>
#include <vector>
#include <list>
#include <cassert>
#include <type_traits>

template< class K >
class SimpleTrie {
    class Citerator;
  public:
    typedef typename K::value_type                       char_type;
    typedef std::basic_string<char_type>                 key_type;
    typedef Citerator                                    iterator;

    SimpleTrie() { };

    void insert(const key_type& key) {
      assert(!key.empty());
      Node* node = &head;
      for (size_t i = 0; i < key.size(); ++i) {
        typename Node::map_type::iterator it = node->next.find(key[i]);
        if (it == node->next.end()) {
          //dodajemy klucz
          node->next.insert(std::make_pair(key[i], make_leaf_node(key, i+1)));
          break;
        }
          
        if (i+1 == key.size()) {
          it->second.isEnd = true;
          break;
        }
        node = &it->second;
      }
    }

    bool find(const key_type& key) {
      const Node* node = &head;
      for (size_t i = 0; i < key.size(); ++i) {
        typename Node::map_type::const_iterator it = node->next.find(key[i]);
        if (it == node->next.end()) {
          return false;
        }
          
        if (i+1 == key.size() && it->second.isEnd) {
          return true;
        }
        node = &it->second;
      }
      return false;
    }

    void erase(const key_type& key) {
      if (head.next.empty()) {
        abort();
      }
      typename Node::map_type::iterator last_branch_it = head.next.begin();
      Node* last_branch_node = &head;
      Node* node = &head;
      for (size_t i = 0; i < key.size(); ++i) {
        typename Node::map_type::iterator it = node->next.find(key[i]);
        if (it == node->next.end()) {
          //abort(); //brak klucza
          return;
        }
        if (node->next.size() > 1 || node->isEnd) {
          last_branch_node = node;
          last_branch_it = it;
        }
          
        if (i+1 == key.size() && it->second.isEnd) {
          if (!it->second.next.empty()) {
            it->second.isEnd = false;
          } else {
            last_branch_node->next.erase(last_branch_it);
          }
          return;
        }
        node = &it->second;
      }
      //abort(); //brak klucza
      return;

    }

    iterator begin() { return iterator(head, ""); }
    iterator end() { return iterator(); }

  private:
    class Node {
      public:
      typedef std::map<typename std::make_unsigned<char_type>::type, Node> map_type; 
      map_type next;
      bool isEnd;
    };

    class Citerator {
      public:
        Citerator() {}
        Citerator(Node& head, const key_type& key) {
          assert(key.empty());
          if (!head.isEnd && head.next.empty()) {
            return;
          }

          Node* node = &head;
          while (!node->isEnd) {
            ss.push_back(node->next.begin()->first);
            path.push_back(make_pair(&node->next, node->next.begin()));
            node = &node->next.begin()->second;
          }
        }
        Citerator(const Citerator& _this) {
          assert(false);
        }

        const std::string operator*() {
          return std::string(ss.data(), ss.size());
        }

        Citerator& operator++() {
          if (path.empty()) abort();

          if (path.back().second->second.next.empty()) {
            while (++path.back().second == path.back().first->end()) {
              ss.pop_back();
              path.pop_back();
              if (path.empty()) return *this;
            }
            ss.back() = path.back().second->first;
            if (path.back().second->second.isEnd) return *this;
          }
          do {
            ss.push_back(path.back().second->second.next.begin()->first);
            path.push_back(make_pair(&path.back().second->second.next, path.back().second->second.next.begin()));
          } while (!path.back().second->second.isEnd);
          return *this;
        }

        friend bool operator !=( const Citerator& it1, const Citerator& it2) {
          if (it1.path.size() != it2.path.size()) {
            return true;
          }
          auto i2 = it2.path.begin();
          for (auto i1 = it1.path.begin(); i1 != it1.path.end(); ++i1) {
              if (*i2 != *i1) {
                return true;
              }
              i2++;
          }
          return false;
        }
        friend bool operator ==( const Citerator& it1, const Citerator& it2) {
          return !(it1 != it2);
        }
      private:
        std::vector<char_type> ss;
        std::list< std::pair<typename Node::map_type*, typename Node::map_type::iterator> > path;
    };

    Node head;

    Node make_leaf_node(const key_type& key, size_t i) {
      Node node;
      if (i < key.size()) {
        node.isEnd = false;
        node.next.insert(std::make_pair(key[i], make_leaf_node(key, i+1)));
      } else {
        node.isEnd = true;
      }
      return node;
    }
};


#endif /* SIMPLETRIE_H_ */
