#pragma once

#include <lug/System/Export.hpp>
#include <lug/System/Memory/Area/IArea.hpp>

namespace lug {
namespace System {
namespace Memory {
namespace Area {

template <size_t size>
class Stack : public IArea {
public:
    Stack() = default;
    Stack(const Stack&) = delete;
    Stack(Stack&&) = delete;

    Stack& operator=(const Stack&) = delete;
    Stack& operator=(Stack&&) = delete;

    ~Stack() = default;

    Page* requestNextPage() override;

private:
    char _data[size];

    Page _page{&_data[0], &_data[size - 1], nullptr};
    bool _nextPage{true};
};

#include <lug/System/Memory/Area/Stack.inl>

}
}
}
}
