#pragma once

#include <bitset>
#include <iostream>
#include <stdio.h>

uint8_t translate_2d(uint8_t i);
uint8_t translate_1d(uint8_t c);
void print_board(std::bitset<33> board);