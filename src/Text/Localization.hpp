#pragma once

#include "Configurable/WorldCompletion.hpp"
#include "Text/Tag.hpp"
#include "Text/Utilities.hpp"
#include <Nexus.h>
#include <format>
#include <magic_enum/magic_enum.hpp>

namespace TWC
{
class TextLocalization
{
  public:
    static void Initialize(const AddonAPI::LocalizationVT &localization);

    template <ConfigurableWorldCompletion T, typename... Args>
    static inline std::string FetchDescription(Args &&...args)
    {
        return FetchValidated(GetIdentifier<T>(args...));
    }

    template <TextTag T> static inline std::string FetchDescription()
    {
        return FetchValidated(GetIdentifier<T>());
    }

  private:
    static LOCALIZATION_TRANSLATE Translate;

    static std::string FetchValidated(std::string id);

    template <TextTag T> static inline std::string GetIdentifier()
    {
        return std::format("{}::{}", ADDON_ACRONYM, magic_enum::enum_name(T));
    }

    template <ConfigurableWorldCompletion T, typename... Args>
    static constexpr inline std::string GetIdentifier(Args &&...args)
    {
        if constexpr (T == ConfigurableWorldCompletion::AllMapsCollectively ||
                      T == ConfigurableWorldCompletion::AllMapsWithCompletionReward)
        {
            static_assert(sizeof...(Args) == 0);
            return std::format("{}::World::{}", ADDON_ACRONYM, magic_enum::enum_name(T));
        }
        else if constexpr (T == ConfigurableWorldCompletion::CurrentContinentMapsOnly ||
                           T == ConfigurableWorldCompletion::CurrentExpansionMapsOnly ||
                           T == ConfigurableWorldCompletion::CurrentOrEarlierExpansionMaps)
        {
            static_assert(sizeof...(Args) == 1);
            using Arg = std::remove_cvref_t<std::tuple_element_t<0, std::tuple<Args...>>>;

            static_assert(std::is_enum_v<Arg>, "Argument must be an enum.");
            auto &&value = std::get<0>(std::forward_as_tuple(std::forward<Args>(args)...));

            return std::format("{}::{}::{}", ADDON_ACRONYM, magic_enum::enum_name(T),
                               magic_enum::enum_name<Arg>(value));
        }
        else
            static_assert(T != T, "No available identifier");
    }
};
} // namespace TWC
