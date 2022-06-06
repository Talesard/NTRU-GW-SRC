#include "NTRU.h"
#include <random>

std::vector<int> slice(const std::vector<int>& v, int start = 0, int end = -1) {
    int oldlen = v.size();
    int newlen;
    if (end == -1 || end >= oldlen) { newlen = oldlen - start; }
    else { newlen = end - start; }
    std::vector<int> nv(newlen);
    for (int i = 0; i < newlen; i++) {
        nv[i] = v[start + i];
    }
    return nv;
}

std::vector<std::string> split_string_to_blocks(std::string str, int block_size) {
    assert(str.length() >= block_size);
    std::vector<std::string> blocks;
    int block_count = str.length() / block_size + (str.length() % block_size == 0 ? 0 : 1);
    for (int i = 0; i < block_count; i++) {
        if (i * block_size > str.length()) {
            block_size = str.length() - (i - 1) * block_size;
        }
        blocks.push_back(str.substr(i * block_size, block_size));
    }
    return blocks;
}

NTRU::NTRU(int _N, int _p, int _q) {
	N = _N;
	p = _p;
	q = _q;
    NTL::ZZ_p::init(NTL::ZZ(q));
    Zx_Ring = NTL::ZZ_pX(NTL::INIT_MONO, N) - 1;
}

std::vector<int> NTRU::cvt_char_to_3_code(char c) {
    std::vector<int> result;
    int ascii_code = static_cast<int>(c);
    if (ascii_code == 0) return std::vector<int> {0};
    int mod = 0;
    while (ascii_code > 0) {
        mod = ascii_code % 3;
        result.push_back(mod);
        ascii_code = ascii_code / 3;
    }
    std::reverse(result.begin(), result.end());
    return result;
}

std::vector<int> NTRU::cvt_string_to_polynomial_coeffs(std::string str) {
    std::vector<int> result;
    const char* str_char = str.c_str();
    for (int i = 0; i < str.length(); i++) {
        std::vector<int> char_3_code = cvt_char_to_3_code(str_char[i]);
        std::vector<int> zeros(6 - char_3_code.size());
        std::fill(zeros.begin(), zeros.end(), 0);
        zeros.insert(zeros.end(), char_3_code.begin(), char_3_code.end());
        result.insert(result.end(), zeros.begin(), zeros.end());
    }
    for (int i = 0; i < result.size(); i++) result[i] -= 1;
    return result;
}

char NTRU::cvt_3_code_to_char(std::vector<int> char_3_code) {
    int char_ascii_code = 0;
    for (int i = 0; i < char_3_code.size(); i++) {
        char_ascii_code += (char_3_code[char_3_code.size() - (i + 1)] * pow(3, i));
    }
    char res = static_cast<char>(char_ascii_code);
    return res;
}

std::string NTRU::cvt_polynomial_coeffs_to_string(std::vector<int> poly_coeffs) {
    std::string res_str = "";
    for (int i = 0; i < poly_coeffs.size(); i++) poly_coeffs[i] += 1;
    int i = 0;
    while (i < poly_coeffs.size()) {
        std::vector<int> vec_slice = slice(poly_coeffs, i, i + 6);
        char c = cvt_3_code_to_char(vec_slice);
        res_str += c;
        i += 6;
    }
    return res_str;
}

std::vector<int> NTRU::random_polynomial_coeffs(int n) {
    std::vector<int> res(n);
    srand((unsigned int)time(NULL));
    const int max = 1;
    const int min = -1;
    for (int i = 0; i < n; i++) {
        res[i] = min + rand() % (max - min + 1);
    }
    return res;
}

NTL::ZZX NTRU::cvt_coeffs_vec_to_ntl_polynomial(std::vector<int> coeffs) {
    NTL::ZZX res_poly;
    for (int i = 0; i < coeffs.size(); i++) {
        NTL::SetCoeff(res_poly, i, coeffs[i]);
    }
    return res_poly;
}

std::vector<int> NTRU::cvt_ntl_polynomial_to_coeffs_vec(NTL::ZZX poly) {
    std::vector<int> res_vec;
    for (int i = 0; i <= NTL::deg(poly); i++) {
        long tmp = 0;
        NTL::conv(tmp, poly[i]);
        res_vec.push_back(static_cast<int>(tmp));
    }
    return res_vec;
}

NTL::ZZX NTRU::random_polynomial(int N, int count_positive, int count_negative) {

    std::vector<int> poly_coeffs_positive(count_positive);
    std::fill(poly_coeffs_positive.begin(), poly_coeffs_positive.end(), 1);

    std::vector<int> poly_coeffs_negative(count_negative);
    std::fill(poly_coeffs_negative.begin(), poly_coeffs_negative.end(), -1);

    std::vector<int> poly_coeffs_zero(N - count_positive - count_negative);
    std::fill(poly_coeffs_zero.begin(), poly_coeffs_zero.end(), 0);

    poly_coeffs_positive.insert(poly_coeffs_positive.end(), poly_coeffs_negative.begin(), poly_coeffs_negative.end());
    poly_coeffs_positive.insert(poly_coeffs_positive.end(), poly_coeffs_zero.begin(), poly_coeffs_zero.end());
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(poly_coeffs_positive.begin(), poly_coeffs_positive.end(), g);

    NTL::ZZX res = cvt_coeffs_vec_to_ntl_polynomial(poly_coeffs_positive);

    return res;
}

void NTRU::create_private_keys() {
    int t = N / 3;
    bool flag = true;
    while (flag) {
        f = random_polynomial(N, t + 1, t);
        g = random_polynomial(N, t, t);
        try {
            self_test_invert();
            flag = false;
        }
        catch (NTL::ArithmeticErrorObject e) {
            //std::cout << e.what() << std::endl;
        }
    }
}

void NTRU::create_public_key() {
    // h = f_q * g (mod q)
    NTL::ZZ_pX f_tmp = cvt_ZZX_to_ZZ_pX(f);
    NTL::ZZ_pX f_tmp_inv = invert_poly(f_tmp);
    NTL::ZZ_pX g_tmp = cvt_ZZX_to_ZZ_pX(g);
    h = NTL::MulMod(f_tmp_inv, g_tmp, Zx_Ring);
}

void NTRU::self_test_invert() {
    NTL::ZZ_p::init(NTL::ZZ(q));
    Zx_Ring = NTL::ZZ_pX(NTL::INIT_MONO, N) - 1;
    NTL::ZZ_pX f_modq = cvt_ZZX_to_ZZ_pX(f);
    NTL::ZZ_pX g_modq = cvt_ZZX_to_ZZ_pX(g);
}

void NTRU::self_test_equals() {
    std::string src_message = "Hello, world!";
    NTL::ZZX message_poly = cvt_coeffs_vec_to_ntl_polynomial(cvt_string_to_polynomial_coeffs(src_message));
    NTL::ZZ_pX encrypted = encrypt(message_poly, true);
    NTL::ZZX decrypted_message_poly = decrypt(encrypted);
    std::string decr_message = cvt_polynomial_coeffs_to_string(cvt_ntl_polynomial_to_coeffs_vec(decrypted_message_poly));
    if (decr_message != src_message) throw NTL::ArithmeticErrorObject("msgs not equal");
}

NTL::ZZ_pX NTRU::cvt_ZZX_to_ZZ_pX(NTL::ZZX poly) {
    NTL::ZZ_pX tmp;
    std::stringstream sstream;
    sstream << poly;
    sstream >> tmp;
    return tmp;
}

NTL::GF2X NTRU::cvt_ZZ_pX_to_GF2X(NTL::ZZ_pX poly) {
    NTL::GF2X tmp;
    std::stringstream sstream;
    sstream << poly;
    sstream >> tmp;
    return tmp;
}

NTL::ZZ_pX NTRU::cvt_GF2X_to_ZZ_pX(NTL::GF2X poly) {
    NTL::ZZ_pX tmp;
    std::stringstream buffer2;
    buffer2 << poly;
    buffer2 >> tmp;
    return tmp;
}

NTL::ZZX NTRU::cvt_ZZ_pX_to_ZZX(NTL::ZZ_pX poly) {
    NTL::ZZ one = NTL::ZZ(1);
    NTL::ZZX tmp;
    NTL::CRT(tmp, one, poly);
    return tmp;
}

NTL::ZZ_pX NTRU::invert_poly(NTL::ZZ_pX poly) {
    int k = 2;
    NTL::GF2X poly_tmp = cvt_ZZ_pX_to_GF2X(poly);
    NTL::GF2X Ring_tmp = NTL::GF2X(NTL::INIT_MONO, N) - 1;
    NTL::GF2X poly2inv = NTL::InvMod(poly_tmp, Ring_tmp);
    NTL::ZZ_p::init(NTL::ZZ(k));
    NTL::ZZ_pX f_inv = cvt_GF2X_to_ZZ_pX(poly2inv);
    NTL::ZZ_pX Zx_Ring;
    while (k < q) {
        k = k * k;
        NTL::ZZ_p::init(NTL::ZZ(k));
        Zx_Ring = NTL::ZZ_pX(NTL::INIT_MONO, N) - 1;
        f_inv = NTL::MulMod(f_inv, 2 - NTL::MulMod(poly, f_inv, Zx_Ring), Zx_Ring);
    }
    NTL::ZZ_p::init(NTL::ZZ(q));
    NTL::ZZ one = NTL::ZZ(1);
    NTL::ZZX tmp;
    NTL::CRT(tmp, one, f_inv);
    return cvt_ZZX_to_ZZ_pX(tmp);
}

NTL::ZZ_pX NTRU::encrypt(NTL::ZZX message, bool check) {
    // e = pr*h+m (mod q)
    int iter_counter = 0;
    while (true) {
        NTL::ZZ_p::init(NTL::ZZ(q));
        Zx_Ring = NTL::ZZ_pX(NTL::INIT_MONO, N) - 1;
        NTL::ZZ_pX r = cvt_ZZX_to_ZZ_pX(random_polynomial(N, N / 3, N / 3));
        NTL::ZZ_pX rh = NTL::MulMod(r, h, Zx_Ring);
        NTL::ZZ_pX m = cvt_ZZX_to_ZZ_pX(message);
        NTL::ZZ_pX e = (p * rh) + m;
        if (!check) return e;
        std::string a = cvt_polynomial_coeffs_to_string(cvt_ntl_polynomial_to_coeffs_vec(decrypt(e)));
        std::string b = cvt_polynomial_coeffs_to_string(cvt_ntl_polynomial_to_coeffs_vec(message));
        if ( a == b || iter_counter > 100) return e;
        iter_counter++;
    }
}

NTL::ZZX NTRU::decrypt(NTL::ZZ_pX encrypted_message) {
    // a = f * e (mod q)
    // m = f_p * a (mod q)
    // fix race condition: tmp ring

    NTL::ZZ_p::init(NTL::ZZ(q));
    NTL::ZZ_pX Zx_Ring_tmp = NTL::ZZ_pX(NTL::INIT_MONO, N) - 1;
    NTL::ZZ_pX f_q = cvt_ZZX_to_ZZ_pX(f);
    NTL::ZZ_pX a_tmp = NTL::MulMod(f_q, encrypted_message, Zx_Ring_tmp);
    NTL::ZZX a = cvt_ZZ_pX_to_ZZX(a_tmp);
    NTL::ZZ_p::init(NTL::ZZ(p));
    Zx_Ring_tmp = NTL::ZZ_pX(NTL::INIT_MONO, N) - 1;
    NTL::ZZ_pX a_p = cvt_ZZX_to_ZZ_pX(a);
    NTL::ZZ_pX f_p = cvt_ZZX_to_ZZ_pX(f);
    NTL::ZZX m = cvt_ZZ_pX_to_ZZX(NTL::MulMod(InvMod(f_p, Zx_Ring_tmp), a_p, Zx_Ring_tmp));
    NTL::ZZ_p::init(NTL::ZZ(q));
    Zx_Ring_tmp = NTL::ZZ_pX(NTL::INIT_MONO, N) - 1;
    return m;
}

void NTRU::save_private_f_to_file(const char* filename) {
    std::ofstream fout(filename, std::ios_base::out | std::ios_base::trunc);
    if (!fout.is_open()) { std::cout << "can't open " << filename << std::endl; exit(-1); }
    for (int i = 0; i <= NTL::deg(f); i++) {
        fout << f[i] << std::endl;
    }
    fout.close();
}

void NTRU::save_private_g_to_file(const char* filename) {
    std::ofstream fout(filename, std::ios_base::out | std::ios_base::trunc);
    if (!fout.is_open()) { std::cout << "can't open " << filename << std::endl; exit(-1); }
    for (int i = 0; i <= NTL::deg(g); i++) {
        fout << g[i] << std::endl;
    }
    fout.close();
}

void NTRU::save_public_h_to_file(const char* filename) {
    std::ofstream fout(filename, std::ios_base::out | std::ios_base::trunc);
    if (!fout.is_open()) { std::cout << "can't open " << filename << std::endl; exit(-1); }
    for (int i = 0; i <= NTL::deg(h); i++) {
        fout << h[i] << std::endl;
    }
    fout.close();
}

void NTRU::save_encrypted_to_file(NTL::ZZ_pX encrypted, const char* filename) {
    std::ofstream fout(filename, std::ios_base::out | std::ios_base::trunc);
    if (!fout.is_open()) { std::cout << "can't open " << filename << std::endl; exit(-1); }
    for (int i = 0; i <= NTL::deg(encrypted); i++) {
        fout << encrypted[i] << std::endl;
    }
    fout.close();
}

void NTRU::blocked_save_encrypted_to_file(std::vector<NTL::ZZ_pX> encrypted_blocks, const char* filename) {
    std::ofstream fout(filename, std::ios_base::out | std::ios_base::trunc);
    if (!fout.is_open()) { std::cout << "can't open " << filename << std::endl; exit(-1); }
    for (int b = 0; b < encrypted_blocks.size(); b++) {
        for (int i = 0; i <= NTL::deg(encrypted_blocks[b]); i++) {
            fout << encrypted_blocks[b][i] << " ";
        }
        fout << std::endl;
    }
    fout.close();
}

void NTRU::load_private_f_from_file(const char* filename) {
    std::fstream input(filename);
    if (!input.is_open()) { std::cout << "can't open " << filename << std::endl; exit(-1); }
    std::vector<int> coeffs;
    while (!input.eof()) {
        std::string s;
        std::getline(input, s);
        if (!s.empty()) coeffs.push_back(std::stoi(s));
    }
    f = cvt_coeffs_vec_to_ntl_polynomial(coeffs);
}

void NTRU::load_private_g_from_file(const char* filename) {
    std::fstream input(filename);
    if (!input.is_open()) { std::cout << "can't open " << filename << std::endl; exit(-1); }
    std::vector<int> coeffs;
    while (!input.eof()) {
        std::string s;
        std::getline(input, s);
        if (!s.empty()) coeffs.push_back(std::stoi(s));
    }
    g = cvt_coeffs_vec_to_ntl_polynomial(coeffs);
}

void NTRU::load_public_h_from_file(const char* filename) {
    std::fstream input(filename);
    if (!input.is_open()) { std::cout << "can't open " << filename << std::endl; exit(-1); }
    std::vector<int> coeffs;
    while (!input.eof()) {
        std::string s;
        std::getline(input, s);
        if (!s.empty()) coeffs.push_back(std::stoi(s));
    }
    for (int i = 0; i < coeffs.size(); i++) {
        NTL::SetCoeff(h, i, coeffs[i]);
    }
}

NTL::ZZ_pX NTRU::load_encrypted_from_file(const char* filename) {
    std::fstream input(filename);
    if (!input.is_open()) { std::cout << "can't open " << filename << std::endl; exit(-1); }
    std::vector<int> coeffs;
    while (!input.eof()) {
        std::string s;
        std::getline(input, s);
        if (!s.empty()) coeffs.push_back(std::stoi(s));
    }
    NTL::ZZ_pX encrypted;
    for (int i = 0; i < coeffs.size(); i++) {
        NTL::SetCoeff(encrypted, i, coeffs[i]);
    }
    return encrypted;
}

std::vector<NTL::ZZ_pX> NTRU::blocked_load_encrypted_from_file(const char* filename) {
    std::vector <NTL::ZZ_pX> encrypted_blocks;
    std::fstream input(filename);
    if (!input.is_open()) { std::cout << "can't open " << filename << std::endl; exit(-1); }

    // blocks loop
    while (!input.eof()) {
        std::string s;
        std::getline(input, s);
        if (!s.empty()) {
            std::vector<int> block_coeffs;
            std::stringstream ss(s);
            std::string item;
            // coeffs loop
            while (std::getline(ss, item, ' ')) {
                block_coeffs.push_back(std::stoi(item));
            }
            // coeffs to zz_px loop
            NTL::ZZ_pX encrypted_block_tmp;
            for (int i = 0; i < block_coeffs.size(); i++) {
                NTL::SetCoeff(encrypted_block_tmp, i, block_coeffs[i]);
            }
            encrypted_blocks.push_back(encrypted_block_tmp);
        }
    }
    return encrypted_blocks;
}

NTL::ZZ_pX NTRU::encrypt_str(std::string str, bool check) {
    std::vector<int> str_coeffs = cvt_string_to_polynomial_coeffs(str);
    NTL::ZZX str_poly = cvt_coeffs_vec_to_ntl_polynomial(str_coeffs);
    NTL::ZZ_pX encrypted_str_poly = encrypt(str_poly, check);
    return encrypted_str_poly;
}

std::string NTRU::decrypt_str(NTL::ZZ_pX encrypted_str_poly) {
    NTL::ZZX str_poly = decrypt(encrypted_str_poly);
    std::vector<int> str_coeffs = cvt_ntl_polynomial_to_coeffs_vec(str_poly);
    std::string str = cvt_polynomial_coeffs_to_string(str_coeffs);
    return str;
}

std::vector<NTL::ZZ_pX> NTRU::blocked_encrypt_str(std::string str, bool check, int block_size) {
    auto src_message_blocks = split_string_to_blocks(str, 12);
    std::vector<NTL::ZZ_pX> encrypted_blocks(src_message_blocks.size());
    char sep = '\\';
#pragma omp parallel for
    for (int i = 0; i < src_message_blocks.size(); i++) {
        encrypted_blocks[i] = encrypt_str(src_message_blocks[i] + sep, true); // !
    }
    return encrypted_blocks;
}

std::string NTRU::blocked_decrypt_str(std::vector<NTL::ZZ_pX> encrypted_blocks) {
    std::vector<std::string> decrypted_blocks(encrypted_blocks.size());
    char sep = '\\';
#pragma omp parallel for
    for (int i = 0; i < encrypted_blocks.size(); i++) {
        decrypted_blocks[i] = decrypt_str(encrypted_blocks[i]);
        decrypted_blocks[i].erase(std::remove(decrypted_blocks[i].begin(), decrypted_blocks[i].end(), sep), decrypted_blocks[i].end());
    }
    std::string decr_message = std::accumulate(decrypted_blocks.begin(), decrypted_blocks.end(), std::string(""));
    return decr_message;
}

// only for perf tests
std::vector<NTL::ZZ_pX> NTRU::blocked_encrypt_str_seq(std::string str, bool check, int block_size) {
    auto src_message_blocks = split_string_to_blocks(str, 12);
    std::vector<NTL::ZZ_pX> encrypted_blocks(src_message_blocks.size());
    char sep = '\\';
    for (int i = 0; i < src_message_blocks.size(); i++) {
        encrypted_blocks[i] = encrypt_str(src_message_blocks[i] + sep, true); // !
    }
    return encrypted_blocks;
}

// only for perf tests
std::string NTRU::blocked_decrypt_str_seq(std::vector<NTL::ZZ_pX> encrypted_blocks) {
    std::vector<std::string> decrypted_blocks(encrypted_blocks.size());
    char sep = '\\';
    for (int i = 0; i < encrypted_blocks.size(); i++) {
        decrypted_blocks[i] = decrypt_str(encrypted_blocks[i]);
        decrypted_blocks[i].erase(std::remove(decrypted_blocks[i].begin(), decrypted_blocks[i].end(), sep), decrypted_blocks[i].end());
    }
    std::string decr_message = std::accumulate(decrypted_blocks.begin(), decrypted_blocks.end(), std::string(""));
    return decr_message;
}
