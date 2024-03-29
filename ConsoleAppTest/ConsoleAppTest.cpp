// ConsoleAppTest.cpp : Defines the entry point for the console application.
//

#include "h/stdafx.h"

#include <iostream>
#include <string>
#include <vector>

#include "h/parse.h"
#include "h/calculator_assigners.h"

// TODO
// Add more complex commands(help, exit, empty Val handling)
// move BracketChecker subTree_


int main()
{
    // brackets require a dependency to work due to the way they change the order of operations
    std::unique_ptr<calculator::BracketAssigner> bracketAssigner(new calculator::BracketAssigner);

    // build up vector of NodeAssigners to pass to Parser
    std::vector<std::unique_ptr<parse::NodeAssigner>> assigners;

    // order of operations is the order of NodeAssigners pushed back into the vector
    assigners.push_back(std::unique_ptr<parse::NodeAssigner>(new calculator::BracketChecker(*bracketAssigner)));

    assigners.push_back(std::unique_ptr<parse::NodeAssigner>(new calculator::OpAssigner<calculator::Add>(" + ")));
    assigners.push_back(std::unique_ptr<parse::NodeAssigner>(new calculator::OpAssigner<calculator::Sub>(" - ")));

    assigners.push_back(std::unique_ptr<parse::NodeAssigner>(new calculator::OpAssigner<calculator::Mul>(" * ")));
    assigners.push_back(std::unique_ptr<parse::NodeAssigner>(new calculator::OpAssigner<calculator::Div>(" / ")));
    assigners.push_back(std::unique_ptr<parse::NodeAssigner>(new calculator::OpAssigner<calculator::Mod>(" % ")));
    assigners.push_back(std::unique_ptr<parse::NodeAssigner>(new calculator::OpAssigner<calculator::Pow>(" ^ ")));

    assigners.push_back(std::unique_ptr<parse::NodeAssigner>(new calculator::ValAssigner));
    assigners.push_back(std::move(bracketAssigner));

    parse::Parser parser(std::move(assigners));


    std::cout << "Hello world!! Type a calculation" << std::endl;

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