/***************************************************************************
                             SRC/MIXMOD/XEMModelType.cpp  description
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
#include "XEMModelType.h"
 
 //------------
// Constructor
//------------
XEMModelType::XEMModelType(){
  _nameModel = defaultGaussianModelName;
  _nbSubDimensionFree = 0;
  _tabSubDimensionFree = NULL;
  _subDimensionEqual = 0;

}


XEMModelType::XEMModelType(XEMModelName name, int64_t nbSubDimensionFree){
  _nbSubDimensionFree = nbSubDimensionFree;
  _tabSubDimensionFree = NULL;
  _subDimensionEqual = 0;

 _nameModel = name; 

}

XEMModelType::XEMModelType(const XEMModelType & iModelType){
  _nameModel = iModelType._nameModel;
  _subDimensionEqual = iModelType._subDimensionEqual;
  _nbSubDimensionFree = iModelType._nbSubDimensionFree;
  if ((_nbSubDimensionFree!=0) && (iModelType._tabSubDimensionFree)){
    _tabSubDimensionFree = new int64_t[_nbSubDimensionFree];
    recopyTab(iModelType._tabSubDimensionFree,_tabSubDimensionFree,_nbSubDimensionFree);
  }
  else{
    _tabSubDimensionFree = NULL;
  }
}


XEMModelType::~XEMModelType(){
  if (_tabSubDimensionFree){
      delete []  _tabSubDimensionFree;
      _tabSubDimensionFree = NULL;
  }
}





void XEMModelType::input(ifstream & fi, int64_t nbCluster){


  _nbSubDimensionFree = nbCluster;
  string keyWord = "";
  int64_t dim;
  string a = "";

    fi>>a;
      if (a.compare("Gaussian_p_L_I") == 0){
        _nameModel =  Gaussian_p_L_I;
      }
      else if (a.compare("Gaussian_p_Lk_I")==0){
        _nameModel =  Gaussian_p_Lk_I;
      }
      else if (a.compare("Gaussian_p_L_B")==0){
        _nameModel =  Gaussian_p_L_B;
      }
      else if (a.compare("Gaussian_p_Lk_B")==0){
        _nameModel = Gaussian_p_Lk_B;
      }
      else if (a.compare("Gaussian_p_L_Bk")==0){
        _nameModel =  Gaussian_p_L_Bk;
      }
      else if (a.compare("Gaussian_p_Lk_Bk")==0){
        _nameModel =  Gaussian_p_Lk_Bk;
      }
      else if (a.compare("Gaussian_p_L_C")==0){
        _nameModel =  Gaussian_p_L_C;
      }
      else if (a.compare("Gaussian_p_Lk_C")==0){
        _nameModel =  Gaussian_p_Lk_C;
      }
      else if (a.compare("Gaussian_p_L_D_Ak_D")==0){
        _nameModel =  Gaussian_p_L_D_Ak_D;
      }
      else if (a.compare("Gaussian_p_Lk_D_Ak_D")==0){
        _nameModel =  Gaussian_p_Lk_D_Ak_D;
      }
      else if (a.compare("Gaussian_p_L_Dk_A_Dk")==0){
       _nameModel =  Gaussian_p_L_Dk_A_Dk;
      }
      else if (a.compare("Gaussian_p_Lk_Dk_A_Dk")==0){
        _nameModel =  Gaussian_p_Lk_Dk_A_Dk;
      }
      else if (a.compare("Gaussian_p_L_Ck")==0){
        _nameModel =  Gaussian_p_L_Ck;
      }
      else if (a.compare("Gaussian_p_Lk_Ck")==0){
        _nameModel =  Gaussian_p_Lk_Ck;
      }
      else if (a.compare("Gaussian_pk_L_I")==0){
        _nameModel =  Gaussian_pk_L_I;
      }
      else if (a.compare("Gaussian_pk_Lk_I")==0){
        _nameModel =  Gaussian_pk_Lk_I;
      }
      else if (a.compare("Gaussian_pk_L_B")==0){
        _nameModel =  Gaussian_pk_L_B;
      }
      else if (a.compare("Gaussian_pk_Lk_B")==0){
        _nameModel = Gaussian_pk_Lk_B;
      }
      else if (a.compare("Gaussian_pk_L_Bk")==0){
        _nameModel =  Gaussian_pk_L_Bk;
      }
      else if (a.compare("Gaussian_pk_Lk_Bk")==0){
        _nameModel =  Gaussian_pk_Lk_Bk;
      }
      else if (a.compare("Gaussian_pk_L_C")==0){
        _nameModel =  Gaussian_pk_L_C;
      }
      else if (a.compare("Gaussian_pk_Lk_C")==0){
        _nameModel =  Gaussian_pk_Lk_C;
      }
      else if (a.compare("Gaussian_pk_L_D_Ak_D")==0){
        _nameModel = Gaussian_pk_L_D_Ak_D;
      }
      else if (a.compare("Gaussian_pk_Lk_D_Ak_D")==0){
        _nameModel =  Gaussian_pk_Lk_D_Ak_D;
      }
      else if (a.compare("Gaussian_pk_Lk_Dk_A_Dk")==0){
        _nameModel =  Gaussian_pk_Lk_Dk_A_Dk;
      }
      else if (a.compare("Gaussian_pk_L_Dk_A_Dk")==0){
        _nameModel =  Gaussian_pk_L_Dk_A_Dk;
      }
      else if (a.compare("Gaussian_pk_L_Ck")==0){
        _nameModel =  Gaussian_pk_L_Ck;
      }
      else if (a.compare("Gaussian_pk_Lk_Ck")==0){
        _nameModel =  Gaussian_pk_Lk_Ck;
      }

      // Binary models //
      else if (a.compare("Binary_p_E")==0){
        _nameModel =  Binary_p_E;
      }
      else if (a.compare("Binary_p_Ek")==0){
        _nameModel =  Binary_p_Ek;
      }
      else if (a.compare("Binary_p_Ej")==0){
        _nameModel =  Binary_p_Ej;
      }
      else if (a.compare("Binary_p_Ekj")==0){
        _nameModel =  Binary_p_Ekj;
      }
      else if (a.compare("Binary_p_Ekjh")==0){
        _nameModel =  Binary_p_Ekjh;
      }
      else if (a.compare("Binary_pk_E")==0){
        _nameModel =  Binary_pk_E;
      }
      else if (a.compare("Binary_pk_Ek")==0){
        _nameModel =  Binary_pk_Ek;
      }
      else if (a.compare("Binary_pk_Ej")==0){
        _nameModel =  Binary_pk_Ej;
      }
      else if (a.compare("Binary_pk_Ekj")==0){
        _nameModel =  Binary_pk_Ekj;
      }
      else if (a.compare("Binary_pk_Ekjh")==0){
        _nameModel =  Binary_pk_Ekjh;
      }

    //HDDA models

      else if (a.compare("Gaussian_HD_pk_AkjBkQkD")==0){
        _nameModel = Gaussian_HD_pk_AkjBkQkD;
        fi>>keyWord;
        ConvertBigtoLowString(keyWord);
        if (keyWord.compare("subdimensionequal")==0){
          fi >> dim;
          _subDimensionEqual = dim;
        }
        else{
          throw wrongSubDimension;
        }
      }
      else if (a.compare("Gaussian_HD_pk_AkjBQkD")==0){
        _nameModel = Gaussian_HD_pk_AkjBQkD;
        fi>>keyWord;
        ConvertBigtoLowString(keyWord);
        if (keyWord.compare("subdimensionequal")==0){
          fi >> dim;
          _subDimensionEqual = dim;
        }
        else{
          throw wrongSubDimension;
        }
      }
      else if (a.compare("Gaussian_HD_pk_AjBkQkD")==0){
        _nameModel = Gaussian_HD_pk_AjBkQkD;
        fi>>keyWord;
        ConvertBigtoLowString(keyWord);
        if (keyWord.compare("subdimensionequal")==0){
          fi >> dim;
          _subDimensionEqual = dim;
        }
        else{
          throw wrongSubDimension;
        }
      }
      else if (a.compare("Gaussian_HD_pk_AjBQkD")==0){
        _nameModel = Gaussian_HD_pk_AjBQkD;
        fi>>keyWord;
        ConvertBigtoLowString(keyWord);
        if (keyWord.compare("subdimensionequal")==0){
          fi >> dim;
          _subDimensionEqual = dim;
        }
        else{
          throw wrongSubDimension;
        }
      }
      else if (a.compare("Gaussian_HD_pk_AkBkQkD")==0){
        _nameModel = Gaussian_HD_pk_AkBkQkD;
        fi>>keyWord;
        ConvertBigtoLowString(keyWord);
        if (keyWord.compare("subdimensionequal")==0){
          fi >> dim;
          _subDimensionEqual = dim;
        }
        else{
          throw wrongSubDimension;
        }
      }
      else if (a.compare("Gaussian_HD_pk_AkBQkD")==0){
        _nameModel = Gaussian_HD_pk_AkBQkD;
        fi>>keyWord;
        ConvertBigtoLowString(keyWord);
        if (keyWord.compare("subdimensionequal")==0){
          fi >> dim;
          _subDimensionEqual = dim;
        }
        else{
          throw wrongSubDimension;
        }
      }
      else if (a.compare("Gaussian_HD_pk_AkjBkQkDk")==0){
        _nameModel = Gaussian_HD_pk_AkjBkQkDk;
        fi>>keyWord;
        ConvertBigtoLowString(keyWord);
        if (keyWord.compare("subdimensionfree")==0){
          _tabSubDimensionFree = new int64_t[_nbSubDimensionFree];
          for (int64_t k=0; k<_nbSubDimensionFree; k++){
            fi >> dim;
            _tabSubDimensionFree[k] = dim;
          }
        }
        else{
          throw wrongSubDimension;
        }
      }
      else if (a.compare("Gaussian_HD_pk_AkBkQkDk")==0){
        _nameModel = Gaussian_HD_pk_AkBkQkDk;
        fi>>keyWord;
        ConvertBigtoLowString(keyWord);
        if (keyWord.compare("subdimensionfree")==0){
          _tabSubDimensionFree = new int64_t[_nbSubDimensionFree];
          for (int64_t k=0; k<_nbSubDimensionFree; k++){
            fi >> dim;
            _tabSubDimensionFree[k] = dim;
          }
        }
        else{
          throw wrongSubDimension;
        }
      }
      else if (a.compare("Gaussian_HD_p_AkjBkQkD")==0){
        _nameModel = Gaussian_HD_p_AkjBkQkD;
        fi>>keyWord;
        ConvertBigtoLowString(keyWord);
        if (keyWord.compare("subdimensionequal")==0){
          fi >> dim;
          _subDimensionEqual = dim;
        }
        else{
          throw wrongSubDimension;
        }
      }
      else if (a.compare("Gaussian_HD_p_AkjBQkD")==0){
        _nameModel = Gaussian_HD_p_AkjBQkD;
        fi>>keyWord;
        ConvertBigtoLowString(keyWord);
        if (keyWord.compare("subdimensionequal")==0){
          fi >> dim;
          _subDimensionEqual = dim;
        }
        else{
          throw wrongSubDimension;
        }
      }
      else if (a.compare("Gaussian_HD_p_AjBkQkD")==0){
        _nameModel = Gaussian_HD_p_AjBkQkD;
        fi>>keyWord;
        ConvertBigtoLowString(keyWord);
        if (keyWord.compare("subdimensionequal")==0){
          fi >> dim;
          _subDimensionEqual = dim;
        }
        else{
          throw wrongSubDimension;
        }
      }
      else if (a.compare("Gaussian_HD_p_AjBQkD")==0){
        _nameModel = Gaussian_HD_p_AjBQkD;
        fi>>keyWord;
        ConvertBigtoLowString(keyWord);
        if (keyWord.compare("subdimensionequal")==0){
          fi >> dim;
          _subDimensionEqual = dim;
        }
        else{
          throw wrongSubDimension;
        }
      }
     else if (a.compare("Gaussian_HD_p_AkBkQkD")==0){
        _nameModel = Gaussian_HD_p_AkBkQkD;
        fi>>keyWord;
        ConvertBigtoLowString(keyWord);
        if (keyWord.compare("subdimensionequal")==0){
          fi >> dim;
          _subDimensionEqual = dim;
        }
        else{
          throw wrongSubDimension;
        }
      }
      else if (a.compare("Gaussian_HD_p_AkBQkD")==0){
        _nameModel = Gaussian_HD_p_AkBQkD;
        fi>>keyWord;
        ConvertBigtoLowString(keyWord);
        if (keyWord.compare("subdimensionequal")==0){
          fi >> dim;
          _subDimensionEqual = dim;
        }
        else{
          throw wrongSubDimension;
        }
      }
      else if (a.compare("Gaussian_HD_p_AkjBkQkDk")==0){
        _nameModel = Gaussian_HD_p_AkjBkQkDk;
        fi>>keyWord;
        ConvertBigtoLowString(keyWord);
        if (keyWord.compare("subdimensionfree")==0){
          _tabSubDimensionFree = new int64_t[_nbSubDimensionFree];
          int64_t k;
              for (k = 0; k < _nbSubDimensionFree ; k++){
                fi >>dim;
                _tabSubDimensionFree[k] = dim;
              }
        }
        else{
          throw wrongSubDimension;
        }
      }
      else if (a.compare("Gaussian_HD_p_AkBkQkDk")==0){
        _nameModel = Gaussian_HD_p_AkBkQkDk;
        fi>>keyWord;
        ConvertBigtoLowString(keyWord);
        if (keyWord.compare("subdimensionfree")==0){
          _tabSubDimensionFree = new int64_t [_nbSubDimensionFree];
          for (int64_t k=0; k<_nbSubDimensionFree;k++){
                fi >>dim;
                _tabSubDimensionFree[k] = dim;
              }
        }
        else{
          throw wrongSubDimension;
        }
      }

      else{
        throw wrongModelType;
      }
//delete[] a;




//delete[] keyWord;


}



// <<
ostream & operator<<(ostream & fo, XEMModelType & modelType){
  string name = XEMModelNameToString(modelType._nameModel);
  fo<<name<<endl;
  if (modelType._subDimensionEqual !=0){
    fo<<"subDimensionEqual = "<<modelType._subDimensionEqual<<endl;;
  }
  if (modelType._nbSubDimensionFree != 0 && modelType._tabSubDimensionFree){
    fo<<"subDimensionFree : "<<endl;
    for (int64_t i=0; i<modelType._nbSubDimensionFree; i++){
      fo<<modelType._tabSubDimensionFree[i]<<endl;
    }
    fo<<endl;
  }
  return fo;
}


