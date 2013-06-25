#ifndef RESOURCE_H_
#define RESOURCE_H_

#include <ostream>
#include <list>
#include <string>

class Resource {
	public:
    Resource() {};
		Resource(const std::string& name) : name(name) {};
   
    template <class Fun>
    void measure(Fun f);
		
    void tick() {
      ticks.push_back(get_tick());
    }

    void setName(const std::string& name) {
      this->name = name;
    }
    const std::string& getName() const {
      return name;
    }
    void clear() {
      ticks.clear();
    }

    uint64_t getMem() {
      return get_tick().mem;
    }
		
    friend std::ostream& operator<<(std::ostream&, const Resource& _this); 

	private:
    struct Tick {
      Tick(const uint64_t m, const uint64_t t) : mem(m), time(t) {}
      uint64_t mem;
      uint64_t time;
    };

    typedef Tick                  tick_type;
    typedef std::list<tick_type>  ticks_type;

    std::string           name;
    ticks_type            ticks;

    tick_type get_tick();
};
    
template <class Fun>
void Resource::measure(Fun f)
{
  tick();
  f();
  tick();
}

uint64_t get_free_mem();

#endif /* RESOURCE_H_ */
