#include <CompactTrieSet.h>

#include <iostream>

using namespace std;

int main(int argc, const char** argv) 
{
  typedef CompactTrieSet<char> set_type;
  
  set_type s;

  s.insert("ala");
  s.insert("ewa");
  s.insert("ela");
  s.insert("adam");
  s.insert("ada");
  cout << "Wszytskie klucze\n";
  for (set_type::iterator it = s.begin(); it != s.end(); ++it) {
    cout << *it << ", ";
  }
  cout << "\n";
  
  if (s.find("ala")) {
    cout << "Ala obecna\n";
  } else {
    cout << "Nie ma Ali\n";
  }
  
  s.erase("ala");

  if (s.find("ala")) {
    cout << "Ala obecna\n";
  } else {
    cout << "Nie ma Ali\n";
  }
  
  cout << "Wszytskie klucze\n";
  for (set_type::iterator it = s.begin(); it != s.end(); ++it) {
    cout << *it << ", ";
  }
  cout << "\n";

  cout << s << endl;
}

