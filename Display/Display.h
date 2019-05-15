#pragma once
/////////////////////////////////////////////////////////////////////
// Display - DisplayFile Package   (Source Code Publisher)         //
// Vijay Thyagarajan, CSE687 - Object Oriented Design, Spring 2019 //
/////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * The Display Package file includes DisplayFile class that stores the
 * files names that needs to be displayed , the methods setApplicationPath() and
 * createCommandLineForFileName() are implemented to uses pass required parameters
 * for the Process package
 * -------------------
 * Required Files:
 * ---------------
 * Process.h, Process.cpp
 * ---------------
 Maintenance History:
  ===================
  ver 1.0 : 17 Jan 2019
  - first release
  - This Class takes in the files to be displayed and uses the process package to 
  - display the file on a browser. 
 */

#include<iostream>
#include<algorithm>
#include<regex>
#include<string>

class DisplayFile {
public:
	DisplayFile() {}
	virtual ~DisplayFile() {}
	using files = std::vector<std::string>;

	// Method to create a list of files that needs to be displayed 
	void createDisplayFileList(const DisplayFile::files& fileList);

	// Method  that returns the list of files
	const DisplayFile::files & getFilesToDisplay();

	//Method to display all the files 
	void displayFileWithPath();

	// Method to set the application path 
	void setApplicationPath(const std::string& appPath);

	// Method to create command line for each file  to display files on a default browser using cmd.
	std::string getFilePath();
	std::string createCommandLineForFileName(const std::string& filePath);

private:
	//storage for the list of files to display
	files filesToDisplay_;
	// parameters used to pass the required fields for the Process Package 
	std::string appPath_;
	std::string commandLine_;
	std::string title_;
};
