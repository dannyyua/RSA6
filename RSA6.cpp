#include <iostream>
#include <stdbool.h>
#include <cmath>
#include <vector>
#include "BigNumber.hpp"

using namespace std;
using pr0crustes::BigNumber;

bool isPrime(BigNumber num) {
    if (num == 0 || num == 1) {
        return false;
    }
    for (BigNumber i = 2; i < num.pow(0.5); i++) {
        if (num % i == 0) {
            return false;
        }
    }
    return true;
}

void printVector(vector<BigNumber> vect) {
    if (vect.size() == 0) {
        cout << "{}" << endl;
        return;
    }
    cout << "{";
    for (unsigned long long i = 0; i < vect.size()-1; i++) {
        cout << vect.at(i) << ", ";
    }
    cout << vect.at(vect.size()-1) << "}" << endl;
}

BigNumber gcd(BigNumber num1, BigNumber num2) {
    if (num1 == 0 || num2 == 0) {
        cout << "Error: Numbers must be non-zero." << endl;
        return 0;
    } else if (max(num1, num2) % min(num1, num2) == 0) {
        return min(num1, num2);
    } else {
        return gcd(min(num1, num2), max(num1, num2) % min(num1, num2));
    }
}

BigNumber lcm(BigNumber num1, BigNumber num2) {
    if (num1 == 0 || num2 == 0) {
        cout << "Error: Numbers must be non-zero." << endl;
        return 0;
    } else {
        return num1*num2/gcd(num1, num2);
    }
}

BigNumber findCoprime(BigNumber num, BigNumber lower) {
    for (BigNumber i = lower; i < num; i++) {
        if (gcd(i, num) == 1) {
            return i;
        }
    }
    cout << "Error: No coprime found." << endl;
    return 0;
}

// Extended euclidean algorithm
// https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm
BigNumber findInverse(BigNumber num1, BigNumber num2) {
    BigNumber old_r = num1, r = num2;
    BigNumber old_s = 1, s = 0;
    BigNumber old_t = 0, t = 1;
    BigNumber temp;
    
    while (r != 0) {
        BigNumber quotient = old_r/r;
		
        temp = r;
        r = old_r - quotient*r;
        old_r = temp;
		
        temp = s;
        s = old_s - quotient*s;
        old_s = temp;
		
        temp = t;
        t = old_t - quotient*t;
        old_t = temp;
    }

    if (max(old_s, old_t) * num1 % num2 == 1) {
        return max(old_s, old_t);
    } else {
        return min(old_s, old_t);
    }
}

// https://en.wikipedia.org/wiki/RSA_(cryptosystem)
BigNumber RSA_CRT(BigNumber data, BigNumber p, BigNumber q, BigNumber d) {
    BigNumber d_p = d % (p-1);
    BigNumber d_q = d % (q-1);
    BigNumber q_inv = findInverse(d_p, p);
    BigNumber m_1 = data.modPow(d_p, p);
    BigNumber m_2 = data.modPow(d_q, q);
    BigNumber h = q_inv * (m_1 - m_2) % p;
    return m_2 + h*q;
}

BigNumber * RSA_generate(BigNumber p, BigNumber q) {
    if (!isPrime(p) || !isPrime(q)) {
        cout << "Error: Number(s) are not prime." << endl;
        return NULL;
    }
    BigNumber n = p * q;
    BigNumber lambda = lcm(p-1, q-1);
    BigNumber e = findCoprime(lambda, 2);
    BigNumber d = findInverse(e, lambda);
    if (d < 0) {
        d += lambda;
    }
    // cout << "Public key: (n = " << n << ", e = " << e << ")" << endl;
    // cout << "Private key: (n = " << n << ", d = " << d << ")" << endl;
	BigNumber * keys = new BigNumber[3];
	keys[0] = n;
	keys[1] = e;
	keys[2] = d;
	return keys;
}

vector<BigNumber> * RSA_encrypt(string data, BigNumber n, BigNumber e) {
    vector<BigNumber> * dataVector = new vector<BigNumber>;
    for (unsigned long long i = 0; i < data.size(); i++) {
        dataVector -> push_back(((BigNumber)data.at(i)).modPow(e, n));
    }
    return dataVector;
}

string * RSA_decrypt(vector<BigNumber> data, BigNumber n, BigNumber d) {
    string * dataString = new string;
    for (unsigned long long i = 0; i < data.size(); i++) {
        dataString -> push_back(((BigNumber)data.at(i)).modPow(d, n).asLongLong());
    }
    return dataString;
}

string * RSA_decrypt(vector<BigNumber> data, BigNumber p, BigNumber q, BigNumber d) {
    string * dataString = new string;
    for (unsigned long long i = 0; i < data.size(); i++) {
        dataString -> push_back(RSA_CRT(data.at(i), p, q, d).asLongLong());
    }
    return dataString;
}