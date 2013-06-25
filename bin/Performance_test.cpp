#include <CompactTrieSet.h>
#include <Resource.h>
#include <SimpleTrie.h>

#include <vector>
#include <unordered_set>

#include <unistd.h>
#include <locale>
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <algorithm>

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pool_allocator.h>

#include <iconv.h>

#include <cstring>
#include <cassert>


#define pbds_set(tag, _T) \
  __gnu_pbds::tree<_T, __gnu_pbds::null_type,std::less<_T>, __gnu_pbds::tag>

using namespace std;
using namespace __gnu_pbds;
using namespace __gnu_cxx;

const int mesure_count = 10;
uint64_t max_mem = 2000000000;

template <class T> struct mycmp : binary_function <T,T,bool> {
    bool operator() (const T& x, const T& y) const {
      for (int i = 0; i < min(y.size(),x.size()); ++i) {
        if (x[i] < y[i]) return true;
        if (y[i] < x[i]) return false;
      }
      return x.size() < y.size();
    }
};


template<class CharT>
class Measure {
public:
  typedef CharT                    char_type;
  typedef basic_string<char_type>  key_type;

  Measure(string id, string name = "") { 
    if (name.empty()) {
      name = id;
    }
    m_name = name;
    m_id = id;
    m_set = NULL;
  }

  void read(istream& in, size_t count = 0) {
    string line;
    size_t i = 0;
    while (getline(in, line)) {
      read(line);
      if (++i == count) break;
    }
    m_set = get_container(m_id, m_name);
  }

  size_t insert_test() {
    res.setName(m_set->getName() + " " + "insert " + to_string(data.size()));
    res.clear();
    size_t check_mem = 100000; 
    clog << res.getName();
    res.measure([&]() mutable {
	    for (size_t i = 0; i < data.size(); ++i) {
		    m_set->insert(data[i].data(), data[i].length());
        if (i % check_mem == 0) {
          if (res.getMem() > max_mem) {
            cerr << "mem: " << i << " " << res.getMem() << " > " << max_mem << endl;
            abort();
          }
          //clog << "." << flush;
        }

	    }
    });
#if 0
    CompactTrieSet<char_type>* s = &dynamic_cast< Set_impl< CompactTrieSet<char_type>  >* >(m_set)->m_set;
    if (s != NULL) {
      cerr << "\nsize: " << s->factory->cm.size() << ", unused_size: " << s->factory->cm.unused_size() << endl;
    }
#endif
    clog << " ok" << endl;
  }
 
  
  size_t find_test() {
    res.setName(m_set->getName() + " " + "find " + to_string(data.size()) );
	  res.clear();
    srand(1000);
    clog << res.getName();
    res.measure([&]() mutable {
	    for (size_t i = 0; i < data.size(); ++i) {
        int r = rand()%data.size();
		    m_set->find(data[r].data(), data[r].length());
	    }
      //clog << "\n";
    });
    clog << " ok" << endl;
  }
  
  size_t erase_test() {
    res.setName(m_set->getName() + " " + "erase " + to_string(data.size()));
	  res.clear();
    clog << res.getName();
    res.measure([&]() mutable {
	    for (size_t i = 0; i < data.size(); ++i) {
		    m_set->erase(data[i].data(), data[i].length());
	    }
      //clog << "\n";
    });
    clog << " ok" << endl;;
  }
  
  size_t erase_insert_test() {
    res.setName(m_set->getName() + " " + "erase+insert " + to_string(data.size()));
	  res.clear();
    clog << res.getName();
    res.measure([&]() mutable {
	    for (size_t i = 0; i < data.size(); ++i) {
		    m_set->erase(data[i].data(), data[i].length());
		    m_set->insert(data[i].data(), data[i].length());
	    }
      //clog << "\n";
    });
    clog << " ok" << endl;;
  }
  size_t list_test() {
    res.setName(m_set->getName() + " " + "list " + to_string(data.size()));
	  res.clear();
    basic_onullstream<char_type> cnull;
    clog << res.getName();

    res.measure([&]() mutable {
      m_set->list(cnull);
    });
    clog << " ok" << endl;;
  }
  friend ostream& operator<<(ostream& out, const Measure& _this) {
    out << _this.res;
    return out;
  }

private:
  template<class T> 
  class Set {
    public:
      typedef T key_type;
      typedef char char_type;
 
      virtual void insert(const key_type& key) = 0;
      virtual void erase(const key_type& key) = 0;
      virtual void find(const key_type& key) = 0;
      virtual void list(ostream& out) = 0;
      
      void setName(const string name) {
        m_name = name;
      }
      const string& getName() {
        return m_name;
      }
      void insert(const char_type* key, size_t len) {
        insert(key_type(key, len));
      }
      void erase(const char_type* key, size_t len) {
        erase(key_type(key, len));
      }
      void find(const char_type* key, size_t len) {
        find(key_type(key, len));
      }
    private:
      string m_name;
  };
  

  template <class C> 
  class Set_impl : public Set<typename C::key_type> {
    public:
      void insert(const key_type& key) {
        m_set.insert(key);
      }
      void erase(const key_type& key) {
        m_set.erase(key);
      }
      void find(const key_type& key) {
        m_set.find(key);
      }
      void list(std::ostream& out) {
        for(auto it = m_set.begin(); it != m_set.end(); ++it) {
          out << *it << endl;
        }
      }
      C m_set;
  };

  typedef Set< key_type >      set_type;
  
  vector<key_type> data;
  set_type* m_set;
  Resource res;
  
  set_type* 
  get_container(string id, string name) {
    set_type* ret = NULL;
    if (id == "rb_set") {
      ret = new Set_impl< pbds_set(rb_tree_tag, key_type) >();
    } else if (id == "stl_set_cmp") { 
      ret = new Set_impl< set<key_type, mycmp<key_type> > >();
    } else if (id == "stl_set") { 
      ret = new Set_impl< set<key_type > >();
    } else if (id == "splay_set") {
		  ret = new Set_impl< pbds_set(splay_tree_tag, key_type) >();
	  } else if (id == "ov_set") {	
      ret = new Set_impl< pbds_set(ov_tree_tag, key_type) >();
	  } else if (id == "pat_trie") {
      ret = new Set_impl< trie<basic_string<char_type>, null_type, trie_string_access_traits< basic_string<char_type> >  > >();
    } else if (id == "stl_unordered_set") {
      ret = new Set_impl< unordered_set<key_type> >();
    } else if (id == "simple_trie") {
      ret = new Set_impl< SimpleTrie<key_type> >();
    } else if (id == "compact_trie") {
      ret = new Set_impl< CompactTrieSet<char_type>  >();
    } else if (id == "stl_set_pool") {
      ret = new Set_impl< set<key_type, mycmp<key_type>, __pool_alloc<key_type> >  >();
    }

    if (ret == NULL) {
      cerr << "Nie znaleziono: \"" << id << "\"" << endl;
      abort();
    }

    ret->setName(name);
    return ret;
  }

  void read(string& line);
  private:
   string m_name, m_id;
};  

  template<> 
  void
  Measure<char>::read(string& line) {
    data.push_back(line);
  }
  
  template<> 
  void
  Measure<char16_t>::read(string& line) {
  }

  template<>
  void
  Measure<char32_t>::read(string& line) {
  }

//#pragma pack(8)  
typedef struct {
  char c[1];
  void* s[2];
} p;

bool startswitch(const string& str, const string& prefix) 
{
  if (str.length() < prefix.length()) {
    return false;
  }
  return str.substr(0, prefix.length()) == prefix;
}

void print_help(const string& selfName) 
{
    cout << "Wywołanie programu: " << selfName << " NazwaKontenera [-lLiczbaLinii] [-mMaxMem] [-fPlik] \n"
      << "gdzie:"
      << "\nNazwaKontenera\t- jedno z:" 
      << "\n              \t\t- rb_set"
      << "\n              \t\t- stl_set_pool"
      << "\n              \t\t- stl_set_cmp"
      << "\n              \t\t- stl_set"
      << "\n              \t\t- splay_set"
      << "\n              \t\t- ov_set"
      << "\n              \t\t- pat_trie"
      << "\n              \t\t- stl_unordered_set"
      << "\n              \t\t- simple_trie"
      << "\n              \t\t- compact_trie"
      << "\nLiczbaLinii   \t - liczba linii do przeczytania (domyślnie: wszystkie)"
      << "\nMaxMem        \t - maksymalna wielkość używanej pamieci"
      << "\nPlik          \t - plik z którego mają być czytane linie"
      << "\n\n" 
      << "Program czyta ze standardowego wyjścia (jeśli nie podano Plik) zbiór linii będacych kluczami wybranego kontenera" 
      << endl;
}

int main(int argc, const char** argv)
{
  if (argc < 2) {
      print_help(argv[0]);
      return 0;
  }
  clog << argv[0] << " ";
  clog << argv[1] << " ";
  size_t read_line = 0; //wszystkie
  fstream filestr;
  istream* in = &cin;
  string filename = "STDIN";
  max_mem = get_free_mem() - 50*1024*1024;
  for (int i = 2; i < argc; ++i) {
    string arg = argv[i];
    if (startswitch(arg, "-l")) {
      read_line = stoull(argv[i]+string("-l").length());
    } else if (startswitch(arg, "-f")) {
      filename = argv[i]+string("-f").length(); 
      filestr.open(filename.c_str(), fstream::in);
      in = &filestr;
    } else if (startswitch(arg, "-m")) {
      max_mem = (uint64_t)stoull(argv[i]+string("-m").length());
    } else {
      print_help(argv[0]);
      return 0;
    }
  }
  clog << "-l" << read_line << " ";
  clog << "-m" << max_mem << " ";
  clog << "-f" << filename << " ";
  clog << endl;
  
  Measure<char> m(argv[1]);
  m.read(*in, read_line);
  m.insert_test();
  cout << m << endl;
  m.find_test();
  cout << m << endl;
  m.list_test();
  cout << m << endl;
  //m.erase_insert_test();
  //cout << m << endl;
  m.erase_test();
  cout << m << endl;
  filestr.close();
}
