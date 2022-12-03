#include <iostream>
#include <string>
#include <bitset>
#include <stdio.h>

uint8_t translate_2d(uint8_t index);
uint8_t translate_1d(uint8_t c);

int main() {
    std::cout << "Hello, world!";
}

uint8_t translate_2d(uint8_t i) {
    if (i < 6) {
        return (i / 3) * 7 + (i % 3) + 2; 
    } else if (i < 27) {
        i -= 6;
        return ((i / 7) + 2) * 7 + (i % 7);
    } else if (i < 33) {
        i -= 27;
        return ((i / 3) + 5) * 7 + (i % 3) + 2;
    } else {
        return -1;
    }
}

uint8_t translate_1d(uint8_t c) {
    uint8_t i, j;
    if (c < 14) {
        i = c / 7;
        j = (c % 7) - 2;
        return j > 2 ? -1 : i * 3 + j;
    } else if (c < 35) {
        i = (c / 7) - 2;
        j = (c % 7);
        return 6 + i * 3 + j;
    } else if (c < 50) {
        i = (c / 7) - 5;
        j = (c % 7) - 2;
        return j > 2 ? -1 : 27 + i * 3 + j;
    } else {
        return -1;
    }
}

// void test_translate() {
//     for (int i = 0; i < 49; i++) {
//         if (!(i % 7)) std::cout << "\n";
//         printf("%5d", (int)translate_1d(i));
//     }

//     std::cout << "\n\n";

//     uint8_t n = 0;
//     int next = translate_2d(n);
//     for (int i = 0; i < 7; i++) {
//         for (int j = 0; j < 7; j++) {
//             if (next / 7 == i && next % 7 == j) {
//                 printf("%5d", (int)n);
//                 next = translate_2d(++n);
//             } else {
//                 std::cout << "     ";
//             }
//         }
//         std::cout << "\n";
//     }
// }