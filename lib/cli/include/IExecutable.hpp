#pragma once
#include "Option.hpp"

#include <functional>
#include <string_view>
#include <unordered_set>

struct IExecutable {
    virtual ~IExecutable(){};

    virtual void Execute(Command c) = 0;
    virtual void SetOpt0(Option o) = 0;
    virtual void SetOpt1(Option o) = 0;
};

using Opt0SetFunc = std::function<void(Option &)>;
using Opt1SetFunc = std::function<void(Option &)>;

class Addfile : public IExecutable {
  public:
    Addfile();
    void Execute(Command c) override;
    void SetOpt0(Option o) override;
    void SetOpt1(Option o) override;

  private:
    std::string_view file_title;
    std::unordered_map<std::string_view, Opt0SetFunc> Opt0Handler;
    std::unordered_map<std::string_view, Opt1SetFunc> Opt1Handler;
};
