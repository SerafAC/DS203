#!/bin/bash

fail()
{
	echo "$@" >&2
	exit 1
}

tmp=/tmp/
prog()
{

infile=$1
inarray=$2
isback=$3
	
	cat <<EOF
	
typedef unsigned char ui8;
#define BACK $isback
#include "$infile"
#include <assert.h>
#include <stdio.h>
#include <memory.h>
#define glyph_height 14
#define pixrow_width 32

ui8 rev(ui8 in) 
{
	ui8 r;
	int i; 
	for (i=0;i<8;i++) 
	{
		r=(r << 1)|(in&1);
		in >>= 1;
	}
	return ~r;
}

int main()
{
		int glyph, row,i; 		
		unsigned char copy[glyph_height*256]; 
		memset(copy,0x55,sizeof(copy));
		printf("#define font_width %d\n",pixrow_width*8);
		printf("#define font_height %d\n",(int)(sizeof(copy)/pixrow_width));
		for (glyph=0;glyph<256;glyph++)
		{
			for (row=0;row<glyph_height;row++)
			{
				int src=glyph*glyph_height+row; 
				int dst=(glyph%pixrow_width) + glyph_height*pixrow_width*(glyph/pixrow_width)+row*pixrow_width; 			
				assert(dst<sizeof(copy));
				assert(src<sizeof(copy));
				#ifdef BACK
				copy[src]=rev(${inarray}[dst]);			
				#else 
				copy[dst]=rev(${inarray}[src]);			
				#endif
			}
		}
		const char *komma=" ";
		#ifdef BACK
		printf("const ui8 font_bits[]={\n");
		#else 
		printf("static unsigned char font_bits[]={\n");
		#endif
		for (i=0;i<sizeof(copy);i++) 
		{
			printf("%s0x%02x",komma,copy[i]); 
			#ifdef BACK
			if ((i%14)==13) printf(" /* 0x%02x */ \n",i/14);
			#else 
			if ((i & 0xf)==0xf) printf("\n");
			#endif
			komma=",";
		}
		printf("\n};\n");
	}

EOF
}


tmp=$(mktemp -d)

arg=$1

case $arg in 
	*.xbm) 
		ffile=$arg
		;; 
	*.psf) 
		ffile=$tmp/$arg.xbm
		psf2xbm $arg >$ffile
		;;
	*)
		fail "Unsupported filetype ($arg)"
		;;
esac
prog $ffile psf_bits 1 | gcc -x c - -o $tmp/cvt
$tmp/cvt >../../Source/HwLayer/Common/font-bits.h 
rm -r $tmp


