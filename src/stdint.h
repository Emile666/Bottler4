/*==================================================================
  File Name    : stdint.h
  Author       : Emile
  ------------------------------------------------------------------
  Purpose : Since the Cosmic STM8 C compiler is not C99 compatible,
            the header-file is made to define these types here.
  ------------------------------------------------------------------
  STDINT is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
 
  STDINT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with STDINT. If not, see <http://www.gnu.org/licenses/>.
  ==================================================================
*/ 
#ifndef _STDINT_H_
#define _STDINT_H_

#define UINT8_MAX  (0xFF)       /* 255U */
#define UINT16_MAX (0xFFFF)     /* 65535U */
#define UINT32_MAX (0xFFFFFFFF) /* 4294967295U */

typedef signed char   int8_t;
typedef unsigned char uint8_t;
typedef unsigned int  uint16_t;
typedef int           int16_t;
typedef unsigned long uint32_t;
typedef long          int32_t;

#endif
