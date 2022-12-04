#pragma once

#include <bitset>
#include <iostream>
#include <iomanip>

#define INITIAL_BOARD 0x1fffeffff
#define CLEAR_BOARD do { std::cout << "\033[8F"; } while (false)

uint8_t translate_2d(uint8_t i);
uint8_t translate_1d(uint8_t c);
void print_board(std::bitset<33> board);
void test_translate();