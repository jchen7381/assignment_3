#ifndef LINEAR_PROBING_H
#define LINEAR_PROBING_H

#include <vector>
#include <algorithm>
#include <functional>


namespace {

// Internal method to test if a positive number is prime.
bool IsPrimeLinear(size_t n) {
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
int NextPrimeLinear(size_t n) {
  if (n % 2 == 0)
    ++n;  
  while (!IsPrimeLinear(n)) n += 2;  
  return n;
}

}  // namespace

template <typename HashedObj>
class HashTableLinear {
 public:
  enum EntryType {ACTIVE, EMPTY, DELETED};

  int get_num_of_elements(){
    return current_size_;
  }
    
  int get_size_of_table(){
    return array_.size();
  }

  float get_load_factor(){
    return static_cast<float>(current_size_) / static_cast<float> (array_.size());
  }

  int get_collisions(){
    return collisions;
  }

  float get_avg_collisions(){
    return static_cast<float>(collisions) / static_cast<float> (current_size_);
  }

  int get_probes(){
    return probes;
  }
  

  explicit HashTableLinear(size_t size = 101) : array_(NextPrimeLinear(size))
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
  mutable int collisions{0};
  mutable int probes{1};

  bool IsActive(size_t current_pos) const
  { return array_[current_pos].info_ == ACTIVE; }


  size_t FindPos(const HashedObj & x) const {
    int temp_probes{1};
    size_t offset = 1;
    size_t current_pos = InternalHash(x);
    
    while (array_[current_pos].info_ != EMPTY &&
	   array_[current_pos].element_ != x) {
      temp_probes++;
      collisions++;
      current_pos += offset;  // Compute ith probe.
      // offset += 2;
      if (current_pos >= array_.size())   //restart the index from the beginning
	current_pos -= array_.size();
    }
    probes = temp_probes;
    return current_pos;       //returns the position, changes only if index is not empty
  }

  void Rehash() {
    std::vector<HashEntry> old_array = array_;

    // Create new double-sized, empty table.
    array_.resize(NextPrimeLinear(2 * old_array.size()));
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

#endif  // LINEAR_PROBING_H
