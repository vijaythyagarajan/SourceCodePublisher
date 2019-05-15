#pragma once
/////////////////////////////////////////////////////////////////////
// Converter - ConverterFile Package   (Source Code Publisher)     //
// Vijay Thyagarajan, CSE687 - Object Oriented Design, Spring 2019 //
/////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * The Converter Package file includes the ConverterFile class which 
 * takes in the list of files of type *h , *cpp and convert all the special characters
 * with their respective escape sequence characters and merges the file into an appropriate
 * template such that the file can be views on a browser.	
 * Takes a list of files that needs to be converted into a html file.
 * Converts the Template file and the converted files and merges it and stores its a .html file.
 * --------------------
 Maintenance History:
 ===================
  ver 1.0 : 17 Jan 2019
- first release
- Dependency table provides a mechanism to store the file name
- and the lsit of the files in which it depends on.
*/

#include<iostream>
#include<Windows.h>
#include<algorithm>
#include<regex>
#include<string>
#include<unordered_map>
#include<stack>

class Node 
{
public:
		std:: string type;
		std:: string name;
		size_t startLineCount = 0;
		size_t endLineCount = 0;
		Node ()
		{
		}
		// parameterised constructor for the Node class
		Node (std:: string type, std:: string name, size_t startLineCount,size_t endLineCount)
		{
			type = type;
			name = name;
			startLineCount = startLineCount;
			endLineCount = endLineCount;
		}
		// copy constructor for Node class 
		Node (const Node &l) 
		{
			type = l.type;
	 		name = l.name;
			startLineCount = l.startLineCount;
			endLineCount = l.endLineCount;
		}
		// initializer list for the node class
		Node (std:: initializer_list <Node> & list) 
		{
			auto it1 = list.begin();
			std:: stack <Node*> s1;
			while (it1 != list.end()) {
				Node *newNode = new Node();
				newNode->name = it1->name;
				newNode->type = it1->type;
				newNode->startLineCount = it1->startLineCount;
				newNode->endLineCount = it1->endLineCount;
				s1.push(newNode);
			}
		}
};


class ConvertFile 
{
	public:
		ConvertFile ()
		{
		}
		virtual ~ConvertFile ()
		{
		}
		using ConverterFiles = std::vector<std::string>;
		// A get method to get all the files that needs to be converted for displaying on thw browser
		const ConvertFile::ConverterFiles& getFilesToConvert();
		// method to get the convert the file into *html file
		std:: vector <std:: string>  createPublisherCode(ConverterFiles &files);
		// method to store the converted file in a perticular directory 
		void storeFilesToConvert(const ConverterFiles& filesList);
		// get the regex used in the command line
		std::string getRegexUsed();
		//  updating the regex to use in the dependency analysis
		void updateRegex(std::string regex);
		//void testDependencies(std::string fileName);
		std::stack<Node*> value;
	private:
		ConverterFiles files;
		ConverterFiles convertedFiles_;
		std::string fileName;
		std::string regex_;
		using filesList = std:: vector <std::string>;
		std:: unordered_map< std:: string,filesList> dependencyFileList;
		// A helper method to replace the occurance of a character with another character
		std:: string replace_escapeCharacter(std:: string const & inputString, std:: string const & oldCharacterSet, std:: string const & newCharacterSet);
		// Create a Template String fron the Template.txt file 
		std:: vector <std:: string>  createTemplateStringFromFile(std::string templateFileName);
		// Convert file to be converted into a bag of strings
		std::vector<std::string> createPublisherCodeStringFromFile(std::string publisherFileName);
		// check the presence of special characters that needs to replaced with their 
		std::vector <std:: string> checkEscapeCharacterInPublisherCode(std::vector<std::string> publisherCodeString);
		// Method to merge the template string and file to display
		std::vector <std:: string> mergeTemplateStringWithPublisherCodeString(std::vector<std::string> templateString, std::vector<std::string> publisherCodeString);
		int dirExists( const char *path);
		// Method to store the converted file 
		std::string storeConvertedCode(std::vector<std::string> mergedCode);
		// add the xml to the publisher code 
		std::vector <std:: string> addXmlActionBlockForPublisherCode(std:: vector <std::string> mergerCode );		
		// handing comments in the publisher code
		void handingCommentsInSourceCode(std::vector<std::string> & mergerCode);
		// replace the occurance of a string with another string
		std::string ReplaceStringInPlace(std::string& subject,std::string & search, std::string replace);
		// find the index of the substring in the string
		int FindSubstringindex( std::string *parentstring,  std::string *substring);
		void handlingKeyWordsInPublisherCode(std::vector<std::string> & mergerCode, std::vector<std::string> keyWordList);
};

