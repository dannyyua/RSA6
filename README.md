# RSA6
This is a simple implementation of the RSA (Rivest–Shamir–Adleman) algorithm in C++ using vectors to hold encrypted data, and strings to hold decrypted data.

Requires the MPIR library (including the C++ interface), found [here](https://mpir.org/).

**NOTE:** As of now, the algorithm does **NOT** utilise padding when encrypting data, so **do not use it to encrypt any actual confidential information!**
Use it as a deterrent.

### Main Functions

p and q are two prime numbers, n = p * q, e is the public exponent (usually 65537), and d is the inverse of e mod λ(n)*

* `RSA_encrypt`
  * Input: Text to encrypt, n, and e
  * Output: mpz_class vector containing ciphertext

* `RSA_decrypt`
  * Input: mpz_class vector, n, d, p, and q
    * OR: mpz_class vector, n, and d (slower)
  * Output: String containing deciphered ciphertext in readable text
  
* `RSA_keygen`
  * Input: p and q
  * Output: Array containing n, e, and d

* `RSA_primegen`
  * Input: RSA bit size
  * Output: Array containing two prime numbers with bit size half of the input

\* [Carmichael's totient function](https://en.wikipedia.org/wiki/Carmichael_function)

### Helper Functions
* `printVector`
* `findCoprime`
* `RSA_CRT`
* `textToVector`

### Current Limitations
* Without padding, encrypted data is very susceptible to a number of attacks (if someone really wanted your data), e.g. a [chosen-plaintext attack](https://en.wikipedia.org/wiki/Chosen-plaintext_attack).
This will be addressed in a future version.

* ~~Only works for primes of 8-9 digits or less, and with small values of e (3 or 5 seem to work ok).~~
Fixed. RSA keys of up to 8192 bits have been tested and are working properly.
