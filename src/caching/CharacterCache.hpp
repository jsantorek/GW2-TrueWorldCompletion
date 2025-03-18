#pragma once

#include "model/Completion.hpp"
#include <filesystem>
#include <mutex>
#include <optional>
#include <unordered_map>

namespace TWC
{
class CharacterCache
{
  public:
    inline std::optional<Completion> GetWorldCompletion(std::string characterId)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (auto it = completion_.find(characterId); it != completion_.end())
            return Completion{it->second, available_};
        return std::nullopt;
    }

    void SetWorldCompletion(Completion completion);

    inline void Clear()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        completion_.clear();
    }

    ~CharacterCache();

    static CharacterCache *Load(std::filesystem::path filepath);

  private:
    decltype(Completion::Available) available_;
    std::unordered_map<std::string, decltype(Completion::Completed)> completion_;
    std::mutex mutex_;
    std::filesystem::path filepath_;
    CharacterCache() = default;
};
} // namespace TWC
