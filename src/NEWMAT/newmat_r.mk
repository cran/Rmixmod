include $(R_HOME)/etc${R_ARCH}/Makeconf

#-----------------------------------------------------------------------
# Variables
# 
LIB = ../mixmod.a

#-----------------------------------------------------------------------
# Sources files
#
SRCS =  bandmat.cpp \
        cholesky.cpp \
        evalue.cpp \
        hholder.cpp \
        jacobi.cpp \
        myexcept.cpp \
        newfft.cpp \
        newmat1.cpp \
        newmat2.cpp \
        newmat3.cpp \
        newmat4.cpp \
        newmat5.cpp \
        newmat6.cpp \
        newmat7.cpp \
        newmat8.cpp \
        newmat9.cpp \
        newmatex.cpp \
        newmatnl.cpp \
        newmatrm.cpp \
        solution.cpp \
        sort.cpp \
        submat.cpp \
        svd.cpp

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
# The rule lib create the library NEWMAT
#
lib: $(LIB)

$(LIB): $(OBJS)
	$(AR) -rc $@ $?
  
mostlyclean: clean

clean:
	@-rm -rf .libs _libs $(LIB)
	@-rm -f *.o
