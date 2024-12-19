#include "RSA.h"
#include "Timer.h"

int main (int argc, char*argv[]) {
    argc-- , argv++;
    Config::load_args(argc, argv);

    if(Config::check_opt("h", "help")) {
        cout << "-m --mode " << "[key-gen | encrypt | decrypt | sign | verify | key-gen-test]" << endl;
        cout    << "------------------ mode key-gen ------------------"
                << endl << "[-l | --key-len] " << "[optional] " << "--pub-path --priv-path" << endl
                << "Gnerate key-pairs with given key-len, store them in files" << endl;
        
        cout << endl << endl;

        cout    << "------------------ mode encrypt ------------------"
                << endl << "[optional] " << "--pub-path " << "[-i | --in-path] " << "[-o | --out-path] " << endl
                << "Encrypt data with given file, public key, store result in files" << endl;

        cout    << "------------------ mode decrypt ------------------" << endl
                << "[optional] " << "--pub-path " << "--priv-path "<< "[-i | --in-path] " << "[-o | --out-path] " << endl
                << "Decrypt data with given file, public key, private key, store result in files" << endl;

        cout << endl << endl;

        cout    << "------------------ mode sign ------------------"<< endl
                << "[optional] " << "--pub-path " << "--priv-path "<< "[-i | --in-path] " << "[-o | --out-path] " << endl
                << "Sign data with given file, public key, private key, store result in files" << endl;

        cout << endl << endl;

        cout    << "------------------ mode verify ------------------"<< endl
                << "[optional] " << "--pub-path " << "[-i | --in-path] " << "[-o | --out-path] " << endl
                << "Sign data with given file, public key, store result in files" << endl;
        
        cout << endl << endl;

        cout    << "------------------ mode key-gen-test ------------------"<< endl
                << "[-l | --key-len] " << "[-t --times ] "  << "[optional] "  << "[-o | -out-path]" << endl
                << "Test time cost in generate key once for given times";

        cout << endl << endl;
        cout << "****************** See ./demo/demo.sh for an example ******************" << endl;
        return 0;
    }

    string mode = Config::get_opt("m", "mode", true);

    if (mode == "key-gen") {
        string key_len_s = Config::get_opt("l", "key-len", true);
        int key_len = 0;
        for (size_t i = 0; i < key_len_s.size(); i++) {
            key_len = key_len * 10 + key_len_s[i] - '0';
        }

        if (! (key_len > 88 && key_len % 4 == 0)) {
            throw runtime_error("Too small or incorrect key-len, please enter a number that is multiple 4 and greater than 88");
        }

        string pub_key_path = Config::get_opt("pub-path", "", false);
        if (pub_key_path == "") {
            pub_key_path = "./id_rsa.pub";
        }

        string priv_key_path = Config::get_opt("priv-path", "", false);
        if (priv_key_path == "") {
            priv_key_path = "./id_rsa";
        }

        RSA::generate_key(key_len, pub_key_path, priv_key_path);
        
        cout << "[Finshed generate_key]" << endl;
        cout << "[public key path] " << pub_key_path << endl;
        cout << "[priv key path] " << pub_key_path << endl;
    }

    else if (mode == "encrypt") {
        string pub_key_path = Config::get_opt("pub-path", "", false);
        if (pub_key_path == "") {
            pub_key_path = "./id_rsa.pub";
        }

        string in_path = Config::get_opt("i", "in-path", false);
        if (in_path == "") {
            in_path = "./plain_context";
        }

        string out_path = Config::get_opt("o", "out-path", false);
        if (out_path == "") {
            out_path = "./cipher_context";
        }

        RSA::encrypt(true, pub_key_path, "", in_path, out_path);
        
        cout << "[Finshed encrpt]" << endl;
        cout << "[plain text path] " << in_path << endl;
        cout << "[cipher context path] " << out_path << endl;
    }

    else if (mode == "decrypt") {
        string pub_key_path = Config::get_opt("pub-path", "", false);
        if (pub_key_path == "") {
            pub_key_path = "./id_rsa.pub";
        }

        string priv_key_path = Config::get_opt("priv-path", "", false);
        if (priv_key_path == "") {
            priv_key_path = "./id_rsa";
        }

        string in_path = Config::get_opt("i", "in-path", false);
        if (in_path == "") {
            in_path = "./cipher_context";
        }

        string out_path = Config::get_opt("o", "out-path", false);
        if (out_path == "") {
            out_path = "./decrypt_context";
        }

        RSA::decrypt(true, pub_key_path, priv_key_path, in_path, out_path);
        
        cout << "[Finshed encrpt]" << endl;
        cout << "[cipher context path] " << in_path << endl;
        cout << "[decrypt context path] " << out_path << endl;
    }

    else if (mode == "sign") {
        string pub_key_path = Config::get_opt("pub-path", "", false);
        if (pub_key_path == "") {
            pub_key_path = "./id_rsa.pub";
        }

        string priv_key_path = Config::get_opt("priv-path", "", false);
        if (priv_key_path == "") {
            priv_key_path = "./id_rsa";
        }

        string in_path = Config::get_opt("i", "in-path", false);
        if (in_path == "") {
            in_path = "./plain_context";
        }

        string out_path = Config::get_opt("o", "out-path", false);
        if (out_path == "") {
            out_path = "./sign_context";
        }

        RSA::encrypt(false, pub_key_path, priv_key_path, in_path, out_path);
        
        cout << "[Finshed sign]" << endl;
        cout << "[plain text path] " << in_path << endl;
        cout << "[sign context path] " << out_path << endl;
    }

    else if (mode == "verify") {
        string pub_key_path = Config::get_opt("pub-path", "", false);
        if (pub_key_path == "") {
            pub_key_path = "./id_rsa.pub";
        }

        string in_path = Config::get_opt("i", "in-path", false);
        if (in_path == "") {
            in_path = "./sign_context";
        }

        string out_path = Config::get_opt("o", "out-path", false);
        if (out_path == "") {
            out_path = "./verify_context";
        }

        RSA::decrypt(false, pub_key_path, "", in_path, out_path);
        
        cout << "[Finshed verify]" << endl;
        cout << "[cipher context path] " << in_path << endl;
        cout << "[verify context path] " << out_path << endl;
    }

    else if (mode == "key-gen-test") {
        string key_len_s = Config::get_opt("l", "key-len", true);
        int key_len = 0;
        for (size_t i = 0; i < key_len_s.size(); i++) {
            key_len = key_len * 10 + key_len_s[i] - '0';
        }

        if (! (key_len > 88 && key_len % 4 == 0)) {
            throw runtime_error("Too small or incorrect key-len, please enter a number that is multiple 4 and greater than 88");
        }

        string out_path = Config::get_opt("o", "out-path", false);
        if (out_path == "") {
            out_path = "./test/log";
        }

        string threshold_s = Config::get_opt("t", "times", true);
        int threshold = 0;
        for (size_t i = 0; i < threshold_s.size(); i++) {
            threshold = threshold * 10 + threshold_s[i] - '0';
        }
        
        ofstream out_file(out_path);
        if (! out_file) {
            throw runtime_error("Error in opening target file!\n");
        }

        Timer timer;
        double unit_cost, total_cost = 0;

        for (int i = 0; i < threshold; i++) {
            timer.start();
            RSA::generate_key(key_len, "./test/id_rsa_" + to_string(i) + ".pub", "./test/id_rsa_" + to_string(i));
            timer.stop();
            unit_cost = timer.elapsed();
            total_cost += unit_cost;
            out_file << "[round " + to_string(i) + "]" << unit_cost << endl;
            cout << "\r" << std::setw(10) << i << " / " << threshold << " finished";
            cout.flush();
        }
        out_file << endl << "[total cost] " << total_cost << endl;
        
        
        cout << "[Finshed test]" << endl;
        cout << "[All] Time " << total_cost << "s, for " << threshold << "rounds" << endl;
    } else {
        throw runtime_error("Please input correct mode, use -h to see them");
    }

    return 0;
}