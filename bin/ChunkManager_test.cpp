#include <iostream>

#include <bit.h>
#include <vector>

using namespace std;

#include <ChunkManager.h>

int main(int argc, const char** argv) 
{

  ChunkManager<uint32_t> storage;
  vector<size_t> p(33);
  for (int i = 1; i < p.size(); ++i) {
    p[i] = storage.allocate(i);
    cerr << "(" << p[i] << " " <<  storage.global_address(p[i])  << ") ";
  }
  cerr << endl;
  vector< vector<size_t> > p2(33);
  for (int i = 1; i < p2.size(); ++i) {
    p2[i].resize(7);
    for (int j = 0; j < p2[i].size(); ++j) {
      p2[i][j] = storage.allocate(i);
      cerr << "(" << p2[i][j] << " " << storage.global_address(p2[i][j]) << ") ";
    }
    cerr << endl;
  }

  for (int i = 1; i < p.size(); ++i) {
    *reinterpret_cast<char*>(storage.global_address(p[i])) = i;
  }
  for (int i = 1; i < p.size(); ++i) {
    cerr << *reinterpret_cast<int*>(storage.global_address(p[i])) << " ";
  }
  cerr << endl;

  cerr << "deallocate" <<endl;
  for (int i = 1; i < p2.size(); ++i) {
    for (int j = 0; j < p2[i].size(); j+=2) {
      cerr << i << " " << p2[i][j] << ", ";
      storage.deallocate(p2[i][j]);
    }
  }

  cerr << "p3:" << endl;
  vector< vector<size_t> > p3(33);
  for (int i = 1; i < p3.size(); ++i) {
    p3[i].resize(10);
    for (int j = 0; j < p3[i].size(); ++j) {
      p3[i][j] = storage.allocate(i);
      cerr << p3[i][j] << " ";
    }
    cerr << endl;
  }
}

