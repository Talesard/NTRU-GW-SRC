#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono> 
#include "NTRU.h"



std::string generateRandomString(size_t length) {
    const char* charmap = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ;!&?";
    const size_t charmapLength = strlen(charmap);
    auto generator = [&]() { return charmap[rand() % charmapLength]; };
    std::string result;
    result.reserve(length);
    generate_n(back_inserter(result), length, generator);
    return result;
}

std::string read_file_to_string(std::string filename) {
    std::ifstream t(filename);
    std::string str;
    t.seekg(0, std::ios::end);
    str.reserve(t.tellg());
    t.seekg(0, std::ios::beg);
    str.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    return str;
}

void write_string_to_file(std::string str, std::string filename) {
    std::ofstream fout(filename, std::ios_base::out | std::ios_base::trunc);
    if (!fout.is_open()) { std::cout << "can't open " << filename << std::endl; exit(-1); }
    fout << str;
    fout.close();
}

void print_help() {
    std::cout << "Args error. Expected:\n\tkeys <f_file> <g_file> <h_file>\n\tenc <input_file> <h_file> <output_file>\n\tdec <input_file> <f_file> <g_file> <output_file>\n";
}

int CLI(int argc, char* argv[]) {
    NTRU ntru(257, 3, 256);
    if (argc < 2) {
        print_help();
        return -1;
    }
    if (std::string(argv[1]) == "keys") {
        std::string key_f_filename = argv[2];
        std::string key_g_filename = argv[3];
        std::string key_h_filename = argv[4];
        ntru.create_private_keys();
        ntru.create_public_key();
        ntru.save_private_f_to_file(key_f_filename.c_str());
        ntru.save_private_g_to_file(key_g_filename.c_str());
        ntru.save_public_h_to_file(key_h_filename.c_str());
    }
    else if (std::string(argv[1]) == "enc") {
        std::string src_filename = argv[2];
        std::string key_f_filename = argv[3]; // !
        std::string key_g_filename = argv[4]; // !
        std::string key_h_filename = argv[5];
        std::string enc_filename = argv[6];
        ntru.load_public_h_from_file(key_h_filename.c_str());
        ntru.load_private_f_from_file(key_f_filename.c_str()); // !
        ntru.load_private_g_from_file(key_g_filename.c_str()); // !
        std::string src_message = read_file_to_string(src_filename);
        auto encrypted_blocks = ntru.blocked_encrypt_str(src_message, true); // !
        ntru.blocked_save_encrypted_to_file(encrypted_blocks, enc_filename.c_str());
    }
    else if (std::string(argv[1]) == "dec") {
        std::string enc_filename = argv[2];
        std::string key_f_filename = argv[3];
        std::string key_g_filename = argv[4];
        std::string dec_filename = argv[5];
        std::vector<NTL::ZZ_pX> encrypted_blocks;
        encrypted_blocks = ntru.blocked_load_encrypted_from_file(enc_filename.c_str());
        ntru.load_private_f_from_file(key_f_filename.c_str());
        ntru.load_private_g_from_file(key_g_filename.c_str());
        std::string decr_message = ntru.blocked_decrypt_str(encrypted_blocks);
        write_string_to_file(decr_message, dec_filename);
    }
    else {
        print_help();
        return -1;
    }
    return 0;
}

int performance_test() {

    typedef std::chrono::high_resolution_clock Time;
    typedef std::chrono::milliseconds ms;
    typedef std::chrono::duration<float> fsec;

    std::string key_f_filename = "perf_f.txt";
    std::string key_g_filename = "perf_g.txt";
    std::string key_h_filename = "perf_h.txt";
    std::string src_filename = "perf_src.txt";
    std::string enc_filename = "perf_enc.txt";
    std::string dec_filename = "perf_dec.txt";

    NTRU ntru(257, 3, 256);

    std::cout << "Create keys performance." << std::endl;
    auto t0 = Time::now();
    ntru.create_private_keys();
    ntru.create_public_key();
    auto t1 = Time::now();
    ntru.save_private_f_to_file(key_f_filename.c_str());
    ntru.save_private_g_to_file(key_g_filename.c_str());
    ntru.save_public_h_to_file(key_h_filename.c_str());
    ms d = std::chrono::duration_cast<ms>(t1 - t0);
    std::cout << "\t time: " << d.count() << " ms" << std::endl;

    std::cout << "Encrypt performance." << std::endl;
    ntru.load_public_h_from_file(key_h_filename.c_str());
    ntru.load_private_f_from_file(key_f_filename.c_str()); // !
    ntru.load_private_g_from_file(key_g_filename.c_str()); // !
    std::string src_message = read_file_to_string(src_filename);
    t0 = Time::now();
    auto encrypted_blocks = ntru.blocked_encrypt_str_seq(src_message, true); // !
    t1 = Time::now();
    ntru.blocked_save_encrypted_to_file(encrypted_blocks, enc_filename.c_str());
    ms d_seq = std::chrono::duration_cast<ms>(t1 - t0);
    std::cout << "\t time seq: " << d_seq.count() << " ms" << std::endl;
    t0 = Time::now();
    encrypted_blocks = ntru.blocked_encrypt_str(src_message, true); // !
    t1 = Time::now();
    ntru.blocked_save_encrypted_to_file(encrypted_blocks, enc_filename.c_str());
    ms d_par = std::chrono::duration_cast<ms>(t1 - t0);
    std::cout << "\t time par: " << d_par.count() << " ms" << std::endl;

    std::cout << "Decrypt performance." << std::endl;
    ntru.load_private_f_from_file(key_f_filename.c_str());
    ntru.load_private_g_from_file(key_g_filename.c_str());
    encrypted_blocks = ntru.blocked_load_encrypted_from_file(enc_filename.c_str());
    t0 = Time::now();
    std::string decr_message = ntru.blocked_decrypt_str_seq(encrypted_blocks);
    t1 = Time::now();
    write_string_to_file(decr_message, dec_filename);
    d_seq = std::chrono::duration_cast<ms>(t1 - t0);
    std::cout << "\t time seq: " << d_seq.count() << " ms" << std::endl;
    encrypted_blocks = ntru.blocked_load_encrypted_from_file(enc_filename.c_str());
    t0 = Time::now();
    decr_message = ntru.blocked_decrypt_str(encrypted_blocks);
    t1 = Time::now();
    write_string_to_file(decr_message, dec_filename);
    d_par = std::chrono::duration_cast<ms>(t1 - t0);
    std::cout << "\t time par: " << d_par.count() << " ms" << std::endl;
}


int main(int argc, char* argv[]) {
    // return(CLI(argc, argv));
    return(performance_test());
}
