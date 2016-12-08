#include <lug/Config.hpp>
#include <lug/System/Exception.hpp>
#include <lug/System/Logger.hpp>
#include <iostream>

int main(int, const char*[]) {
    auto logger = lug::System::makeLogger("myLogger");

    //auto logger = lug::System::makeStdoutLogger("myLogger");
    logger->addHandler<lug::System::StdoutHandler>();
    // LoggerFacility.getHandler("test", )
    //logger->addHandler<lug::System::FileHandler>("marcheeeee.log", true);
    logger->info("TESTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT!");
    /*try {
        LUG_EXCEPT(IOException, "My exception worked");
    }
    catch (const lug::System::IOException& e) {
        logger->info("Hello world! Exception at line {} ({})", e.getLine(), e.getDescription());
    }*/
    return 0;
}
