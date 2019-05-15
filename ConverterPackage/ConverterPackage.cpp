/////////////////////////////////////////////////////////////////////
// Converter - ConverterFile Package   (Source Code Publisher)     //
// Vijay Thyagarajan, CSE687 - Object Oriented Design, Spring 2019 //
/////////////////////////////////////////////////////////////////////


#include<iostream>
#include<algorithm>
#include<string>
#include<regex>
#include<fstream>
#include<stdlib.h>
#include<direct.h>
#include<unordered_map>

#include"../CppParser/AbstractSyntaxTree/AbstrSynTree.h"
#include"../DirectoryNavigator/FileSystem/FileSystem.h"
#include"../DepedencyAnalyser/DependencyAnalyser.h"
#include"../CppParser/Utilities/Utilities.h"
#include"../CppParser/ScopeStack/ScopeStack.h"
#include"../DepedencyAnalyser/KeywordsList.h"
#include"../Display/Display.h"
#include "ConverterPackage.h"
#include"../Executive/Executive.h"


using files = std::vector<std::string>;
using ConvertedFilesList = std::vector<std::string>;

using namespace CodeAnalysis;

// <------- Method to convert the occurance of a string by another string  -----> 
std::string ConvertFile::replace_escapeCharacter(std::string const & inputString, std::string const & oldCharacterSet, std::string const & newCharacterSet)
{
	return std::regex_replace(inputString, std::regex(oldCharacterSet), newCharacterSet);
} 

//<-------  Method returns the files -----> 
const ConvertFile::ConverterFiles & ConvertFile::getFilesToConvert() 
{
	return files;
}
//<---------- Method to create the publisher code into a html code ----------------->
std::vector<std::string> ConvertFile::createPublisherCode(ConverterFiles & files) 
{
	DependencyAnalyser  dpAnalyserObj_;
	dpAnalyserObj_.considerFilesForDependencyAnalysis(files);
	dependencyFileList =  dpAnalyserObj_.updateDependencyTable();
	///std::cout << " Demonstrating Requirement 6: The selected files are converted into a html document by Converter package \n";
	//std::cout << " Demonstrating Requirement 7: The Depenency analyser package is used to add the xml blocks to the comments, class and function blocks \n";
	//std::cout << " \n Only the dependencies that is within the regex  and the current directory of the file used are displayed as file dependencies \n";
	for (auto file : files) {
	 std::vector<std::string> code = addXmlActionBlockForPublisherCode(checkEscapeCharacterInPublisherCode( createPublisherCodeStringFromFile(file)));
	convertedFiles_.push_back(storeConvertedCode(mergeTemplateStringWithPublisherCodeString(createTemplateStringFromFile("../Template.txt"), code)));
	}
	return convertedFiles_;
}
//<-------------- Method to store the list of files ------------->
void ConvertFile::storeFilesToConvert(const ConverterFiles & filesList)
{
	files = filesList;
}

//<--------------- Method to access the regex used from the Executive package-------------->
std::string ConvertFile::getRegexUsed()
{
	return regex_;
}

//<------- Method to convert html template file into bag of strings  -----> 

files ConvertFile::createTemplateStringFromFile(std::string templateFileName)
{

	std::ifstream xmlFile(templateFileName);
	std::vector<std::string> xmlString;
	std::string tempString;
	
	if (xmlFile.good() && xmlFile.is_open()) {
		while (!xmlFile.eof()) {
			std::getline(xmlFile, tempString);
			xmlString.push_back(tempString);
		}
	}
	xmlFile.close();

	return xmlString;
}

//<------- Method to convert file  into bag of Strings  -----> 
files ConvertFile::createPublisherCodeStringFromFile(std::string publisherFileName) 
{
	std::ifstream publisherCode(publisherFileName);
	fileName = publisherFileName;

	std::vector<std::string> publisherCodeString;
	std::string tempString;
	
	if (publisherCode.good() && publisherCode.is_open()) {
		while (!publisherCode.eof()) {
			getline(publisherCode, tempString);
			publisherCodeString.push_back(tempString);
		}
	}
	publisherCode.close();

	return publisherCodeString;
}
//<------- Method to check the presence of special characters in the file that needs to be converted and replace wit appropriate ones -----> 
files ConvertFile::checkEscapeCharacterInPublisherCode(std::vector<std::string> publisherCodeString)
{

	for (auto it1 = publisherCodeString.begin(); it1 != publisherCodeString.end(); ++it1) {

		if ((*it1).find("<")) {
			*it1 = replace_escapeCharacter((*it1), "<", "&lt;");
		}
		if ((*it1).find(">")) {
			*it1 = replace_escapeCharacter((*it1), ">", "&gt;");
		}
	}

	return publisherCodeString;
}

//< ------- Method to Merge the template String and the strings of the file that needs to be converted ----->
files ConvertFile::mergeTemplateStringWithPublisherCodeString(std::vector<std::string> templateString,std:: vector<std::string> publisherCodeString)
{
	std::vector<std::string> mergedString;
	for (auto it1 = templateString.begin(); it1 != templateString.end(); it1++) {
		if ((*it1) == "<pre>") {
			mergedString.push_back(*it1);
			for (auto it2 = publisherCodeString.begin(); it2 != publisherCodeString.end(); ++it2) {
				mergedString.push_back(*it2);
			}
			it1++;
		}
		mergedString.push_back((*it1));
	}
	return mergedString;
}

int ConvertFile::dirExists(const char *path)
{
	struct stat info;

	if (stat(path, &info) != 0)
		return 0;
	else if (info.st_mode & S_IFDIR)
		return 1;
	else
		return 0;
}

// <------- Method to check and store the converted html files -----> 
std::string ConvertFile::storeConvertedCode(std::vector<std::string> mergedCode) 
{
	std::ofstream out;
	const char *dPath = "../convertedPages/";
	if (!dirExists(dPath)){
		int directoryCreated = _mkdir(dPath);
		if (directoryCreated) {
			std::cout << " The convertedPages folder is created to save the converted files\n";
		}
	}
	Executive exeObj_;
	std::string regex_ = getRegexUsed();
	std::regex r(regex_);
	std::string file = FileSystem::Path::getName(fileName);
	std::string storagePath = dPath+ file + ".html";
	
	out.open(storagePath);

	for (auto it1 = mergedCode.begin(); it1 != mergedCode.end(); it1++) {
		out << *it1 << std::endl;
		if (*it1 == "<body>") {
			if (!(FileSystem::Path::getName(fileName).empty())) {
				auto checkIterator = dependencyFileList.find(FileSystem::Path::getName(fileName));
				std::string temp;
				if (checkIterator != dependencyFileList.end()) {
					out << "		<h3> Dependencies:\n";
					for (auto files : (checkIterator->second)) {
						if (std::regex_match(FileSystem::Path::getName(files), r)) {
							files.erase(std::remove(files.begin(), files.end(), '"'), files.end());
							files.erase(std::remove(files.begin(), files.end(), ' '), files.end());
							out << "		<a href=\"" << FileSystem::Path::getName(files) << ".html\">" << FileSystem::Path::getName(files) << "</a>" << std::endl;
						}	
					}
					out << "		</h3> \n";
				}
			}
		}
	}
	out.close();
	return FileSystem::Path::getFullFileSpec(storagePath);
}
//<------------------ adding the xml characters to the publisher code ------------------------>
std::vector<std::string> ConvertFile::addXmlActionBlockForPublisherCode(std::vector<std::string> mergerCode) 
{
	Keyword keyListObj;
	//for class <span class="toggleClassDisplay"> and </span> str.insert (str.end(),3,'.'); str.insert(6,str2);  
	std::string ClassNodeType = "class"; std::string functionNodeType = "function";
	std::vector<std::string> mergedString;
	DependencyAnalyser dpAnalyserObj_;
	dpAnalyserObj_.getAllDependencies(fileName);
	value  = dpAnalyserObj_.getParsedResult();
	while (value.size() > 0) {
		//std::cout << value.top()->type << std::endl;
		Node *testNode = value.top();
		keyListObj.updateKeyWordSet(testNode->name);
		size_t size = testNode->endLineCount - testNode->startLineCount + 1;
		if (testNode->type == ClassNodeType) {
			//<button id = "hide-button">Hide button< / button>
			std::string tempStart =  mergerCode[testNode->startLineCount - 1].insert(0, " <span class='toggleClassDisplay'> ");
			mergerCode[testNode->startLineCount - 1] = tempStart;
			std::string tempEnd = mergerCode[testNode->endLineCount - 2] + "</span>";
			mergerCode[testNode->endLineCount-2] = tempEnd;
		}
		else if (testNode->type == functionNodeType) {
			std::string tempStart = mergerCode[testNode->startLineCount - 1].insert(0, " <span class='toggleMethodDisplay'>");
			mergerCode[testNode->startLineCount - 1] = tempStart;
			std::string tempEnd = mergerCode[testNode->endLineCount - 1] + "</span>";
			mergerCode[testNode->endLineCount - 1] = tempEnd;
		}
			value.pop();

	}

	handingCommentsInSourceCode(mergerCode);
	
	//std::vector<std::string> keyList = keyListObj.getKeyWordList();

	//handlingKeyWordsInPublisherCode(mergerCode, keyList);
	
	return mergerCode;
}
//<------------ method to handle the comments in the publisher code ------------>
void ConvertFile::handingCommentsInSourceCode(std::vector<std::string> & mergerCode)
{
	size_t count = 0;
	static bool commentCondition = true;
	if (mergerCode.size() > 0) {

		for (auto line : mergerCode) {
			size_t posOfSingleSlash = line.find('/');
			if ((posOfSingleSlash != line.npos) && (posOfSingleSlash != line.size() - 1) &&(line[posOfSingleSlash + 1] == '/') ){
				line = line.insert(posOfSingleSlash, "<div class =\"toggleCommentDisplay\">/");
				line += "</div>";
				mergerCode[count] = line;
			}
			else
			{
				size_t findLink = line.find("size_t posOfOpenComment");
				if (findLink == line.npos) {
					size_t posOfOpenComment = line.find("/*");
					size_t posOfCloseComment = line.find("*/");
					if (posOfOpenComment != line.npos) {
						commentCondition = true;
						line = line.insert(posOfOpenComment, "<div class =\"toggleCommentDisplay\">/");
						mergerCode[count] = line;
					}
					if (posOfCloseComment != line.npos && commentCondition) {
						line += "</div>";
						mergerCode[count] = line;
						commentCondition = false;
					}
				}
			}
			count++;
		}
	}
}
//<-------------------------- find the word in a string ---------------------------->
int ConvertFile::FindSubstringindex(std::string *parentstring,std::string *substring) {
	int e = 0, indx = -1;
	const char *parentcstring = parentstring->c_str();
	const char *csubstring = substring->c_str();

	for (size_t i = 0; i < strlen(parentcstring); i++) {
		if (parentcstring[i] == csubstring[e]) {
			if (e == 0) {
				std::string temp = parentstring->substr(i, strlen(csubstring));
				//
				if ((temp == *substring) &&  (isspace(parentcstring[i++ + strlen(csubstring)]))) {
					indx = i;
					return indx;
				}
			}
			e = 0;

		}
	}
	return -1;
}
//<------------------------------------ adding the xml to the key word in the publisher code --------------------------->
void ConvertFile::handlingKeyWordsInPublisherCode(std::vector<std::string> & mergerCode, std::vector<std::string> keyWordList)
{
	for (size_t i = 0 ; i < mergerCode.size(); i++) {
		for (auto key : keyWordList) {
			std::string replaceString = "<div class = \"keyWordClass\">" + key + "</div>";
		    
			mergerCode[i] = ReplaceStringInPlace(mergerCode[i], key, replaceString);
		}
	}
}
//<--------- updating the regex --------------->
void ConvertFile::updateRegex(std::string regex)
{
	regex_ = regex;
}

//<------------------------- replace the occurance of a string with another ------------------------------->
std::string ConvertFile::ReplaceStringInPlace(std::string & subject,std::string & search,std::string  replace)
{
	size_t pos = 0;
	while ((pos = FindSubstringindex(&subject,&search)) != std::string::npos) {
		subject.replace(pos-1, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

//-----------------------------< Test Stub  for Converter package >----------------------------------------
#ifdef TEST_CONVERTERPACKAGE

int main()
{
 // creating the instance of the class ConvertFile()
 ConvertFile convertObj_;
 // variable to store files 
 files fileToTest;
 // Sample files to test the convertClass functionality
 fileToTest.push_back("Convert.cpp");
 fileToTest.push_back("Convert.h");
 convertObj_.createPublisherCode(fileToTest);

 return 0;
}

#endif  


