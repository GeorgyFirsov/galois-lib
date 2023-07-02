# galois-lib

This library implements some arithmetic operations in finite fields.

## Implemented operations

`galois-lib` implements not so many algorithms, because it is designed to be a helper library 
for cryptographic libraries. Hence it contains only few necessary operations.

### Multiplication in $GF(2 ^ {8})$

Multiplication in finite field of size $2 ^ {8}$ is implemented using precomputed
logarithm and exponents tables. It works according to the following obvious formula:

$$
a \cdot b = \exp \left( \log a + \log b \right),
$$

where all operations are assumed to be in the finite field.

### multiplication in $GF(2 ^ {128})$

Multiplication in $GF(2 ^ {128})$ is implemented using some kind of Karatsuba algorithm
with help of `pclmulqdq` instruction. More information here: [Intel® Carry-Less Multiplication 
Instruction and its Usage for Computing the GCM Mode][1].

[1]: https://www.intel.com/content/dam/develop/external/us/en/documents/clmul-wp-rev-2-02-2014-04-20.pdf
