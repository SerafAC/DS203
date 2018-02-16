#!/bin/sh
grep -r $1 ../Source | egrep "class[ ]*(DLLAPI)?[ ]*$1[ :{]" | while IFS=':' read f r 
do
 l="#include <${f#../}>"
 echo $l
 echo $l | xsel -b

done
