#pragma once

#include <map>
#include <list>
#include <functional>
#include <iostream>
#include <string>

class UnitTests
{
public:
	typedef std::function<void()> testCaseFunction;

	class testCase
	{
	public:
		testCase(const std::string& name, testCaseFunction func)
			: m_name(name)
			, m_func(func)
		{}
	
		std::string m_name;
		testCaseFunction m_func;
	};

public:
	static void add(const std::string& testName, const std::string& testCaseName, testCaseFunction func)
	{
		s_tests[testName].push_back(testCase(testCaseName, func));
	}

	static void run()
	{
		for (auto i : s_tests) {
			std::cout << "Test: " << i.first << "\n";
			for (auto j : i.second) {
				std::cout << "\tCase: " << j.m_name << "\n";
				j.m_func();
			}
		}
	}
private:
	static std::map<std::string, std::list<testCase>> s_tests;
};

std::map<std::string, std::list<UnitTests::testCase>> UnitTests::s_tests;


class UnitTestAdder
{
public:
	UnitTestAdder(const std::string& testName, const std::string& testCaseName, UnitTests::testCaseFunction func)
	{
		UnitTests::add(testName, testCaseName, func);
	}
};

#ifndef PRE_CHECK_ARGS
#define PRE_CHECK_ARGS(testName, testCaseName);
#endif // !PRE_CHECK_ARGS

#ifndef TEST
#define TEST(testName, testCaseName)  \
	PRE_CHECK_ARGS(testName, testCaseName)	\
	void func_##testCaseName##_of_##testName(); 	\
	UnitTestAdder adder_##testCaseName##_for_##testName(#testName, #testCaseName, \
							    func_##testCaseName##_of_##testName); \
	void func_##testCaseName##_of_##testName()
#endif // !TEST
