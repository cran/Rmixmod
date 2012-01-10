/***************************************************************************
                             SRC/NEWMAT/controlw.h  description
                             ------------------------
    copyright            : (C) MIXMOD Team - 2001-2003-2004-2005-2006-2007-2008-2009
    email                : mixmod@univ-fcomte.fr
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
                                                                          
 ***************************************************************************/
//$$ controlw.h                Control word class

#ifndef CONTROL_WORD_LIB
#define CONTROL_WORD_LIB 0

// for organising an int as a series of bits which indicate whether an
// option is on or off.

class ControlWord
{
protected:
   int cw;                                      // the control word
public:
   ControlWord() : cw(0) {}                     // do nothing
   ControlWord(int i) : cw(i) {}                // load an integer

      // select specific bits (for testing at least one set)
   ControlWord operator*(ControlWord i) const
      { return ControlWord(cw & i.cw); }
   void operator*=(ControlWord i)  { cw &= i.cw; }

      // set bits
   ControlWord operator+(ControlWord i) const
      { return ControlWord(cw | i.cw); }
   void operator+=(ControlWord i)  { cw |= i.cw; }

      // reset bits
   ControlWord operator-(ControlWord i) const
      { return ControlWord(cw - (cw & i.cw)); }
   void operator-=(ControlWord i) { cw -= (cw & i.cw); }

      // check if all of selected bits set or reset
   bool operator>=(ControlWord i) const { return (cw & i.cw) == i.cw; }
   bool operator<=(ControlWord i) const { return (cw & i.cw) == cw; }

      // flip selected bits
   ControlWord operator^(ControlWord i) const
      { return ControlWord(cw ^ i.cw); }
   ControlWord operator~() const { return ControlWord(~cw); }

      // convert to integer
   int operator+() const { return cw; }
   int operator!() const { return cw==0; }
   FREE_CHECK(ControlWord)
};


#endif
