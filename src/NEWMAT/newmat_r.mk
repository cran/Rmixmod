include $(R_HOME)/etc${R_ARCH}/Makeconf

#-----------------------------------------------------------------------
# Variables
# 
LIB = ../mixmod.a

#-----------------------------------------------------------------------
# Sources files
#
SRCS = $(wildcard *.cpp)

#-------------------------------------------------------------------------
# generate the variable OBJS containing the names of the object files
#
OBJS= $(SRCS:%.cpp=%.o)

#-------------------------------------------------------------------------
# rule for compiling the cpp files
#
%.o: %.cpp
	$(CXX)  $(CXXFLAGS) ${CPICFLAGS} $< -c -o $@

#-----------------------------------------------------------------------
# The rule lib create the library MIXMOD [??!]
#
lib: $(LIB)

$(LIB): $(OBJS)
	$(AR) -rc $@ $?
  
mostlyclean: clean

clean:
	@-rm -rf .libs _libs $(LIB)
	@-rm -f *.o
