#pragma once
/////////////////////////////////////////////////////////////////////
// AbstractPublisher - Executive Package   (Source Code Publisher) //
// Vijay Thyagarajan, CSE687 - Object Oriented Design, Spring 2019 //
/////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * The Executive Package file includes Executive class and the AbstractPublisher class with Object factory.
 * The AbstractPublisher class has virtual methods convertFiles() , setCommandLineArgumentParamaeter() and 
 * ConvertFilesList() files that are mplemented in the exective class.
 * -------------------
 * Required Files:
 * ---------------
 * AbstractPublisher.cpp 
 * ---------------
  Maintenance History:
  ===================
  ver 1.0 : 5 April 2019
  - first release
 */
#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif
#include<iostream>
#include<string>
#include<vector>

class AbstractPublisher {
public:
	virtual void setCommandLineArgumentParameter(std::string path, std::string option,std::string pattern, std::string regex) = 0;
	virtual void convertFiles() = 0;
	virtual std::vector<std::string>  convertedFilesList() = 0;
};

extern "C" {
	// Instantiates publisher* as *executive
	struct ObjectFactory {
		// See Executive.cpp for implementation
		DLL_DECL AbstractPublisher* createClient();
	};
}
