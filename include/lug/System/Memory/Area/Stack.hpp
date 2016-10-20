#pragma once

#include <lug/System/Export.hpp>
#include <lug/System/Memory/Area/IArea.hpp>

namespace lug {
namespace System {
namespace Memory {
namespace Area {

template <size_t Size>
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
    char _data[Size];

    Page _page{&_data[0], &_data[Size - 1], nullptr, nullptr};
    bool _nextPage{true};
};

#include <lug/System/Memory/Area/Stack.inl>

}
}
}
}
