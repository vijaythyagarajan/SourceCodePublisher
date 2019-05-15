/////////////////////////////////////////////////////////////////////
// Executive - Executive Package   (Source Code Publisher)         //
// Vijay Thyagarajan, CSE687 - Object Oriented Design, Spring 2019 //
/////////////////////////////////////////////////////////////////////
#include<iostream>
#include"../DirectoryNavigator/DirExplorer-Naive/DirExplorerN.h"
#include"../DirectoryNavigator/Utilities/StringUtilities/StringUtilities.h"
#include"../DirectoryNavigator/Utilities/CodeUtilities/CodeUtilities.h"
#include"../ConverterPackage/ConverterPackage.h"
#include"../Display/Display.h"
#include"../Display/Display.cpp"
#include "Executive.h"
#include"AbstractExecutive.h"


using namespace FileSystem;
using namespace Utilities;

using filesList = std::vector<std::string>;

ProcessCmdLine::Usage customUsage() {
	std::string usage;
	usage += "\n  Command Line: path [/option]* [/pattern]*";
	usage += "\n    path is relative or absolute path where processing begins";
	usage += "\n    [/option]* are one or more options of the form:";
	usage += "\n      /s - walk directory recursively";
	usage += "\n    [pattern]* are one or more pattern strings of the form:";
	usage += "\n      *.h *.cpp *.cs *.txt or *.*";
	usage += "\n";
	return usage;
}


void Executive::setCommandLineArgumentParameter(std::string path, std::string option, std::string pattern,std::string regex)
{
	std::istringstream issPattern(pattern);
	std::vector<std::string> patternResult((std::istream_iterator<std::string>(issPattern)),
		std::istream_iterator<std::string>());
	std::istringstream issResult(regex);
	std::vector<std::string> regexResult((std::istream_iterator<std::string>(issResult)),
		std::istream_iterator<std::string>());
	std::istringstream issOption(option);
	std::vector<std::string> optionResult((std::istream_iterator<std::string>(issOption)),
		std::istream_iterator<std::string>());

	char *pth = (const_cast<char*>(path.c_str()));
	char *opt = (const_cast<char*>(option.c_str()));

	std::cout << pth << std::endl;
	std::cout << opt << std::endl;

	v1.push_back(nullptr);

	v1.push_back(pth);
	
	for (size_t i = 0; i < optionResult.size(); i++) {
		v1.push_back((const_cast<char*>(optionResult[i].c_str())));
	}

	for (size_t i = 0; i < patternResult.size(); i++) {
		v1.push_back((const_cast<char*>(patternResult[i].c_str())));
	}

	for (size_t i = 0; i < regexResult.size(); i++) {
		v1.push_back((const_cast<char*>(regexResult[i].c_str())));
	}
	
	argv_ = &v1[0];
	argc_ = v1.size();
	
	 char ** p = new char*[argc_];
	p = &v1[0];

	argv_ = p;
	argc_ = v1.size();

	
	convertFiles();

}

void Executive::convertFiles()
{
	argv_ = &v1[0];
	ProcessCmdLine processCmdLineObj_(argc_, &v1[0]);
	bool isCommandLineValid = checkCommandLinePath(argc_, &v1[0]);
	filesList files;
	if (isCommandLineValid) {
		argv_ = &v1[0];
		std::cout << " \n The CommandLine arugment is validated \n";
		files = searchForFiles(argc_,argv_);

		std::vector<std::string> filesToProcess;

		for (auto regex_str : processCmdLineObj_.regexes()) {
			std::regex r(regex_str);
			updateRegexBeingUsed(regex_str);
			for (auto file : files) {
				std::string fileName = FileSystem::Path::getName(file);

				if (std::regex_match(fileName, r)) {
					filesToProcess.push_back(file);
				}
			}
		}
		if (filesToProcess.empty()) {
			std::cout << " No Matches found \n";
			return ;
		}
		preface ("Executive Package (loader Package) , CovertedPackage , Dependency Analyser and DisplayPackage are used to Display the converted Files on the browser \n");
		std::cout << " --- The instance of the Executive() class from the Executive Package is created ---\n";
		std::cout << std::endl;

		std::cout << " \n The following files are filtered using the given Regex \n" << std::endl;
		printFiles(filesToProcess);

		std::cout << "\n The files are getting converted \n";
		convertedFilesList_ = convertFiles(filesToProcess);

		/*displayFiles(convertedFilesList_);*/
	}
}

std::vector<std::string> Executive::convertedFilesList()
{
	return convertedFilesList_;
}

// <------ Method to check for the commandline  error  ---->
bool Executive::checkCommandLinePath(int argc, char** argv) {
	std::cout << " The following is the CommandLine argument ";
	ProcessCmdLine processCmdLineObj_(argc, argv);
	processCmdLineObj_.usage(customUsage());
	preface(" Command Line: ");
	processCmdLineObj_.showCmdLine();
	putline();
	if (processCmdLineObj_.parseError())
	{
		processCmdLineObj_.usage();
		return false;
	}
	return true;
}

// <------ Method to search for files in the directory using DirExplorerN ---->
filesList Executive::searchForFiles(int argc, char** &argv) {
	ProcessCmdLine processCmdLineObj_(argc, argv);
	processCmdLineObj_.usage(customUsage());
	DirExplorerN de(processCmdLineObj_.path());
	for (auto patt : processCmdLineObj_.patterns()) {
		de.addPattern(patt);
	}
	if (processCmdLineObj_.hasOption('s')) {
		de.recurse();
	}
	de.search();

	return de.filesToProcess();
}

void Executive::printFiles(const filesList & files) {
	if (files.size() > 0) {
		for (auto file : files) {
			std::cout << file << std::endl;
		}
	}
}

std::string Executive::getRegexString()
{
	return regexString_;
}

void Executive::updateRegexBeingUsed(std::string regex)
{
	regexString_ = regex;
}

// <------ Method to initiate the  conversion of selected Files ---->

filesList Executive::convertFiles(filesList & files) {
	ConvertFile convertObj_;
	convertObj_.updateRegex(regexString_);
	return convertObj_.createPublisherCode(files);
}
// <------ Method to initiate the display of converted files ---->

void Executive::displayFiles(filesList & files) {
	DisplayFile displayObj_;
	displayObj_.createDisplayFileList(files);
	displayObj_.displayFileWithPath();
}

 AbstractPublisher *ObjectFactory ::createClient() {
	 std::cout << "Requirment 3b: Object facotory is created" << std::endl;
	return new  Executive;
}

#ifdef TEST_EXECUTIVE



 int main(int argc, char *argv[]) {
	std::cout << " \n Demonstrating Requirement 3: Executive package accpeting the commandline argument \n";
	Executive exeObj_;
	ProcessCmdLine processCmdLineObj_(argc, argv);
	bool isCommandLineValid = exeObj_.checkCommandLinePath(argc, argv);
	filesList files;

	if (isCommandLineValid) {
		std::cout << " \n Demonstrating Requirement 3b: The CommandLine arugment is validated \n";
		files = exeObj_.searchForFiles(argc, argv);

		std::vector<std::string> filesToProcess;

		for (auto regex_str : processCmdLineObj_.regexes()) {
			std::regex r(regex_str);
			exeObj_.updateRegexBeingUsed(regex_str);
			preface(" \n Demonstrating Requirement 3c: Regex used for the filtering files in the directory:");
			std::cout << regex_str << std::endl;
			for (auto file : files) {
				std::string fileName = FileSystem::Path::getName(file);

				if (std::regex_match(fileName, r)) {
					filesToProcess.push_back(file);
				}
			}
		}
		if (filesToProcess.empty()) {
			std::cout << "Demonstrating Requirement 3d: ";
			std::cout << " No Matches found \n";
			return 1;
		}
		std::cout << " Demonstrating Requirement 4:Executive Package (loader Package) , CovertedPackage , Dependency Analyser and DisplayPackage are used to Display the converted Files on the browser \n";
		std::cout << " --- The instance of the Executive() class from the Executive Package is created ---\n";
		std::cout << std::endl;

		std::cout << " \n Demonstrating Requirement 5: The following files are filtered using the given Regex and they are considered for displaying on the browser \n" << std::endl;
		exeObj_.printFiles(filesToProcess);

		auto convertedFilesList = exeObj_.convertFiles(filesToProcess);
		exeObj_.displayFiles(convertedFilesList);

	}
	std::cout << "To Exit the demostration of the Project press any key and then press Enter  \n";
	std::cin.get();
	return 0;
}
#endif // TEST_EXECUTIVE