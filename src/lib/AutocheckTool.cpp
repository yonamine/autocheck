//===--- AutocheckTool.cpp - Main interface to run autocheck --------------===//
//
// Copyright (C) 2023 SYRMIA
// Part of the Autocheck Project, under the Apache License v2.0.
// See LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
//
// This file implements the AutocheckTool class.
//
//===----------------------------------------------------------------------===//

#include "AutocheckTool.h"
#include "AutocheckAction.h"
#include "AutocheckContext.h"
#include "clang/Basic/Diagnostic.h"

using namespace clang::tooling;
using namespace llvm;

namespace autocheck {

static ArgumentsAdjuster
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
    if (Context.isEnabled(
            autocheck::AutocheckWarnings::unusedFunctionOrMethod) ||
        Context.isEnabled(autocheck::AutocheckWarnings::unusedFunction))
      WarningsToEnable.push_back("-Wunused-function");
    if (Context.isEnabled(autocheck::AutocheckWarnings::unusedTypedef))
      WarningsToEnable.push_back("-Wunused-local-typedef");
    if (Context.isEnabled(
            autocheck::AutocheckWarnings::deprecatedDynamicExceptionSpec))
      WarningsToEnable.push_back("-Wdynamic-exception-spec");
    if (Context.isEnabled(autocheck::AutocheckWarnings::cStyleCastUsed))
      WarningsToEnable.push_back("-Wold-style-cast");
    if (Context.isEnabled(autocheck::AutocheckWarnings::castRemovesQual))
      WarningsToEnable.push_back("-Wcast-qual");
    if (Context.isEnabled(autocheck::AutocheckWarnings::unusedParameter))
      WarningsToEnable.push_back("-Wunused-parameter");
    if (Context.isEnabled(
            autocheck::AutocheckWarnings::exceptionHandlerInversion))
      WarningsToEnable.push_back("-Wexceptions");
    if (Context.isEnabled(autocheck::AutocheckWarnings::returnStackAddress))
      WarningsToEnable.push_back("-Wreturn-stack-address");
    if (Context.isEnabled(autocheck::AutocheckWarnings::returnNonVoidFunction))
      WarningsToEnable.push_back("-Wreturn-type");
    if (Context.isEnabled(autocheck::AutocheckWarnings::breakSwitchCase))
      WarningsToEnable.push_back("-Wimplicit-fallthrough");
    if (Context.isEnabled(autocheck::AutocheckWarnings::uninitializedMemory))
      WarningsToEnable.push_back("-Wuninitialized");
    if (Context.isEnabled(autocheck::AutocheckWarnings::registerKeywordUsed))
      WarningsToEnable.push_back("-Wregister");
    if (Context.isEnabled(autocheck::AutocheckWarnings::invalidNoreturn))
      WarningsToEnable.push_back("-Winvalid-noreturn");
    if (Context.isEnabled(autocheck::AutocheckWarnings::arrayBounds)) {
      WarningsToEnable.push_back("-Warray-bounds");
      WarningsToEnable.push_back("-Warray-bounds-pointer-arithmetic");
    }
    if (Context.isEnabled(autocheck::AutocheckWarnings::deleteIncomplete))
      WarningsToEnable.push_back("-Wdelete-incomplete");
    if (Context.isEnabled(autocheck::AutocheckWarnings::switchBool))
      WarningsToEnable.push_back("-Wswitch-bool");
    if (Context.isEnabled(autocheck::AutocheckWarnings::writableString))
      WarningsToEnable.push_back("-Wwritable-strings");
    if (Context.isEnabled(autocheck::AutocheckWarnings::undefMacroUsed))
      WarningsToEnable.push_back("-Wundef");
    if (Context.isEnabled(autocheck::AutocheckWarnings::unusedVariable))
      WarningsToEnable.push_back("-Wunused-variable");

    if (!WarningsToEnable.empty())
      AdjustedArgs.insert(llvm::find(AdjustedArgs, "--"),
                          WarningsToEnable.cbegin(), WarningsToEnable.cend());
    return AdjustedArgs;
  };
}

ArgumentsAdjuster getResourceDirAdjuster(const char *ExecPath) {
  // Find the absolute path to the directory of the executable.
  static int Symbol;
  std::string ExecutablePath = sys::fs::getMainExecutable(ExecPath, &Symbol);
  StringRef ExecutableDir = sys::path::parent_path(ExecutablePath);

  // Resource dir should be in ../lib/autocheck relative to the executable
  // directory.
  SmallString<128> ResourceDir = sys::path::parent_path(ExecutableDir);
  sys::path::append(ResourceDir, "lib", "autocheck");

  // Create the argument aduster.
  return getInsertArgumentAdjuster(
      {Twine("-resource-dir=").concat(ResourceDir).str()},
      ArgumentInsertPosition::BEGIN);
}

AutocheckTool::AutocheckTool(const AutocheckContext &Context,
                             clang::tooling::CompilationDatabase &CD,
                             const std::string &SourceFile)
    : Context(Context), Tool(CD, {SourceFile}) {
  // Set up built-in warning flags.
  Tool.appendArgumentsAdjuster(getBuiltinWarningAdjuster(Context));
}

void AutocheckTool::appendArgumentsAdjuster(ArgumentsAdjuster Adjuster) {
  Tool.appendArgumentsAdjuster(Adjuster);
}

void AutocheckTool::setDiagnosticConsumer(
    clang::DiagnosticConsumer *DiagConsumer) {
  Tool.setDiagnosticConsumer(DiagConsumer);
}

class AutocheckActionFactory : public FrontendActionFactory {
public:
  AutocheckActionFactory(const AutocheckContext &Context) : Context(Context) {}

  std::unique_ptr<clang::FrontendAction> create() override {
    return std::make_unique<AutocheckAction>(Context);
  }

private:
  const AutocheckContext &Context;
};

int AutocheckTool::run() {
  return Tool.run(std::make_unique<AutocheckActionFactory>(Context).get());
}

} // namespace autocheck
