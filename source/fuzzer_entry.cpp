import jork.parser;
#include <string>

extern "C" int LLVMFuzzerTestOneInput(const char* data, long long size) {
    jork::parse_command(std::string(data, size));
    return 0;
}