// Test all token types, keywords, literals, operators, and preprocessor

#define VERSION 1
#define FEATURE_ENABLED

#ifndef DISABLE_FEATURE
#define FEATURE 42
#else
#define FEATURE 0
#endif

#include <stdio.h>
#include <stdlib.h>

/* Multi-line comment
   spanning several lines
*/

// Single line comment with tricky chars: /* */ // \n

typedef unsigned long long ull;

struct Vector3 {
    float x, y, z;
};

enum Status {
    OK = 0,
    ERROR = -1,
    UNKNOWN = 100,
};

static const char* message = "Test string with escape sequences: \n\t\"\\\'";

inline int add(int a, int b) {
    return a + b;
}

void process(Vector3* vec) {
    ull count = 0;
    for (int i=0; i<10; i++) {
        if (i % 2 == 0 && FEATURE_ENABLED) {
            count += i * FEATURE;
        } else if (i == 5 || i == 7) {
            continue;
        } else {
            break;
        }
    }

    while(count < 1000) {
        count <<= 1;
        if (count & 0x8000) {
            goto overflow;
        }
    }

    do {
        count--;
    } while(count > 0);

overflow:
    switch(count) {
        case 0: message = "Zero"; break;
        case 1: message = "One"; break;
        default: message = "Many"; break;
    }

    // Ternary operator usage
    int result = (count > 100) ? 1 : 0;

    // Pointer to function example
    int (*funcPtr)(int,int) = &add;
    result = funcPtr(10, 20);

    // sizeof operator
    size_t size = sizeof(struct Vector3);

    // Complex number (C99)
    _Complex float complexNum = 1.0f + 2.0f * _Imaginary_I;

    // volatile variable
    volatile int flag = 0;

    // auto and register
    auto int autoVar = 5;
    register int regVar = 10;

    // restrict keyword (C99)
    int* restrict ptr = &count;

    // inline assembly (just as tokens)
    asm("nop");

    // Static assert (C11)
    _Static_assert(sizeof(int) == 4, "int is not 4 bytes");

    // Binary literals (gcc extension)
    int binVal = 0b101010;

    // Octal literal
    int octVal = 0777;

    // Hex literal with suffix
    unsigned long hexVal = 0xFFAAuL;

    // Floating literals
    float f1 = 1.23e-4f;
    double d1 = 3.14E+10;
    long double ld1 = 0.1L;

    // Character literals with escapes
    char c1 = '\n';
    char c2 = '\x41';  // 'A'

    // String concatenation
    char* s = "Hello, " "world!";

    // Macro expansion test
    int x = VERSION * FEATURE;

    // Nested #ifdef inside function (should be skipped or processed accordingly)
#ifdef FEATURE_ENABLED
    x += 100;
#endif

    // Complex expression with comma operator
    int y = (x++, x + 5);

    // Arrow operator usage
    vec->x = 1.0f;

    // Dot operator usage
    float z = vec.x;

    // Use of sizeof on expression
    size_t arrSize = sizeof(int[10]);

    // Use of type cast
    int castVal = (int)3.14;

    // Use of unary operators
    int neg = -x;
    int notVal = ~x;
    int preInc = ++x;
    int postDec = x--;

    // Use of logical operators
    if (!flag && (x > 0 || y < 0)) {
        flag = 1;
    }
}
