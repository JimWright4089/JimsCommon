//----------------------------------------------------------------------------
//
//  $Workfile: propertyFile.cpp$
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
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <iostream>
#include <fstream>
#include <stdint.h>
#include "loggerSingleton.h"
#include "propertyFile.h"

std::string indent(int level)
{
    std::string s;
    for (int i = 0; i<level; i++) s += "  ";
    return s;
}

PropertyFile::PropertyFile()
{
}

PropertyFile::PropertyFile(std::string fileName)
{
    loadFile(fileName);
}

void PropertyFile::loadFile(std::string fileName)
{
    mFileName = fileName;
    
    try
    {
        boost::property_tree::ini_parser::read_ini(fileName, mProperyTree);
    }
    catch(const std::exception& e)
    {
        LoggerSingleton::getInstance()->writeError(e.what());
    }
}

std::string PropertyFile::getString(std::string entry, std::string defaultString="")
{
    std::string returnValue = defaultString;
    try
    {
        returnValue = mProperyTree.get<std::string>(entry);
    }
    catch(const std::exception& e)
    {
        LoggerSingleton::getInstance()->writeError(e.what() + std::string(" for ") + entry);
    }
    return returnValue;
}

int PropertyFile::getInt(std::string entry, int defaultInt=-1)
{
    int returnValue = defaultInt;
    try
    {
        returnValue = mProperyTree.get<int>(entry);
    }
    catch(const std::exception& e)
    {
        LoggerSingleton::getInstance()->writeError(e.what() + std::string(" for ") + entry);
    }
    return returnValue;
}

std::string PropertyFile::printTree(boost::property_tree::ptree &propertyTree, int level, std::string full_text)
{
    if (propertyTree.empty())
    {
        full_text += "\"" + propertyTree.data() + "\"";
    }
    else
    {
        if (level)
        {
            full_text += "\n";
        }

        full_text += indent(level) += "{\n";

        for (boost::property_tree::ptree::iterator pos = propertyTree.begin(); pos != propertyTree.end();)
        {
            full_text += indent(level + 1) + "\"" + pos->first + "\": ";

            full_text = printTree(pos->second, level + 1, full_text);
            ++pos;
        }

        full_text +=  indent(level) + " }";
    }
    full_text += "\n";
    return full_text;
}

void PropertyFile::printTree()
{
    std::string full_text = "";
    full_text = printTree(mProperyTree,0,full_text);
    LoggerSingleton::getInstance()->writeInfo(full_text);

}


