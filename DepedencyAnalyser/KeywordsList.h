#pragma once
////////////////////////////////////////////////////////////////////////////
// Keywordslist  - DependencyAnalyser Package (Source Code Publisher)     //
// Vijay Thyagarajan, CSE687 - Object Oriented Design, Spring 2019        //
////////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * The package gets the list of c++ key words from the file enclosed in the project
 * finds the addition key words like class name , functions namespaces and adds to the list
 * -------------------
 * Required Files:
 * ---------------
 * ---------------
 Maintenance History:
  ===================
  ver 1.0 : 27 Feb 2019
  - first release
  - The package makes a list of keywords using the treewalk in dependency analyser 
 */
#include<iostream>
#include<vector>
#include<fstream>
#include<string>
class Keyword
{
public:
	using keywordList = std::vector<std::string>;

	Keyword();
	~Keyword()
	{
	}
	// update the kewords list 
	void updateKeyWordSet(std::string keyword);
	// get the list of keyword 
	keywordList getKeyWordList();

private:
	keywordList keyWords_;

};
//<------------------ default constructor - Keyword class -----------------> 
Keyword::Keyword()
{
	std::ifstream keyWordFile("../c++KeyWords.txt");
	std::string keyWord;
	if (keyWordFile.good() && keyWordFile.is_open()) 
	{

		while (!keyWordFile.eof())
		{
			std::getline(keyWordFile, keyWord);
			keyWords_.push_back(keyWord);
		}
	}

}
//<--------- update the keywords list ------------------->
inline void Keyword::updateKeyWordSet(std::string keyword)
{
	if (!keyword.empty()) {
		if (std::find(keyWords_.begin(), keyWords_.end(), keyword) == keyWords_.end()) {
			keyWords_.push_back(keyword);
		}
	}
}
//<-------- returns the keyword list ----------------------->
inline std::vector<std::string> Keyword::getKeyWordList()
{
	return keyWords_;
}


