/*
 * $Id$
 * Copyright (c) 2006, IRIT-UPS.
 *
 * test/test_option.cpp -- option module test.
 */

#include <elm/util/test.h>
#include <elm/options.h>
#include <elm/option/BoolOption.h>

using namespace elm;
using namespace elm::option;

// EnumOption test
typedef enum val_t {
	one,
	two,
	three
} val_t;
EnumOption<int>::value_t vals[] = {
	{ "value", one },
	{ "one", one },
	{ "two", two },
	{ "three", three },
	{ "", one }
};


// manager initialization
static const cstring program = "my-command",
					 copyright = "copyright (c) 2010",
					description = "a test application",
					author = "H. Cassé <casse@irit.fr",
					arg_free = "free argument";
static const Version version(1, 2, 3);

class MyCommand: public Manager {
public:

	MyCommand(void)
	: Manager(
		PROGRAM, &::program,
		VERSION, new Version(::version),
		COPYRIGHT, &::copyright,
		DESCRIPTION, &::description,
		AUTHOR, &::author,
		ARG_FREE, &::arg_free,
		END
	) {
	}
};


// options
MyCommand man;
BoolOption b(man, 'b', "boolean", "boolean test", false);
EnumOption<int> enum_opt(man, 'e', "enum", "", vals);
StringOption s(man, 's', "string", "string test", "", "");
SwitchOption c(man, CMD, "command", END);
SwitchOption sw(man, 'S', "switch option");


// test_option()
void test_option(void) {
	CHECK_BEGIN("Option");

	try {

		// manager test
		{
			CHECK_EQUAL(man.getProgram(), program);
			CHECK_EQUAL(man.getAuthor(), author);
			CHECK_EQUAL(man.getDescription(), description);
			CHECK_EQUAL(man.getCopyright(), copyright);
			CHECK_EQUAL(man.getVersion(), version);
			CHECK_EQUAL(man.getFreeArgumentDescription(), arg_free);
		}

		// boolean option
		{
			const char *argv[] = { "command", "-b", 0 };
			FAIL_ON_EXCEPTION(OptionException, man.parse(2, argv));
			CHECK_EQUAL(*b, true);
			b = false;
		}
		{
			const char *argv[] = { "command", 0 };
			FAIL_ON_EXCEPTION(OptionException, man.parse(1, argv));
			CHECK_EQUAL(*b, false);
			b = false;
		}
		{
			const char *argv[] = { "command", "--boolean", 0 };
			FAIL_ON_EXCEPTION(OptionException, man.parse(2, argv));
			CHECK_EQUAL(*b, true);
			b = false;
		}

		// string option
		{
			cstring value = "hello, world !";
			const char *argv[] = { "command", "--string", &value, 0 };
			FAIL_ON_EXCEPTION(OptionException, man.parse(3, argv));
			CHECK_EQUAL(*s, string(value));
			s.set("");
		}
		{
			cstring value = "hello, world !";
			const char *argv[] = { "command", "--string=hello, world !", 0 };
			FAIL_ON_EXCEPTION(OptionException, man.parse(2, argv));
			CHECK_EQUAL(*s, string(value));
			s.set("");
		}
		{
			cstring value = "hello, world !";
			const char *argv[] = { "command", "-s", &value, 0 };
			FAIL_ON_EXCEPTION(OptionException, man.parse(3, argv));
			CHECK_EQUAL(*s, string(value));
			s.set("");
		}
		{
			cstring value = "hello, world !";
			const char *argv[] = { "command", "-s=hello, world !", 0 };
			FAIL_ON_EXCEPTION(OptionException, man.parse(2, argv));
			CHECK_EQUAL(*s, string(value));
			s.set("");
		}

		// SwicthOption test
		{
			const char *argv[] = { "command", "-S", 0 };
			FAIL_ON_EXCEPTION(OptionException, man.parse(2, argv));
			CHECK_EQUAL(*sw, true);
		}

		// free command
		{
			const char *argv[] = { "command", "command", 0 };
			FAIL_ON_EXCEPTION(OptionException, man.parse(2, argv));
			CHECK_EQUAL(*c, true);
		}

		// Unknown option
		{
			const char *argv[] = { "command", "--bad", 0 };
			CHECK_EXCEPTION(OptionException, man.parse(2, argv));
		};

		// EnumOption test
		{
			CHECK(enum_opt == one);
			enum_opt = two;
			CHECK(enum_opt == two);
			{
				const char *argv[] = { "command", "-e", "one", 0 };
				man.parse(3, argv);
				CHECK(enum_opt == one);
			}
			{
				const char *argv[] = { "command", "-e", "two", 0 };
				man.parse(3, argv);
				CHECK(enum_opt == two);
			}
			{
				const char *argv[] = { "command", "-e", "four", 0 };
				CHECK_EXCEPTION(OptionException, man.parse(3, argv));
			}
		}
	}
	catch(OptionException& e) {
		__case.failed();
		cerr << "FAILURE: " << e.message() << io::endl;
	}

	CHECK_END
}
