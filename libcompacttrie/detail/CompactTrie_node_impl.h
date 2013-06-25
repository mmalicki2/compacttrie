#include <Debug.h>

#define TNODE_DEF  template<class Char, class Value, class ChunkManager, class BasePoinerType>
#define TNODE_SPEC Node<Char, Value, ChunkManager, BasePoinerType>

static int SSglobal = 0;

TNODE_DEF
class TNODE_SPEC::Data {
  public: 
    typedef this_type base_type;
    typedef struct {
      NodeFactory<base_type>* factory;
      base_type::chunk_manager_type* cm;
      base_type* parent;
      base_type* one;       
      base_type* zero;     
      NodePointerType pointerType;
      bool isOne;           
      bool isStorageDirty;  
      bool isCharsDirty;    
      bool isConnectDirty;  
      bool isDataDirty;     

      char* chunk_ptr;      
      typename chunk_manager_type::pointer pointer; 
    } location_type;


    location_type loc;

    NodePointerType zeroType;
    NodePointerType oneType;
    base_type::size_type zeroSize;
    base_type::size_type oneSize;
    base_type::size_type charsSize;
    bool hasData;
    base_type::size_type zero;
    base_type::size_type one;
    const base_type::char_type* chars;
    base_type::data_type* data;
    
    std::ostream& print(std::ostream& out) {
      out 
        << "\nthis:              " << (void*)this 
        << "\nloc.parent         " << (void*)loc.parent         
        << "\nloc.one            " << (void*)loc.one                 
        << "\nloc.zero           " << (void*)loc.zero              
        << "\nloc.pointerType    " << loc.pointerType    
        << "\nloc.isOne          " << loc.isOne                  
        << "\nloc.isStorageDirty " << loc.isStorageDirty 
        << "\nloc.isCharsDirty   " << loc.isCharsDirty    
        << "\nloc.isConnectDirty " << loc.isConnectDirty 
        << "\nloc.isDataDirty    " << loc.isDataDirty      
        << "\nloc.chunk_ptr      " << (void*)loc.chunk_ptr          
        << "\nloc.pointer        " << loc.pointer        
        << "\nzeroType           " << zeroType           
        << "\noneType            " << oneType            
        << "\nzeroSize           " << zeroSize           
        << "\noneSize            " << oneSize            
        << "\ncharsSize          " << charsSize          
        << "\nhasData            " << hasData            
        << "\nzero               " << zero               
        << "\none                " << one                
        << "\ndata               " << (void*)data               
        << "\nchars              " << (void*)chars;
        out << " \"";
        out.write(chars, charsSize/(sizeof(char_type)*8));
        out << "\" " << charsSize/(sizeof(char_type)*8) << std::endl;
      return out;
    }
};

TNODE_DEF
TNODE_SPEC::Node(chunk_manager_type* cm)
{
  assert(cm);
  clear(cm);
  data.loc.cm = cm;
}

TNODE_DEF
TNODE_SPEC::Node(this_type* parent, bool isOne, typename chunk_manager_type::pointer pointer)
{
  assert(parent);
  assert(!parent->data.loc.cm->isNULL(pointer));
  clear(parent->data.loc.cm);
  data.loc.pointer = pointer;
  data.loc.isOne = isOne;
  data.loc.parent = parent;
  data.loc.pointerType = npt_chunk;
  bind(data.loc.cm->global_address(pointer));
}

TNODE_DEF
TNODE_SPEC::Node(this_type* parent, bool isOne, void* pointer)
{
  assert(parent);
  assert(pointer);
  clear(parent->data.loc.cm);
  data.loc.isOne = isOne;
  data.loc.parent = parent;
  data.loc.pointerType = npt_global;
  bind(pointer);
}

TNODE_DEF
TNODE_SPEC::Node(this_type* node)
{
  assert(node);
  clear(node->data.loc.cm);
  data.loc.chunk_ptr = node.loc.chunk_ptr;
  data.loc.pointer = node->data.loc.pointer;
  data.loc.pointerType = node->data.loc.pointerType;
  bind(data.loc.chunk_ptr);
}

TNODE_DEF
TNODE_SPEC::~Node()
{
  return;
  switch (getType()) {
    case npt_global:
      delete[] data.loc.chunk_ptr;
      break;
    case npt_chunk:
      data.loc.cm->deallocate(data.loc.pointer);
      break;
    default: abort();
  }
}

TNODE_DEF
void
TNODE_SPEC::clear(chunk_manager_type* cm)
{
  data.loc.cm = cm;
  data.loc.factory = NULL;
  data.loc.parent = NULL;
  data.loc.zero = NULL;
  data.loc.one = NULL;
  data.loc.isOne = true;
  data.loc.isStorageDirty = true;
  data.loc.isCharsDirty = true;
  data.loc.isDataDirty = true;
  data.loc.isConnectDirty = true;
  data.loc.chunk_ptr = NULL;
  data.loc.pointerType = npt_chunk;
  data.loc.pointer = cm->null();
  data.zeroType = npt_chunk;
  data.oneType = npt_chunk;
  data.zero = cm->null();
  data.one = cm->null();
  data.zeroSize = value_type::size(data.zero);
  data.oneSize = value_type::size(data.one);
  data.charsSize = 0;
  setBranchBit(sizeof(char_type)*8-1);
  data.hasData = false;
  data.chars = NULL;
  data.data = NULL;
}

TNODE_DEF
bool
TNODE_SPEC::isDirty()
{
  return data.loc.isConnectDirty || data.loc.isStorageDirty || data.loc.isCharsDirty || data.loc.isDataDirty;
}

TNODE_DEF
typename TNODE_SPEC::size_type
TNODE_SPEC::getFlags()
{
  return value_type::fromInt(
      (data.charsSize << 3)  | (data.hasData ? 1 << 2 : 0) | (data.oneType << 1) | data.zeroType);
}


TNODE_DEF
void 
TNODE_SPEC::bind(void* chunk)
{
  bind(data, chunk);
}

TNODE_DEF
void 
TNODE_SPEC::bind(Data& data, void* chunk)
{
  assert (chunk);
  typename value_type::value_type v;
  char* ptr = (char*)chunk;
  data.loc.chunk_ptr = ptr;
  v = value_type::fromMem(ptr);
  size_type flagsSize = value_type::size(v);
  data.zeroType = value_type::value(v) & 0x1 ? npt_chunk : npt_global;
  data.oneType = value_type::value(v) & 0x2 ?  npt_chunk : npt_global;
  data.hasData = (value_type::value(v) & 0x4) != 0; 
  data.charsSize = value_type::value(v) >> 3;
  ptr += flagsSize;
  switch (data.zeroType) {
    case npt_global: 
      data.zero = *reinterpret_cast<size_type*>(ptr);
      data.zeroSize = sizeof(void*); 
      break;
    case npt_chunk: 
      data.zero = value_type::fromMem(ptr);
      data.zeroSize = value_type::size(data.zero); 
      break;
    default: abort();
  }
  ptr += data.zeroSize;
  switch (data.oneType) {
    case npt_global: 
      data.one = *reinterpret_cast<size_type*>(ptr);
      data.oneSize = sizeof(void*); 
      break;
    case npt_chunk: 
      data.one = value_type::fromMem(ptr);
      data.oneSize = value_type::size(data.one); 
      break;
    default: abort();
  }
  ptr += data.oneSize;
  data.chars = reinterpret_cast<char_type*>(ptr);
  data.loc.isCharsDirty = false;
  if (data.hasData) {
    data.data = reinterpret_cast<data_type*>(ptr + getCharsLength()*sizeof(char_type));
  }
  data.loc.isDataDirty = false; 
  data.loc.isStorageDirty = false;
  data.loc.isConnectDirty = false;
}
    
TNODE_DEF
void 
TNODE_SPEC::store()
{
  typename chunk_manager_type::pointer oldPointer = data.loc.cm->null();
  char* oldChunk = NULL;
  NodePointerType oldType = npt_global;
  
  if (data.loc.isStorageDirty) {
    oldType = getType();
    oldChunk = data.loc.chunk_ptr;
    oldPointer = data.loc.pointer;

    size_type s = need_chunk_size();
    if (s <= data.loc.cm->get_max_chunk_size()) {
      data.loc.pointer = data.loc.cm->allocate(s);
      data.loc.chunk_ptr  = (char*)data.loc.cm->global_address(data.loc.pointer);
      data.loc.pointerType = npt_chunk;
    } else {
      data.loc.pointer = data.loc.cm->null();
      data.loc.chunk_ptr = new char[s];
      data.loc.pointerType = npt_global;

      SSglobal++;
    }
    if (data.loc.parent) {
      updateParent();
    }
  }
  store(data.loc.chunk_ptr);
  data.loc.isStorageDirty = false;

  switch (oldType) {
    case npt_global:
      if (oldChunk) SSglobal--;
      delete[] oldChunk;
      assert(oldPointer == data.loc.cm->null());
      break;
    case npt_chunk: 
      data.loc.cm->deallocate(oldPointer); 
      break;
    default: abort();
  }
}

TNODE_DEF
void 
TNODE_SPEC::store(void* chunk)
{
  char* ptr = (char*)chunk;
  size_type flags = getFlags(); 
  size_type flagsSize = value_type::size(flags);
  value_type::toMem(ptr, flags);
  ptr += flagsSize;
  switch (data.zeroType) {
    case npt_global: 
      *reinterpret_cast<void**>(ptr) = reinterpret_cast<void*>(data.zero); 
      break;
    case npt_chunk: 
      value_type::toMem(ptr, data.zero); 
      break;
    default: abort();
  }
  ptr += data.zeroSize;
  switch (data.oneType) {
    case npt_global: 
      *reinterpret_cast<void**>(ptr) = reinterpret_cast<void*>(data.one); 
      break;
    case npt_chunk: 
      value_type::toMem(ptr, data.one); 
      break;
    default: abort();
  }
  ptr += data.oneSize;
  if (data.loc.isCharsDirty || data.loc.isStorageDirty) {
    std::copy(data.chars, data.chars + getCharsLength(), reinterpret_cast<char_type*>(ptr));
    data.chars = ptr;
    data.loc.isCharsDirty = false;
  }
  assert(data.chars == ptr);

  ptr += getCharsLength()*sizeof(char_type);
  if (data.hasData && (data.loc.isDataDirty || data.loc.isStorageDirty)) {
    assert(data.data);
    *reinterpret_cast<data_type*>(ptr) = *data.data;
  }
  data.loc.isDataDirty = false;
  data.loc.isStorageDirty = false;
  data.loc.isConnectDirty = false;
}


TNODE_DEF
typename TNODE_SPEC::size_type 
TNODE_SPEC::need_chunk_size()
{
  size_type s = value_type::size(getFlags()) + data.zeroSize + data.oneSize + getCharsLength()*sizeof(char_type) + (data.hasData ? sizeof(data_type) : 0);
  return s;
}

TNODE_DEF
bool 
TNODE_SPEC::hasData()
{
  return data.hasData;
}

TNODE_DEF
NodePointerType
TNODE_SPEC::getType()
{
  return data.loc.pointerType;
}

TNODE_DEF
typename TNODE_SPEC::size_type 
TNODE_SPEC::getCharsLength()
{
  return data.charsSize / (sizeof(char_type)*8);
}

TNODE_DEF
typename TNODE_SPEC::size_type 
TNODE_SPEC::getBranchBit()
{
  return data.charsSize % (sizeof(char_type)*8);
}

TNODE_DEF
typename TNODE_SPEC::this_type*  
TNODE_SPEC::getZero()
{
  if (data.loc.zero == NULL) {
    this_type* t;
    switch (data.zeroType) {
      case npt_global: 
        if ((void*)data.zero == NULL) {
          return NULL; 
        }
        assert(data.loc.factory);
        t = data.loc.factory->createNode(this, false, (void*)data.zero);
        break;
      case npt_chunk:
        if (data.loc.cm->isNULL((typename chunk_manager_type::pointer)value_type::value(data.zero))) {
          return NULL;
        }
        assert(data.loc.factory);
        t = data.loc.factory->createNode(this, false, (typename chunk_manager_type::pointer)value_type::value(data.zero));
        break;
      default: abort();
    }
    data.loc.zero = t;
  }
  assert(data.loc.zero->data.loc.parent == this);
  return data.loc.zero;
}
  
TNODE_DEF
typename TNODE_SPEC::this_type*
TNODE_SPEC::getOne()
{
  if (data.loc.one == NULL) {
    this_type* t;
    switch (data.oneType) {
      case npt_global: 
        if ((void*)data.one == NULL) {
          return NULL; 
        }
        assert(data.loc.factory);
        t = data.loc.factory->createNode(this, true, (void*)data.one);
        break;
      case npt_chunk:
        if (data.loc.cm->isNULL((typename chunk_manager_type::pointer)value_type::value(data.one))) {
          return NULL;
        }
        assert(data.loc.factory);
        t = data.loc.factory->createNode(this, true, (typename chunk_manager_type::pointer)value_type::value(data.one));
        break;
      default: abort();
    }
    data.loc.one = t;
  }
  assert(data.loc.one->data.loc.parent == this);
  return data.loc.one;
}

TNODE_DEF
typename TNODE_SPEC::this_type*
TNODE_SPEC::getNextNode(char_type c) {
  if (asm_bt(c, getBranchBit())) {
    return getOne();
  } else {
    return getZero();
  }
}

TNODE_DEF
typename TNODE_SPEC::this_type*
TNODE_SPEC::getNextSiblingNode(char_type c) {
  if (!asm_bt(c, getBranchBit())) {
    return getOne();
  } else {
    return getZero();
  }
}

TNODE_DEF
typename TNODE_SPEC::this_type*
TNODE_SPEC::getNextCharNode() {
    this_type* n = this;
    do {
      this_type* nn = n->getZero();
      if (nn == NULL) {
        nn = n->getOne();
      }
      n = nn;
      assert(n != NULL);
    } while(n->getCharsLength() == 0);
    return n;
}


TNODE_DEF
typename TNODE_SPEC::this_type*
TNODE_SPEC::setSubtree(this_type* node) {
  data = node->data; 
  if (data.loc.one != NULL) {
    data.loc.one->setParent(this, true);
    assert(data.loc.one->data.loc.parent == this);
  } 
  if (data.loc.zero != NULL) {
    data.loc.zero->setParent(this, false);
    assert(data.loc.zero->data.loc.parent == this);
  }
}

TNODE_DEF
const typename TNODE_SPEC::char_type* 
TNODE_SPEC::getChars()
{
  assert(data.chars);
  return data.chars;
}

TNODE_DEF
typename TNODE_SPEC::data_type* 
TNODE_SPEC::getData()
{
  assert(data.data);
  return data.data;
}
    
TNODE_DEF
typename TNODE_SPEC::size_type 
TNODE_SPEC::getNodeSize()
{
  return value_type::size(getFlags())  + data.zeroSize + data.oneSize + getCharsLength()*sizeof(char_type) + (data.hasData ? sizeof(data_type) : 0);
}

TNODE_DEF
typename TNODE_SPEC::this_type*
TNODE_SPEC::getParent()
{
  return data.loc.parent;
}

TNODE_DEF
void
TNODE_SPEC::setParent(this_type* parent, bool isOne)
{
  assert(parent);
  data.loc.isOne = isOne;
  if (isOne) {
    parent->data.loc.one = this;
  } else {
    parent->data.loc.zero = this;
  }
  data.loc.parent = parent;
  updateParent();
}

TNODE_DEF
void
TNODE_SPEC::updateParent()
{
  this_type* parent = data.loc.parent;

  size_type p;
  size_type s;
  switch (getType()) {
    case npt_global:
      p = *reinterpret_cast<size_type*>(&data.loc.chunk_ptr);
      s = sizeof(void*);
      break;
    case npt_chunk:
      p = value_type::fromInt(data.loc.pointer);
      s = value_type::size(p);
      break;
    default: abort();
  }
  if (data.loc.isOne) {
    if (parent->data.one != p) {  
      if (parent->data.oneSize != s) {
        parent->data.oneSize = s;
        parent->data.loc.isStorageDirty = true;
      }
      parent->data.oneType = getType();
      parent->data.one = p;
      parent->data.loc.isConnectDirty = true;
    }
  } else {
    if (parent->data.zero != p) {
      if (parent->data.zeroSize != s) {
        parent->data.zeroSize = s;
        parent->data.loc.isStorageDirty = true;
      }
      parent->data.zeroType = getType();
      parent->data.zero = p;
      parent->data.loc.isConnectDirty = true;
    }
  }
}

TNODE_DEF
void
TNODE_SPEC::replace(this_type* node)
{
  assert(node);
  if (node->getParent() != NULL) {
    setParent(node->getParent(), node->data.loc.isOne);
  }
}

TNODE_DEF
void 
TNODE_SPEC::setBranchBit(size_type bit)
{
  data.charsSize = (data.charsSize & (~(sizeof(char_type)*8 - 1))) | bit;
  assert(getBranchBit() == bit);
}

TNODE_DEF
void 
TNODE_SPEC::setZero(this_type* node)
{
  node->setParent(this, false);
}

TNODE_DEF
void 
TNODE_SPEC::setOne(this_type* node)
{
  node->setParent(this, true);
}

TNODE_DEF
void 
TNODE_SPEC::setChars(const char_type* chars, size_type size, size_type bitBranch)
{
  size_type newCharsSize = (size*sizeof(char_type))*8 | bitBranch;
  if (data.charsSize != newCharsSize) {
    data.loc.isStorageDirty = true;
  }
  data.chars = chars;
  assert(bitBranch < sizeof(char_type)*8);
  data.charsSize = newCharsSize;
  data.loc.isCharsDirty = true;
}

TNODE_DEF
void 
TNODE_SPEC::setChars(const char_type* chars, size_type size)
{
  setChars(chars, size, sizeof(char_type)*8-1);
}

TNODE_DEF
void 
TNODE_SPEC::setData(data_type* data_)
{
  if (data.hasData) {
    data.data->~data_type();
  } else {
    data.loc.isStorageDirty = true;
  }
  data.hasData = true;
  data.data = data_;
  data.loc.isDataDirty = true;
}

TNODE_DEF
void 
TNODE_SPEC::eraseData()
{
  if (data.hasData) {
    data.data->~data_type();
    data.loc.isStorageDirty = true;
  }
  data.data = NULL;
  data.hasData = false;
  data.loc.isDataDirty = true;
}

TNODE_DEF
std::ostream& operator<<(std::ostream& out, TNODE_SPEC& _this) {
  static int indent = 0;
  indent++;
  std::string f(indent, ' ');
  out << "at " << (void*)&_this << " " << (void*)_this.getChars() << " " << "\"";
  out.write(_this.getChars(), _this.getCharsLength());
  out << "\", BranchBit: " << _this.getBranchBit() 
    << ", type: " << _this.data.loc.pointerType
    << ",  chunksize: " << _this.data.loc.cm->getChunkSize(_this.data.loc.pointer)
    << ", data: " << (_this.hasData() ? "TAK" : "NIE") << "\n";
  TNODE_SPEC* n = _this.getZero();
  out << f;
  if (n) out << n->data.loc.parent << " (" << (n->data.loc.parent  == &_this) << ") ";
  out << "zero: ";
  if (n == NULL) {
    out << "NULL\n";
  } else {
    out << *n;
  }
  n = _this.getOne();
  out <<f;
  if(n) out << n->data.loc.parent << " (" << (n->data.loc.parent  == &_this) << ") "; 
  out << "one: ";
  if (n == NULL) {
    out << "NULL\n";
  } else {
    out << *n;
  }
  indent--;
  return out;
}

TNODE_DEF
void 
TNODE_SPEC::printNode()
{
  data.print(CLOG) << std::endl;
}

TNODE_DEF
void 
TNODE_SPEC::destroy()
{
  switch (data.loc.pointerType) {
    case npt_global:
      delete[] data.loc.chunk_ptr;
      break;
    case npt_chunk:
      data.loc.cm->deallocate(data.loc.pointer);
      break;
    default: abort();
  }
  data.loc.pointerType = npt_chunk;
  data.loc.pointer = data.loc.cm->null();
}

TNODE_DEF
void 
TNODE_SPEC::disconnect()
{
  destroy();
  if (data.loc.parent) {
    updateParent();
    if (data.loc.isOne) {
      data.loc.parent->data.loc.one = NULL;
    } else {
      data.loc.parent->data.loc.zero = NULL;
    }
  }
}

