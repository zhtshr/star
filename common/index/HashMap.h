//
// Created by Yi Lu on 7/14/18.
//

#pragma once

#include "common/SpinLock.h"
#include "index.h"
#include <atomic>
#include <glog/logging.h>
#include <unordered_map>

namespace star {

template <std::size_t N, class KeyType, class ValueType>
class HashMap : public Index<N,  KeyType, ValueType> {
public:
  using hasher = typename std::unordered_map<KeyType, ValueType>::hasher;

  HashMap<N, KeyType, ValueType>() {}
  ~HashMap<N, KeyType, ValueType>() {}

  bool Delete(const KeyType &key) override {
    return _applyAt(
        [&key](std::unordered_map<KeyType, ValueType> &map) {
          auto it = map.find(key);
          if (it == map.end()) {
            return false;
          } else {
            map.erase(it);
            return true;
          }
        },
        bucketNo(key));
  }

  bool Contains(const KeyType &key) override {
    return _applyAt(
        [&key](const std::unordered_map<KeyType, ValueType> &map) {
          return map.find(key) != map.end();
        },
        bucketNo(key));
  }

  bool Insert(const KeyType &key, const ValueType &value) override {
    return _applyAt(
        [&key, &value](std::unordered_map<KeyType, ValueType> &map) {
          if (map.find(key) != map.end()) {
            return false;
          }
          auto &row = map[key];
          std::get<0>(row).store(0);
          std::get<1>(row) = std::get<1>(value);
          // map[key] = value;
          return true;
        },
        bucketNo(key));
  }

  bool Update(const KeyType &key, const ValueType &value) override {
    return _applyAt(
        [&key, &value](std::unordered_map<KeyType, ValueType> &map) {
          if (map.find(key) != map.end()) {
            return false;
          }
          // map[key] = value;
          return true;
        },
        bucketNo(key));
  }

  ValueType &Search(const KeyType &key) override {
    return _applyAtRef(
        [&key](std::unordered_map<KeyType, ValueType> &map) -> ValueType & {
          return map[key];
        },
        bucketNo(key));
  }

  std::size_t size() {
    return _fold(0, [](std::size_t totalSize,
                       const std::unordered_map<KeyType, ValueType> &map) {
      return totalSize + map.size();
    });
  }

  void clear() {
    _map([](std::unordered_map<KeyType, ValueType> &map) { map.clear(); });
  }

  Index<N, KeyType, ValueType>* newIndex() override {return nullptr;}

  // Returns a iterator of index.
  Iterator* NewIterator() override {return nullptr;}
  void* NewIterator_Node() override { return nullptr; };

private:
  template <class ApplyFunc>
  auto &_applyAtRef(ApplyFunc applyFunc, std::size_t i) {
    DCHECK(i < N) << "index " << i << " is greater than " << N;
    locks_[i].lock();
    auto &result = applyFunc(maps_[i]);
    locks_[i].unlock();
    return result;
  }

  template <class ApplyFunc> auto _applyAt(ApplyFunc applyFunc, std::size_t i) {
    DCHECK(i < N) << "index " << i << " is greater than " << N;
    locks_[i].lock();
    auto result = applyFunc(maps_[i]);
    locks_[i].unlock();
    return result;
  }

  template <class MapFunc> void _map(MapFunc mapFunc) {
    for (auto i = 0u; i < N; i++) {
      locks_[i].lock();
      mapFunc(maps_[i]);
      locks_[i].unlock();
    }
  }

  template <class T, class FoldFunc>
  auto _fold(const T &firstValue, FoldFunc foldFunc) {
    T finalValue = firstValue;
    for (auto i = 0u; i < N; i++) {
      locks_[i].lock();
      finalValue = foldFunc(finalValue, maps_[i]);
      locks_[i].unlock();
    }
    return finalValue;
  }

  auto bucketNo(const KeyType &key) const { return hasher_(key) % N; }

private:
  hasher hasher_;
  std::unordered_map<KeyType, ValueType> maps_[N];
  SpinLock locks_[N];
};

} // namespace star
