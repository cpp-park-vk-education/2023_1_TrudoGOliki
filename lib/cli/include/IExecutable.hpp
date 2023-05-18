#pragma once
#include "Option.hpp"

#include <functional>
#include <memory>
#include <string_view>
#include <unordered_set>

template <typename T> using handler = std::unordered_map<std::string_view, T>;
using checker = std::unordered_set<std::string_view>;

struct IExecutable {
    virtual ~IExecutable(){};

    virtual void Execute(const Command &c) = 0;
    virtual void SetOption(const Option &o) = 0;
};

using IExecutablePtr = std::shared_ptr<IExecutable>;

using OptSetFunc = std::function<void(const Option &)>;

class LookFile : public IExecutable {
  public:
    LookFile();
    void Execute(const Command &c) override;
    void SetOption(const Option &o) override;

  private:
    handler<OptSetFunc> Opt0Handler;
    handler<OptSetFunc> Opt1Handler;

    // usable data field
    std::string_view FID;
};
