////////////////////////////////////////////////////////////////////////////////////
// Dependency Analysis - DependencyAnalyser Package   (Source Code Publisher)     //
// Vijay Thyagarajan, CSE687 - Object Oriented Design, Spring 2019				  //
////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include<queue>
#include "../DirectoryNavigator/FileSystem/FileSystem.h"
#include"../CppParser/AbstractSyntaxTree/AbstrSynTree.h"
#include<string>
#include "DependencyAnalyser.h"

using namespace CodeAnalysis;
//<------- get the list of files for performing dependency analysis
void DependencyAnalyser::considerFilesForDependencyAnalysis(const filesList & files) 
{
	filesList_ = files;
}
// < ------- updating the dependency table with the extracted dependencies for the given file name ------>
std::unordered_map< std::string, filesList> DependencyAnalyser::updateDependencyTable()
{
	std::unordered_map< std::string, filesList> dependencyFiles;
	std::vector<std::string> files = getFilesForAnalaysis();
	std::string fileSpec;
	DependencyTable dpTableObj_;
	for (auto file : files) {
		getAllDependencies(file);
		std::vector<std::string> files = extractTrueDependencies();
		
		for (auto file_ : files) {
			dpTableObj_.addDependencyToFileName(fileName_, file_);
		}
		allDependenciesList_.clear();
		trueDependenciesList_.clear();
		filesList_.clear();
	}
	dependencyFiles = dpTableObj_.getTable();
	return dependencyFiles;
}
//<---------- to get the list of files for analysis ------->
 filesList DependencyAnalyser::getFilesForAnalaysis() 
 {
	return filesList_;
 }
 //<---------------- return the stack of parser result for the file name ------->
 std::stack<Node*> DependencyAnalyser::getParsedResult()
 {
	 return stack_;
 }
//<--------- extract the dependencies without the library dependencies --------->
 filesList DependencyAnalyser::extractTrueDependencies()
 {
	 std::string hastag = "#"; std::string includeKeyWord = "include";
	 for (auto file : allDependenciesList_) {
		 if (file.find(hastag) != std::string::npos && file.find(includeKeyWord) != std::string::npos ) {
			 auto filterIterator =  file.find('"');
			 if(filterIterator != std::string::npos) {
				 std::string fileName = file.substr(filterIterator, filterIterator + file.size() - 2);
					 if (std::find(trueDependenciesList_.begin(), trueDependenciesList_.end(), fileName) == trueDependenciesList_.end()) {
						 trueDependenciesList_.push_back(fileName);
					 }
			 }
			 
		 }
	 }
	 return trueDependenciesList_;
 }
 //<--------------- to extract the file's full extension -----------------> 
 std::string DependencyAnalyser::getFileExtensionForFile(std::string fileName) {
	 std::string fileExtension ;
	 std::string copyFileName_ = FileSystem::Path::getFullFileSpec(fileName);
	 size_t found = copyFileName_.find_last_of("\\");
	 if(found != std::string::npos)
		 fileExtension = copyFileName_.substr(0, found);

	 return fileExtension;
 }
 //<----------------- to extract all the file dependencies ------------------->
 int DependencyAnalyser::getAllDependencies( std::string file)
 {
	     std::string fileSpec;
		 fileSpec = FileSystem::Path::getFullFileSpec(file);
		 std::string msg = "Processing file" + fileSpec;
		 ConfigParseForCodeAnal configure;
		 Parser* pParser = configure.Build();
		 std::string name;
		 try
		 {
			 if (pParser)
			 {
				 name = FileSystem::Path::getName(file);
				 fileName_ = name;
				 if (!configure.Attach(fileSpec))
				 {
					 std::cout << "\n  could not open file " << name << std::endl;
					 return 1;
				 }
			 }
			 else
			 {
				 return 1;
			 }
			 Repository* pRepo = Repository::getInstance();
			 pRepo->package() = name;
			 while (pParser->next()) {
				 pParser->parse();
			 }
			 ASTNode* pGlobalScope = pRepo->getGlobalScope();
			 auto iter2 = pGlobalScope->statements_.begin();
			 while (iter2 != pGlobalScope->statements_.end()) {
				 allDependenciesList_.push_back((*iter2)->ToString());
				 iter2++;
			 }
			 complexityEval(pGlobalScope);
			 treeWalk(pGlobalScope);
		 }
		 catch (std::exception& ex)
		 {
			 std::cout << "\n\n    " << ex.what() << "\n  exception caught at line " << __LINE__ << "\n  in package \"" << name << "\"";
		 }
	 return 0;
 }
//<------------- perform tree walk to get the parser result and store in the stack_ ---------->
 void DependencyAnalyser::treeWalk(ASTNode * item) 
 {
	 static std::string path;
	 static size_t indentLevel = 0;
	 std::ostringstream out;
	 auto iter = item->children_.begin();
	 ++indentLevel;
	 Node *newNode = new Node();
	 newNode->name = item->name_;
	 newNode->type = item->type_;
	 newNode->startLineCount = item->startLineCount_;
	 newNode->endLineCount = item->endLineCount_;
	 stack_.push(newNode);
	 while (iter != item->children_.end())
	 { 
		 treeWalk(*iter);
		 ++iter;
	 }
	 --indentLevel;
 }

 //<------ test strub for dependency analyer -------------->
#ifdef DEPENDENCY_ANALYSIS_TEST

 int main(int argc, char* argv[]) {

	 DependencyAnalyser d1;
	 Util::Title("Testing Parser Class");
	 putline();

	 std::vector<std::string > files;
	 // Analyzing files, named on the command line

	 if (argc < 2)
	 {
		 std::cout
			 << "\n  please enter name of file to process on command line\n\n";
		 return 1;
	 }

	 std::string fileSpec;

	 for (int i = 1; i < argc; ++i)  // iterate over files
	 {
		 fileSpec = FileSystem::Path::getFullFileSpec(argv[i]);
		 std::string msg = "Processing file" + fileSpec;
		 Util::title(msg);

		 files.push_back(argv[i]);
	 }

	 d1.considerFilesForDependencyAnalysis(files);
	 d1.updateDependencyTable();
	 std::cin.get();

}

#endif // DEPENDENCY_ANALYSIS_TEST

 
