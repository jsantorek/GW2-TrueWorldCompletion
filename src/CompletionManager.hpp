#pragma once

#include "Options.hpp"
#include "model/ContentDescriptor.hpp"
namespace TWC
{
class CompletionManager
{
  public:
    CompletionManager();
    void Update(TWC::Options::CompletionMode, TWC::Options::CompletionMode, Options::ExpansionAssignmentMode);
    std::function<ContentDescriptor(void)> WorldCompletionMask;
    std::function<ContentDescriptor(void)> CharacterSelectionMask;

  private:
    std::function<ContentDescriptor(void)> MakeMask(TWC::Options::CompletionMode);
    ContentDescriptor CoreMask = ContentDescriptor(flags::empty_t{});
};
} // namespace TWC
namespace G
{
extern TWC::CompletionManager *Completion;
}
