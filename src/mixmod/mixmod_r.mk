include $(R_HOME)/etc${R_ARCH}/Makeconf
include ../XmlMakevars

#-----------------------------------------------------------------------
# Variables
#
LIB = ../mixmod.a
SRC_DIR = ..
#EIGENDIR = "../eigen3"

#-----------------------------------------------------------------------
# Sources files
#
SRCS = $(wildcard */*.cpp) $(wildcard */*/*.cpp) $(wildcard */*/*/*.cpp)

#-------------------------------------------------------------------------
# generate the variable OBJS containing the names of the object files
#
OBJS = $(SRCS:%.cpp=%.o)

#-------------------------------------------------------------------------
# rule for compiling the cpp files
# NOTE: $(ALL_CPPFLAGS) contains include path for RcppEigen; do not remove!
#
# Flags actuels : -std=gnu++11 -I/usr/include/R/ -DNDEBUG  -I"/home/auder/.R/library/Rcpp/include" -I"/home/auder/.R/library/RcppEigen/include" -D_FORTIFY_SOURCE=2  -DRPACKAGE -DXEM_RESOURCES_PATH='"/home/auder/.R/library/Rmixmod/XML_specs"'   -I. -I./mixmod_iostream   -fpic  -march=x86-64 -mtune=generic -O2 -pipe -fstack-protector-strong -fno-plt
# TODO: -std=gnu++11-I "/home/auder/.R/library/RcppEigen/include" -DRPACKAGE -I. -fpic -march=x86-64 -O2 -pipe -fno-plt
#
#%.o: %.cpp
#	$(CXX) $(CXXFLAGS) $(ALL_CPPFLAGS) ${CPICFLAGS} $(OMPSTATUS) -DRPACKAGE -I${SRC_DIR} $< -c -o $@
#	$(CXX) -std=gnu++11 -I"/home/auder/.R/library/RcppEigen/include" -DRPACKAGE -I. -fpic -march=x86-64 -O2 -pipe -fno-plt $< -c -o $@
#	$(CXX) -std=gnu++11 $(ALL_CPPFLAGS) -DRPACKAGE -I. -I.. -fpic -march=x86-64 -O2 -pipe $< -c -o $@
#-fno-plt : removed for CRAN (Windows compilation failed...)

PKG_CPPFLAGS = -DRPACKAGE -I. -I..

#-----------------------------------------------------------------------
# The rule lib create the library MIXMOD
#
lib: $(LIB)

$(LIB): $(OBJS)
	$(AR) -rc $@ $?

clean:
	@-rm -rf .libs _libs $(LIB)
	@-rm -f *.o
