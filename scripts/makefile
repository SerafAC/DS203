hc_src=$(wildcard hc*.cpp)
hc_obj=$(addsuffix .o, $(hc_src))
CXXFLAGS=-I..

%.cpp.o: %.cpp
		g++ -Werror -Wall -D_VERSION2 -c --std=gnu++11 -D_SDL2 -I.. $^ -o $@
		rm $^ $@
		
hc: $(hc_obj)
	
info:
	echo $(hc_obj)
