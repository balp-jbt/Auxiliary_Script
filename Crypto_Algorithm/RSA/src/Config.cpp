#include "Config.h"

unordered_map<string, string> Config::options;

void Config::load_args(int argc, char* argv[]) {
    for (int i = 0; i < argc; i++) {
        string arg = argv[i];
        string key;
        string value;
       if (arg.substr(0, 2) == "--") {
            key = arg.substr(2);
            value = ((i + 1) < argc && argv[i+1][0] != '-') ? argv[++i] : "";
            options[key] = value;
       } else if (arg[0] == '-') {
            key = arg.substr(1);
            value = ((i + 1) < argc && argv[i+1][0] != '-') ? argv[++i] : "";
            options[key] = value;
       } else {
            throw runtime_error("Unexpected option");
       }
    }
}

string Config::get_opt(string target1, string target2, bool force) {
    if (Config::options.find(target1) != Config::options.end()) {
        return Config::options[target1];
    } else if (Config::options.find(target2) != Config::options.end()) {
        return Config::options[target2];
    }

    if (force) {
        throw runtime_error("need option [" + target1 + "] or [" + target2 + "]");
    }

    return "";
}

bool Config::check_opt(string target1, string target2) {
    if (Config::options.find(target1) != Config::options.end()) {
        return true;
    } else if (Config::options.find(target2) != Config::options.end()) {
        return true;
    }
    return false;
}