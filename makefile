ifndef LIB_ROOT
LIB_ROOT		= . -I ./include/succint/include -I ./Vers/Builder -I ./Vers/Interrogator -I ./include -I ./include/mph/cmph-2.0/src
endif

CXX		=g++ -std=c++11 
INCLUDES	= -I $(LIB_ROOT) -L ./include/succint/lib -L ./include/mph/lib
CXXFLAGS  	= -ggdb -DNDEBUG

LDFLAGS 	= 	 -lsdsl -ldivsufsort -ldivsufsort64 -lcmph
OPTFLAGS	= -O3

TARGETS		= Builder_Main Interrogator_Main  Queryset_generator

.PHONY: all clean cleanall
.SUFFIXES: .cpp 


%: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OPTFLAGS) -o $@ $< $(LDFLAGS)

all		: $(TARGETS)
clean		: 
	rm -f $(TARGETS)
cleanall	: clean
	\rm -f *.o *~



