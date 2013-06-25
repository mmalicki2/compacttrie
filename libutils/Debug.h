#ifndef DEBUG_H_
#define DEBUG_H_

#include <sstream>
#include <string>
#include <cstring>
#include <algorithm>
#include <iomanip>
#include <iostream>

template <class T>
inline std::string to_string (const T& t)
{
  std::stringstream ss;
  ss << t;
  return ss.str();
}

template<class T>
class print_ptr_filter_ {
  public:
  typedef T value;
};

template<>
class print_ptr_filter_<const char*> {
  public:
  typedef const void* value;
};

template<>
class print_ptr_filter_<char*> {
  public:
  typedef void* value;
};


template<class T>
const typename print_ptr_filter_<T>::value print_ptr_filter(const T t) {
  return t;
}

/*  http://stackoverflow.com/questions/7818371/printing-to-nowhere-with-ostream */
/* http://bytes.com/topic/c/answers/428285-null-ostream */ 
#include <streambuf>
#include <ostream>

template <class cT, class traits = std::char_traits<cT> >
class basic_nullbuf: public std::basic_streambuf<cT, traits> {
  typename traits::int_type overflow(typename traits::int_type c)
  {
    return traits::not_eof(c); // indicate success
  }
};

template <class cT, class traits = std::char_traits<cT> >
class basic_onullstream: public std::basic_ostream<cT, traits> {
  public:
    basic_onullstream():
      std::basic_ios<cT, traits>(&m_sbuf),
      std::basic_ostream<cT, traits>(&m_sbuf)
    {
    }

  private:
    basic_nullbuf<cT, traits> m_sbuf;
};

typedef basic_onullstream<char> onullstream;
typedef basic_onullstream<wchar_t> wonullstream;

/* http://stackoverflow.com/questions/7818371/printing-to-nowhere-with-ostream */

#define _SFHEAD(pole) setw(max(strlen(#pole)+1,to_string(print_ptr_filter(pole)).size()+1)) << #pole
#define _SFROW(pole) setw(max(strlen(#pole)+1, to_string(print_ptr_filter(pole)).size()+1)) << print_ptr_filter(pole)

#define CLOG std::cout
#define CDBG std::cout

#endif /* DEBUG_H */
