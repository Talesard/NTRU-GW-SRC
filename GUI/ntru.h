#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <NTL/ZZ_pE.h>
#include <NTL/ZZX.h>
#include <NTL/GF2X.h>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cassert>
#include <numeric>

// helper function that prints a vector
template <typename T>
void print_vec(std::vector<T> vec) {
    for (int i = 0; i < vec.size(); i++) std::cout << vec[i] << " ";
    std::cout << std::endl;
}

// split string to blocks
std::vector<std::string> split_string_to_blocks(std::string str, int block_size);

class NTRU {

public:
    // Counstructor. N - deg, _p, _q - params
    NTRU(int _N, int _p, int _q);

    // encrypt string message to poly
    NTL::ZZ_pX encrypt_str(std::string str, bool check = true);

    // decrypt poly message to string
    std::string decrypt_str(NTL::ZZ_pX encrypted_str_poly);

    // blocked (and parallel) encrypt string message to poly
    std::vector<NTL::ZZ_pX> blocked_encrypt_str(std::string str, bool check = true, int block_size = 12);

    // blocked (and parallel) decrypt poly message to string
    std::string blocked_decrypt_str(std::vector<NTL::ZZ_pX> encrypted_blocks);

    // blocked (seq) encrypt string message to poly
    std::vector<NTL::ZZ_pX> blocked_encrypt_str_seq(std::string str, bool check = true, int block_size = 12);

    // blocked (seq) decrypt (seq) poly message to string
    std::string blocked_decrypt_str_seq(std::vector<NTL::ZZ_pX> encrypted_blocks);

    // save private key f to file private_f.txt
    void save_private_f_to_file(const char* filename = "private_f.txt");

    // save private key g to file private_g.txt
    void save_private_g_to_file(const char* filename = "private_g.txt");

    // save public key h to file public_h.txt
    void save_public_h_to_file(const char* filename = "public_h.txt");

    // save encrypted message to file encrypted.txt
    void save_encrypted_to_file(NTL::ZZ_pX encrypted, const char* filename = "encrypted.txt");

    // save encrypted blocked message to file encrypted.txt
    void blocked_save_encrypted_to_file(std::vector<NTL::ZZ_pX> encrypted_blocks, const char* filename = "encrypted.txt");

    // read private key f from file private_f.txt
    void load_private_f_from_file(const char* filename = "private_f.txt");

    // read private key g from file private_g.txt
    void load_private_g_from_file(const char* filename = "private_g.txt");

    // read public key h from file public_h.txt
    void load_public_h_from_file(const char* filename = "public_h.txt");

    // read encrypted message from file encrypted.txt
    NTL::ZZ_pX load_encrypted_from_file(const char* filename = "encrypted.txt");

    // read encrypted blocked message from file encrypted.txt
    std::vector<NTL::ZZ_pX> blocked_load_encrypted_from_file(const char* filename = "encrypted.txt");

    // create private keys f and g
    void create_private_keys();

    // calculate public key h
    void create_public_key();

private:
    int N, p, q; // NTRU params
    NTL::ZZX f; // private key
    NTL::ZZX g; // private key
    NTL::ZZ_pX h; // public key
    NTL::ZZ_pX Zx_Ring; // ring

    // convert char to ternary code vector
    std::vector<int> cvt_char_to_3_code(char c);

    // convert string to polynomial coefficients vector (-1, 0, 1)
    std::vector<int> cvt_string_to_polynomial_coeffs(std::string str);

    // convert ternary code vector to char
    char cvt_3_code_to_char(std::vector<int> char_3_code);

    // convert polynomial coefficients vector to string
    std::string cvt_polynomial_coeffs_to_string(std::vector<int> poly_coeffs);

    // get random polynomial coeffs vector (-1, 0, 1)
    std::vector<int> random_polynomial_coeffs(int n);

    // convert coeffs vector to NTL polynomial
    NTL::ZZX cvt_coeffs_vec_to_ntl_polynomial(std::vector<int> coeffs);

    // convert NTL polynomial to coeffs vector
    std::vector<int> cvt_ntl_polynomial_to_coeffs_vec(NTL::ZZX poly);

    // random poly deg N with count_positive 1, count_negative -1
    NTL::ZZX random_polynomial(int N, int count_positive, int count_negative);

    // test poly can be invert
    void self_test_invert();

    // test correct
    void self_test_equals();

    // covert ZZX to ZZ_pX
    NTL::ZZ_pX cvt_ZZX_to_ZZ_pX(NTL::ZZX poly);

    // covert ZZ_pX to GF2X
    NTL::GF2X cvt_ZZ_pX_to_GF2X(NTL::ZZ_pX poly);

    // convert GF2X to ZZ_pX
    NTL::ZZ_pX cvt_GF2X_to_ZZ_pX(NTL::GF2X poly);

    // convert ZZ_pX to ZZX
    NTL::ZZX cvt_ZZ_pX_to_ZZX(NTL::ZZ_pX poly);

    // invert poly, before calling, select a module for the ring
    NTL::ZZ_pX invert_poly(NTL::ZZ_pX poly);

    // encrypt NTL::ZZX message
    NTL::ZZ_pX encrypt(NTL::ZZX message, bool check);

    // decrypt NTL::ZZ_pX encrypted message
    NTL::ZZX decrypt(NTL::ZZ_pX encrypted_message);
};
