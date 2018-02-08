///////////////////////////////////////////////////////////////////////
// TestNoSqlDb.h - Contains tests for each requirement of solution   //
// ver 1.0                                                           //
// Author : Namrata Arkalgud                                         //
///////////////////////////////////////////////////////////////////////

/*
* Package Operations:
* -------------------
* This package contains tests for each requirement.
* It calls the different packages mentioned below to execute requirement.
*
* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* Utilities.h, Utilities.cpp
* Persistence.h
* Query.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 5 Feb 2018
* - first release
*/

#pragma once
#include <iostream>
#include <iomanip>
#include <functional>
#include <unordered_map>
#include"../DbCore/DbCore.h"
#include"../Utilities/TestUtilities/TestUtilities.h"
#include"../Utilities/StringUtilities/StringUtilities.h"
#include "../Persistence/Persistence.h"
#include "../Query/Query.h"


using namespace NoSqlDb;
using namespace XmlProcessing;

//----< reduce the number of characters to type >----------------------

auto putLine = [](size_t n = 1, std::ostream& out = std::cout)
{
	Utilities::putline(n, out);
};


/////////////////////////////////////////////////////////
//////////////// DbProvider class
// - provides mechanism to share a test database between test functions
//   without explicitly passing as a function argument.

class DbProvider
{
public:
	DbCore<std::string>& db() { return db_; }
private:
	static DbCore<std::string> db_;
};

DbCore<std::string> DbProvider::db_;

///////////////////////////////////////////////////////////////////////
// test functions

//----< demo requirement #1 >------------------------------------------

bool testR1()
{
	Utilities::title("Demonstrating Requirement #1");
	std::cout << "\n  " << typeid(std::function<bool()>).name()
		<< ", declared in this function, "
		<< "\n  is only valid for C++11 and later versions.";
	putLine();
	return true; // would not compile unless C++11
}

//----< demo requirement #2 >------------------------------------------

bool testR2()
{
	Utilities::title("Demonstrating Requirement #2");
	std::cout << "\n  A visual examination of all the submitted code "
		<< "will show only\n  use of streams and operators new and delete.";
	putLine();
	return true;
}

//----< demo first part of requirement #3 >----------------------------

bool testR3a()
{
	Utilities::title("Demonstrating Requirement #3a - creating DbElement");
	std::cout << "\n  Creating a db element with key \"Fawcett\":";

	// create database to hold std::string payload

	DbCore<std::string> db;
	DbProvider dbp;
	dbp.db() = db;

	// create some demo elements and insert into db

	DbElement<std::string> demoElem;

	demoElem.name("Jim");
	demoElem.descrip("Instructor for CSE687");
	demoElem.dateTime(DateTime().now());
	demoElem.payLoad("The good news is ...");

	if (demoElem.name() != "Jim")
		return false;
	if (demoElem.descrip() != "Instructor for CSE687")
		return false;
	if (demoElem.dateTime().now() != DateTime().now())
		return false;
	if (demoElem.payLoad() != "The good news is ...")
		return false;

	showHeader();
	showElem(demoElem);

	db["Fawcett"] = demoElem;
	dbp.db() = db;
	putLine();
	return true;
}
//----< demo second part of requirement #3 >---------------------------

bool testR3b()
{
	Utilities::title("Demonstrating Requirement #3b - creating DbCore");

	DbProvider dbp;
	DbCore<std::string> db = dbp.db();

	DbElement<std::string> demoElem = db["Fawcett"];

	demoElem.name("Ammar");
	demoElem.descrip("TA for CSE687");
	demoElem.payLoad("You should try ...");
	db["Salman"] = demoElem;
	if (!db.contains("Salman"))
		return false;

	demoElem.name("Jianan");
	demoElem.payLoad("Dr. Fawcett said ...");
	db["Sun"] = demoElem;

	demoElem.payLoad("You didn't demonstrate Requirement #4");
	demoElem.name("Nikhil");
	db["Prashar"] = demoElem;

	demoElem.payLoad("You didn't demonstrate Requirement #5");
	demoElem.name("Pranjul");
	db["Arora"] = demoElem;

	demoElem.payLoad("You didn't demonstrate Requirement #6");
	demoElem.name("Akash");
	db["Anjanappa"] = demoElem;

	if (db.size() != 6)
		return false;

	std::cout << "\n  after adding elements with keys: ";
	DbCore<std::string>::Keys keys = db.keys();
	showKeys(db);
	putLine();

	std::cout << "\n  make all the new elements children of element with key \"Fawcett\"";
	db["Fawcett"].children().push_back("Salman");
	db["Fawcett"].children().push_back("Sun");
	db["Fawcett"].children().push_back("Prashar");
	db["Fawcett"].children().push_back("Arora");
	db["Fawcett"].children().push_back("Anjanappa");

	showHeader();
	showElem(db["Fawcett"]);

	db["Salman"].children().push_back("Sun");
	db["Salman"].children().push_back("Prashar");
	db["Salman"].children().push_back("Arora");
	db["Salman"].children().push_back("Anjanappa");

	// display the results

	putLine();
	std::cout << "\n  showing all the database elements:";
	showDb(db);
	putLine();

	std::cout << "\n  database keys are: ";
	showKeys(db);

	putLine();
	return true;
}

//----< requirement #4 and #5 >------------------------------------------
bool testR4and5()
{
	Utilities::title("Demonstrating Requirement #4, #5 - adding, deleting and modifying DB records");
	Query<std::string> q;
	DbElement<std::string> dbe;
	std::vector<std::string> children;
	dbe.name("Namrata");
	dbe.descrip("Student of CSE 687");
	dbe.dateTime(DateTime().now());
	dbe.payLoad("Hello world!");
	children.push_back("Child1");
	children.push_back("Child2");
	dbe.children(children);

	std::cout << "\nAdding Key: Arkalgud\n\n";
	q.addKey("Arkalgud", dbe).show();

	dbe.descrip("An interested student of CSE 687");
	std::cout << "\nModifying key: Arkalgud\n";
	q.modifyDb("Arkalgud", dbe).show();

	dbe.name("Sowmya");
	dbe.payLoad("Hello world from me too!");
	children.push_back("Child3");
	children.push_back("Child4");
	dbe.children(children);

	std::cout << "\nAdding Key: Sarkar\n";
	q.addKey("Sarkar", dbe).show();
	std::cout << "\nDeleting key: aaa\n";
	q.deleteKey("aaa").show();
	std::cout << "\nDeleting key: Sarkar\n";
	q.deleteKey("Sarkar").show();
	q.addKey("Sarkar", dbe);
	return true;
}

//----< requirement #6 and #7 >------------------------------------------
bool testR6and7()
{
	Utilities::title("Demonstrating Requirement #6 - Support querying");
	Query<PayLoad> q;
	DbElement<PayLoad> dbe;
	std::vector<std::string> children;
	dbe.name("Namrata");
	dbe.descrip("Student of CSE 687");
	dbe.dateTime(DateTime().now());
	children.push_back("Child1");
	children.push_back("Child2");
	dbe.children(children);
	q.addKey("Arkalgud", dbe);

	dbe.name("Sowmya");
	children.push_back("Child3");
	children.push_back("Child4");
	dbe.children(children);
	q.addKey("Sarkar", dbe);

	std::vector<std::string> cat;
	cat.push_back("name");
	cat.push_back("description");
	q.getDbCore().dbStore().at("Arkalgud").payLoad().setFilePath("Namrata");
	q.getDbCore().dbStore().at("Arkalgud").payLoad().setCategories(cat);

	dbe.name("Nammu");
	q.addKey("Prabhakara", dbe);

	q.show();

	std::cout << "\nRunning query 1: Value for key \"Arkalgud\"\n";
	q.valueForKey("Arkalgud").showElement();

	std::cout << "\nRunning query 2: Children for key \"Arkalgud\"\n";
	q.childrenForKey("Arkalgud");

	std::cout << "\nRunning query 3: Keys starting with \"Ark\"\n";
	q.from().select("key").where("(Ark)(.*)");

	std::cout << "\nRunning query 4: Keys with names starting with \"Nam\"\n";
	q.from().select("name").where("(Nam)(.*)");

	std::cout << "\nRunning query 5: checking for keys with date range from January to February 2018\n";
	bool res = q.dateRange(DateTime().makeTime(2018, 1, 5, 12, 50, 30) - DateTime().makeDuration(48, 30, 15, 10),
		DateTime().makeTime(2018, 2, 7, 12, 50, 30) + DateTime().makeDuration(48, 30, 15, 10));

	Utilities::title("Demonstrating Requirement #7 - Anding and Oring of keys returned from querying");
	std::cout << "\nResult from first query which gets keys with name Nam:\n";
	std::vector<std::string> v1 = q.from().select("name").where("(Nam)(.*)").getQueryKeys();
	std::cout << "\nResult from second query which gets keys with name Namm:\n";
	std::vector<std::string> v2 = q.from().select("name").where("(Namm)(.*)").getQueryKeys();
	std::cout << "\nResult from anding query 1 and 2:\n";
	q.andKeys(v1, v2);
	std::cout << "\nResult from oring query 1 and 2:\n";
	q.orKeys(v1, v2);
	return true;
}

//----< requirement #8 >------------------------------------------
bool testR8()
{
	Utilities::title("Demonstrating Requirement #8 - Persist and Restore the database to and from XML file");
	DbCore<std::string> db;
	Persistence<std::string> p;

	//Converts XML to DbCore object
	db = p.convertIntoObj("test.xml");

	//Converts DbCore to XML
	std::string res = toXml(db);
	std::cout << "\n\n Result after converting to XML: " << res << "\n\n";

	/*DbElement<std::string> dbe = db["Namrata"];
	dbe.name("Sam");
	dbe.descrip("Former student of CSE 687");
	dbe.dateTime(DateTime().now());
	std::cout << "\n\n Now adding record to DB and converting to XML: " << "\n";
	res = toXml(db); 
	std::cout << res << "\nPersisted XML can be found under \\Debug under project folder.\n\n";*/
	return true;
}

//----< requirement #9 >------------------------------------------
bool testR9()
{
	Utilities::title("Demonstrating Requirement #9 - Implementing payload as a class which contains a string for a file path and vector of strings for categories");
	DbCore<PayLoad> db;
	DbElement<PayLoad> dbe;
	dbe.name("Namrata");
	dbe.dateTime(DateTime().now());
	dbe.descrip("Demonstrating DB core instantiated with PayLoad");
	dbe.payLoad().setFilePath("../HelloWorld");
	std::vector<std::string> v;
	v.push_back("Child 1");
	v.push_back("Child 2");
	dbe.children(v);
	v.clear();
	v.push_back("Category 1");
	v.push_back("Category 2");
	dbe.payLoad().setCategories(v);
	db["Arkalgud"] = dbe;
	std::cout << "\nDisplaying DB with PayLoad:\n";
	showDb(db);
	return true;
}

//----< requirement #10 >------------------------------------------
bool testR10()
{
	Utilities::title("Demonstrating Requirement #10 - Solution contains DBCore, Executive, Query, Test and Persistence");
	std::cout << "\nVisual examination will show the package structure.\n";
	std::cout << "DBCore: Contains the classes and corresponding functionality required for implementing NoSQLDB.\n";
	std::cout << "Query: This package contains the functionality required to query from NoSQLDB.\n";
	std::cout << "Persistence: Contains the functionality to persist the DB as an XML file on the disk.\n";
	std::cout << "Test: Contains the functions to execute each test case.\n";
	std::cout << "Executive: This is the entry point of execution of the test cases defined in Test package.\n";
	std::cout << "\nAlternatively, running \"dir\" command under solution folder displays the packages.\n\n";
	return true;
}

//----< requirement #11 >------------------------------------------
bool testR11() {
	Utilities::title("Demonstrating Requirement #11 - XML containing project's package structure and dependancy relationship");
	std::string filePath = "PkgStr.txt";
	std::ifstream in(filePath);
	if (!in.good())
		throw(std::exception(("can't open source file " + filePath).c_str()));
	std::ostringstream out;
	out << in.rdbuf();
	std::string forOpening = std::move(out.str());
	std::cout << "\nThe XML with package structure is:\n" << forOpening << "\n\n";
	return true;
}

//----< requirement #12 >------------------------------------------
bool testR12() {
	Utilities::title("Demonstrating Requirement #12 - Requirements #2 to #9 are met");
	std::cout << "\nIf the statement \"all tests passed\" is printed in the end, requirement #12 is met.\n";
	return true;
}

//----< requirement #13 >------------------------------------------
bool testR13() {
	Utilities::title("Demonstrating Requirement #13 - PDF of the package diagram");
	std::string path = "..\\PackageDiagram.pdf";
	std::cout << "\nThe PDF is present in path: " << path << " i.e. in the same path as solution file.\n\n";
	return true;
}

//----< test stub >----------------------------------------------------
