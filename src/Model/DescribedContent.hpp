#pragma once

#include "Content/Descriptor.hpp"
#include "Content/Identifier.hpp"

namespace TWC
{
template <ContentType T> struct DescribedContent : public ContentDescriptor, public Identifier<T>
{
    DescribedContent(const ContentDescriptor &desc, const Identifier<T> &id)
        : ContentDescriptor(desc), Identifier<T>(id)
    {
    }
};
} // namespace TWC
