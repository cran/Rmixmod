/** @file Random.h
 *  @brief Random numbers generator.
 **/

#ifndef XEM_RANDOM_H
#define XEM_RANDOM_H

#include <stdint.h>

namespace XEM {

extern double rnd();
extern int64_t flip(double x);

extern void initRandomize(int seed);

extern void randomize();

extern void antiRandomize(int seed);

}

#endif
