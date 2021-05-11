#include <iostream>
#include <fstream>
#include <stdbool.h>
#include <vector>
#include <mpirxx.h>

using namespace std;

void printVector(vector<mpz_class> vect) {
    if (vect.size() == 0) {
        cout << "{}" << endl;
        return;
    }
    cout << "{";
    for (unsigned int i = 0; i < vect.size() - 1; i++) { // vectors shorter than UINT_MAX
        cout << vect.at(i) << ", ";
    }
    cout << vect.at(vect.size() - 1) << "}" << endl;
}

void findCoprime(mpz_t num, unsigned int lower, mpz_t coprime) {
    mpz_t gcd, i;
    mpz_init(gcd);
    mpz_init(i);

    for (mpz_set_ui(i, lower); mpz_cmp(i, num) < 0; mpz_add_ui(i, i, 1)) {
        mpz_gcd(gcd, i, num);
        if (mpz_cmp_ui(gcd, 1) == 0) {
            mpz_set(coprime, i);
            mpz_clear(gcd);
            mpz_clear(i);
            return;
        }
    }
    cout << "Error: No coprime found." << endl;
    mpz_clear(gcd);
    mpz_clear(i);
    return;
}

// https://en.wikipedia.org/wiki/RSA_(cryptosystem)
void RSA_CRT(mpz_t data, mpz_t p, mpz_t q, mpz_t d, mpz_t result) {
    mpz_t d_p, d_q, q_inv, m_1, m_2, h;
    mpz_t p_less, q_less;
    mpz_init(d_p);
    mpz_init(d_q);
    mpz_init(q_inv);
    mpz_init(m_1);
    mpz_init(m_2);
    mpz_init(h);

    mpz_init_set(p_less, p);
    mpz_init_set(q_less, q);
    mpz_sub_ui(p_less, p_less, 1);
    mpz_sub_ui(q_less, q_less, 1);

    mpz_mod(d_p, d, p_less);
    mpz_mod(d_q, d, q_less);
    mpz_invert(q_inv, d_p, p);
    mpz_powm(m_1, data, d_p, p);
    mpz_powm(m_2, data, d_q, q);

    mpz_sub(h, m_1, m_2);
    mpz_mul(h, q_inv, h);
    mpz_mod(h, h, p);

    mpz_mul(result, h, q);
    mpz_add(result, result, m_2);

    // cleanup
    mpz_clear(d_p);
    mpz_clear(d_q);
    mpz_clear(q_inv);
    mpz_clear(m_1);
    mpz_clear(m_2);
    mpz_clear(h);
    mpz_clear(p_less);
    mpz_clear(q_less);
}

mpz_t * RSA_keygen(mpz_t p, mpz_t q) {
    gmp_randstate_t state;
    gmp_randinit_default(state);

    mpz_t n, lambda, e, d, p_less, q_less;
    mpz_init(n);
    mpz_init(lambda);
    mpz_init(e);
    mpz_init(d);
    mpz_init_set(p_less, p);
    mpz_init_set(q_less, q);
    mpz_sub_ui(p_less, p_less, 1);
    mpz_sub_ui(q_less, q_less, 1);

    if (mpz_likely_prime_p(p, state, 0) == 0 || mpz_likely_prime_p(q, state, 0) == 0) {
        cout << "Error: Number(s) are not prime." << endl;
        mpz_clear(n);
        mpz_clear(lambda);
        mpz_clear(e);
        mpz_clear(d);
        mpz_clear(p_less);
        mpz_clear(q_less);
        return NULL;
    }

    mpz_mul(n, p, q);
    mpz_lcm(lambda, p_less, q_less);
    // by default, sets e to 2^16+1, but can also be found using coprime function (less secure)
    //findCoprime(lambda, 2, e);
    mpz_set_ui(e, 65537);
    mpz_invert(d, e, lambda);
    if (mpz_cmp_ui(d, 0) < 0) {
        mpz_add(d, d, lambda);
    }

    cout << "Public key: (n = " << n << ", e = " << e << ")" << endl;
    cout << "Private key: (n = " << n << ", d = " << d << ")" << endl;

    mpz_t * keys = new mpz_t[3];
    for (int i = 0; i < 3; i++) {
        mpz_init(keys[i]);
    }
    mpz_set(keys[0], n);
    mpz_set(keys[1], e);
    mpz_set(keys[2], d);

    // cleanup
    mpz_clear(n);
    mpz_clear(lambda);
    mpz_clear(e);
    mpz_clear(d);
    mpz_clear(p_less);
    mpz_clear(q_less);

    return keys;
}

vector<mpz_class>* RSA_encrypt(string data, mpz_t n, mpz_t e) {
    vector<mpz_class>* dataVector = new vector<mpz_class>;
    mpz_t num;
    mpz_init(num);
    for (unsigned int i = 0; i < data.size(); i++) {
        mpz_set_ui(num, data.at(i));
        mpz_powm(num, num, e, n);
        dataVector->push_back(mpz_class(num));
    }
    mpz_clear(num);
    return dataVector;
}

// overloaded to allow optional passing of p and q (faster)
string* RSA_decrypt(vector<mpz_class> data, mpz_t n, mpz_t d) {
    string* dataString = new string;
    mpz_t num;
    mpz_init(num);
    for (unsigned int i = 0; i < data.size(); i++) {
        mpz_powm(num, data.at(i).get_mpz_t(), d, n);
        //cout << data.at(i) << " ^ " << d << " mod " << n << " = " << num << endl;
        dataString->push_back((char)mpz_get_ui(num));
    }
    return dataString;
}

string* RSA_decrypt(vector<mpz_class> data, mpz_t n, mpz_t d, mpz_t p, mpz_t q) {
    string* dataString = new string;
    mpz_t num;
    mpz_init(num);
    for (unsigned int i = 0; i < data.size(); i++) {
        RSA_CRT(data.at(i).get_mpz_t(), p, q, d, num);
        //cout << data.at(i) << " ^ " << d << " mod " << n << " = " << num << endl;
        dataString->push_back((char)mpz_get_ui(num));
    }
    mpz_clear(num);
    return dataString;
}

mpz_t* RSA_primegen(unsigned int key_bits) {
    mpz_t p1, p2;
    mpz_init(p1);
    mpz_init(p2);
    gmp_randstate_t state;
    gmp_randinit_default(state);

    srand(time(NULL));
    gmp_randseed_ui(state, rand() % UINT_MAX); // pseudo-random

    while (mpz_likely_prime_p(p1, state, 0) == 0) {
        mpz_urandomb(p1, state, key_bits/2);
    }
    while (mpz_likely_prime_p(p2, state, 0) == 0) {
        mpz_urandomb(p2, state, key_bits/2);
    }
    mpz_t* keys = new mpz_t[2];
    mpz_init(keys[0]);
    mpz_init(keys[1]);
    mpz_set(keys[0], p1);
    mpz_set(keys[1], p2);

    // cleanup
    mpz_clear(p1);
    mpz_clear(p2);
    cout << "Prime numbers: " << keys[0] << ", " << keys[1] << endl;
    return keys;
}

vector<mpz_class> * textToVector(const string filename) {
    ifstream file(filename);
    if (file.fail()) {
        cout << "Error: File not found." << endl;
        return NULL;
    }

    string numstr;
    vector<mpz_class> * vect = new vector<mpz_class>;

    while (!file.eof()) {
        char c = file.get();
        if (c >= '0' && c <= '9') {
            numstr += c;
        } else if (c == ',' || c == '}') {
            mpz_class num;
            num = numstr;
            vect -> push_back(num);
            numstr = "";
        }
    }
    file.close();
    return vect;
}