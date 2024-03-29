#include <math.h>

#include "h/calculator_nodes.h"

namespace calculator
{
    Val::Val(float value) : val_(value) {}
    Val::~Val() {}
    float Val::execute()
    {
        return val_;
    }


    Bracket::Bracket(std::unique_ptr<CommandNode> subTree) : subTree_(std::move(subTree)) {}
    Bracket::~Bracket() {}
    float Bracket::execute()
    {
        return subTree_->execute();
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


    Sub::Sub(std::unique_ptr<CommandNode> sub1, std::unique_ptr<CommandNode> sub2) :
        sub1_(std::move(sub1)),
        sub2_(std::move(sub2))
    {}
    Sub::~Sub() {}
    float Sub::execute()
    {
        return sub1_->execute() - sub2_->execute();
    }


    Mul::Mul(std::unique_ptr<CommandNode> mul1, std::unique_ptr<CommandNode> mul2) :
        mul1_(std::move(mul1)),
        mul2_(std::move(mul2))
    {}
    Mul::~Mul() {}
    float Mul::execute()
    {
        return mul1_->execute() * mul2_->execute();
    }


    Div::Div(std::unique_ptr<CommandNode> div1, std::unique_ptr<CommandNode> div2) :
        div1_(std::move(div1)),
        div2_(std::move(div2))
    {}
    Div::~Div() {}
    float Div::execute()
    {
        return div1_->execute() / div2_->execute();
    }


    Mod::Mod(std::unique_ptr<CommandNode> split1, std::unique_ptr<CommandNode> split2) :
        split1_(std::move(split1)),
        split2_(std::move(split2))
    {}
    Mod::~Mod() {}
    float Mod::execute()
    {
        return std::fmod(split1_->execute(), split2_->execute());
    }


    Pow::Pow(std::unique_ptr<CommandNode> split1, std::unique_ptr<CommandNode> split2) :
        split1_(std::move(split1)),
        split2_(std::move(split2))
    {}
    Pow::~Pow() {}
    float Pow::execute()
    {
        return std::powf(split1_->execute(), split2_->execute());
    }
}