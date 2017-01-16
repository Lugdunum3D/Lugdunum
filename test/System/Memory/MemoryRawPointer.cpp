#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <lug/System/Memory.hpp>
#include <System/Memory/Utils.hpp>

using namespace ::testing;

TEST(MemoryRawPointer, One) {
    auto testRawPointerOne = [](size_t alignment) {
        const char* file = "Test";
        const size_t line = 42;

        MockArena arena;
        char buffer[4096];

        EXPECT_CALL(arena, allocate(_, alignment, _, file, line))
            .Times(2)
            .WillRepeatedly(Return(&(buffer)));

        EXPECT_CALL(arena, free(&(buffer)))
            .Times(2);

        {
            int* ptr = lug::System::Memory::new_one<int>(alignment, file, line, arena);
            ASSERT_NE(ptr, nullptr);
            lug::System::Memory::delete_one(ptr, arena);
        }

        {
            int* ptr = lug::System::Memory::new_one<int>(alignment, file, line, arena, 42);
            ASSERT_NE(ptr, nullptr);
            ASSERT_EQ(*ptr, 42);
            lug::System::Memory::delete_one(ptr, arena);
        }
    };

    auto testRawPointerOneNull = [](size_t alignment) {
        const char* file = "Test";
        const size_t line = 42;

        NullArena arena;

        {
            int* ptr = lug::System::Memory::new_one<int>(alignment, file, line, arena);
            ASSERT_EQ(ptr, nullptr);
            lug::System::Memory::delete_one(ptr, arena);
        }

        {
            int* ptr = lug::System::Memory::new_one<int>(alignment, file, line, arena, 42);
            ASSERT_EQ(ptr, nullptr);
            lug::System::Memory::delete_one(ptr, arena);
        }
    };

    auto testRawPointerOneDtr = [](size_t alignment) {
        const char* file = "Test";
        const size_t line = 42;

        MockArena arena;
        char buffer[4096];

        EXPECT_CALL(arena, allocate(_, alignment, _, file, line))
            .Times(1)
            .WillRepeatedly(Return(&(buffer)));

        EXPECT_CALL(arena, free(&(buffer)))
            .Times(1);

        MockObject* ptr = lug::System::Memory::new_one<MockObject>(alignment, file, line, arena);

        ASSERT_NE(ptr, nullptr);

        EXPECT_CALL(*ptr, destructor())
            .Times(1);

        lug::System::Memory::delete_one(ptr, arena);
    };

    testMultipleAlignments(testRawPointerOne);
    testMultipleAlignments(testRawPointerOneNull);
    testMultipleAlignments(testRawPointerOneDtr);
}

TEST(MemoryRawPointer, ArrayPOD) {
    auto testRawPointerArrayPod = [](size_t alignment) {
        const char* file = "Test";
        const size_t line = 42;
        MockArena arena;
        char buffer[4096];
        {
            EXPECT_CALL(arena, allocate(_, alignment, _, file, line))
                .Times(1)
                .WillRepeatedly(Return(&(buffer)));
            EXPECT_CALL(arena, free(&(buffer)))
                .Times(1);
            int* ptr = lug::System::Memory::new_array<int>(alignment, 5, file, line, arena);
            ASSERT_NE(ptr, nullptr);
            lug::System::Memory::delete_array(ptr, arena);
        }
        {
            EXPECT_CALL(arena, allocate(0, alignment, _, file, line))
                .Times(1)
                .WillRepeatedly(Return(nullptr));
            EXPECT_CALL(arena, free(nullptr))
                .Times(1);
            int* ptr = lug::System::Memory::new_array<int>(alignment, 0, file, line, arena);
            ASSERT_EQ(ptr, nullptr);
            lug::System::Memory::delete_array(ptr, arena);
        }
    };

    auto testRawPointerArrayPodNull = [](size_t alignment) {
        const char* file = "Test";
        const size_t line = 42;
        NullArena arena;
        {
            int* ptr = lug::System::Memory::new_array<int>(alignment, 5, file, line, arena);
            ASSERT_EQ(ptr, nullptr);
            lug::System::Memory::delete_one(ptr, arena);
        }
        {
            int* ptr = lug::System::Memory::new_array<int>(alignment, 0, file, line, arena);
            ASSERT_EQ(ptr, nullptr);
            lug::System::Memory::delete_one(ptr, arena);
        }
    };

    testMultipleAlignments(testRawPointerArrayPod);
    testMultipleAlignments(testRawPointerArrayPodNull);
}

TEST(MemoryRawPointer, ArrayNonPOD) {
    struct FakeNonPOD {
        FakeNonPOD(int foo) : bar{foo} {};

        int bar;
    };

    auto testRawPointerArrayNonPod = [](size_t alignment) {
        const char* file = "Test";
        const size_t line = 42;

        MockArena arena;
        char buffer[4096];

        {
            EXPECT_CALL(arena, allocate(_, alignment, _, file, line))
                .Times(1)
                .WillRepeatedly(Return(&(buffer)));

            EXPECT_CALL(arena, free(&(buffer)))
                .Times(1);

            FakeNonPOD* ptr = lug::System::Memory::new_array<FakeNonPOD>(alignment, 5, file, line, arena, 42);

            ASSERT_NE(ptr, nullptr);

            for (uint8_t i = 0; i < 5; ++i) {
                ASSERT_EQ(ptr[i].bar, 42);
            }

            lug::System::Memory::delete_array(ptr, arena);
        }

        {
            EXPECT_CALL(arena, allocate(_, alignment, _, file, line))
                .Times(0);

            EXPECT_CALL(arena, free(nullptr))
                .Times(1);

            FakeNonPOD* ptr = lug::System::Memory::new_array<FakeNonPOD>(alignment, 0, file, line, arena, 42);
            ASSERT_EQ(ptr, nullptr);
            lug::System::Memory::delete_array(ptr, arena);
        }
    };

    auto testRawPointerArrayNonPodNull = [](size_t alignment) {
        const char* file = "Test";
        const size_t line = 42;

        NullArena arena;

        {
            FakeNonPOD* ptr = lug::System::Memory::new_array<FakeNonPOD>(alignment, 5, file, line, arena, 42);
            ASSERT_EQ(ptr, nullptr);
            lug::System::Memory::delete_one(ptr, arena);
        }

        {
            FakeNonPOD* ptr = lug::System::Memory::new_array<FakeNonPOD>(alignment, 0, file, line, arena, 42);
            ASSERT_EQ(ptr, nullptr);
            lug::System::Memory::delete_one(ptr, arena);
        }
    };

    auto testRawPointerArrayNonPodDtr = [](size_t alignment) {
        const char* file = "Test";
        const size_t line = 42;

        MockArena arena;
        char buffer[4096];

        EXPECT_CALL(arena, allocate(_, alignment, _, file, line))
            .Times(1)
            .WillRepeatedly(Return(&(buffer)));

        EXPECT_CALL(arena, free(&(buffer)))
            .Times(1);

        MockObject* ptr = lug::System::Memory::new_array<MockObject>(alignment, 5, file, line, arena);

        ASSERT_NE(ptr, nullptr);

        for (uint8_t i = 0; i < 5; ++i) {
            EXPECT_CALL(ptr[i], destructor())
                .Times(1);
        }

        lug::System::Memory::delete_array(ptr, arena);
    };

    testMultipleAlignments(testRawPointerArrayNonPod);
    testMultipleAlignments(testRawPointerArrayNonPodNull);
    testMultipleAlignments(testRawPointerArrayNonPodDtr);
}
