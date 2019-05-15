#pragma once
/////////////////////////////////////////////////////////////////////
// Executive - Executive Package   (Source Code Publisher)         //
// Vijay Thyagarajan, CSE687 - Object Oriented Design, Spring 2019 //
/////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * The Executive Package file includes Executive class that takes in the commandLine
 * argument and checks for the validity of the commandPath specified and searches for the files
 * in the given directory considering the filters provided in the commandLine by the user.
 * Once a list of files are generated they are converted into .html and then displayed on the
 * default browser.
 * -------------------
 * Required Files:
 * ---------------
 * DirExplorerN.h,FileSystem.h
 * StringUtilities.h,CodeUtilities.h
 * Process.h , Process.cpp
 * Display.h ,Display.cpp
 * Converter.h ,Converter.cpp
 * ---------------
  Maintenance History:
  ===================
  ver 1.0 : 11 Feb 2019
  - first release
  - Exectuive class converts and displays the file selected based on the regex supplied from the commandline argument.
  ver 2.0 : 5 April 2019
  - Making the exectuive class derived from abstractPublisher class and implementing the public methods
 */
#include "../Display/Display.h"
#include"AbstractExecutive.h"

class Executive :public AbstractPublisher {
public:
	using filesList = std::vector<std::string>;

	// Method to initiate the  conversion of selected Files
	filesList convertFiles(filesList &files);

	//Method to initiate the display of converted files
	void displayFiles(filesList &files);

	// Method to check for the commandline  error
	bool checkCommandLinePath(int argc, char** argv);

	//Method to search for files in the directory using DirExplorerN
	filesList searchForFiles(int argc, char** &argv);

	//Method to print the list of files on console
	void printFiles(const filesList &files);

    // Method to get the regular expression used for filtering 
	std::string getRegexString();

	// update the regex from commandline

	void updateRegexBeingUsed(std::string regex);


	void  setCommandLineArgumentParameter(std::string path, std::string option, std::string pattern, std::string regex) override;
	void convertFiles() override;
	std::vector<std::string>  convertedFilesList() override;

private:
	std::vector<std::string> convertedFilesList_;
	std::string regexString_;
	std::vector<char*> v1;

	int argc_;
	char **argv_;
	

};

