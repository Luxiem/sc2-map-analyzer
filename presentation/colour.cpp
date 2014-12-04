#include "colour.h"



void Colour::ToRGB(unsigned long a_colour, float& a_r, float& a_g, float& a_b)
{
	a_r = float( ( a_colour >> 16 ) & 0xff ) / 255.f;
	a_g = float( ( a_colour >> 8 ) & 0xff ) / 255.f;
	a_b = float( ( a_colour & 0xff ) ) / 255.f;
}


void Colour::ToARGB(unsigned long a_colour, Uint8& a_a, Uint8& a_r, Uint8& a_g, Uint8& a_b)
{
	a_a = ( a_colour >> 24 ) & 0xff;
	a_r = ( a_colour >> 16 ) & 0xff;
	a_g = ( a_colour >> 8 ) & 0xff;
	a_b = ( a_colour & 0xff );
}


DWORD Colour::ColorToDword(Color* a_color, int a)
{
    if (a_color == 0) return 0xffaaaaaa;
    
    return DWORD_ARGB(a, (int)(255 * a_color->r), (int)(255 * a_color->g), (int)(255 * a_color->b));
}

// r,g,b values are from 0 to 1
// h = [0,360], s = [0,1], v = [0,1]
//		if s == 0, then h = -1 (undefined)
void Colour::RGBtoHSV( float r, float g, float b, float *h, float *s, float *v )
{
	float min, max, delta;
	min = MIN( r, g, b );
	max = MAX( r, g, b );
	*v = max;				// v
	delta = max - min;
	if( max != 0 )
		*s = delta / max;		// s
	else {
		// r = g = b = 0		// s = 0, v is undefined
		*s = 0;
		*h = -1;
		return;
	}
	if( r == max )
		*h = ( g - b ) / delta;		// between yellow & magenta
	else if( g == max )
		*h = 2 + ( b - r ) / delta;	// between cyan & yellow
	else
		*h = 4 + ( r - g ) / delta;	// between magenta & cyan
	*h *= 60;				// degrees
	if( *h < 0 )
		*h += 360;
}
void Colour::HSVtoRGB( float *r, float *g, float *b, float h, float s, float v )
{
	int i;
	float f, p, q, t;
	if( s == 0 ) {
		// achromatic (grey)
		*r = *g = *b = v;
		return;
	}
	h /= 60;			// sector 0 to 5
	i = floor( h );
	f = h - i;			// factorial part of h
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );
	switch( i ) {
		case 0:
			*r = v;
			*g = t;
			*b = p;
			break;
		case 1:
			*r = q;
			*g = v;
			*b = p;
			break;
		case 2:
			*r = p;
			*g = v;
			*b = t;
			break;
		case 3:
			*r = p;
			*g = q;
			*b = v;
			break;
		case 4:
			*r = t;
			*g = p;
			*b = v;
			break;
		default:		// case 5:
			*r = v;
			*g = p;
			*b = q;
			break;
	}
}


void Colour::toHSV(DWORD a_colour, float* h, float* s, float* v)
{
    float r, g, b;
    ToRGB(a_colour, r, g, b);
    RGBtoHSV(r, g, b, h, s, v);
}


DWORD Colour::HSVBlend(DWORD a_colour1, DWORD a_colour2, double a_blend)
{
    float h1 = 0;
    float s1 = 0;
    float v1 = 0;
    
    float h2 = 0;
    float s2 = 0;
    float v2 = 0;
    
    float rb, gb, bb;
    
    toHSV(a_colour1, &h1, &s1, &v1);
    toHSV(a_colour2, &h2, &s2, &v2);
    
    float hb = a_blend * h1 + (1.f - a_blend) * h2;
    float sb = a_blend * s1 + (1.f - a_blend) * s2;
    float vb = a_blend * v1 + (1.f - a_blend) * v2;
    
    HSVtoRGB(&rb, &gb, &bb, hb, sb, vb);
    
    int ir = ((int)(255.f * rb));
    int ig = ((int)(255.f * gb));
    int ib = ((int)(255.f * bb));
    
    return DWORD_ARGB(0xff, ir, ig, ib);
}
