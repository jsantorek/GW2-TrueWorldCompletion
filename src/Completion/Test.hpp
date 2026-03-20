#pragma once
#include "Completion/Value.hpp"
#include "Content/Type.hpp"
#include <Content/Identifier.hpp>

namespace GW2RE
{
struct BitMask;
union ProgressManager_t;
}; // namespace GW2RE

namespace TWC
{
class CompletionTest
{
  public:
    CompletionTest();

    static void Validate();

    bool IsAvailable() const;

    template <ContentType T, typename Collection> inline CompletionValue GetCompletion(const Collection &ids)
    {
        auto value = CompletionValue{};
        for (const auto &id : ids)
        {
            if (IsComplete<T>(static_cast<Identifier<T>>(id)))
            {
                value.Completed++;
            }
        }
        value.Available = static_cast<uint32_t>(ids.size());
        return value;
    }

    template <ContentType T> bool IsComplete(const Identifier<T> &id) const;
    template <typename IdentifierType> bool operator()(const IdentifierType &id) const
    {
        using T = std::remove_cvref_t<IdentifierType>;

        if constexpr (std::is_base_of_v<Identifier<ContentType::Landmark>, T>)
        {
            return IsComplete<ContentType::Landmark>(static_cast<const Identifier<ContentType::Landmark> &>(id));
        }
        else if constexpr (std::is_base_of_v<Identifier<ContentType::HeroChallenge>, T>)
        {
            return IsComplete<ContentType::HeroChallenge>(
                static_cast<const Identifier<ContentType::HeroChallenge> &>(id));
        }
        else if constexpr (std::is_base_of_v<Identifier<ContentType::RenownHeart>, T>)
        {
            return IsComplete<ContentType::RenownHeart>(static_cast<const Identifier<ContentType::RenownHeart> &>(id));
        }
        else
        {
            static_assert(std::false_type::value, "Unsupported Identifier type passed to CompletionTest::operator()");
        }
    }

    template <typename IdentifierType> CompletionValue operator()(CompletionValue value, const IdentifierType &id) const
    {
        using T = std::remove_cvref_t<IdentifierType>;

        ++value.Available;
        if constexpr (std::is_base_of_v<Identifier<ContentType::Landmark>, T>)
        {
            if (IsComplete<ContentType::Landmark>(static_cast<const Identifier<ContentType::Landmark> &>(id)))
                ++value.Completed;
        }
        else if constexpr (std::is_base_of_v<Identifier<ContentType::HeroChallenge>, T>)
        {
            if (IsComplete<ContentType::HeroChallenge>(static_cast<const Identifier<ContentType::HeroChallenge> &>(id)))
                ++value.Completed;
        }
        else if constexpr (std::is_base_of_v<Identifier<ContentType::RenownHeart>, T>)
        {
            if (IsComplete<ContentType::RenownHeart>(static_cast<const Identifier<ContentType::RenownHeart> &>(id)))
                ++value.Completed;
        }
        else
        {
            static_assert(std::false_type::value, "Unsupported Identifier type passed to CompletionTest::operator()");
        }
        return value;
    }

  private:
    GW2RE::BitMask *IsTaskComplete = nullptr;
    GW2RE::BitMask *IsLandmarkDiscovered = nullptr;
    GW2RE::ProgressManager_t *ProgressMgr = nullptr;
};
template <> bool CompletionTest::IsComplete<ContentType::Landmark>(const Identifier<ContentType::Landmark> &id) const;
template <>
bool CompletionTest::IsComplete<ContentType::HeroChallenge>(const Identifier<ContentType::HeroChallenge> &id) const;
template <>
bool CompletionTest::IsComplete<ContentType::RenownHeart>(const Identifier<ContentType::RenownHeart> &id) const;

} // namespace TWC
