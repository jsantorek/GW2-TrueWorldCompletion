#pragma once
#include "Configurable/IncompleteMapHint.hpp"
#include <Hint/Interface.hpp>
#include <chrono>
#include <mutex>

namespace TWC
{
class HintManager
{
  public:
    HintManager();
    void MarkStale();
    void SetHintType(ConfigurableIncompleteMapHint);
    void RequestHint();

  private:
    std::unique_ptr<HintInterface> Hint;
    bool Stale;
    std::mutex Mutex;
    std::chrono::time_point<std::chrono::system_clock> LastActivation;
};
} // namespace TWC

namespace G
{
extern TWC::HintManager *Hints;
}
