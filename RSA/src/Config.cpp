#include "Config.h"

Config::Config(int argc, char* argv[]) {
    for (int i = 0; i < argc; i++) {
        string arg = argv[i];
        cout << "[arg] " << arg << endl; 
    }
}