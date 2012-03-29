/***************************************************************************
                             SRC/MIXMOD/XEMRandom.cpp  description
    copyright            : (C) MIXMOD Team - 2001-2011
    email                : contact@mixmod.org
 ***************************************************************************/

/***************************************************************************
    This file is part of MIXMOD
    
    MIXMOD is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    MIXMOD is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with MIXMOD.  If not, see <http://www.gnu.org/licenses/>.

    All informations available on : http://www.mixmod.org                                                                                               
***************************************************************************/

/*
Tiny Encryption Algorithm
-------------------------
*/


#include "XEMRandom.h"  /* prototypes */
#include <sys/timeb.h>  /* time_t */ 
#include <fstream>





/* Modification de l'implementation de l'algo pour être portable 32-64 bits, ...*/

const double m = 4294967296.0;
/*const uint32_t d = 2654435769U;
const uint32_t k0 = 3352799412U;
const uint32_t k1 = 2596254646U;
const uint32_t k2 = 1943803523U;
const uint32_t k3 = 397917763U;

uint32_t y = 123456789U;
uint32_t z = 987654321U;
*/
const uint32_t d = 0x09E3779B9L;
const uint32_t k0 = 0x0C7D7A8B4L;
const uint32_t k1 = 0x09ABFB3B6L;
const uint32_t k2 = 0x073DC1683L;
const uint32_t k3 = 0x017B7BE43L;

uint32_t y = 123456789L;
uint32_t z = 987654321L;


// Return a value in [0...1[ //
double rnd()
{
    uint32_t s = 0;
    uint32_t n = 8;
    while (n-- > 0) {
        s += d;
        y += (z << 4) + (k0^z) + (s^(z >> 5)) + k1;
        z += (y << 4) + (k2^y) + (s^(y >> 5)) + k3;
    }
    return (z + y / m) / m;
}

int64_t flip(double x)
{
    return (rnd() <= x);
}

void antiRandomise()
{
    z = 0;
    y = 0;
    rnd();
}

// Init random seed //
void randomise()
{
    timeb q;
    ftime(&q);
    z = (uint32_t) q.millitm;
    y = (uint32_t) q.time;
    rnd();
}

