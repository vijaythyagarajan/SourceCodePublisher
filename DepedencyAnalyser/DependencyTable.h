#pragma once
////////////////////////////////////////////////////////////////////////////
// Dependency table  - DependencyAnalyser Package (Source Code Publisher) //
// Vijay Thyagarajan, CSE687 - Object Oriented Design, Spring 2019        //
////////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * The dependency table is a unorderMap datastructure with key being a file name as 
 * string and the value is a vector of string holding the corresponding dependencies
 * for the file.
 * Used by the dependency analyser package to hold the denpendencies for further use.
 * Provides public interface methods to add file and its dependencies,
 * remove a file and its entries in the table,
 * clear the entire dependency table ie.. earse option
 * check if the file has an entry in the table
 * -------------------
 * Required Files:
 * ---------------
 * unordered_map 
 * ---------------
 Maintenance History:
  ===================
  ver 1.0 : 12 Feb 2019
  - first release
  -  Dependency table provides a mechanism to store the file name 
  -  and the lsit of the files in which it depends on.
 */
#include"DependencyTable.h"
#include<unordered_map>


using filesList = std::vector<std::string>;

class DependencyTable {

public:
	using dependencyTable = std::unordered_map< std::string, filesList>;
	DependencyTable()
	{

	}
	~DependencyTable()
	{

	}
	bool addDependencyToFileName(std::string fileName, const std::string &files);
	bool removeDependencyForFileName(std::string const &fileName);
	const filesList getAllDependenciesForFileName(std::string const &fileName);

	const filesList & operator[](const std::string &fileName);
	void clearTable();
	dependencyTable getTable();
	void DisplayDependenciesForFileName(std::string const &fileName);

	bool hasDependency(std::string const & fileName);

private:
	dependencyTable table_;
	bool hasEntry(std::string const &fileName);

};

