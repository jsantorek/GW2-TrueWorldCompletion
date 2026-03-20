#pragma once

#include "model/ContentDescriptor.hpp"
#include <vector>

namespace TWC::Retired
{

template <typename T> struct DescribedContent
{
    auto operator&(Retired::ContentDescriptor mask)
    {
        if (mask == DescriptorMask::Everything())
            return Content;
        std::vector<T> subset;
        for (std::size_t i = 0; i < size(); i++)
            if (Descriptors[i] & mask)
                subset.emplace_back(Content[i]);
        return subset;
    }
    inline auto size() const
    {
        return Content.size();
    }

    inline void reserve(std::size_t newSize)
    {
        Content.clear();
        Content.reserve(newSize);
        Descriptors.clear();
        Descriptors.reserve(newSize);
    }

    inline void emplace(T &&cnt, Retired::ContentDescriptor &&cls)
    {
        Content.emplace_back(std::move(cnt));
        Descriptors.emplace_back(std::move(cls));
    }

    inline void emplace(std::pair<T, Retired::ContentDescriptor> &&pair)
    {
        Content.emplace_back(std::move(pair.first));
        Descriptors.emplace_back(std::move(pair.second));
    }

    std::vector<T> Content;
    std::vector<Retired::ContentDescriptor> Descriptors;
};
} // namespace TWC::Retired