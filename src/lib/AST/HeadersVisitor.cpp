//===--- HeadersVisitor.cpp - Visitor for header related checks -----------===//
//
// Part of the Autocheck Project, under the Apache License v2.0 with Syrmia
// Exceptions. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the HeadersVisitor class and all classes derived from
// HeadersVisitorInterface.
//
//===----------------------------------------------------------------------===//

#include "AST/HeadersVisitor.h"

#include "Diagnostics/AutocheckDiagnostic.h"

namespace autocheck {

HeadersVisitorInterface::~HeadersVisitorInterface() {}

using HVI = HeadersVisitorInterface;
void HVI::PostWork() {}
bool HVI::PreTraverseDecl(clang::Decl *) { return true; }
bool HVI::PostTraverseDecl(clang::Decl *) { return true; }
bool HVI::VisitCallExpr(const clang::CallExpr *) { return true; }
bool HVI::VisitCXXConstructExpr(const clang::CXXConstructExpr *) {
  return true;
}
bool HVI::VisitCXXRecordDecl(const clang::CXXRecordDecl *) { return true; }
bool HVI::VisitDeclRefExpr(const clang::DeclRefExpr *) { return true; }
bool HVI::VisitEnumDecl(const clang::EnumDecl *) { return true; }
bool HVI::VisitGNUNullExpr(const clang::GNUNullExpr *) { return true; }
bool HVI::VisitTranslationUnitDecl(const clang::TranslationUnitDecl *) {
  return true;
}
bool HVI::VisitTypedefDecl(const clang::TypedefDecl *) { return true; }
bool HVI::VisitTypeLoc(const clang::TypeLoc &) { return true; }
bool HVI::VisitUsingDirectiveDecl(const clang::UsingDirectiveDecl *) {
  return true;
}
bool HVI::VisitVarDecl(const clang::VarDecl *) { return true; }

/* Implementation of HeadersUnusedVisitor */

HeadersUnusedVisitor::HeadersUnusedVisitor(clang::DiagnosticsEngine &DE,
                                           const AutocheckContext &Context,
                                           IncludeInfo &IncludeData)
    : DE(DE), IncludeData(IncludeData),
      SystemSmart(
          Context.isEnabled(AutocheckWarnings::headersUnusedSystemSmart)),
      SystemOff(Context.isEnabled(AutocheckWarnings::headersUnusedSystemOff)) {
  // Clean list of includes from the ones that are used by macros.
  for (const auto &It : IncludeData.IncludesUsedByMacros)
    removeFromIncludes(It);
}

void HeadersUnusedVisitor::PostWork() {
  // Any file not removed by HeadersUnusedVisitor is unused one.
  for (const auto &It : IncludeData.Includes) {

    if (SystemSmart) {
      // Check if this headers is already included through some other top level
      // include directive.
      const auto SubInclude =
          IncludeData.SubSystemHeaderIncludes.find(It.first);
      if (SubInclude != IncludeData.SubSystemHeaderIncludes.end()) {
        bool stopVisitor =
            AutocheckDiagnostic::reportWarning(
                DE, It.second, AutocheckWarnings::headersUnused, It.first.str())
                .limitReached();
        AutocheckDiagnostic::reportWarning(
            DE, SubInclude->second.second,
            AutocheckWarnings::noteHeadersUnusedIncluded,
            SubInclude->second.first.str());
        if (stopVisitor)
          return;
        continue;
      }
    }

    // Basic version.
    if (!SystemSmart && !SystemOff) {
      // Check if this header is used by one or more of it's own #include
      // directives.
      if (SubincludeUsed.count(It.first) != 0) {
        bool stopVisitor =
            AutocheckDiagnostic::reportWarning(
                DE, It.second, AutocheckWarnings::headersUnused, It.first.str())
                .limitReached();
        AutocheckDiagnostic::reportWarning(
            DE, It.second, AutocheckWarnings::noteHeadersUnusedUsed);
        if (stopVisitor)
          return;
        continue;
      }
    }

    // Report a regular warning in all other cases.
    if (AutocheckDiagnostic::reportWarning(
            DE, It.second, AutocheckWarnings::headersUnused, It.first.str())
            .limitReached())
      return;
  }
}

bool HeadersUnusedVisitor::isFlagPresent(const AutocheckContext &Context) {
  return Context.isEnabled(AutocheckWarnings::headersUnused) ||
         Context.isEnabled(AutocheckWarnings::headersUnusedSystemOff) ||
         Context.isEnabled(AutocheckWarnings::headersUnusedSystemSmart);
}

bool HeadersUnusedVisitor::VisitTranslationUnitDecl(
    const clang::TranslationUnitDecl *TUD) {
  // Constructor can leave include list empty. In that case there is no work
  // that need to be done and visitor can be stopped immediately.
  return !IncludeData.Includes.empty();
}

bool HeadersUnusedVisitor::VisitDeclRefExpr(const clang::DeclRefExpr *DRE) {
  if (DE.getSourceManager().isInMainFile(DRE->getLocation())) {
    const clang::NamedDecl *ND = DRE->getFoundDecl();
    return removeFromIncludes(ND->getBeginLoc());
  }
  return true;
}

bool HeadersUnusedVisitor::VisitCXXConstructExpr(
    const clang::CXXConstructExpr *CCE) {
  if (DE.getSourceManager().isInMainFile(CCE->getLocation())) {
    const clang::CXXConstructorDecl *CD = CCE->getConstructor();
    return removeFromIncludes(CD->getLocation());
  }
  return true;
}

bool HeadersUnusedVisitor::VisitCallExpr(const clang::CallExpr *CE) {
  if (DE.getSourceManager().isInMainFile(CE->getBeginLoc())) {
    if (const clang::FunctionDecl *FD = CE->getDirectCallee()) {
      const clang::SourceLocation SLDecl = FD->getNameInfo().getLoc();
      return removeFromIncludes(SLDecl);
    }
  }
  return true;
}

bool HeadersUnusedVisitor::VisitCXXRecordDecl(const clang::CXXRecordDecl *CRD) {
  const clang::SourceLocation SL = CRD->getBeginLoc();
  if (!DE.getSourceManager().isInMainFile(SL))
    CXXRecords.insert(std::make_pair(CRD->getIdentifier(), SL));
  else {
    const IdentifierMap::iterator It = CXXRecords.find(CRD->getIdentifier());
    if (It != CXXRecords.end())
      return removeFromIncludes(It->second);
  }
  return true;
}

bool HeadersUnusedVisitor::VisitTypedefDecl(const clang::TypedefDecl *TD) {
  const clang::SourceLocation SL = TD->getBeginLoc();
  if (!DE.getSourceManager().isInMainFile(SL))
    Typedefs.insert(std::make_pair(TD->getQualifiedNameAsString(), SL));
  return true;
}

bool HeadersUnusedVisitor::VisitVarDecl(const clang::VarDecl *VD) {
  if (DE.getSourceManager().isInMainFile(VD->getLocation())) {
    const clang::IdentifierInfo *II = VD->getType().getBaseTypeIdentifier();
    const IdentifierMap::iterator MapIt = CXXRecords.find(II);
    if (MapIt != CXXRecords.end())
      return removeFromIncludes(MapIt->second);

    std::string TypeName = VD->getType().getAsString();
    removeTypeModifiers(TypeName);
    removeKeywords(TypeName);
    const TrackingSet::iterator SetIt = Typedefs.find(TypeName);
    if (SetIt != Typedefs.end())
      return removeFromIncludes(SetIt->second);
  }
  return true;
}

bool HeadersUnusedVisitor::VisitUsingDirectiveDecl(
    const clang::UsingDirectiveDecl *UDD) {
  if (DE.getSourceManager().isInMainFile(UDD->getUsingLoc()))
    if (const clang::NamespaceDecl *ND = UDD->getNominatedNamespace())
      return removeFromIncludes(ND->getLocation());
  return true;
}

bool HeadersUnusedVisitor::VisitGNUNullExpr(const clang::GNUNullExpr *GNE) {
  const clang::SourceManager &SM = DE.getSourceManager();
  if (SM.isInMainFile(GNE->getBeginLoc()))
    return removeFromIncludes(SM.getSpellingLoc(GNE->getTokenLocation()));
  return true;
}

bool HeadersUnusedVisitor::VisitEnumDecl(const clang::EnumDecl *ED) {
  if (DE.getSourceManager().isInMainFile(ED->getBeginLoc())) {
    const clang::QualType PT = ED->getPromotionType();
    if (const clang::TypedefType *TDT =
            PT.getTypePtr()->getAs<clang::TypedefType>())
      if (const clang::TypedefNameDecl *TDND = TDT->getDecl())
        return removeFromIncludes(TDND->getLocation());
  }
  return true;
}

bool HeadersUnusedVisitor::VisitTypeLoc(const clang::TypeLoc &TL) {
  if (DE.getSourceManager().isInMainFile(TL.getBeginLoc())) {
    const clang::Type *T = TL.getTypePtr();
    if (const clang::TypedefType *TDT = T->getAs<clang::TypedefType>()) {
      if (const clang::TypedefNameDecl *TDND = TDT->getDecl())
        return removeFromIncludes(TDND->getLocation());
    } else if (const clang::RecordDecl *RD = T->getAsRecordDecl()) {
      return removeFromIncludes(RD->getLocation());
    } else if (const clang::TemplateSpecializationType *TST =
                   T->getAs<clang::TemplateSpecializationType>()) {
      if (const clang::RecordDecl *RD = TST->getAsRecordDecl()) {
        return removeFromIncludes(RD->getLocation());
      }
    }
  }
  return true;
}

bool HeadersUnusedVisitor::removeFromIncludes(
    const clang::SourceLocation &Loc) {
  if (DE.getSourceManager().isInMainFile(Loc))
    return true;

  const llvm::StringRef Id = DE.getSourceManager().getFilename(Loc);
  return removeFromIncludes(Id);
}

bool HeadersUnusedVisitor::removeFromIncludes(const llvm::StringRef Id) {
  const auto It = IncludeData.Includes.find(Id);
  if (It != IncludeData.Includes.end()) {
    IncludeData.Includes.erase(It);
    // Also remove from SubSystemHeaderIncludes if it exists.
    if (!SystemOff)
      IncludeData.SubSystemHeaderIncludes.erase(Id);
    return !IncludeData.Includes.empty();
  }

  if (SystemOff)
    return true;

  // If this is a system header included through different system header then
  // get that top level include instead.
  const auto SSIt = IncludeData.SubSystemHeaderIncludes.find(Id);
  if (SSIt != IncludeData.SubSystemHeaderIncludes.end()) {
    const auto It = IncludeData.Includes.find(SSIt->second.first);
    if (It != IncludeData.Includes.end()) {
      if (SystemSmart) {
        IncludeData.Includes.erase(It);
        IncludeData.SubSystemHeaderIncludes.erase(SSIt);
        return !IncludeData.Includes.empty();
      } else {
        SubincludeUsed.insert(It->first);
      }
    }
  }
  return true;
}

void HeadersUnusedVisitor::removeTypeModifiers(std::string &TypeName) {
  if (TypeName.back() == '*' || TypeName.back() == '&') {
    do {
      TypeName.pop_back();
    } while (TypeName.back() == '*' || TypeName.back() == '&');
    TypeName.pop_back(); // Remove trailing space.
  }
}

void HeadersUnusedVisitor::removeKeywords(std::string &TypeName) {
  if (startsWith(TypeName, "class ") || startsWith(TypeName, "union "))
    TypeName.erase(0, 6);
  else if (startsWith(TypeName, "struct "))
    TypeName.erase(0, 7);
}

inline bool HeadersUnusedVisitor::startsWith(const std::string &Str,
                                             const std::string &Start) const {
  if (Start.size() > Str.size())
    return false;
  return std::equal(Start.begin(), Start.end(), Str.begin());
}

/* Implementation of HeadersVisitor */

HeadersVisitor::HeadersVisitor(clang::DiagnosticsEngine &DE,
                               AutocheckPPCallbacks &Callbacks) {
  const AutocheckContext &Context = AutocheckContext::Get();
  if (HeadersUnusedVisitor::isFlagPresent(Context))
    AllVisitors.push_front(std::make_unique<HeadersUnusedVisitor>(
        DE, Context, Callbacks.getIncludeInfo()));
}

void HeadersVisitor::run(clang::TranslationUnitDecl *TUD) {
  if (!AllVisitors.empty()) {
    this->TraverseDecl(TUD);
    for (auto &Visitor : AllVisitors)
      Visitor->PostWork();
  }
}

bool HeadersVisitor::TraverseDecl(clang::Decl *D) {
  if (!D) // Can be null if code had errors.
    return true;

  AllVisitors.remove_if([D](std::unique_ptr<HeadersVisitorInterface> &V) {
    return !V->PreTraverseDecl(D);
  });

  clang::RecursiveASTVisitor<HeadersVisitor>::TraverseDecl(D);

  AllVisitors.remove_if([D](std::unique_ptr<HeadersVisitorInterface> &V) {
    return !V->PostTraverseDecl(D);
  });
  return true;
}

bool HeadersVisitor::VisitCallExpr(const clang::CallExpr *CE) {
  AllVisitors.remove_if([CE](std::unique_ptr<HeadersVisitorInterface> &V) {
    return !V->VisitCallExpr(CE);
  });
  return true;
}

bool HeadersVisitor::VisitCXXConstructExpr(const clang::CXXConstructExpr *CCE) {
  AllVisitors.remove_if([CCE](std::unique_ptr<HeadersVisitorInterface> &V) {
    return !V->VisitCXXConstructExpr(CCE);
  });
  return true;
}

bool HeadersVisitor::VisitCXXRecordDecl(const clang::CXXRecordDecl *CRD) {
  AllVisitors.remove_if([CRD](std::unique_ptr<HeadersVisitorInterface> &V) {
    return !V->VisitCXXRecordDecl(CRD);
  });
  return true;
}

bool HeadersVisitor::VisitDeclRefExpr(const clang::DeclRefExpr *DRE) {
  AllVisitors.remove_if([DRE](std::unique_ptr<HeadersVisitorInterface> &V) {
    return !V->VisitDeclRefExpr(DRE);
  });
  return true;
}

bool HeadersVisitor::VisitEnumDecl(const clang::EnumDecl *ED) {
  AllVisitors.remove_if([ED](std::unique_ptr<HeadersVisitorInterface> &V) {
    return !V->VisitEnumDecl(ED);
  });
  return true;
}

bool HeadersVisitor::VisitGNUNullExpr(const clang::GNUNullExpr *GNE) {
  AllVisitors.remove_if([GNE](std::unique_ptr<HeadersVisitorInterface> &V) {
    return !V->VisitGNUNullExpr(GNE);
  });
  return true;
}

bool HeadersVisitor::VisitTranslationUnitDecl(
    const clang::TranslationUnitDecl *TUD) {
  AllVisitors.remove_if([TUD](std::unique_ptr<HeadersVisitorInterface> &V) {
    return !V->VisitTranslationUnitDecl(TUD);
  });
  return true;
}

bool HeadersVisitor::VisitTypedefDecl(const clang::TypedefDecl *TD) {
  AllVisitors.remove_if([TD](std::unique_ptr<HeadersVisitorInterface> &V) {
    return !V->VisitTypedefDecl(TD);
  });
  return true;
}

bool HeadersVisitor::VisitTypeLoc(const clang::TypeLoc &TL) {
  AllVisitors.remove_if([TL](std::unique_ptr<HeadersVisitorInterface> &V) {
    return !V->VisitTypeLoc(TL);
  });
  return true;
}

bool HeadersVisitor::VisitUsingDirectiveDecl(
    const clang::UsingDirectiveDecl *UDD) {
  AllVisitors.remove_if([UDD](std::unique_ptr<HeadersVisitorInterface> &V) {
    return !V->VisitUsingDirectiveDecl(UDD);
  });
  return true;
}

bool HeadersVisitor::VisitVarDecl(const clang::VarDecl *VD) {
  AllVisitors.remove_if([VD](std::unique_ptr<HeadersVisitorInterface> &V) {
    return !V->VisitVarDecl(VD);
  });
  return true;
}

} // namespace autocheck
