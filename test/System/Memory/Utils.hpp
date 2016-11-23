#include <gmock/gmock.h>

using ::testing::_;
using ::testing::Ge;
using ::testing::Return;
using ::testing::Invoke;

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

}

#define TEST_MULTIPLE_ALIGNMENTS(macro) \
    macro(1) \
    macro(2) \
    macro(4) \
    macro(8) \
    macro(16) \
    macro(32) \
    macro(64) \
    macro(128) \
    macro(256) \
    macro(512)
