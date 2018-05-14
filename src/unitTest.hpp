#pragma	once

#include "common.hpp"

#include <map>
#include <list>
#include <functional>
#include <iostream>
#include <string>
#include <sstream>
#include <memory>


class Test
{
public:
	typedef std::shared_ptr<Test> ptr;

	virtual void run() = 0;
};

class TestCreatorBase
{
public:
	typedef std::shared_ptr<TestCreatorBase> ptr;

	virtual Test::ptr createTest() const = 0;
};

template <typename TestType>
class TestCreator : public TestCreatorBase
{
public:
	virtual Test::ptr createTest() const
	{
		return Test::ptr(new TestType());
	}
};

class TestInfo
{
public:
	typedef std::shared_ptr<TestInfo> ptr;
private:
	TestInfo(const std::string& testName, const std::string& testCaseName, 
		 const common::CodeLocation& codeLocation, TestCreatorBase::ptr creator)
		: m_testName(testName)
		, m_testCaseName(testCaseName)
		, m_codeLocation(codeLocation)
		, m_creator(creator)
	{
	}
public:
	static ptr registerTestInfo(const std::string& testName, const std::string& testCaseName, 
				    const common::CodeLocation& codeLocation, TestCreatorBase::ptr creator);

	std::string getTestName() const
	{
		return  m_testName;
	}

	std::string getTestCaseName() const
	{
		return  m_testCaseName;
	}

	const common::CodeLocation& getCodeLocation() const
	{
		return m_codeLocation;
	}

	Test::ptr createTest() const
	{
		return m_creator->createTest();
	}
private:
	const std::string m_testName;
	const std::string m_testCaseName;
	common::CodeLocation m_codeLocation;
	TestCreatorBase::ptr m_creator;
};

class UnitTest
{
public:
	static void regiterTest(TestInfo::ptr p)
	{
		s_testInfos[p->getTestName()].push_back(p);
	}

	static void run()
	{
		//TODO: implement
		/*for (auto it : s_testInfos) {
			std::cout << "Runing... Test: " << it.first << std::endl;
			for (TestInfo::ptr testInfo : it.second) {
				std::cout << "\t\tTest-case: " << testInfo->getTestCaseName() << std::endl;
				Test::ptr test = testInfo->createTest();
				test->run();
			}
		}*/
	}
private:
	static std::map<std::string, std::list<TestInfo::ptr> > s_testInfos;
};

std::map<std::string, std::list<TestInfo::ptr>> UnitTest::s_testInfos;

TestInfo::ptr 
TestInfo::registerTestInfo(const std::string& testName, const std::string& testCaseName,
			   const common::CodeLocation& codeLocation, TestCreatorBase::ptr creator)
{
	TestInfo::ptr p(new TestInfo(testName, testCaseName, codeLocation, creator));
	UnitTest::regiterTest(p);
	return p;
}

#define CLASS_NAME(testName, testCaseName) testName##_##testCaseName

// Helper macro for defining tests.
#ifndef TEST
#define TEST(testName, testCaseName) \
	class CLASS_NAME(testName, testCaseName) : public Test \
	{ \
	public:	\
		virtual void run(); \
		static TestInfo::ptr s_testInfo; \
	}; \
	TestInfo::ptr CLASS_NAME(testName, testCaseName)::s_testInfo = \
			    TestInfo::registerTestInfo(#testName, #testCaseName, common::CodeLocation(__FILE__, __LINE__), \
					               TestCreatorBase::ptr(new TestCreator<CLASS_NAME(testName, testCaseName)>)); \
	void CLASS_NAME(testName, testCaseName)::run()
#endif // !TEST



/*
usage:
	TEST(<test name>, <test case name>)
	{
		< test body>
	}
*/