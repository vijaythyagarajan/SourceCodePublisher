#pragma once
/////////////////////////////////////////////////////////////////////////////
// Dependency Analyser- DependencyAnalyser Package (Source Code Publisher) //
// Vijay Thyagarajan, CSE687 - Object Oriented Design, Spring 2019         //
/////////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * Dependency Analyser package along with Parser package is used to find all the dependencies 
 * for a file. The package can also be used to get all the dependencies, only the dependencies
 * within the files current directory.
 * -------------------
 * Required Files:
 * ---------------
 * Parser.h,ConfigureParser.h,AbstrSynTree.h
 * ConverterPackage.h
 * ---------------
 Maintenance History:
  ===================
  ver 1.0 : 12 Feb 2019
  - first release
  -  Dependency Analyser find the list of dependent file names
  -	 given a file name
  */
#include<iostream>
#include"DependencyTable.h"
#include<string>
#include "../CppParser/Parser/Parser.h"
#include "../CppParser/Parser/ConfigureParser.h"
#include"../CppParser/AbstractSyntaxTree/AbstrSynTree.h"
#include<unordered_map>
#include"../ConverterPackage/ConverterPackage.h"
#include<stack>


using namespace CodeAnalysis;

class DependencyAnalyser 
{

public:
	using filesList = std::vector<std::string>;
	~DependencyAnalyser()
	{

	}
	using dependencyFileList = std::unordered_map< std::string, filesList>;

	using parserResult = std::unordered_map< std::string, std::list<ASTNode>>;
	 // updating the list of files that needs to be analysed
	void considerFilesForDependencyAnalysis(const filesList &files);
	//  to update dependencies for a file 
	dependencyFileList updateDependencyTable();
	// method to get a list of files that needs to be analysed for dependencies
	filesList getFilesForAnalaysis();
	// method to get all the dependencies for the file name specified
	int getAllDependencies(std::string file);
	// to get the stack that holds the parser result
	std::stack<Node*> getParsedResult();

private:
	filesList filesList_;
	// to hold the file name that is currently processed by the parser
	std::string  fileName_;
	// a vector to hold all the dependencies for a file 
	filesList allDependenciesList_;
	// a vector to hold only the dependencies exculding the library ones
	filesList trueDependenciesList_;
	// To get all true dependencies
	filesList extractTrueDependencies();
	// perform a tree walk 
	void treeWalk(ASTNode *item);
	// a stack to hold the parser result
	std::stack<Node*> stack_;
	// To get the file extension for the file name
	std::string getFileExtensionForFile(std::string fileName);
};


