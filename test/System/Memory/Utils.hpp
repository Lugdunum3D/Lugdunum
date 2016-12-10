#include <gmock/gmock.h>

namespace {

class NullArena {
public:
    void* allocate(size_t, size_t, size_t, const char*, size_t) {
        return nullptr;
    }

    void free(void*) {}
};

class MockArena {
public:
    MOCK_METHOD5(allocate, void*(size_t size, size_t alignment, size_t offset, const char* file, size_t line));
    MOCK_METHOD1(free, void(void* ptr));
};

class MockObject {
public:
    MockObject() {}

    ~MockObject() {
        destructor();
    }

    MOCK_METHOD0(destructor, void());
};

inline void testMultipleAlignments(const std::function<void(size_t alignment)> &function) {
    function(1);
    function(2);
    function(4);
    function(8);
    function(16);
    function(32);
    function(64);
    function(128);
    function(256);
    function(512);
}

}
