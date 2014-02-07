The DSO uses a 8x14 bitmapped font, encoded in codepage 437. The font 
data is directly included to the via 
"Source/HwLayer/Common/font-bits.h" head. Customizing this font can be 
done using the included 'makefont.sh' script. The script expects a 256x112 XBM bitmap or a 8x14 
pixel psf font, shuffles the bytes along and outputs the result to the said header file. 

Usage: ./makefont.sh file.xbm or ./makefont.sh file.pdf

If you want to convert psf fonts, then you will need the "PSF Tools" 
package (http://www.seasip.demon.co.uk/Unix/PSF/). 

The included font files (ter-i14*.psf) a processed output from the "Terminus Font" Project 
(http://terminus-font.sourceforge.net/) and covered by the SIL Open 
Font License (http://scripts.sil.org/OFL)
