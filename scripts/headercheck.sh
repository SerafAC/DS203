S=../Source
n=0
find $S/Core $S/Framework $S/Gui $S/HwLayer/SDL2 $S/Library $S/Main $S/User -name *.h | sed 's;\.\./;;g' | while read h
do
cat >hc_$n.cpp <<EOF
#include <$h>
EOF
# g++ -c -I.. ck.cpp >/dev/null 2>&1
n=$((n+1))
done


