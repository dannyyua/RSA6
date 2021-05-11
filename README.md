# RSA6
This is a simple implementation of the RSA algorithm in C++ using vectors to hold encrypted data, and strings to hold decrypted data.

### Main Functions

p and q are two prime numbers, n = p * q, e is the public exponent, and d is the inverse of e mod λ(n)*

* RSA_encrypt
  * Input: Text to encrypt, n, and e
  * Output: BigNumber vector containing ciphertext

* RSA_decrypt
  * Input: BigNumber vector, p, q, and d
    * OR: BigNumber vector, n, and d **(slow!)**
  * Output: String containing deciphered ciphertext in readable text
  
* RSA_generate
  * Input: p and q
  * Output: Array containing n, e, and d

\* [Carmichael's totient function](https://en.wikipedia.org/wiki/Carmichael_function)

### Helper Functions
* isPrime
* printVector
* gcd
* lcm
* findCoprime
* findInverse
* RSA_CRT


### Current Limitations
Only works for primes of 8-9 digits or less, and with small values of e (3 or 5 seem to work ok).


### Credits
This program utilises the BigNumber library by pr0crustes, found here: https://github.com/pr0crustes/BigNumber
