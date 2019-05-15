/////////////////////////////////////////////////////////////////////////////
// Shim.cpp       : provides definitions and test stub for Shim.h          //
// ver 1.0                                                                 //
//                                                                         //
// Application    : CSE-687 C++/CLI Project 3                              //
// Platform       : Visual Studio 2017 Community Edition                   //                 
// Author         : Vijay Thyagarajan, EECS Department, Syracuse University//
/////////////////////////////////////////////////////////////////////////////

#include "Shim.h"

// -----< Shim constructor - instantiates ICppClass >------------------
Shim::Shim() {
  Console::Write("\n Requirement 3a: Shim created\n");
  ObjectFactory factory;
  // use factory to get instance of CppClient
  client = factory.createClient();
}


// -----< Shim destructor - deletes ICppClass >------------------------
Shim::~Shim() {
  delete client;
  Console::Write("\n  Shim destroyed\n\n");
}

// ------------< convert files : Calling the convertfiles() in cppclass Executive>-------------------
void Shim::convertFiles()
{
	client->convertFiles();
}
//-------------< set commmand line argument: calling setCommandLineArgumentParameter() in cpp Executive------
void Shim::setCommandLineArgumentParameter(String^ path, String^ option, String^ pattern, String^ regex)
{
	Console::Write("\n Requirement 5:Passing the command line argument to the publisher package \n");
	client->setCommandLineArgumentParameter(sysStrToStdStr(path),sysStrToStdStr(option),sysStrToStdStr(pattern), sysStrToStdStr(regex));
}
//---------------< converting vector of string files into list of string files 
System::Collections::Generic::List<System::String^>^ Shim::convertedFilesList()
{
	Console::Write("Converting the files list from the converter into a list of string file names for .net");
	return StdVectorSysList(client->convertedFilesList());
}

//----< convert std::string to System.String >-------------------------
String^ Shim::stdStrToSysStr(const std::string& str) {
  return gcnew String(str.c_str());
}
//------------<Convert System list into std vector >-------------------------------------
std::vector<std::string> Shim::sysListStdVector(System::Collections::Generic::List<System::String^> list)
{
	std::vector<std::string> vec;

	for each (System::String^ string in list)
	{
		vec.push_back(sysStrToStdStr(string));
	}

	return vec;
}
//------------------------< Convert std vector to system list>-----------------
System::Collections::Generic::List<System::String^>^  Shim::StdVectorSysList(std::vector<std::string> vec) {
	System::Collections::Generic::List<System::String^> ^list = gcnew System::Collections::Generic::List<System::String^>;

	for (size_t in = 0; in < vec.size(); in++) {
		list->Add(stdStrToSysStr(vec[in]));
	}
	return list;
}


//----< convert System.String to std::string >-------------------------
std::string Shim::sysStrToStdStr(String^ str) {
  std::string temp;
  for (int i = 0; i < str->Length; ++i)
    temp += char(str[i]);
  return temp;
}


// -----< Shim Test Stub >---------------------------------------------
#ifdef TEST_SHIM

int main(array<System::String^> ^args) {
  Shim localShim;  // C++ style construction
  // Shim^ shim = gcnew Shim(); // managed heap allocation and construction

  localShim.setCommandLineArgumentParameter("..", "/s", "*.h *.cpp","[A-H](.*)"); // passing the commandline argument 
  System::Collections::Generic::List<System::String^> ^list = localShim.convertedFilesList(); // converting the std vector to system list
  std::cin.get();
  return 0;
}
// NOTE: Shim destructor will automatically be called here just like in native C++

#endif