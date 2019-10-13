// ConsoleAppTest.cpp : Defines the entry point for the console application.
//

#include "h/stdafx.h"

#include <iostream>
#include <string>
#include <sstream>
#include <regex>

// TODO
// Shove out calculator and input namespaces to .h files
// Add more commands (-, *, /, %)
// Add more complex commands((), help, scroll, empty Val handling)




namespace calculator
{
    // a parse tree is made up of nodes
    // CommandNodes specifically have a built in execution function that can be called through the tree once parsed
    class CommandNode
    {
    public:
        CommandNode() {}
        ~CommandNode() {}
        virtual float execute() = 0;
    };




    class Val : public CommandNode
    {
    public:
        Val(float value);
        ~Val();
        float execute();
    private:
        float val_;
    };


    class Add : public CommandNode
    {
    public:
        Add(std::unique_ptr<CommandNode> add1, std::unique_ptr<CommandNode> add2);
        ~Add();
        float execute();
    private:
        std::unique_ptr<CommandNode> add1_;
        std::unique_ptr<CommandNode> add2_;
    };




    Val::Val(float value) : val_(value) {}
    Val::~Val() {}
    float Val::execute()
    {
        return val_;
    }


    Add::Add(std::unique_ptr<CommandNode> add1, std::unique_ptr<CommandNode> add2) :
        add1_(std::move(add1)),
        add2_(std::move(add2))
    {}
    Add::~Add() {}
    float Add::execute()
    {
        return add1_->execute() + add2_->execute();
    }


    
}

namespace input
{
    class Parser
    {
    public:
        Parser();
        ~Parser();
        std::unique_ptr<calculator::CommandNode> parse(std::string inputString);
    };
}


namespace calculator
{
    class NodeAssigner
    {
    public:
        NodeAssigner() {}
        ~NodeAssigner() {}
        virtual bool check(std::string inputString) = 0;
        virtual void assign(
            input::Parser* parser,
            std::string inputString,
            std::unique_ptr<calculator::CommandNode>& commandNode) = 0;
    };




    class ValAssigner : public NodeAssigner
    {
    public:
        ValAssigner();
        ~ValAssigner();
        bool check(std::string inputString);
        void assign(
            input::Parser* parser,
            std::string inputString,
            std::unique_ptr<calculator::CommandNode>& commandNode);
    private:
        std::regex check_;
    };


    class AddAssigner : public NodeAssigner
    {
    public:
        AddAssigner();
        ~AddAssigner();
        bool check(std::string inputString);
        void assign(
            input::Parser* parser,
            std::string inputString,
            std::unique_ptr<calculator::CommandNode>& commandNode);
    private:
        std::string check_;
        size_t result_;
    };




    ValAssigner::ValAssigner() : check_("[-|+]?[0-9]+(.[0-9])?[0-9]*") {}
    ValAssigner::~ValAssigner() {}
    bool ValAssigner::check(std::string inputString)
    {
        // remove whitespace
        inputString.erase(std::remove(inputString.begin(), inputString.end(), ' '), inputString.end());
        return std::regex_match(inputString, check_);
    }
    void ValAssigner::assign(
        input::Parser* parser,
        std::string inputString,
        std::unique_ptr<calculator::CommandNode>& commandNode)
    {
        commandNode = std::unique_ptr<calculator::CommandNode>(
            new calculator::Val(std::stof(inputString)));
    }


    AddAssigner::AddAssigner() : check_(" + ") {}
    AddAssigner::~AddAssigner() {}
    bool AddAssigner::check(std::string inputString)
    {
        result_ = inputString.find(check_);
        return result_ != std::string::npos;
    }
    void AddAssigner::assign(
        input::Parser* parser,
        std::string inputString,
        std::unique_ptr<calculator::CommandNode>& commandNode)
    {
        std::string split1 = inputString.substr(0, result_);
        std::string split2 = inputString.substr(result_ + check_.length());

        commandNode = std::unique_ptr<calculator::CommandNode>(
            new calculator::Add(parser->parse(split1), parser->parse(split2)));
    }
}


namespace input
{
    Parser::Parser() {}
    Parser::~Parser() {}
    std::unique_ptr<calculator::CommandNode> Parser::parse(std::string inputString)
    {
        std::unique_ptr<calculator::CommandNode> commandNode;

        std::unique_ptr<calculator::NodeAssigner> addCheck( new calculator::AddAssigner );
        std::unique_ptr<calculator::NodeAssigner> valCheck(new calculator::ValAssigner);

        if (addCheck->check(inputString))
        {
            addCheck->assign(this, inputString, commandNode);
            return commandNode;
        }

        if (valCheck->check(inputString))
        {
            valCheck->assign(this, inputString, commandNode);
            return commandNode;
        }


        std::cout << "ERROR!!" << std::endl;
        commandNode = std::unique_ptr<calculator::CommandNode>(
            new calculator::Val(0));
        return commandNode;
    }
}




int main()
{
    std::cout << "Hello world!! Type a calculation (plusses only)" << std::endl;
    std::string inputString;
    std::getline(std::cin, inputString);

    input::Parser parser;
    std::unique_ptr<calculator::CommandNode> calculator = parser.parse(inputString);
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
