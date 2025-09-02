#pragma once

#include <Options.hpp>
#include <mutex>
namespace TWC
{
class Hint;
class HintManager
{
  public:
    HintManager();
    ~HintManager();

    void MarkStale();
    void SetHintType(Options::MissingMapsHintMode type);
    void RequestHint();

  private:
    void RefreshHint();
    std::unique_ptr<Hint> hint_;
    bool stale_;
    std::mutex mutex_;
};
} // namespace TWC

namespace G
{
extern TWC::HintManager *Hints;
}
