// ConsoleAppTest.cpp : Defines the entry point for the console application.
//

#include "h/stdafx.h"

#include <iostream>
#include <string>
#include <vector>

#include "h/parse.h"
#include "h/calculator_assigners.h"

// TODO
// Add more commands (-, *, /, %)
// Add more complex commands((), help, scroll, empty Val handling)

// All the opperator assigners function pretty similarly, check " op " then assign the op
// Maybe something like a generic OpAssigner that get's passed an InputCommand and a check string would work better




int main()
{
    // build up vector of NodeAssigners to pass to Parser
    std::vector<std::unique_ptr<parse::NodeAssigner>> assigners;

    // order of operations is the order of NodeAssigners pushed back into the vector
    assigners.push_back(std::unique_ptr<parse::NodeAssigner>(new calculator::AddAssigner));
    assigners.push_back(std::unique_ptr<parse::NodeAssigner>(new calculator::SubAssigner));

    assigners.push_back(std::unique_ptr<parse::NodeAssigner>(new calculator::MulAssigner));
    assigners.push_back(std::unique_ptr<parse::NodeAssigner>(new calculator::DivAssigner));

    assigners.push_back(std::unique_ptr<parse::NodeAssigner>(new calculator::ValAssigner));

    parse::Parser parser(std::move(assigners));


    std::cout << "Hello world!! Type a calculation (plusses only)" << std::endl;

    // currently it will exit if there is a parse error
    while (parser.state_ == parse::run)
    {
        std::cout << "\n--------------------------------------------------------\n" << std::endl;

        // read input calculation string
        std::string inputString;
        std::getline(std::cin, inputString);

        // parse string
        std::unique_ptr<parse::CommandNode> calculator = parser.parse(inputString);
        
        // execute parse tree and return answer!
        float answer = calculator->execute();
        std::cout << "\n= " << answer << std::endl;
    }

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
