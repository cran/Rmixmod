include $(R_HOME)/etc${R_ARCH}/Makeconf

#-----------------------------------------------------------------------
# Variables
# 
LIB = ../mixmod.a
SRC_DIR = ..
#-----------------------------------------------------------------------
# Sources files
#
SRCS =  XEMAlgo.cpp \
	XEMBICCriterion.cpp \
	XEMBinaryData.cpp \
	XEMBinaryEParameter.cpp \
	XEMBinaryEjParameter.cpp \
	XEMBinaryEkParameter.cpp \
	XEMBinaryEkjParameter.cpp \
	XEMBinaryEkjhParameter.cpp \
	XEMBinaryModel.cpp \
	XEMBinaryParameter.cpp \
	XEMBinarySample.cpp \
	XEMCEMAlgo.cpp \
	XEMCVCriterion.cpp \
	XEMClusteringInput.cpp \
	XEMClusteringMain.cpp \
	XEMClusteringModelOutput.cpp \
	XEMClusteringOutput.cpp \
	XEMClusteringStrategy.cpp \
	XEMClusteringStrategyInit.cpp \
	XEMColumnDescription.cpp \
	XEMCriterion.cpp \
	XEMCriterionOutput.cpp \
	XEMData.cpp \
	XEMDataDescription.cpp \
	XEMDescription.cpp \
	XEMDiagMatrix.cpp \
	XEMEMAlgo.cpp \
	XEMError.cpp \
	XEMGaussianData.cpp \
	XEMGaussianDiagParameter.cpp \
	XEMGaussianEDDAParameter.cpp \
	XEMGaussianGeneralParameter.cpp \
	XEMGaussianHDDAParameter.cpp \
	XEMGaussianParameter.cpp \
	XEMGaussianSample.cpp \
	XEMGaussianSphericalParameter.cpp \
	XEMGeneralMatrix.cpp \
	XEMICLCriterion.cpp \
	XEMIndividualColumnDescription.cpp \
	XEMInput.cpp \
	XEMLabel.cpp \
	XEMLabelDescription.cpp \
	XEMLearnInput.cpp \
	XEMLearnMain.cpp \
	XEMLearnModelOutput.cpp \
	XEMLearnOutput.cpp \
	XEMLearnStrategy.cpp \
	XEMLikelihoodOutput.cpp \
	XEMMAPAlgo.cpp \
	XEMMAlgo.cpp \
	XEMMatrix.cpp \
	XEMModel.cpp \
	XEMModelOutput.cpp \
	XEMModelType.cpp \
	XEMNECCriterion.cpp \
	XEMParameter.cpp \
	XEMParameterDescription.cpp \
	XEMPartition.cpp \
	XEMPredictInput.cpp \
	XEMPredictMain.cpp \
	XEMPredictModelOutput.cpp \
	XEMPredictOutput.cpp \
	XEMPredictStrategy.cpp \
	XEMProba.cpp \
	XEMProbaDescription.cpp \
	XEMProbaOutput.cpp \
	XEMQualitativeColumnDescription.cpp \
	XEMQuantitativeColumnDescription.cpp \
	XEMRandom.cpp \
	XEMSEMAlgo.cpp \
	XEMSample.cpp \
	XEMSphericalMatrix.cpp \
	XEMSymmetricMatrix.cpp \
	XEMUtil.cpp \
	XEMWeightColumnDescription.cpp


#-------------------------------------------------------------------------
# generate the variable OBJS containing the names of the object files
#
OBJS= $(SRCS:%.cpp=%.o)

#-------------------------------------------------------------------------
# rule for compiling the cpp files
#
%.o: %.cpp
	$(CXX) $(CXXFLAGS) ${CPICFLAGS} -I${SRC_DIR} $< -c -o $@

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
