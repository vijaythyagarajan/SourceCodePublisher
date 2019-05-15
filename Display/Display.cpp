/////////////////////////////////////////////////////////////////////
// Display - DisplayFile Package   (Source Code Publisher)         //
// Vijay Thyagarajan, CSE687 - Object Oriented Design, Spring 2019 //
/////////////////////////////////////////////////////////////////////
#include"Display.h"
#include<iostream>
#include <regex>
#include"../Process/Process/Process.h"
#include "../DirectoryNavigator/FileSystem/FileSystem.h"

//----<  createDisplayFileList stores the files into the storage filesToDisplay_ >---- -
void DisplayFile::createDisplayFileList(const DisplayFile::files &fileList) {
	filesToDisplay_ = fileList;
}

//----< gets the list of files needs to be displayed  >---- -

const DisplayFile::files & DisplayFile::getFilesToDisplay() {
	return filesToDisplay_;
}

//----< Displays converted *html files on Browser using Process Class instance from the list of available *html files   >-----
void DisplayFile::displayFileWithPath() {
	std::cout << "\n Demonstrating Requirment 8:";
	std::cout << " Converted files are displayed using child process on browser." << std::endl;
	std::cout << "\n  You need to kill each window (upper right button) to continue.\n";
	std::cout << "\n 12 files are displayed and there respective path is displayed on the console \n";
	std::cout << "\n Each child process on the browser consist of Show/hide comments, Class and function blocks\n";
	// the displayCount is to keep the number of files displayed to a small number.
	size_t displayCount = 12;
	Process processObj_;
	processObj_.title(title_);
	std::string appPath = "C:\\Program Files (x86)\\Mozilla Firefox\\firefox.exe";  // path to application to start
	processObj_.application(appPath);
	auto fileIteratorStartPos = filesToDisplay_.begin();
	auto fileIteratorEndPos = filesToDisplay_.end();
	if (filesToDisplay_.size() > displayCount) {
		 fileIteratorEndPos = fileIteratorStartPos + 20;
	}

	while( fileIteratorStartPos != fileIteratorEndPos) {
			std::string val = "--new-window " + *(fileIteratorStartPos);
			std::string fileName = *fileIteratorStartPos;
			processObj_.commandLine(val);
			processObj_.create();
			CBP callback = [fileName]() {
				std::cout << fileName << std::endl;
			};
			processObj_.setCallBackProcessing(callback);
			processObj_.registerCallback();
			WaitForSingleObject(processObj_.getProcessHandle(), INFINITE);	

			fileIteratorStartPos++;
	}
}


//----< Get the applicationPath for the browser >-----
std::string DisplayFile::getFilePath() {
	return appPath_;
}

//----< Set the applcationPath of browser used to display the converted files >---- -
void DisplayFile::setApplicationPath(const std::string & appPath) {
	appPath_ = appPath;
}

//----< Helper method for creating commandLine for files to display >-----
std::string DisplayFile::createCommandLineForFileName(const std::string & filePath) {
	std::string commandLineForFile = "/c start \"link\" \"" + filePath;
	return  commandLineForFile;
}


#ifdef TEST_DISPLAY

int main(int argc, char *argv[]) {

	std::vector<std::string> filesList;
	filesList.push_back("Display.h");

	DisplayFile DisObj_;

	DisObj_.createDisplayFileList(filesList);
	DisObj_.displayFileWithPath();

	std::cin.get();
	std::cin.get();
}

#endif