#ifndef CRC32_H
#define CRC32_H

#include <cstdint>
#include <ostream>
#include <cstdio>
#include <vector>
#include <iterator>
#include "FileReader.h"
extern uint_fast32_t const crctab[8][256];

unsigned long check_sum(std::string argv);

#endif // CRC32_H
