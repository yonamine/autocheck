//===--- Driver.cpp - Main driver for autocheck ---------------------------===//
//
// Copyright (C) 2023 SYRMIA
// Part of the Autocheck Project, under the Apache License v2.0.
// See LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
//
// This file provides the implementation of the main driver.
//
//===----------------------------------------------------------------------===//

#include "AutocheckContext.h"
#include "AutocheckTool.h"
#include "Diagnostics/DiagnosticHelper.h"
#include "Version.h"
#include "clang/Basic/Version.h"
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
    cl::ReallyHidden, cl::cat(AutocheckCategory));

static cl::opt<unsigned> WarningLimit(
    "warning-limit",
    cl::desc("Set the limit of warnings per autosar rule (0 = no limit)"),
    cl::value_desc("value"), cl::init(0), cl::cat(AutocheckCategory));

static cl::opt<bool>
    CheckSystemHeaders("check-system-headers",
                       cl::desc("Check Autosar rules in system headers."),
                       cl::init(false), cl::cat(AutocheckCategory));

static cl::opt<bool>
    DontCheckHeaders("dont-check-headers",
                     cl::desc("Dont check Autosar rules in headers."),
                     cl::init(false), cl::cat(AutocheckCategory));

static cl::list<std::string> CheckBetweenLines(
    "check-between-lines", cl::desc("Run Autocheck only between given lines"),
    cl::value_desc("from, to"), cl::CommaSeparated, cl::cat(AutocheckCategory));

static cl::opt<bool> DontCheckMacroExpansions(
    "dont-check-macro-expansions",
    cl::desc("Dont check Autosar rules in macro expansions."), cl::init(false),
    cl::cat(AutocheckCategory));

static cl::opt<std::string> Output("o", cl::desc("Write output to <file>"),
                                   cl::value_desc("file"), cl::ValueRequired,
                                   cl::cat(AutocheckCategory));

static cl::opt<std::string> OutputType("output-type", cl::desc("Output type"),
                                       cl::value_desc("full|summary"),
                                       cl::ValueRequired,
                                       cl::cat(AutocheckCategory));

static cl::opt<bool>
    ListRules("list-rules",
              cl::desc("Print a list of supported rules and their identifiers"),
              cl::init(false), cl::ValueDisallowed, cl::cat(AutocheckCategory));

static ArgumentsAdjuster getVerifyModeAdjuster(const std::string &Verify) {
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

  cl::SetVersionPrinter([](llvm::raw_ostream &os) {
    os << "Autocheck version " << AUTOCHECK_VERSION << "\n"
       << "Based on Clang " << CLANG_VERSION_STRING << "\n";
  });

  auto ExpectedParser = CommonOptionsParser::create(
      argc, argv, AutocheckCategory, cl::NumOccurrencesFlag::Optional);
  if (!ExpectedParser) {
    llvm::errs() << ExpectedParser.takeError();
    return 1;
  }
  CommonOptionsParser &OptionsParser = ExpectedParser.get();

  if (ListRules) {
    autocheck::printRuleIdentifierTable();
    return 0;
  }

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
  Context.CheckSystemHeaders = CheckSystemHeaders;
  Context.DontCheckHeaders = DontCheckHeaders;
  if (!CheckBetweenLines.empty()) {
    if (CheckBetweenLines.size() == 2)
      Context.CheckBetweenLines = CheckBetweenLines;
    else
      llvm::errs() << "Invalid range for \"-check-between-lines\" flag. "
                      "Ignoring option\n";
  }
  Context.DontCheckMacroExpansions = DontCheckMacroExpansions;
  if (!Output.empty()) {
    // Make sure output path is absolute since the current directory might
    // change when the tool is run.
    if (llvm::sys::path::is_relative(Output)) {
      llvm::SmallString<256> Storage;
      if (std::error_code EC = llvm::sys::fs::current_path(Storage)) {
        // It is possible for llvm::sys::fs::current_path to fail in rare cases.
        // If it does, the output would become relative to the compile commands
        // directory which is not what the user expects. Instead we ignore the
        // option completely.
        llvm::errs()
            << "Warning: Couldn't resolve output path. Ignoring option\n";
        Output.clear();
      } else {
        llvm::sys::path::append(Storage, Output);
        Output.setValue(Storage.c_str());
      }
    }
    Context.OutputPath = Output;
  }
  if (OutputType.getNumOccurrences() > 0) {
    if (Output.empty()) {
      llvm::errs() << "Warning: Output type is specified but no output path is "
                      "given. No output will be emitted\n";
    } else {
      constexpr std::string_view OutputTypeFull = "full";
      constexpr std::string_view OutputTypeSummary = "summary";
      if (OutputType == OutputTypeFull) {
        Context.FullOutput = true;
      } else if (OutputType == OutputTypeSummary) {
        Context.FullOutput = false;
      } else {
        llvm::errs() << "Warning: Unkown output type '" << OutputType
                     << "'. Using 'full' instead\n";
      }
    }
  }

  if (OptionsParser.getSourcePathList().empty()) {
    llvm::errs() << "No source file specified\n";
    return 1;
  }

  AutocheckTool Tool(OptionsParser.getCompilations(),
                     OptionsParser.getSourcePathList()[0]);

  // Set up verify mode.
  if (Verify.getNumOccurrences() > 0) {
    if (Verify.empty())
      Verify = "expected";
    Tool.appendArgumentsAdjuster(getVerifyModeAdjuster(Verify));
  }

  // We ship our own clang headers and specify where to find them. This is done
  // to support systems where those headers might not be available in the
  // package manager.
  Tool.appendArgumentsAdjuster(getResourceDirAdjuster(argv[0]));

  return Tool.run();
}
