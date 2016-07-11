/***************************************************************************
							 SRC/MIXMOD_IOSTREAM/XEMIOStreamUtil.h  description
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

#ifndef XEM_IOSTREAMUTIL_H
#define XEM_IOSTREAMUTIL_H

#include <typeinfo>

#include <libxml++/libxml++.h>
#include <sys/stat.h>

#include "mixmod/Clustering/ClusteringInput.h"
#include "mixmod/Clustering/ClusteringOutput.h"
#include "mixmod/Utilities/Util.h"

namespace XEM {

  const string XML_RESOURCES_PATH = XEM_RESOURCES_PATH;
  
class ClusteringMain;

enum class IOStreamFormat {
	XML,
	FLAT // old format (flat format)
};
const IOStreamFormat defaultIOStreamFormat = IOStreamFormat::XML;

enum class IOStreamColumnType {
	Qualitative,
	Quantitative,
	Individual,
	Weight,
	Unused
};

enum class IOStreamErrorType {
	noIOStreamError,
	badIOStreamFormat,
	badNumericFormat,
	badIOWriteFormat,
	badOpenedFile,
	badLoadXML,
	badSchema,
	badXML,
	badElementInXML,
	badElementInDataXML,
	badColumnUsedInCreateMixmodDataFileFromUserDataFile,
	errorInCreateMixmodDataFileFromUserDataFile,
	fileAlreadyExist,
	fileDontExist,
	fileCantBeOpened,
	notEnoughValuesInDataFile,
	notAbsoluteFileDataName
};

enum class IOStreamXMLFile {
	Project,
	Data,
	Partition,
	Parameter,
	Weights
};

//IOStreamErrorTypeToString
string IOStreamErrorTypeToString(const IOStreamErrorType & errorType);

///read input and output data
ClusteringMain * IStream(const std::string& s, 
		IOStreamFormat format = defaultIOStreamFormat, bool bOnlyInput = false, IoMode iomode = IoMode::NUMERIC);

///read input data in XML
ClusteringMain * IStream_XML(const string & s, bool bOnlyInput);

///read input data in FLAT
ClusteringMain * IStream_FLAT(const string & s);

///validate schema according to XML File
void ValidateSchema(const string& s, const IOStreamXMLFile& xmlFile);

///write input and output Data
void OStream(const string& s, IOStreamFormat format = defaultIOStreamFormat,
		ClusteringMain* cMain = NULL, IoMode iomode = IoMode::NUMERIC);

///write input and output Data in XML for clustering study
void OStream_Clustering_XML(const string & s, ClusteringMain * cMain);

///write output Data in FLAT format (txt) for clustering study
void OStream_Clustering_FLAT(ClusteringMain * cMain);

void OStream_DiscriminantAnalysis_XML(const string & s, ClusteringMain * cMain);

//VariableTypeToString
string ColumnTypeToString(const IOStreamColumnType & columnType);

//StringToVariableType
IOStreamColumnType StringToColumnType(const string & strColumnType);

/// convert XML File Type To QString
Glib::ustring XMLFileTypeToString(IOStreamXMLFile file);

void removeIfExists(const std::string& filename); 
 
/// createMixmodDataFileFromUserDataFile
/*
Note : if nbSample==0 : file is read (and write) until the end and nbSample is updated
  else : nbSample lines will be read (and write) 
 */
void createMixmodDataFileFromUserDataFile(string userDataFileName, string mixmodDataFileName, 
		vector<bool> & columnUsed, FormatNumeric::FormatNumericFile format, 
		bool individualNameMustBeGenerated, int64_t & nbSample);

inline Glib::ustring XMLFileTypeToString(IOStreamXMLFile file) {
	Glib::ustring res;
	switch (file) {
	case IOStreamXMLFile::Project:
		res = "Input";
		break;
	case IOStreamXMLFile::Data:
		res = "Data";
		break;
	case IOStreamXMLFile::Partition:
		res = "Partition";
		break;
	case IOStreamXMLFile::Parameter:
		res = "Parameter";
		break;
	case IOStreamXMLFile::Weights:
		res = "Weights";
		break;
	}
	return res;
}

//CPOLI 
xmlpp::Element *get_first_child_element(xmlpp::Node *parent);
// Boolean indicating if we save relative or absolute files names (inside .mixmod and .mx* files)
//    - relative paths are useful for non-regression tests
//    - absolute paths are useful to a mixmodGUI final user
// NOTE: always let it at 'false' when committing to SVN.
#define RELATIVE_PATHS true

// JF
// TODO
//XEMInput * XEMIStream_XML(istream & flux);
//void XEMOStream(ostream & flux, XEMIOStreamFormat format=defaultXEMIOStreamFormat, XEMInput & Input);
//ofstream flux("...", ios::out) puis XEMOStream(flux, XML, input)

//HACK: pass nbVariables in global variables (TODO: refactor)
struct Global {
	static int nbVariables_binary;
	static int nbVariables_gaussian;
	static std::vector<int64_t> vNbFactor;
};

} //end namespace

#endif // XEM_IOSTREAMUTIL_H
