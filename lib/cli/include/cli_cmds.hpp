#pragma once
#include "IExecutable.hpp"
#include "Option.hpp"

//----------------Help----------------//
class Help : public IExecutable {
  public:
    Help();
    void Execute(const Command &c) override;
    void SetOption(const Option &o) override;

  private:
    handler<OptSetFunc> Opt0Handler;
    handler<OptSetFunc> Opt1Handler;

    // START usable data field //
    // END usable data field //
};

//----------------LookFile----------------//
class LookFile : public IExecutable {
  public:
    LookFile();
    void Execute(const Command &c) override;
    void SetOption(const Option &o) override;

  private:
    handler<OptSetFunc> Opt0Handler;
    handler<OptSetFunc> Opt1Handler;

    // START usable data field //
    std::string_view FID;
    // END usable data field //
};

//----------------ShareFile----------------//
class ShareFile : public IExecutable {
  public:
    ShareFile();
    void Execute(const Command &c) override;
    void SetOption(const Option &o) override;

  private:
    handler<OptSetFunc> Opt0Handler;
    handler<OptSetFunc> Opt1Handler;

    // START usable data field //
    std::string_view FID;
    // END usable data field //
};