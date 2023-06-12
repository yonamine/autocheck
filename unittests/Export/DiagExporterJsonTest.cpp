#include "Export/DiagExporterJson.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/SourceManager.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/raw_ostream.h"
#include <system_error>

#include "gtest/gtest.h"

using namespace autocheck;

#define ASSERT_NO_ERROR(x)                                                     \
  if (std::error_code ASSERT_NO_ERROR_ec = x) {                                \
    llvm::SmallString<128> MessageStorage;                                     \
    llvm::raw_svector_ostream Message(MessageStorage);                         \
    Message << #x ": did not return errc::success.\n"                          \
            << "error number: " << ASSERT_NO_ERROR_ec.value() << "\n"          \
            << "error message: " << ASSERT_NO_ERROR_ec.message() << "\n";      \
    GTEST_FATAL_FAILURE_(MessageStorage.c_str());                              \
  } else {                                                                     \
  }

class JsonExportTest : public testing::Test {
protected:
  /// Unique temporary directory in which all created filesystem entities must
  /// be placed. It is removed at the end of each test (must be empty).
  llvm::SmallString<128> TestDirectory;

  clang::DiagnosticsEngine Diags;
  clang::FileSystemOptions Opts;
  clang::FileManager FM;
  clang::SourceManager SM;

  JsonExportTest()
      : Diags(new clang::DiagnosticIDs(), new clang::DiagnosticOptions,
              new clang::IgnoringDiagConsumer()),
        FM(Opts), SM(Diags, FM) {
    const char *Source = "";
    std::unique_ptr<llvm::MemoryBuffer> Buf =
        llvm::MemoryBuffer::getMemBuffer(Source);
    const clang::FileEntry *MainFile =
        FM.getVirtualFile("<built-in>", Buf->getBufferSize(), 0);
    SM.overrideFileContents(MainFile, std::move(Buf));
    clang::FileID MainFileID =
        SM.getOrCreateFileID(MainFile, clang::SrcMgr::C_User);
    SM.setMainFileID(MainFileID);
  }

  void SetUp() override {
    ASSERT_NO_ERROR(llvm::sys::fs::createUniqueDirectory("file-system-test",
                                                         TestDirectory));
    llvm::errs() << "Test Directory: " << TestDirectory << '\n';
    llvm::errs().flush();
  }

  void TearDown() override {
    ASSERT_NO_ERROR(llvm::sys::fs::remove(TestDirectory.str()));
  }
};

TEST_F(JsonExportTest, exportEmpty) {
  // Create output file path.
  llvm::SmallString<128> OutputPath(TestDirectory);
  llvm::sys::path::append(OutputPath, "exportEmpty.json");

  // Run exporter.
  DiagExporterJson JsonExporter(OutputPath, SM);
  JsonExporter.Open();
  JsonExporter.Close();

  // Parse exported json.
  const auto Output = llvm::MemoryBuffer::getFile(OutputPath);
  llvm::Expected<llvm::json::Value> ExpectedParsedOutput =
      llvm::json::parse(Output->get()->getBuffer());
  bool IsValidJson = (bool)ExpectedParsedOutput;
  EXPECT_TRUE(IsValidJson);
  if (!IsValidJson) {
    llvm::handleAllErrors(ExpectedParsedOutput.takeError(),
                          [](const llvm::json::ParseError &M) {});
  } else {
    // Check fields.
    llvm::json::Value &ParsedOutput = ExpectedParsedOutput.get();
    llvm::json::Object *ParsedObject = ParsedOutput.getAsObject();

    EXPECT_TRUE(ParsedObject->getInteger("version").has_value());

    EXPECT_TRUE(ParsedObject->getString("filename").has_value());

    EXPECT_TRUE(ParsedObject->getInteger("generated_on").has_value());

    EXPECT_NE(ParsedObject->getArray("enabled_rules"), nullptr);
    if (const llvm::json::Array *EnabledRules =
            ParsedObject->getArray("enabled_rules")) {
      EXPECT_EQ(EnabledRules->size(), 0);
    }

    const llvm::json::Object *ViolationsPerFile =
        ParsedObject->getObject("violations_per_file");
    EXPECT_NE(ViolationsPerFile, nullptr);
    if (ViolationsPerFile) {
      EXPECT_EQ(ViolationsPerFile->size(), 0);
    }

    const llvm::json::Object *FileSummaries =
        ParsedObject->getObject("file_summaries");
    EXPECT_NE(FileSummaries, nullptr);
    if (FileSummaries) {
      EXPECT_EQ(FileSummaries->size(), 0);
    }

    const auto TotalViolations = ParsedObject->getInteger("total_violations");
    EXPECT_EQ(TotalViolations.value_or(-1), 0);
  }

  ASSERT_NO_ERROR(llvm::sys::fs::remove(llvm::Twine(OutputPath)));
}

TEST_F(JsonExportTest, exportSummary) {
  // Create output file path.
  llvm::SmallString<128> OutputPath(TestDirectory);
  llvm::sys::path::append(OutputPath, "exportSummary.json");

  // Run exporter.
  DiagExporterJson JsonExporter(OutputPath, SM, false);
  JsonExporter.Open();
  JsonExporter.Close();

  // Parse exported json.
  const auto Output = llvm::MemoryBuffer::getFile(OutputPath);
  llvm::Expected<llvm::json::Value> ExpectedParsedOutput =
      llvm::json::parse(Output->get()->getBuffer());
  bool IsValidJson = (bool)ExpectedParsedOutput;
  EXPECT_TRUE(IsValidJson);
  if (!IsValidJson) {
    llvm::handleAllErrors(ExpectedParsedOutput.takeError(),
                          [](const llvm::json::ParseError &M) {});
  } else {
    // Check fields.
    llvm::json::Value &ParsedOutput = ExpectedParsedOutput.get();
    llvm::json::Object *ParsedObject = ParsedOutput.getAsObject();

    const llvm::json::Object *ViolationsPerFile =
        ParsedObject->getObject("violations_per_file");
    EXPECT_EQ(ViolationsPerFile, nullptr);

    const llvm::json::Object *FileSummaries =
        ParsedObject->getObject("file_summaries");
    EXPECT_NE(FileSummaries, nullptr);
    if (FileSummaries) {
      EXPECT_EQ(FileSummaries->size(), 0);
    }
  }

  ASSERT_NO_ERROR(llvm::sys::fs::remove(llvm::Twine(OutputPath)));
}

TEST_F(JsonExportTest, exportDiagnostic) {
  // Create output file path.
  llvm::SmallString<128> OutputPath(TestDirectory);
  llvm::sys::path::append(OutputPath, "exportDiagnostic.json");

  // Run exporter.
  DiagExporterJson JsonExporter(OutputPath, SM);
  JsonExporter.Open();
  AutocheckDiagnostic::reportWarning(
      Diags, SM.getLocForStartOfFile(SM.getMainFileID()),
      AutocheckWarnings::unreachableCode);
  JsonExporter.Close();

  // Parse exported json.
  const auto Output = llvm::MemoryBuffer::getFile(OutputPath);
  llvm::Expected<llvm::json::Value> ExpectedParsedOutput =
      llvm::json::parse(Output->get()->getBuffer());
  bool IsValidJson = (bool)ExpectedParsedOutput;
  EXPECT_TRUE(IsValidJson);
  if (!IsValidJson) {
    llvm::handleAllErrors(ExpectedParsedOutput.takeError(),
                          [](const llvm::json::ParseError &M) {});
  } else {
    // Check fields.
    llvm::json::Value &ParsedOutput = ExpectedParsedOutput.get();
    llvm::json::Object *ParsedObject = ParsedOutput.getAsObject();

    const llvm::json::Object *ViolationsPerFile =
        ParsedObject->getObject("violations_per_file");
    EXPECT_NE(ViolationsPerFile, nullptr);
    if (ViolationsPerFile) {
      EXPECT_EQ(ViolationsPerFile->size(), 1);
      const llvm::json::Array *FileDiagnostics =
          ViolationsPerFile->getArray("<built-in>");
      EXPECT_NE(FileDiagnostics, nullptr);
      if (FileDiagnostics != nullptr) {
        EXPECT_EQ(FileDiagnostics->size(), 1);

        if (FileDiagnostics->size() == 1) {

          const llvm::json::Value &Violation = (*FileDiagnostics)[0];
          EXPECT_NE(Violation.getAsObject(), nullptr);
          if (const llvm::json::Object *FileViolationsObj =
                  Violation.getAsObject()) {
            EXPECT_EQ(FileViolationsObj->getString("id").value_or(""),
                      "M0-1-1");
            EXPECT_EQ(FileViolationsObj->getInteger("line").value_or(-1), 1);
            EXPECT_EQ(FileViolationsObj->getInteger("column").value_or(-1), 1);
          }
        }
      }
    }

    const llvm::json::Object *FileSummaries =
        ParsedObject->getObject("file_summaries");
    EXPECT_NE(FileSummaries, nullptr);
    if (FileSummaries) {
      EXPECT_EQ(FileSummaries->size(), 1);

      const llvm::json::Object *FileSummary =
          FileSummaries->getObject("<built-in>");
      EXPECT_NE(FileSummary, nullptr);

      if (FileSummary != nullptr) {
        EXPECT_EQ(FileSummary->size(), 1);
        if (FileSummary->size() == 1) {
          EXPECT_EQ(FileSummary->getInteger("M0-1-1").value_or(-1), 1);
        }
      }
    }

    const auto TotalViolations = ParsedObject->getInteger("total_violations");
    EXPECT_EQ(TotalViolations.value_or(-1), 1);
  }

  ASSERT_NO_ERROR(llvm::sys::fs::remove(llvm::Twine(OutputPath)));
}
