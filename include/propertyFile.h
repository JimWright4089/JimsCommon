//----------------------------------------------------------------------------
//
//  $Workfile: propertyFile.h$
//
//  $Revision: 1$
//
//  Modification History:
//  $Log:
//   1    Embedded   1.0         7/28/24, 8:29:21 PM PDTJim Wright (NE) Add the
//         properties file code
//  $
//
//  Notes:
//     This is the code for reading property files
//
//----------------------------------------------------------------------------
#ifndef PROPERTY_FILES_H
#define PROPERTY_FILES_H

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include <string>
#include <boost/property_tree/ptree.hpp>

//----------------------------------------------------------------------------
//  Class Declarations
//----------------------------------------------------------------------------
//
// Class Name: Connection
// 
// Purpose:
//      The storage location for the SQL Connection
//
//----------------------------------------------------------------------------
class PropertyFile
{
  public:

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
    PropertyFile();
    PropertyFile(std::string fileName);
    virtual ~PropertyFile() {}
    void loadFile(std::string fileName);

    std::string getString(std::string entry, std::string defaultString);
    int getInt(std::string entry, int defaultInt);
    void printTree();

  private:
    //----------------------------------------------------------------------------
    //  Class Atributes
    //----------------------------------------------------------------------------
    std::string mFileName = "";
    boost::property_tree::ptree mProperyTree;

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
    std::string printTree(boost::property_tree::ptree &propertyTree, int level, std::string full_text);
};
#endif
