#pragma once
#ifndef __ENGINE_DEFINE_H__
#define __ENGINE_DEFINE_H__

namespace Engine {
namespace Interface {

class UIENGINE_API IUninheritable {
public:
    IUninheritable(){};
    IUninheritable(const IUninheritable&)            = delete;
    IUninheritable& operator=(const IUninheritable&) = delete;

    virtual ~IUninheritable() = default;
};

template <class OnlyFriend>
class UIENGINE_API ISealable : IUninheritable {
    friend typename OnlyFriend;

private:
    ISealable(){};
};

} // namespace Interface
} // namespace Engine

#endif // !__ENGINE_DEFINE_H__
