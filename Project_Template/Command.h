#pragma once

class Unit;
class Command
{
public:

    virtual void Execute(Unit* unit,float elapsedtime) = 0;
    virtual bool IsFinished() const = 0;

private:

};
