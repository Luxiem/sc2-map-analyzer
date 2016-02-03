#ifndef SC2MA_colour_h
#define SC2MA_colour_h


#ifndef WINDOWS
// Some definitions
#include <StormPort.h>
#endif
#include "sc2mapTypes.hpp"


#define MIN(x, y, z) x < y ? (x < z ? x : (z < y ? z : y)) : (y < z ? y : z)
#define MAX(x, y, z) x > y ? (x > z ? x : (z > y ? z : y)) : (y > z ? y : z)

#define DWORD_ARGB(a,r,g,b) \
((DWORD)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))


class Colour
{
public:

    static void ToRGB(unsigned long a_colour, float& a_r, float& a_g, float& a_b);

    static void ToARGB(unsigned long a_colour, unsigned char& a_a, unsigned char& a_r, unsigned char& a_g, unsigned char& a_b);


    static DWORD ColorToDword(Color* a_color, int a = 0xff);
    
    // r,g,b values are from 0 to 1
    // h = [0,360], s = [0,1], v = [0,1]
    //		if s == 0, then h = -1 (undefined)
    static void RGBtoHSV( float r, float g, float b, float *h, float *s, float *v );
    static void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v );
    static void toHSV(DWORD a_colour, float* h, float* s, float* v);
    static DWORD RGBBlend(DWORD a_colour1, DWORD a_colour2, double a_blend);
	static DWORD HSVBlend(DWORD a_colour1, DWORD a_colour2, double a_blend);

};

#endif
