/***************************************************************************
                             SRC/MIXMOD/XEMOutputControler.cpp  description
    copyright            : (C) MIXMOD Team - 2001-2011
    email                : contact@mixmod.org
 ***************************************************************************/

/***************************************************************************
    This file is part of MIXMOD
    
    MIXMOD is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    MIXMOD is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with MIXMOD.  If not, see <http://www.gnu.org/licenses/>.

    All informations available on : http://www.mixmod.org                                                                                               
***************************************************************************/
#include <string.h>
#include "XEMOutputControler.h"
#include "XEMUtil.h"



//------------
// Constructor
//------------
XEMOutputControler::XEMOutputControler(){
  int64_t i;
   
  _output = NULL;
  _nbOutputFiles = maxNbOutputFiles;
  for (i=0; i<_nbOutputFiles; i++){
    _tabOutputTypes[i] = (XEMOutputType) i;
  }
  createEmptyFiles();
}



//------------
// Constructor
//------------
XEMOutputControler::XEMOutputControler(XEMOutput * output){
  int64_t i;
   
  _output = output;
  _nbOutputFiles = maxNbOutputFiles;
  for (i=0; i<_nbOutputFiles; i++){
    _tabOutputTypes[i] = (XEMOutputType) i;
  }
  createEmptyFiles();
}



//-----------
// Destructor
//-----------
XEMOutputControler::~XEMOutputControler(){
  int64_t i;
//cout<<"tabFileName"<<endl;
  for (i=0; i<maxNbOutputFiles; i++){
//cout<<"tabFileName :  "<<_tabFiles[i]<<endl;
    delete _tabFiles[i];
  }
}



//---------
// Selector
//---------
void XEMOutputControler::setOutput(XEMOutput * output){
  _output = output;
}



//---------
// editFile
//---------
void XEMOutputControler::editFile(){
  int64_t i;
   if (_output){
    _output->editFile(_tabFiles);
    for (i=0; i<_nbOutputFiles; i++){
      _tabFiles[i]->close();
    }
  }
}



//---------
// editErroMixmodFile
//---------
void XEMOutputControler::editErroMixmodFile(XEMErrorType error){
  // errorMixmodOutput
  int64_t i = (int64_t)errorMixmodOutput;
  *_tabFiles[i]<<error<<endl;
}
  
  
  

//--------------------------
// Create Empty output Files
//--------------------------
void XEMOutputControler::createEmptyFiles(){
  int64_t i,j;
  string charCriterionType= "";
  string fileName = "";
  int64_t nbOutputFileType = 9;

  string * fileNameTmp = new string[nbOutputFileType];
  for (i=0 ; i<nbOutputFileType; i++){
    fileNameTmp[i] = "";
  }
  fileNameTmp[0] = "standard.txt";
  fileNameTmp[1] = "numericStandard.txt";
  fileNameTmp[2] = "label.txt";
  fileNameTmp[3] = "parameter.txt";
  fileNameTmp[4] = "posteriorProbabilities.txt";
  fileNameTmp[5] = "partition.txt";
  fileNameTmp[6] = "likelihood.txt";
  fileNameTmp[7] = "Error.txt";
  fileNameTmp[8] = "numericLikelihood.txt";


  // CV, BIC, NEC, ICL, DCV ... files
  //---------------------------------
  for (i=0;i<nbMaxSelection;i++){
    if (i==0){
      charCriterionType = "BIC";
    }
    if (i==1){
      charCriterionType = "CV";
    }
    if (i==2){
      charCriterionType = "ICL";
    }
    if (i==3){
      charCriterionType = "NEC";
    }
    if (i==4){
      charCriterionType = "DCV";
    }

    for (j=0; j<nbOutputFileType; j++){
      fileName = charCriterionType;
     _tabFiles[nbOutputFileType*i+j] = new ofstream(fileName.append(fileNameTmp[j]).c_str(), ios::out);
      _tabFiles[nbOutputFileType*i+j]->setf(ios::fixed, ios::floatfield);
    }
  }


  // other files
  //------------
  //char * completeFileName ="complete.txt";
  _tabFiles[(int64_t)completeOutput] = new ofstream("complete.txt", ios::out);
  _tabFiles[(int64_t)completeOutput]->setf(ios::fixed, ios::floatfield);

  //char * numericCompleteFileName ="numericComplete.txt";
  _tabFiles[(int64_t)numericCompleteOutput] = new ofstream("numericComplete.txt", ios::out);
  _tabFiles[(int64_t)numericCompleteOutput]->setf(ios::fixed, ios::floatfield);

  //char * CVlabelClassificationFileName = "CVlabelClassification.txt";
  _tabFiles[(int64_t)CVlabelClassificationOutput] = new ofstream("CVlabelClassification.txt", ios::out);
  _tabFiles[(int64_t)CVlabelClassificationOutput]->setf(ios::fixed, ios::floatfield);

  //char * errorMixmodFileName = "errorMixmod.txt";
  _tabFiles[(int64_t)errorMixmodOutput] = new ofstream("errorMixmod.txt", ios::out);
  _tabFiles[(int64_t)errorMixmodOutput]->setf(ios::fixed, ios::floatfield);

  //char * errorModelFileName = "errorModel.txt";
  _tabFiles[(int64_t)errorModelOutput] = new ofstream("errorModel.txt", ios::out);
  _tabFiles[(int64_t)errorModelOutput]->setf(ios::fixed, ios::floatfield);


  // DCV files
  _tabFiles[(int64_t)DCVinfo] = new ofstream("DCVinfo.txt", ios::out);
  _tabFiles[(int64_t)DCVinfo]->setf(ios::fixed, ios::floatfield);
  _tabFiles[(int64_t)DCVnumericInfo] = new ofstream("DCVnumericInfo.txt", ios::out);
  _tabFiles[(int64_t)DCVnumericInfo]->setf(ios::fixed, ios::floatfield);

  
    
  // verify if files are open
  for (i=0 ;i<_nbOutputFiles; i++){
    if (! _tabFiles[i]->is_open())
      throw errorOpenFile;
  }

  delete [] fileNameTmp;
  /*delete completeFileName;
  delete numericCompleteFileName;
  delete CVlabelClassificationFileName;
  delete errorMixmodFileName;
  delete errorModelFileName;
*/
  
}
