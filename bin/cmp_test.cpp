#include <Resource.h>

#include <cstring>

#include <vector>
#include <iostream>

using namespace std;

inline bool mycmp(const char* x, const char* y) {
  while ( *x != '\0' && *y != '\0' && *x == *y) {
    x++; 
    y++;
  }
  return *x < *y;
}

void gen_data(vector<string>& data, int len, int count) {
  char c[len+1];
  c[len] = '\0';
  data.reserve(count);
  for (int i = 0; i < count; ++i) {
    for (int j = 0; j < len; ++j) {
      c[j] = 'A' + rand() % ('Z'-'A');
    }
    data.push_back(c);
  }
}

bool test_strcmp(vector<string>& data)
{
  bool ret = false;
  for (int i = 0; i < data.size(); ++i) {
    for (int j = 0; j < data.size(); ++j) {
      ret |= strcmp(data[i].c_str(), data[j].c_str());
    }
  }
  return ret;
}

bool test_mycmp(vector<string>& data)
{
  bool ret = false;
  for (int i = 0; i < data.size(); ++i) {
    for (int j = 0; j < data.size(); ++j) {
      ret |= mycmp(data[i].c_str(), data[j].c_str());
    }
  }
  return ret;
}

int main(int argc, const char** argv)
{
  if (argc < 2) {
    cout << argv[0] << " " << "len " << "count" << endl;
    return 0;
  }
  const int len = atoi(argv[1]);
  const int count = atoi(argv[2]);
  cout << argv[0] << " len: " << len << ", count: " << count << endl;

  bool ret;
  srand(1000);
  vector<string> data;
  gen_data(data, len, count);
  cout << "start" << endl;
  Resource res;
  res.setName("strcmp " + to_string(data.size()) + " " + to_string(len));
  res.clear();
  res.measure([&]() mutable {
      ret = test_strcmp(data);
  });
  cout << res << " " << ret << endl;
  res.setName("mycmp " + to_string(data.size()) + " " + to_string(len));
  res.clear();
  res.measure([&]() mutable {
    ret = test_mycmp(data);
  });
  cout << res << " " << ret << endl;

}
