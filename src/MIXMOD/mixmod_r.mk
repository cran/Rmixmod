include $(R_HOME)/etc${R_ARCH}/Makeconf

#-----------------------------------------------------------------------
# Variables
# 
LIB = ../mixmod.a

#-----------------------------------------------------------------------
# Sources files
#
SRCS =  XEMAlgo.cpp \
        XEMBICCriterion.cpp \
        XEMBinaryData.cpp \
        XEMBinaryEjParameter.cpp \
        XEMBinaryEkjhParameter.cpp \
        XEMBinaryEkjParameter.cpp \
        XEMBinaryEkParameter.cpp \
        XEMBinaryEParameter.cpp \
        XEMBinaryParameter.cpp \
        XEMBinarySample.cpp \
        XEMCEMAlgo.cpp \
        XEMClusteringInput.cpp \
        XEMClusteringMain.cpp \
        XEMClusteringModelOutput.cpp \
        XEMClusteringOutput.cpp \
        XEMClusteringStrategy.cpp \
        XEMClusteringStrategyInit.cpp \
        XEMColumnDescription.cpp \
        XEMCondExe.cpp \
        XEMCriterion.cpp \
        XEMCriterionOutput.cpp \
        XEMCVCriterion.cpp \
        XEMData.cpp \
        XEMDataDescription.cpp \
        XEMDCVCriterion.cpp \
        XEMDescription.cpp \
        XEMDiagMatrix.cpp \
        XEMEMAlgo.cpp \
        XEMError.cpp \
        XEMEstimation.cpp \
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
        XEMInputControler.cpp \
        XEMInput.cpp \
        XEMLabel.cpp \
        XEMLabelDescription.cpp \
        XEMLikelihoodOutput.cpp \
        XEMMain.cpp \
        XEMMAlgo.cpp \
        XEMMAPAlgo.cpp \
        XEMMatrix.cpp \
        XEMModel.cpp \
        XEMModelOutput.cpp \
        XEMModelType.cpp \
        XEMNECCriterion.cpp \
        XEMOldInput.cpp \
        XEMOldModelOutput.cpp \
        XEMOutputControler.cpp \
        XEMOutput.cpp \
        XEMParameter.cpp \
        XEMParameterDescription.cpp \
        XEMPartition.cpp \
        XEMProba.cpp \
        XEMProbaDescription.cpp \
        XEMProbaOutput.cpp \
        XEMQualitativeColumnDescription.cpp \
        XEMQuantitativeColumnDescription.cpp \
        XEMRandom.cpp \
        XEMSample.cpp \
        XEMSelection.cpp \
        XEMSEMAlgo.cpp \
        XEMSphericalMatrix.cpp \
        XEMStrategy.cpp \
        XEMStrategyInit.cpp \
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
	$(CXX)  $(ALL_CXXFLAGS) -Wunused-variable $< -c -o $@

#-----------------------------------------------------------------------
# The rule lib create the library NEWMAT
#
lib: $(LIB)

$(LIB): $(OBJS)
	$(AR) -rus $@ $?

mostlyclean: clean

clean:
	@-rm -rf .libs _libs $(LIB)
	@-rm -f *.o
	
	