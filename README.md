# goals
i created these functions with the goal of exploring the field of cryptography, not making the next AES.

# dream
reference implementation of the sponge function dream, following the style of BLAKE2's reference implementation. provides two instances: dream[r, c] -> dream[512,256] and dream[1024,512].

make tests && ./tests from the root directory will build a quick sanity check

similarly, make avx_tests && avx_tests will build the avx implementation (compare outputs of both versions to ensure accuracy). avx2 required, avx512f + avx512vl for the most optimized version (single instruction packed rotates added).

(not updated to most recent permutation!) see [this](https://docs.google.com/document/d/1qwd6pyg_UqVCj62hyIIiU8OoCUr5BeuD3Yn_uVZ18tw/edit?usp=sharing) link for a
more in-depth description (stream of consciousness) of the algorithm 

# hash
dream128 provides 32 byte digests, while dream256 provides 64. these functions have a security level of 2<sup>128</sup>
and 2<sup>256</sup> respectively against collision, preimage, and second preimage.

# authenticated encryption
single pass authenticated encryption is implemented via the wrap and unwrap functions, following the SpongeWrap mode
described by Keccak's authors. dream128 supports keys of 16 bytes and associated data of <= 63 bytes, while dream256 supports keys of 32 bytes and associated data of <= 127 bytes.
