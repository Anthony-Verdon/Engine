#pragma once

class AProgramState
{
  public:
    AProgramState() {}
    virtual ~AProgramState() {}
};

class AProgram
{
  public:
    AProgram([[maybe_unused]] AProgramState *state = nullptr) {}
    virtual ~AProgram() {}

    virtual void Run() = 0;
};