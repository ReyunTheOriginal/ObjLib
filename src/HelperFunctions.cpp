#include "HelperFunctions.hpp"

#ifdef _WIN32
#include <windows.h>
#elif __APPLE__
#include <mach-o/dyld.h>
#elif __linux__
#include <unistd.h>
#endif

std::random_device rnd;
std::mt19937 gen(rnd());

namespace obj{
    //captialized string helpers
    std::string Lower(std::string stri){
        std::transform(stri.begin(), stri.end(), stri.begin(), ::tolower);
        return stri;
    }
    std::string Upper(std::string stri){
        std::transform(stri.begin(), stri.end(), stri.begin(), ::toupper);
        return stri;
    }

    //Platform Dynamic path to the executale to the app
    std::string exePath(){
        namespace fs = std::filesystem;
        #ifdef _WIN32
            char buffer[MAX_PATH];
            GetModuleFileNameA(NULL, buffer, MAX_PATH);
            return fs::path(buffer).parent_path().string();
        #elif __linux__
            char buffer[1024];
            ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
            if (len != -1) {
                buffer[len] = '\0';
                return fs::path(buffer).parent_path().string();
            }
        #elif __APPLE__
            char buffer[1024];
            uint32_t size = sizeof(buffer);
            if (_NSGetExecutablePath(buffer, &size) == 0) {
                return fs::path(buffer).parent_path().string();
            }
        #endif

        return fs::current_path().string();
    }
}