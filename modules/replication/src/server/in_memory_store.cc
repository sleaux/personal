#include "in_memory_store.h"

#include <cstddef>
#include <mutex>

std::byte InMemoryStore::Get(std::byte key) {
    std::lock_guard<std::mutex> lock(mutex_);
    return store_.at(key);
}

void InMemoryStore::Set(std::byte key, std::byte value) {
    std::lock_guard<std::mutex> lock(mutex_);
    store_[key] = value;
}

bool InMemoryStore::Has(std::byte key) {
    std::lock_guard<std::mutex> lock(mutex_);
    return store_.find(key) != store_.end();
}
