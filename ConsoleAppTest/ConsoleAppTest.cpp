// ConsoleAppTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <string>
#include <sstream>
#include <regex>

// TODO
// Decouple Parser from CalculatorCommands
// Shove out calculator and input namespaces to .h files
// Add a CommandAssigner class
// Add more commands (-, *, /, %)
// Add more complex commands((), help, scroll, empty Val handling)



// think abt current coupled dependencies as thought experiment
// CalculatorCommand: none
// Parser: CalculatorCommand
// Add: Parser

// calculator.h issue with Add dependant on Parser defined with parse()
// because it has to be defined with functions the only real option is:
// #include "input.h"
// This is what I plan to decouple though so this issue should be resolved soon

// input.h issue with Parser dependant on CalculatorCommands to return
// could start with a declaration w/o definition of CalculatorCommand class, maybe rename to CommandToken or summat
// bigger issue is with when parse() creates and returns a specific CommandToken
// parse() here needs definitions of specific class constructors which is awkward
// again could use an include but that's just awful and would use more .h files than really needed
// what I really want is for parser to not care about how to assign CommandTokens because they should be generic
// hence the TODO: Add a CommandAssigner interface and pass specific classes in from Parser constructor

// lol, it's like C++ shoves in your face all the coupling and pushes you to structure code better


namespace calculator
{
	class CalculatorCommand
	{
	public:
		CalculatorCommand() {}
		~CalculatorCommand() {}
		virtual float execute() = 0;
	};
}

namespace input
{
	class Parser
	{
	public:
		Parser();
		~Parser();
		std::unique_ptr<calculator::CalculatorCommand> parse(std::string inputString);
	};

}

namespace calculator
{
	class Val : public CalculatorCommand
	{
	public:
		Val(std::string valueString);
		~Val();
		float execute();
	private:
		float val_;
	};


	class Add : public CalculatorCommand
	{
	public:
		Add(std::string split1, std::string split2, input::Parser* parser);
		~Add();
		float execute();
	private:
		std::unique_ptr<CalculatorCommand> add1_;
		std::unique_ptr<CalculatorCommand> add2_;
	};
}


namespace input
{
	Parser::Parser() {}
	Parser::~Parser() {}
	std::unique_ptr<calculator::CalculatorCommand> Parser::parse(std::string inputString)
	{
		std::unique_ptr<calculator::CalculatorCommand> command;

		std::string checker = " + ";
		std::size_t found = inputString.find(checker);
		if (found != std::string::npos)
		{
			std::string split1 = inputString.substr(0, found);
			std::string split2 = inputString.substr(found + checker.length());

			command = std::unique_ptr<calculator::CalculatorCommand>(
				new calculator::Add(split1, split2, this));

			return command;
		}


		// after all operations are processed, whitespace needs to be removed to get individual values
		inputString.erase(std::remove(inputString.begin(), inputString.end(), ' '), inputString.end());

		if (std::regex_match(inputString, std::regex("[-|+]?[0-9]+(.[0-9])?[0-9]*")))
		{
			command = std::unique_ptr<calculator::CalculatorCommand>(
				new calculator::Val(inputString));

			return command;
		}


		std::cout << "ERROR!!" << std::endl;
		command = std::unique_ptr<calculator::CalculatorCommand>(
			new calculator::Val("0"));
		return command;
	}
}

namespace calculator
{
	Val::Val(std::string valueString) : val_(std::stof(valueString)) {}
	Val::~Val() {}
	float Val::execute()
	{
		return val_;
	}


	Add::Add(std::string split1, std::string split2, input::Parser* parser) :
		add1_(parser->parse(split1)),
		add2_(parser->parse(split2))
	{}
	Add::~Add() {}
	float Add::execute()
	{
		return add1_->execute() + add2_->execute();
	}
}


int main()
{
	std::cout << "Hello world!! Type a calculation (plusses only)" << std::endl;
	std::string inputString;
	std::getline(std::cin, inputString);

	input::Parser parser;
	std::unique_ptr<calculator::CalculatorCommand> calculator = parser.parse(inputString);
	std::cout << "Got it! The answer is..." << std::endl;

	float answer = calculator->execute();
	std::cout << answer << std::endl;

    return 0;
}

// if it is reasonably generic, don't put it in an object?
// if it's interchangable/swappable, use objects
// The issue is that reasonably generic functions may need to be swapped later because of changes
// Maybe a more efficiant approach is needed? Maybe the current function doesn't allow for a certain kind of useful extendability
// Maybe an extern function() would be used instead of a whole class definition though

// A class/object makes more sense where we need to assign a function or we need to alter some kind of state
// keep the assign a function thought in mind though because some languages can have functions as variables and even functions with state as variables

// In which case when do we even use classes? Well, classes are a good and easy way of interfacing. I don't think functions can inherit from an interface
// The exception being .h files where you can change the .cpp file defining the function

// I guess try to match the way the code currently is, if everything is an object keep that style
// For personal code I don't think I'll use objects as a way to structure code
// I'll use them when being able to assign an object provides a nicely structured solution though
// ie linked lists, commands, etc.
