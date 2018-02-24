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
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(ALL_CPPFLAGS) ${CPICFLAGS} $(OMPSTATUS) -DRPACKAGE -Wno-ignored-attributes -I${SRC_DIR} $< -c -o $@

#-----------------------------------------------------------------------
# The rule lib create the library MIXMOD
#
lib: $(LIB)

$(LIB): $(OBJS)
	$(AR) -rc $@ $?

clean:
	@-rm -rf .libs _libs $(LIB)
	@-rm -f *.o
