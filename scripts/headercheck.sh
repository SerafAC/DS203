S=../Source
n=0
find $S/Core $S/Framework $S/Gui $S/HwLayer/SDL2 $S/Library $S/Main $S/User -name *.h | sed 's;\.\./;;g' | while read h
do
cat >hc_$n.cpp <<EOF
#define ADD_MODULE(X,Y)
bool HasOverlay();
#include <$h>
EOF
n=$((n+1))
done


cat >makefile.headercheck <<EOF
hc_src=\$(wildcard hc*.cpp)
hc_obj=\$(addsuffix .o, \$(hc_src))
CXXFLAGS=-I..

%.cpp.o: %.cpp
		@g++ -Werror -Wall -D_VERSION2 -c --std=gnu++11 -D_SDL2 -I.. \$^ -o \$@
		@rm \$^ \$@
		
hc: \$(hc_obj)
	
info:
	echo \$(hc_obj)
EOF

make -j -k -f makefile.headercheck
