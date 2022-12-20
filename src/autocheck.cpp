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

static cl::extrahelp MoreHelp("\nMore help text...\n");

class ExampleAction : public clang::ASTFrontendAction {
  std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, StringRef InFile) override {
    return nullptr;
  }

  bool BeginSourceFileAction(clang::CompilerInstance &CI) override {
    outs() << "Processing " << getCurrentFile() << "\n";
    return false;
  }
};

int main(int argc, const char **argv) {
  outs() << "=== Autocheck - Modern and Free Autosar checker\n";

  auto ExpectedParser = CommonOptionsParser::create(argc, argv, AutocheckCategory);
  if (!ExpectedParser) {
    llvm::errs() << ExpectedParser.takeError();
    return 1;
  }
  CommonOptionsParser &OptionsParser = ExpectedParser.get();
  
  ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());

  return Tool.run(newFrontendActionFactory<ExampleAction>().get());
}
