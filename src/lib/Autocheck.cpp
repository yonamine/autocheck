//===-------- autocheck.cpp - Main driver for the tool --------------------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides the implementation of the main driver.
//
//===----------------------------------------------------------------------===//

#include "AutocheckAction.h"
#include "AutocheckContext.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/StaticAnalyzer/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang::tooling;
using namespace llvm;

// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static cl::OptionCategory AutocheckCategory("Autocheck options");

static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// TODO Write help text
static cl::extrahelp MoreHelp("\nMore help text...\n");

static cl::list<std::string> Warnings("W",
                                      cl::desc("Enable the specified warning"),
                                      cl::value_desc("warning"), cl::ZeroOrMore,
                                      cl::AlwaysPrefix, cl::ValueRequired,
                                      cl::cat(AutocheckCategory));

static cl::opt<std::string> Verify(
    "verify",
    cl::desc("Verify diagnostic output using comment directives that start "
             "with prefixes in the comma-separated sequence <prefixes>"),
    cl::value_desc("prefixes"), cl::ValueOptional, cl::init("expected"),
    cl::cat(AutocheckCategory));

static cl::opt<unsigned> WarningLimit(
    "warning-limit",
    cl::desc("Set the limit of warnings per autosar rule (0 = no limit)"),
    cl::value_desc("value"), cl::init(0), cl::cat(AutocheckCategory));

ArgumentsAdjuster
getBuiltinWarningAdjuster(const autocheck::AutocheckContext &Context) {
  return [&Context](const CommandLineArguments &Args, StringRef /*unused*/) {
    CommandLineArguments AdjustedArgs(Args);

    // Disable all clang warnings and reenable only those that autocheck relies
    // on.
    CommandLineArguments WarningsToEnable{"-Wno-everything"};
    if (Context.isEnabled(autocheck::AutocheckWarnings::embeddedDirective))
      WarningsToEnable.push_back("-Wembedded-directive");
    if (Context.isEnabled(autocheck::AutocheckWarnings::trigraphsUsed))
      WarningsToEnable.push_back("-Wtrigraphs");
    if (Context.isEnabled(autocheck::AutocheckWarnings::reservedIdentifiers)) {
      WarningsToEnable.push_back("-Wbuiltin-macro-redifined");
      WarningsToEnable.push_back("-Wreserved-macro-identifier");
    }

    if (!WarningsToEnable.empty())
      AdjustedArgs.insert(llvm::find(AdjustedArgs, "--"),
                          WarningsToEnable.cbegin(), WarningsToEnable.cend());
    return AdjustedArgs;
  };
}

ArgumentsAdjuster getVerifyModeAdjuster(const std::string &Verify) {
  return [Verify](const CommandLineArguments &Args, StringRef /*unused*/) {
    CommandLineArguments AdjustedArgs(Args);
    CommandLineArguments VerifyArgs{"-Xclang", "-verify=" + Verify};
    AdjustedArgs.insert(llvm::find(AdjustedArgs, "--"), VerifyArgs.cbegin(),
                        VerifyArgs.cend());
    return AdjustedArgs;
  };
}

int main(int argc, const char **argv) {
  outs() << "=== Autocheck - Modern and Free Autosar checker\n";

  auto ExpectedParser =
      CommonOptionsParser::create(argc, argv, AutocheckCategory);
  if (!ExpectedParser) {
    llvm::errs() << ExpectedParser.takeError();
    return 1;
  }
  CommonOptionsParser &OptionsParser = ExpectedParser.get();

  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());

  // Initialize context.
  autocheck::AutocheckContext &Context = autocheck::AutocheckContext::Get();
  if (Warnings.getNumOccurrences() > 0) {
    for (const std::string &Warning : Warnings) {
      if (!Context.enableWarning(Warning)) {
        // TODO: Use DiagnosticsEngine to emit this warning.
        llvm::errs() << "unknown warning '" << Warning << "'\n";
      }
    }
  } else {
    Context.enableWarning("all");
  }
  Context.WarningLimit = WarningLimit;

  // Set up built-in warning flags.
  Tool.appendArgumentsAdjuster(getBuiltinWarningAdjuster(Context));

  // Set up verify mode.
  if (Verify.getNumOccurrences() > 0) {
    if (Verify.empty())
      Verify = "expected";
    Tool.appendArgumentsAdjuster(getVerifyModeAdjuster(Verify));
  }

  // Create and run autocheck checks.
  return Tool.run(newFrontendActionFactory<autocheck::AutocheckAction>().get());
}
