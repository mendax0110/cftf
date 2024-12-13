#ifndef MODERNIZER_H
#define MODERNIZER_H

#include <clang/AST/Decl.h>
#include <clang/AST/Stmt.h>
#include <clang/AST/Type.h>
#include <clang/Basic/SourceLocation.h>
#include <string>
#include <clang/Rewrite/Core/Rewriter.h>

namespace modernizer
{
    // Upgrade old-style C++ for-loops to modern C++ range-based loops
    std::string UpgradeForLoop(clang::ForStmt* forStmt, clang::Rewriter& rewriter);

    // Replace raw pointers with smart pointers (e.g., std::unique_ptr, std::shared_ptr)
    std::string ReplaceRawPointers(clang::Type* type, clang::SourceLocation location, clang::Rewriter& rewriter);

    // Refactor typedefs into using declarations
    std::string RefactorTypedefs(clang::TypedefDecl* typedefDecl, clang::Rewriter& rewriter);

} // namespace modernizer

#endif // MODERNIZER_H
