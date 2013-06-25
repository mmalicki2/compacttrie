#include <Resource.h>

#include <sys/resource.h>

#include <iostream>
#include <fstream>
#include <algorithm>


using namespace std;

std::ostream& operator<<(std::ostream& out, const Resource& _this) 
{
  out << _this.name << " mem";
  for_each(_this.ticks.begin(), _this.ticks.end(), [&](const Resource::ticks_type::value_type& tick) {
    out << " " << (tick.mem - _this.ticks.front().mem);
  });

  out << "\n" << _this.name << " time";
  for_each(_this.ticks.begin(), _this.ticks.end(), [&](const Resource::ticks_type::value_type& tick) {
    out << " " << tick.time - _this.ticks.front().time;
  }); 
  return out;
}

uint64_t get_micro(struct timeval& t) {
  return t.tv_sec*1000000+t.tv_usec;
}

Resource::tick_type
Resource::get_tick()
{
  struct rusage u;
  getrusage(RUSAGE_SELF, &u);

  return Tick(u.ru_maxrss*1024, get_micro(u.ru_utime) + get_micro(u.ru_stime));
}


//http://stackoverflow.com/questions/349889/how-do-you-determine-the-amount-of-linux-system-ram-in-c

uint64_t get_free_mem() {
  std::string token;
  std::ifstream file("/proc/meminfo");
  while(file >> token) {
    if(token == "MemFree:") {
      uint64_t mem;
      if(file >> mem) {
        return mem*1024;
      } else {
        return 0;       
      }
    }

    // ignore rest of the line
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  return 0; // nothing found
}

//http://stackoverflow.com/questions/349889/how-do-you-determine-the-amount-of-linux-system-ram-in-c
