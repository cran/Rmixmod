/***************************************************************************
                             SRC/MIXMOD/XEMCVCriterion.cpp  description
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
#include "XEMCVCriterion.h"
#include "XEMGaussianData.h"
#include "XEMRandom.h"
#include <list>



//------------
// Constructor
//------------
XEMCVCriterion::XEMCVCriterion(){
  _tabCVBlock = NULL;
  _nbCVBlock = 0;
  _CVinitBlocks = defaultCVinitBlocks;
}


//-----------
// Constructor
//------------
XEMCVCriterion::XEMCVCriterion(XEMOldInput * input){
  //updates _CVinitBlocks and _nbCVBlocks     
  _CVinitBlocks = input->_CVinitBlocks; 
  // _nbCVBlock is initialzed to the numberOfCVBlocks that the user wanted (issued from input)(or the default value)
  // this value could chane if nbSample is too small (will be done in createCVBlock)  
  _nbCVBlock = input->_numberOfCVBlocks;
  _tabCVBlock = NULL;
}


//----------
//Destructor
//----------
XEMCVCriterion::~XEMCVCriterion(){
  
  if (_tabCVBlock != NULL){
    for (int64_t v=0; v<_nbCVBlock; v++){
        delete [] _tabCVBlock[v]._tabWeightedIndividual;
    }
  delete [] _tabCVBlock;
  }
}



//---
//run
//---
void XEMCVCriterion::run(XEMModel * model, double & value, int64_t *& cvLabel, XEMErrorType & error){
  /* Compute CV (Cross Validation Criterion) */
  error = noError;
  XEMModel * CVModel = new XEMModel(model);
  cvLabel = new int64_t[model->getNbSample()];
  
  try{
  /*cout<<endl<<"Parametre en entree de CVCriterion::run"<<endl;
    (model->getParameter())->edit();*/
    double missClass = 0.0;
    XEMData * data   = model->getData();
    XEMSample * x;
    int64_t i, known_ki, v;
    
    //cout<<"weight XEMCVCriterion "<<data[0]._weightTotal<<endl;
    createCVBlocks(model);
    for(v=0; v<_nbCVBlock; v++){
  //cout<<"block :  "<<v<<endl;
      /*cout<<"XEMCVCriterion::run, block N "<<v<<endl;
      cout<<"fik sur le modele de d�art :"<<endl;
      model->editFik();*/
      CVModel->updateForCV(model, _tabCVBlock[v]);
    /* cout<<"fik sur le modele modifi�:"<<endl;
      CVModel->editFik();*/
      /*cout<<"parametre du modele modifi�:"<<endl;
      CVModel->getParameter()->edit();*/
      
      
      for (int64_t ii=0; ii<_tabCVBlock[v]._nbSample; ii++){
        i = _tabCVBlock[v]._tabWeightedIndividual[ii].val;
        //cout<<"individu : "<<i<<endl;
        known_ki  = model->getKnownLabel(i);
        x         = data->_matrix[i];
        cvLabel[i] = CVModel->computeLabel(x); 
        if (cvLabel[i] != known_ki){
          /*cout<<"labels differents dans CV pour l'individu : "<<i<<" de poids : "<<_tabCVBlock[v]._tabWeightedIndividual[ii].weight<<endl;*/
          missClass += _tabCVBlock[v]._tabWeightedIndividual[ii].weight;
        }
        cvLabel[i] += 1; // because computeLable returns an integer between 0 and nbCluster-1
      } 
    }
    delete CVModel; 
    value = missClass / data->_weightTotal;
  }
  catch(XEMErrorType & e){
    error = e;
    delete CVModel;
    delete [] cvLabel;
  }
}



//-------------------
//- CreateCVBlock
//-------------------
void XEMCVCriterion::createCVBlocks(XEMModel * model){
  int64_t i, v, index;
  XEMData * data = model->getData();
  double * weight = data->_weight;
  int64_t weightTotal = (int64_t) data->_weightTotal;
  int64_t nbSample = model->getNbSample();
  double sumWeight = 0.0;
  int64_t value = 0, sizeList;
  
  
  if(_nbCVBlock > weightTotal){ 
    _nbCVBlock = weightTotal;
  }
  _tabCVBlock    = new XEMCVBlock[_nbCVBlock];
    
  // creation of blocks
  if(_nbCVBlock == weightTotal){
    v = 0;
    for(i=0; i<nbSample; i++){
      sumWeight = 0.0;
      while (sumWeight< weight[i]){
        _tabCVBlock[v]._nbSample = 1;
        _tabCVBlock[v]._weightTotal = 1;
        _tabCVBlock[v]._tabWeightedIndividual = new TWeightedIndividual[1];
        _tabCVBlock[v]._tabWeightedIndividual[0].val = i;
        _tabCVBlock[v]._tabWeightedIndividual[0].weight = 1;
        sumWeight += 1;
        v++;              
      }
    }
    if (v != _nbCVBlock){
      //cout<<"cout erreur 4 ds XEMCVCriterion"<<endl;
      throw internalMixmodError;
    }
  }
  else{ // weightTotal > _nbCVBlocks
    if (_CVinitBlocks == CV_RANDOM){
      //cout<<"XEMCVCriterion CV_RANDOM"<<endl;
      // random 
      double * tabRandom = new double[weightTotal];
      int64_t * tabIndex    = new int64_t[weightTotal];
      index = 0;
      for(i=0; i<nbSample; i++){
        sumWeight = 0.0;
        while (sumWeight< weight[i]){
          tabRandom[index] = rnd();
          //cout<<"XEMCVCriterion tabRandom["<<index<<"]  "<<tabRandom[index]<<endl;
          tabIndex[index]  = i;
          sumWeight ++;
          index ++;
        }
      }
      if (index != weightTotal){
        //cout<<"cout erreur 5 ds XEMCVCriterion"<<endl;
        throw internalMixmodError;
      }
      quickSortWithOrder(tabRandom, tabIndex, 0, (weightTotal)-1);
    
      int64_t nbElt = (int64_t)( floor( ((double)(weightTotal)) / (double)(_nbCVBlock) )) ; 
      int64_t remaining = weightTotal - nbElt*_nbCVBlock;
      int64_t index = 0;
      for (int64_t v=0; v<_nbCVBlock; v++){
        int64_t weightTotalInCVBlock = nbElt;
        // List
        list<TWeightedIndividual*>  listCVBlock;
        list<TWeightedIndividual*>::iterator listIterator;
        list<TWeightedIndividual*>::iterator listBegin;
        list<TWeightedIndividual*>::iterator listEnd;
        sizeList = 0;
        
        if (remaining > 0){
          weightTotalInCVBlock++ ;
          remaining--;
        }
        
        for (i=0; i<weightTotalInCVBlock; i++){
          // add tabIndex[index] in List
          value = tabIndex[index];
          
          // Search if sample already exist in list //
          listBegin = listCVBlock.begin();
          listEnd   = listCVBlock.end();

          listIterator = listBegin;    
          while ( (listIterator != listEnd) && (value > (*listIterator)->val)){
            listIterator++;
          }
          // list empty //  
          if (listBegin==listEnd){
            TWeightedIndividual * elem = new TWeightedIndividual;
            elem->val = value;
            elem->weight = 1;
            listCVBlock.push_front(elem);
            sizeList++;      
          }
          else{
            // if elemn in end of list //
            if (listIterator==listEnd){
              listIterator--;
              if ((*listIterator)->val==value)
                (*listIterator)->weight += 1;
              else{
                TWeightedIndividual * elem = new TWeightedIndividual;
                elem->val = value;
                elem->weight = 1;
                listCVBlock.push_back(elem);
                sizeList++;
              }
            }    
            // elemen in begin or in middle of list //
            else{
              if ((*listIterator)->val==value)
                (*listIterator)->weight += 1;
              else{
                TWeightedIndividual * elem = new TWeightedIndividual;
                elem->val = value;
                elem->weight = 1;

                if (listIterator == listCVBlock.begin())
                  listCVBlock.push_front(elem);
                else
                  listCVBlock.insert(listIterator,1,elem);

                sizeList++;
              }
            }
          }
          
          index ++;
        }
         
        // update _tabCVBlock
        _tabCVBlock[v]._nbSample = sizeList;
        _tabCVBlock[v]._weightTotal = weightTotalInCVBlock;
        _tabCVBlock[v]._tabWeightedIndividual = new TWeightedIndividual[_tabCVBlock[v]._nbSample];
        
        listBegin = listCVBlock.begin();
        listEnd   = listCVBlock.end();
        listIterator = listBegin;    
        i=0;
        while ( (listIterator != listEnd) && i<_tabCVBlock[v]._nbSample){
          _tabCVBlock[v]._tabWeightedIndividual[i].val =  (*listIterator)->val;
          _tabCVBlock[v]._tabWeightedIndividual[i].weight =  (*listIterator)->weight;
          listIterator++;
          i++;
        }
        if (i != _tabCVBlock[v]._nbSample){
          //cout<<"cout erreur 6 ds XEMCVCriterion"<<endl;
          throw internalMixmodError;
        }
        while(! listCVBlock.empty()){
          TWeightedIndividual * pelem = * listCVBlock.begin();
          listCVBlock.pop_front();
          delete pelem;
        }
      } // end for v
  delete[] tabRandom;
  delete[] tabIndex;

    }
    //------------------------------
    else if(_CVinitBlocks == CV_DIAG){
      //cout<<"XEMCVCriterion CV_DIAG"<<endl;
    //------------------------------
      // Lists
      list<TWeightedIndividual*>   * listCVBlock = new list<TWeightedIndividual*> [_nbCVBlock];
      
      list<TWeightedIndividual*>::iterator listIterator;
      list<TWeightedIndividual*>::iterator listBegin;
      list<TWeightedIndividual*>::iterator listEnd;
      
      
      int64_t v = 0, i, w, nbTraited=0;
      for (i=0; i<nbSample; i++){
        for (w=0; w<weight[i]; w++){
          // add i in listCVBlock[v]
          //------------------------
         // cout<<"ajout de l'individu :  "<<i<<" au block : "<<v<<endl;
          listBegin = listCVBlock[v].begin();
          listEnd   = listCVBlock[v].end();
          listIterator = listBegin;    
          while ( (listIterator != listEnd) && (i > (*listIterator)->val)){
            listIterator++;
          }
          // list empty //  
          if (listBegin==listEnd){
            TWeightedIndividual * elem = new TWeightedIndividual;
            elem->val = i;
            elem->weight = 1;
            listCVBlock[v].push_front(elem);
          }
          else{
            // if elemn in end of list //
            if (listIterator==listEnd){
              listIterator--;
              if ((*listIterator)->val==i)
                (*listIterator)->weight += 1;
              else{
                TWeightedIndividual * elem = new TWeightedIndividual;
                elem->val = i;
                elem->weight = 1;
                listCVBlock[v].push_back(elem);
              }
            }    
            // elemen in begin or in middle of list //
            else{
              if ((*listIterator)->val==i)
                (*listIterator)->weight += 1;
              else{
                TWeightedIndividual * elem = new TWeightedIndividual;
                elem->val = i;
                elem->weight = 1;

                if (listIterator == listCVBlock[v].begin())
                  listCVBlock[v].push_front(elem);
                else
                  listCVBlock[v].insert(listIterator,1,elem);
              }
            }  
          
          } 
          
          v++;
          if (v == _nbCVBlock){
            v = 0;
          }
          nbTraited++;   
        }
      }
      if (nbTraited != weightTotal){
        //cout<<"cout erreur 1 ds XEMCVCriterion"<<endl;
        throw internalMixmodError;
      }
      
      // update _tabCVBlock
      for (v=0; v<_nbCVBlock; v++){
        _tabCVBlock[v]._nbSample = listCVBlock[v].size();
        _tabCVBlock[v]._tabWeightedIndividual = new TWeightedIndividual[_tabCVBlock[v]._nbSample];
        double weightTotalBlockV = 0;
        
        listBegin = listCVBlock[v].begin();
        listEnd   = listCVBlock[v].end();
        listIterator = listBegin;     
        i=0;
        while ( (listIterator != listEnd) && i<_tabCVBlock[v]._nbSample){
          _tabCVBlock[v]._tabWeightedIndividual[i].val =  (*listIterator)->val;
          _tabCVBlock[v]._tabWeightedIndividual[i].weight =  (*listIterator)->weight;
          weightTotalBlockV += _tabCVBlock[v]._tabWeightedIndividual[i].weight;
          listIterator++;
          i++;
        }
        _tabCVBlock[v]._weightTotal = weightTotalBlockV;
        if (i != _tabCVBlock[v]._nbSample){
          //cout<<"cout erreur 2 ds XEMCVCriterion"<<endl;
          throw internalMixmodError;
        }
      }


     for (v=0; v<_nbCVBlock; v++){
         while(! listCVBlock[v].empty()){
          TWeightedIndividual * pelem = * listCVBlock[v].begin();
          listCVBlock[v].pop_front();
          delete pelem;
        }
    }
      delete [] listCVBlock;
    }

    else{ //_CVinitBlocks != CV_RANDOM and != CV_DIAG){
      //cout<<"cout erreur 3 ds XEMCVCriterion"<<endl;
      throw internalMixmodError;
    }
  
  }// and of else (weightTotal > _nbCVBlocks)
  
 
 /* for (v=0; v<_nbCVBlock; v++){
    cout<<endl<<"bloc "<<v<<" taille : "<<_tabCVBlock[v]._nbSample<<endl;
    cout<<"bloc "<<v<<" poids total : "<<_tabCVBlock[v]._weightTotal<<endl;
    for (int64_t i=0; i<_tabCVBlock[v]._nbSample; i++){
      cout<<"individu n : "<<_tabCVBlock[v]._tabWeightedIndividual[i].val<<" - poids : "<<_tabCVBlock[v]._tabWeightedIndividual[i].weight<<endl;
}
}*/
    
}

