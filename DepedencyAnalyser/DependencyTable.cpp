//////////////////////////////////////////////////////////////////////////////////
// Dependency table - Dependency Analyser Package   (Source Code Publisher)     //
// Vijay Thyagarajan, CSE687 - Object Oriented Design, Spring 2019              //
//////////////////////////////////////////////////////////////////////////////////
#include<iostream>
#include<string>
#include<unordered_map>

#include"DependencyTable.h"

//<--- adding dependency file to the fileList under each file name  in the dependency table ----->
bool DependencyTable::addDependencyToFileName(std::string fileName, const std::string & files) {
	if (!fileName.empty()) {
		if (hasEntry(fileName)) {
			auto iterator = table_.find(fileName);
			std::string copy = files;
			copy.erase(0, 1);
			copy.pop_back();
			if (std::find(iterator->second.begin(), iterator->second.end(), copy) == iterator->second.end())
			iterator->second.push_back(copy);
			return true;
		}
		else {
			std::vector<std::string> fileList;
			std::string copy = files;
			copy.erase(0, 1);
			copy.pop_back();
			if (std::find(fileList.begin(), fileList.end(), copy) == fileList.end()) {
				fileList.push_back(copy);
				table_[fileName] = fileList;
				fileList.clear();
			}
			return true;
		}
	}
	return false;
}
//<--- removing all the dependencies against the file Name specified from the dependency table  ----->
bool DependencyTable::removeDependencyForFileName(std::string const & fileName) {
	if (!fileName.empty()) {
		if (hasEntry(fileName)) {
			auto iterator = table_.find(fileName);
			iterator->second.clear();
			return true;
		}
	}
	return false;
}
//<--- to get all the dependency file Name as a vector from the dependency table  ----->
const filesList DependencyTable::getAllDependenciesForFileName(std::string const & fileName) {
	std::vector<std::string >v ;
	if (!fileName.empty()) {
		if (hasEntry(fileName)) {
			auto iterator = table_.find(fileName);
			return iterator->second;
		}
	}
	return v;
}

//<--- OperatorOverloading of [] to get all the dependencies of a file from the depenedency table ----->
const filesList & DependencyTable::operator[](const std::string & fileName) {
	auto iterator = table_.find(fileName);
	return iterator->second;
}

//<--- Clear the depenendency table ie., to make it empty ----->
void DependencyTable::clearTable() {
	table_.clear();
}
//<---- return the table ---->
std::unordered_map< std::string, filesList> DependencyTable::getTable()
{
	return table_;
}

//<----- method to display the dependecies for the file ----->
void DependencyTable::DisplayDependenciesForFileName(std::string const & fileName) {
	if (!fileName.empty()) {
		if (hasEntry(fileName)) {
			auto iterator = table_.find(fileName);
			std::cout << "File Name : " << fileName << std::endl;
			for (auto files : (iterator->second)) {
				std::cout << files << std::endl;
			}
		}
	}
}

// <------- check if the dependency table has any an entry for the file name
bool DependencyTable::hasDependency(std::string const & fileName)
{
	return hasEntry(fileName);
}

//<--- to check if the specified file name has an entry in the dependency tavble ----->
bool DependencyTable::hasEntry(std::string const & fileName) {

	if (! fileName.empty()) {
		if (table_.find(fileName) != table_.end()) {
			return true;
		}
	}
	return false;
}

// <------ Test stub for dependency table -------->
#ifdef DEPENDENCY_TABLE_TEST
iint main(int argc, char* argv[]) {
	// creating the dependency table object as d1
	DependencyTable d1;
	// geting the files and storing them in a vector
	std::vector<std::string> f1;
	for (int i = 0; i < argc; i++) {
		f1.pushBack(arrgv[i]);
	}
	// adding the dependency of file to a file name called newlist
	for (auto file : f1) {
		d1.addDependencyToFileName("newList", file);
	}
	// geting ghe dependencies for the file name newList
	std::vector<std::string> f2 = d1.getAllDependenciesForFileName("newList");
	// displaying the dependecies of the file 
	auto it1 = f2.begin();
	while (it1 != f2.end()) {
		std::cout << *it1 << std::endl;
		it1++;
	}
	// removing the dependencies for the file with file name newList
	std::cout << d1.removeDependencyForFileName("newList") << std::endl;
	std::string val = "newList";
	// checking the use of operator[] designed for the dependency table 
	std::vector<std::string> f3 = d1[val];
	// displaying the dependencies for the file 
	auto it2 = f3.begin();
	while (it2 != f3.end()) {
		std::cout << *it2 << std::endl;
		it2++;
	}
	std::cin.get();
	std::cin.get();
	return 0;
}
#endif // DEPENDENCY_TABLE_TEST

