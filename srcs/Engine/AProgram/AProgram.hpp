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

    virtual void Init() = 0;
    virtual void Run() = 0;
};