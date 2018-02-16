#!/bin/bash
function checkfile() 
{
	echo $1
	x=$(echo ${1#../Source/} | sed 's;[\./];_;g')
	x="DSO_${x^^}"
	./fix $x <$1 >tmpfile
    cp $1 $1.bak
    cp tmpfile $1
}
gcc fix.c -o fix
S=../Source
find $S/Core $S/Framework $S/Gui $S/HwLayer/SDL2 $S/Library $S/Main $S/User -name *.h | while read f
do
 checkfile $f
done
