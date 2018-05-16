#pragma	once

#include "common.hpp"

#include <map>
#include <list>
#include <functional>
#include <iostream>
#include <string>
#include <set>
#include <sstream>
#include <memory>

class TestPoint
{
public:
	TestPoint(const std::string& name)
		: m_name(name)
	{}

	virtual ~TestPoint()
	{}

	const std::string& getName() const
	{
		return m_name;
	}

	typedef std::shared_ptr<TestPoint> ptr;
	virtual void run() = 0;
private:
	const std::string m_name;
};

class UnitTest
{
public:
	typedef std::shared_ptr<UnitTest> ptr;
	typedef std::list<TestPoint::ptr> TestPoints;
public:
	explicit UnitTest(const std::string& name)
		: m_name(name)
	{}

	const std::string& getName() const
	{
		return m_name;
	}

	const TestPoints& getTestPoints() const
	{
		return m_points;
	}

	void addTestPoint(TestPoint::ptr p)
	{
		m_points.push_back(p);
	}
private:
	const std::string m_name;
	TestPoints m_points;
};

class TesttSuites
{
private:
	struct UnitTestComparator
	{
		bool operator()(const UnitTest::ptr& a, const UnitTest::ptr& b) const
		{
			return a->getName() < b->getName();
		}
	};
	typedef std::set<UnitTest::ptr, UnitTestComparator> tests;
public:
	static void registerUnitTest(UnitTest::ptr p)
	{
		s_tests.insert(p);
	}

	static UnitTest::ptr findUnitTest(const std::string& name)
	{
		UnitTest::ptr test(new UnitTest(name));
		auto found = s_tests.find(test);
		return found != s_tests.end() ? *found : UnitTest::ptr();
	}

	static void run()
	{
		for (auto it : s_tests) {
			std::cout << "Runing... Test: " << it->getName()<< std::endl;
			for (TestPoint::ptr testPoint : it->getTestPoints()) {
				std::cout << "\t\tTest-point: " << testPoint->getName() << std::endl;
				testPoint->run();
			}
		}
	}
private:
	static tests s_tests;
};

TesttSuites::tests TesttSuites::s_tests;

template <typename TestPointType>
UnitTest::ptr getOrCreateOwner(const std::string& name,
				const common::CodeLocation& codeLocation)
{
	TestPoint::ptr testPoint(new TestPointType);
	UnitTest::ptr test = TesttSuites::findUnitTest(name);
	if (0 == test) {
		test.reset(new UnitTest(name));
		TesttSuites::registerUnitTest(test);
	}
	test->addTestPoint(testPoint);
	return test;
}

#define CLASS_NAME(testName, testPointName) testName##_##testPointName

// Helper macro for defining tests.
#ifndef TEST
#define TEST(testName, testPointName) \
	class CLASS_NAME(testName, testPointName)  \
		: public TestPoint \
	{ \
	public:	\
		CLASS_NAME(testName, testPointName)() \
			: TestPoint(#testPointName) \
		{} \
		virtual void run(); \
		static UnitTest::ptr s_owner; \
	}; \
	UnitTest::ptr CLASS_NAME(testName, testPointName)::s_owner = \
			    getOrCreateOwner<CLASS_NAME(testName, testPointName)>(#testName, \
							common::CodeLocation(__FILE__, __LINE__)); \
	void CLASS_NAME(testName, testPointName)::run()
#endif // !TEST

//TODO: implement
//#define EXPECTED_EQUAL(a, b)
//#define EXPECTED_NOT_EQUAL(a, b)
//#define EXPECTED_TRUE(a)
//#define EXPECTED_FALSE(a)

#define RUN_UNIT_TESTS TesttSuites::run();

/*
usage:
	TEST(<test name>, <test point name>)
	{
		<test body>
	}
*/
