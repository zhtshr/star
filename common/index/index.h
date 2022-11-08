#pragma once

#include "common/iterator.h"
// #include "HashMap.h"

#include <functional>

namespace star {

typedef enum{
  kHashMap,
  kHot,
  kMassTree,
  kAlex,
  kLipp
} Option;

template <std::size_t N, class KeyType, class ValueType>
class Index {
 public:
  // Insert key into the index.
  // Returns true iff an entry that compares equal to key is in the index.
  virtual bool Insert(const KeyType &key, const ValueType &value) = 0;

  // Update key in the index.
  // Returns true iff an entry that compares equal to key is in the index.
  virtual bool Update(const KeyType &key, const ValueType &value) = 0;

  // Delete key in the index.
  // Returns true iff an entry that compares equal to key is in the index.
  virtual bool Delete(const KeyType &key) = 0;

  // Returns true iff an entry that compares equal to key is in the index.
  virtual bool Contains(const KeyType &key) = 0;

  // Returns value of an entry that compares equal to key is in the index.
  virtual ValueType &Search(const KeyType &key) = 0;

  virtual Index<N, KeyType, ValueType>* newIndex() = 0;

  // Returns a iterator of index.
  virtual Iterator* NewIterator() = 0;
  virtual void* NewIterator_Node() { return nullptr; };
};


}  // namespace star
