/*
 * $Id$
 * Copyright (c) 2005, IRIT UPS.
 *
 * util/test.cpp -- facilities for performing unit test.
 */

#include <elm/io.h>
#include <elm/util/test.h>

namespace elm {

/**
 * @def CHECK_BEGIN(name)
 * This is the first statement of a unit test case. Usually, a unit test case
 * is structured as below:
 * @code
 * 	CHECK_BEGIN(test case name);
 * 	CHECK(test1);
 * 	CHECK(test2);
 * 	...
 * 	CHECK_END;
 * @endcode
 * This statement display a state header, each CHECK displays a check status line
 * and TEST_END a summary of the full test case. A test case defines also
 * an environment like in C blocks { ... }.
 * @param name	Name of the test case (string).
 * @note	These macros definition may be discarded defining the ELM_NO_SHORTCUT
 * identifier, for avoiding name clashes for example.
 *
 * @ingroup utilities
 */


/**
 * @def CHECK(test)
 * See @ref CHECK_BEGIN.
 * @param test	Test to check.
 * @ingroup utilities
 */


/**
 * @def CHECK_END
 * See @ref CHECK_END.
 * @ingroup utilities
 */


/**
 * @def ELM_CHECK_BEGIN(name)
 * Same as @ref CHECK_BEGIN.
 * @ingroup utilities
 */


/**
 * @def ELM_CHECK(test)
 * Same as @ref CHECK.
 * @ingroup utilities
 */


/**
 * @def ELM_CHECK_END
 * Same as @ref CHECK_END
 * @ingroup utilities
 */

/**
 * @def ELM_CHECK_EQUAL(res, ref)
 * Test if the result of a test is equal to a reference value.
 * @param res	Result of the test.
 * @param ref	Reference value.
 * @ingroup utilities
 */


/**
 * @def CHECK_EQUAL(res, ref)
 * See ELM_CHECK_EQUAL.
 * @ingroup utilities
 */


/**
 * @class TestCase
 * This class is used for implementing macro for unit tests.
 * @ingroup utilities
 */


/**
 * Build a new tes case.
 * @param name	Name of the test case.
 */
TestCase::TestCase(CString name): _name(name), tests(0), errors(0) {
#if defined(__LINUX)
	cout << "\x1b[1;4mTEST CASE: " << name << "\x1b[0m\n";
#elif defined(__WIN32) || defined(__WIN64)
	cout << "TEST CASE: " << name << "\n";
#endif
}


/**
 * Prepare to do a test.
 * @param file	Source file.
 * @param line	Source line.
 * @param text	Code text to test.
 */
void TestCase::test(CString file, int line, CString text) {
	tests++;
	cout << file << ':' << line << ": " << text << '\t';
}


/**
 * Inform that the current test has failed.
 */
void TestCase::failed(void) {
#if defined(__LINUX)
	cout << "\x1b[31m[FAILED]\x1b[0m\n";
#elif defined(__WIN32) || defined(__WIN64)
	cout << "FAILED\n";
#endif
	errors++;
}


/**
 * Inform that the current test has succeeded.
 */
void TestCase::succeeded(void) {
#if defined(__LINUX)
	cout << "\x1b[32m[OK]\x1b[0m\n";
#elif defined(__WIN32) || defined(__WIN64)
	cout << "[OK] \n";
#endif
}


/**
 * Check if a test has returned true.
 * @param file		Source file containing the test.
 * @param line		Line in the source file of the test.
 * @param text		Explanation about the test.
 * @param result	Result of the test.
 */
void TestCase::check(CString file, int line, CString text, bool result) {
	test(file, line, text);
	if(result)
		succeeded();
	else
		failed();
}


/**
 * Final display of the test case.
 */
void TestCase::complete(void) {
	if(errors)
#if defined(__LINUX)
		cout << "\x1b[1;31mFAILURE: \x1b[0m";
#elif defined(__WIN32)
		cout << "FAILURE :";
#endif
	else
#if defined(__LINUX)
		cout << "\x1b[1;32mSUCCESS: \x1b[0m";
#elif defined(__WIN32) || defined(__WIN64)
		cout << "SUCCESS: ";
#endif
	cout << tests << " tests, " << errors  << " errors\n";
}


/**
 * Same as check but also returns value and specific message for ending the
 * test due to main failure.
 * @param file		File containing the test source.
 * @param line		Line where the test is implemented.
 * @param text		Text describing the test.
 * @param result	Result of the test.
 * @return			Result of the test.
 */
bool TestCase::require(CString file, int line, CString text, bool result) {
	check(file, line, text, result);
	if(!result)
#if defined(__LINUX)
		cout << "\x1b[1;31mMain failure: test must be interrupted.\x1b[0m\n";
#elif defined(__WIN32) || defined(__WIN64)
		cout << "Main failure: test must be interrupted\n";
#endif
	return result;
}

} //elm
