/////////////////////////////////////////////////////////////////////////////
// Shim.h  : defines a warpper for ICppClass which works with .NET code    //
// ver 1.0                                                                 //
//                                                                         //
// Application    : CSE-687 C++/CLI managed class for Project3             //
// Platform       : Visual Studio 2017 Community Edition                   //
//                                                                         //
// Author         : Vijay Thyagarajan, EECS Department, Syracuse University//
/////////////////////////////////////////////////////////////////////////////
/*
*  Package Description:
* ======================
*  This package defines Shim class which can be used by .NET applications.
*  In this demo, Shim is used in WPF project to allow making calls from
*  WPF C# code to CppClass native C++ code. The Shim is a mangaged (.NET) 
*  wrapper around ICppClass interface which can make calls to native C++
*  classes and, in the same time, can be called by any .NET code (C# here).
*
*  Public Interface:
* ===================
*  Shim localShim;
*  void convertFiles();
*  void setCommandLineArgumentParameter(System::String^ path,String^ option, String^ pattern,String^ regex);
*  System::Collections::Generic::List<System::String^>^  convertedFilesList();
*
*  Required Files:
* =================
*  Shim.h Shim.cpp ICppLibrary.h
*
*  Build Command:
* ================
*  msbuild Shim.vcxproj
*
*  Maintainence History:
* =======================
*  ver 1.0 - april 5th 2019
*    - first release
*
*/

#pragma once

#include <string>
#include "../Executive/AbstractExecutive.h"

// 'public ref' identifier tells the compiler this is
// a managed class, not native C++ class. this class
// has to be managed for the C# code in WPF to be able
// to use it. You CAN define native classes here but in
// this demo there is no point to do so. 

using namespace System;
public ref class Shim {
public:
  Shim();
  ~Shim();
  using str = System::String^;
  using list = System::Collections::Generic::List<str>;
  void convertFiles();
  void setCommandLineArgumentParameter(System::String^ path,String^ option, String^ pattern,String^ regex);
  System::Collections::Generic::List<System::String^>^  convertedFilesList();

private:
  // convert System::String to std::string
  std::string sysStrToStdStr(String^ str);
  // convert std::string to System::String
   String^ stdStrToSysStr(const std::string& str);
  // convert list<Sys::string> to vector<std::string>
  std::vector<std::string> sysListStdVector(System::Collections::Generic::List<System::String^> list);
  //convert vector<std::string> to list<Sys:string>
  System::Collections::Generic::List<System::String^>^ StdVectorSysList(std::vector<std::string> vec);
  // this will point to CppClass when instantiated through the ObjectFactory
  AbstractPublisher* client;
};
