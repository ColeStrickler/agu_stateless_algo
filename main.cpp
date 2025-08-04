#include <stdio.h>
#include <stdint.h>
#include <chrono>
#include <iostream>
#include <x86intrin.h> // For __rdtsc()


typedef struct {
    uint32_t M;
    int32_t s;
    int32_t add_indicator;
} Magic;

Magic magicu(uint32_t d) {
    Magic mag;
    uint32_t p;
    uint32_t anc, delta, q1, r1, q2, r2, t;
    const uint32_t two31 = 0x80000000;

    mag.add_indicator = 0;
    p = 31;
    uint32_t nc = -1 - (-d) % d;

    q1 = two31 / nc;
    r1 = two31 - q1 * nc;

    q2 = two31 / d;
    r2 = two31 - q2 * d;

    do {
        p = p + 1;
        if (r1 >= nc - r1) {
            q1 = 2 * q1 + 1;
            r1 = 2 * r1 - nc;
        } else {
            q1 = 2 * q1;
            r1 = 2 * r1;
        }

        if (r2 + 1 >= d - r2) {
            if (q2 >= 0x7FFFFFFF) mag.add_indicator = 1;
            q2 = 2 * q2 + 1;
            r2 = 2 * r2 + 1 - d;
        } else {
            if (q2 >= 0x80000000) mag.add_indicator = 1;
            q2 = 2 * q2;
            r2 = 2 * r2 + 1;
        }

        delta = d - 1 - r2;
    } while (p < 64 && (q1 < delta || (q1 == delta && r1 == 0)));

    mag.M = q2 + 1;
    mag.s = p - 32;
    return mag;
}
inline unsigned long long rdtsc(){
    unsigned int hi, lo;
    __asm__ __volatile__ (
        "cpuid\n"
        "rdtsc\n"
        : "=a"(lo), "=d"(hi)
        :: "%rbx", "%rcx"
    );
    return ((unsigned long long)hi << 32) | lo;
}


volatile uint32_t check(uint32_t x, uint32_t d)
{   
    unsigned long long start = rdtsc();

    Magic m = magicu(d);
    uint32_t regular = x / d;
    
    uint64_t mul = (uint64_t)x * m.M;
    uint32_t magic_result = mul >> 32;
    if (m.add_indicator) {
        magic_result = (magic_result + ((x - magic_result) >> 1)) >> m.s;
    } else {
        magic_result = magic_result >> m.s;
    }
    unsigned long long end = rdtsc();
    printf("m.s %d\n", m.s);
     std::cout << "Cycles elapsed: " << (end - start) << std::endl;
    return magic_result;
    

   

//printf("x = %u, d = %u | Regular: %u, Magic: %u\n", x, d, regular, magic_result);
}


int main() {
    uint32_t d = 655367389072618;
    auto start = std::chrono::high_resolution_clock::now();
    //printf("Magic M = 0x%X, Shift s = %d\n", m.M, m.s);
    auto x = check(65517063, d);
    x = check(65517063, 5517063);
    x = check(655, 5517);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Elapsed time: " << duration_us.count() << " microseconds\n";
    std::cout << x << std::endl;
    return 0;
}
