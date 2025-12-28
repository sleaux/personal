#include <map>
#include <mutex>

#include "store.h"

class InMemoryStore : public Store {
  public:
    void Set(std::byte key, std::byte value) override;
    std::byte Get(std::byte key) override;
    bool Has(std::byte key) override;

  private:
    std::map<std::byte, std::byte> store_;
    std::mutex mutex_;
};
