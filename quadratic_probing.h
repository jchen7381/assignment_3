#ifndef QUADRATIC_PROBING_H
#define QUADRATIC_PROBING_H

#include <vector>
#include <algorithm>
#include <functional>


namespace {

// Internal method to test if a positive number is prime.
bool IsPrime(size_t n) {
  if( n == 2 || n == 3 )
    return true;
  
  if( n == 1 || n % 2 == 0 )
    return false;
  
  for( int i = 3; i * i <= n; i += 2 )
    if( n % i == 0 )
      return false;
  
  return true;
}


// Internal method to return a prime number at least as large as n.
int NextPrime(size_t n) {
  if (n % 2 == 0)
    ++n;  
  while (!IsPrime(n)) n += 2;  
  return n;
}

}  // namespace


// Quadratic probing implementation.
template <typename HashedObj>
class HashTable {
 public:
  enum EntryType {ACTIVE, EMPTY, DELETED};

  // all the getters: num of element, tablesize, load factor, collisions, avg collision, and # of probes.
  int get_num_of_elements(){
    return current_size_;
  }
    
  int get_size_of_table(){
    return array_.size();
  }

  float get_load_factor(){
    return static_cast<float>(current_size_) / static_cast<float>(array_.size());
  }

  int get_collisions(){
    return collisions;
  }

  float get_avg_collisions(){
    return static_cast<float>(collisions) / static_cast<float>(current_size_);
  }

  int get_probes(){
    return probes;
  }
  //probes

  explicit HashTable(size_t size = 101) : array_(NextPrime(size))
    { MakeEmpty(); }
  
  bool Contains(const HashedObj & x) const {
    return IsActive(FindPos(x));
  }
  
  void MakeEmpty() {
    current_size_ = 0;
    for (auto &entry : array_)
      entry.info_ = EMPTY;
  }

  bool Insert(const HashedObj & x) {
    // Insert x as active
    size_t current_pos = FindPos(x);    //finds a position for x
    if (IsActive(current_pos))          //if there is an item in position 
      return false;                     //returns false;
    
    array_[current_pos].element_ = x;
    array_[current_pos].info_ = ACTIVE;
    
    // Rehash; see Section 5.5
    if (++current_size_ > array_.size() / 2)
      Rehash();    
    return true;
  }
    
  bool Insert(HashedObj && x) {
    // Insert x as active
    size_t current_pos = FindPos(x);    //current position = find postion of x , possible after probing
    if (IsActive(current_pos))          //if current position is taken return false.
      return false;
    
    array_[current_pos] = std::move(x);
    array_[current_pos].info_ = ACTIVE;

    // Rehash; see Section 5.5
    if (++current_size_ > array_.size() / 2)
      Rehash();

    return true;
  }

  bool Remove(const HashedObj & x) {
    size_t current_pos = FindPos(x);
    if (!IsActive(current_pos))
      return false;

    array_[current_pos].info_ = DELETED;
    return true;
  }

  size_t size(){
    return current_size_;
  }

  

 private:        
  struct HashEntry {
    HashedObj element_;
    EntryType info_;
    
    HashEntry(const HashedObj& e = HashedObj{}, EntryType i = EMPTY)
    :element_{e}, info_{i} { }
    
    HashEntry(HashedObj && e, EntryType i = EMPTY)
    :element_{std::move(e)}, info_{ i } {}
  };
    
  std::vector<HashEntry> array_;
  size_t current_size_;
  mutable int collisions{0};        //collision
  mutable int probes{1};            //default probe of 1

  bool IsActive(size_t current_pos) const
  { return array_[current_pos].info_ == ACTIVE; }


  size_t FindPos(const HashedObj & x) const {
    int temp_probe{1};                //temp probe
    size_t offset = 1;
    size_t current_pos = InternalHash(x);
    
    while (array_[current_pos].info_ != EMPTY &&
      array_[current_pos].element_ != x) {
      temp_probe++;                       //temp probe + 1 every probe
      collisions++;                       // collision + 1 everytime theres a collision
      
      current_pos += offset;  // Compute ith probe.
      offset += 2;
      if (current_pos >= array_.size())
	current_pos -= array_.size();
    }
    
    probes = temp_probe;                //set probe to tempprobe
    return current_pos;
  }

  void Rehash() {
    std::vector<HashEntry> old_array = array_;

    // Create new double-sized, empty table.
    array_.resize(NextPrime(2 * old_array.size()));
    for (auto & entry : array_)
      entry.info_ = EMPTY;
    
    // Copy table over.
    current_size_ = 0;
    for (auto & entry :old_array)
      if (entry.info_ == ACTIVE)
	Insert(std::move(entry.element_));
  }
  
  size_t InternalHash(const HashedObj & x) const {
    static std::hash<HashedObj> hf;
    return hf(x) % array_.size( );
  }
};

#endif  //QUADRATIC_PROBING_H
