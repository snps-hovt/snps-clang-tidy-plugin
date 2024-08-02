//===--- AvoidCurrentTimeCheck.cpp - clang-tidy ---------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidCurrentTimeCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace snps {

namespace 
{
bool isDateTimeFunctoin(const CallExpr* callExpr)
{
  if (!callExpr) return false;
  static const StringRef dateOrTimeFunctions [] = {"time", "std::time", "gettimeofday"};
  const FunctionDecl* functionDecl = callExpr->getDirectCallee();
  if (!functionDecl 
    || functionDecl->getLexicalDeclContext()->isRecord() 
    || functionDecl->getLexicalDeclContext()->isNamespace()) return false;

  StringRef functionName = functionDecl->getName();

  for (StringRef dateOrTimeFunction : dateOrTimeFunctions)
  {
    if (functionName.equals(dateOrTimeFunction)) return true;
  }
  return false;
}
} // unnamed ma,espace

void AvoidCurrentTimeCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(callExpr().bind("function_call"), this);
  Finder->addMatcher(varDecl(hasType(asString("time_t"))).bind("time_t_decl"), this);
}

void AvoidCurrentTimeCheck::check(const MatchFinder::MatchResult &Result) {
  const CallExpr *callExpr = Result.Nodes.getNodeAs<CallExpr>("function_call");
  if (isDateTimeFunctoin(callExpr))
    diag(callExpr->getBeginLoc(), "function %0 is making the application non-deterministic.") << callExpr->getDirectCallee();
  const VarDecl *timeTDecl = Result.Nodes.getNodeAs<VarDecl>("time_t_decl");
  if (timeTDecl)
    diag(timeTDecl->getLocation(), "Using %0 could making the application non-deterministic.") << timeTDecl->getType();
}

} // namespace snps
} // namespace tidy
} // namespace clang
