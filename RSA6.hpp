#include <iostream>
#include <fstream>
#include <stdbool.h>
#include <vector>
#include <mpirxx.h>

void printVector(std::vector<mpz_class> vect);

void findCoprime(mpz_t num, unsigned int lower, mpz_t coprime);

void RSA_CRT(mpz_t data, mpz_t p, mpz_t q, mpz_t d, mpz_t result);

mpz_t * RSA_keygen(mpz_t p, mpz_t q);

std::vector<mpz_class>* RSA_encrypt(std::string data, mpz_t n, mpz_t e);

std::string* RSA_decrypt(std::vector<mpz_class> data, mpz_t n, mpz_t d);

std::string* RSA_decrypt(std::vector<mpz_class> data, mpz_t n, mpz_t d, mpz_t p, mpz_t q);

mpz_t* RSA_primegen(unsigned int key_bits);

std::vector<mpz_class> * textToVector(const std::string filename);