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
#include "Version.h"
#include "clang/Basic/Version.h"
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

ArgumentsAdjuster getVerifyModeAdjuster(const std::string &Verify) {
  return [Verify](const CommandLineArguments &Args, StringRef /*unused*/) {
    CommandLineArguments AdjustedArgs(Args);
    CommandLineArguments VerifyArgs{"-Xclang", "-verify=" + Verify};
    AdjustedArgs.insert(llvm::find(AdjustedArgs, "--"), VerifyArgs.cbegin(),
                        VerifyArgs.cend());
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

int main(int argc, const char **argv) {
  outs() << "=== Autocheck - Modern and Free Autosar checker\n";

  cl::SetVersionPrinter([](llvm::raw_ostream &os) {
    os << "Autocheck version " << AUTOCHECK_VERSION << "\n"
       << "Based on Clang " << CLANG_VERSION_STRING << "\n";
  });

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

  // Set up built-in warning flags.
  Tool.appendArgumentsAdjuster(getBuiltinWarningAdjuster(Context));

  // Set up verify mode.
  if (Verify.getNumOccurrences() > 0) {
    if (Verify.empty())
      Verify = "expected";
    Tool.appendArgumentsAdjuster(getVerifyModeAdjuster(Verify));
  }

  // TODO: Since no linux distro currently has clang 16, we ship our own clang
  // headers and specify where to find them. We should change this to be a
  // package dependency when clang 16 comes to repositories.
  Tool.appendArgumentsAdjuster(getResourceDirAdjuster(argv[0]));

  // Create and run autocheck checks.
  return Tool.run(newFrontendActionFactory<autocheck::AutocheckAction>().get());
}
