#include <lug/System/Path.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#if defined(LUG_SYSTEM_WINDOWS)

#include <Shlobj.h>
#include <utility>
#include <windows.h>
#include <string>

namespace lug {
namespace System {

using namespace ::testing;

TEST(Path, Root) {
    SetEnvironmentVariable("HOMEDRIVE", "C:\\");
    EXPECT_EQ(Path::root(), "C:\\");
}

TEST(Path, Home) {
    SetEnvironmentVariable("HOMEDRIVE", "C:\\");
    SetEnvironmentVariable("HOMEPATH", "Users\\Lugdunum");
    EXPECT_EQ(Path::home(), "C:\\Users\\Lugdunum");
}

TEST(Path, Cwd) {
    char cwd[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, cwd);
    EXPECT_EQ(Path::cwd(), cwd);
}

TEST(Path, Save) {
    char appDataPath[MAX_PATH];
    SHGetFolderPath(nullptr, CSIDL_APPDATA, nullptr, 0, appDataPath);
    std::string savePath(appDataPath);
    EXPECT_EQ(Path::save("lugdunum"), savePath + "\\lugdunum");
}

}
}

#elif defined(LUG_SYSTEM_LINUX)

#include <pwd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <utility>
#include <unistd.h>

namespace lug {
namespace System {

using namespace ::testing;

TEST(Path, Root) {
    EXPECT_EQ(Path::root(), "/");
}

TEST(Path, Home) {
    struct passwd* pw = getpwuid(getuid());
    EXPECT_EQ(Path::home(), pw->pw_dir);
}

TEST(Path, Cwd) {
    chdir("/");
    EXPECT_EQ(Path::cwd(), "/");
}

TEST(Path, Save) {
    struct passwd* pw = getpwuid(getuid());
    EXPECT_EQ(Path::save("lugdunum"), pw->pw_dir + "/lugdunum");
}

}
}

#endif
