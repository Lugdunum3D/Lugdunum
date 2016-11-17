#include <lug/Config.hpp>
#include <lug/System/Exception.hpp>
#include <iostream>

int main() {
    try {
        LUG_EXCEPT(IOException, "My exception worked");
    }
    catch (const lug::System::IOException &e) {
        std::cout << e.getLine() << std::endl;
    }
    return 0;
}
