#pragma once

struct IExecutable {
    virtual ~IExecutable(){};

    virtual void Execute() = 0;
};
