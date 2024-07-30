#pragma once

#include "clang-tidy/ClangTidyCheck.h"

namespace clang::tidy::snps 
{

/// FIXME: Write a short description.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/snps/avoid-current-time.html
class AvoidCurrentTimeCheck : public ClangTidyCheck {
public:
  AvoidCurrentTimeCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::snps