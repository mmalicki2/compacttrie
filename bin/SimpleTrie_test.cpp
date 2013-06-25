#include <SimpleTrie.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include <Debug.h>

using namespace std;

template<class S>
void insert_find(S& s, string str) 
{
  cerr << "\ninsert (" << str << ")...";
  s.insert(str);
  cerr << "\nfind...";
  bool f = s.find(str);
  cerr << (f ? "ok" : "err") << endl; 

}

void read(istream& in) {
  SimpleTrie<string> s;
  vector<string> data;
  string line;
  while ( in >> line) {
    data.push_back(line);
  }

  for (int i = 0; i < data.size(); ++i) {
    s.insert(data[i]);
    for (int j = 0; j <= i; ++j) {
      if (!s.find(data[j])) {
        CLOG << "ERR: " << i << " " << j << " insert: \"" << data[i] << "\"" << " find: \"" << data[j] << "\"" << endl;
        abort();
      } else {
        //CLOG << "OK: " << i << " " << j << endl;
      }
    }
  }
  
  for (int i = 0; i < data.size(); ++i) {
    if (!s.find(data[i])) {
      CLOG << "ERR: " << i << " \"" << data[i] << "\"" << endl;
      abort();
    } else {
      //CLOG << "OK: " << i << endl;
    }
  }

  vector<string> sdata(data);  
  sort(sdata.begin(), sdata.end());
  typename SimpleTrie<string>::iterator it = s.begin();
  for (int i = 0; i < sdata.size(); ++i) {
    string str = *it;
    if (it == s.end() || (sdata[i] != *it)) {//   string(it.getKey(), it.getKeyLength()))) {
      //CLOG << s << endl;
      typename SimpleTrie<string>::iterator its = s.begin(); 
      for (int j = 0; j < sdata.size(); ++j) {
        CLOG << j << ": \"" << sdata[j] << "\" cmp \"";
        CLOG << *it;
        CLOG << "\"";
        CLOG << std::endl;
        ++its;
      }
      CLOG << "error: on:" << i << std::endl;
      abort();
    }
    ++it;
  }

  for (int i = 0; i < data.size(); ++i) {
    s.erase(data[i]);
    for (int j = 0; j < i; ++j) {
      if (s.find(data[j])) {
        CLOG << "ERR: " << i << " " << j << " erase: \"" << data[i] << "\"" << " finded: \"" << data[j] << "\"" << endl;
        abort();
      } else {
        //CLOG << "OK: " << i << " " << j << endl;
      }
    }
    for (int j = i+1; j < data.size(); ++j) {
      if (!s.find(data[j])) {
        CLOG << "ERR: " << i << " " << j << " erase: \"" << data[i] << "\"" << " can not find: \"" << data[j] << "\"" << endl;
        abort();
      } else {
        //CLOG << "OK: " << i << " " << j << endl;
      }
    }
  }
  
  
  //CLOG << s << endl;
  CLOG << "ALL OK" << endl;
}

bool startswitch(const string& str, const string& prefix) 
{
  if (str.length() < prefix.length()) {
    return false;
  }
  return str.substr(0, prefix.length()) == prefix;
}

int main(int argc, const char** argv) 
{
  //test_vint();
  fstream filestr;
  istream* in = &cin;
  string filename = "STDIN";
  for (int i = 1; i < argc; ++i) {
    string arg = argv[i];
    if (startswitch(arg, "-f")) {
      filename = argv[i]+string("-f").length(); 
      filestr.open(filename.c_str(), fstream::in);
      in = &filestr;
    }
  }
  read(*in);
  
  filestr.close();
}

