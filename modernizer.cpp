#include "modernizer.h"
#include <clang/AST/ASTContext.h>
#include <clang/AST/Stmt.h>
#include <clang/AST/Decl.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/Lex/Lexer.h>
#include <clang/AST/PrettyPrinter.h>
#include <clang/AST/Type.h>
#include <clang/Frontend/CompilerInstance.h>

namespace modernizer
{

    std::string UpgradeForLoop(clang::ForStmt* forStmt, clang::Rewriter& rewriter)
    {
        if (!forStmt) return "";

        auto init = forStmt->getInit();
        auto cond = forStmt->getCond();
        auto inc = forStmt->getInc();
        auto body = forStmt->getBody();

        if (!init || !cond || !inc)
        {
            llvm::errs() << "Unsupported loop structure for upgrade.\n";
            return "";
        }

        std::string rangeExpr;
        if (auto* binaryOp = llvm::dyn_cast<clang::BinaryOperator>(cond))
        {
            if (auto* declRef = llvm::dyn_cast<clang::DeclRefExpr>(binaryOp->getLHS()))
            {
                rangeExpr = declRef->getNameInfo().getAsString();
            }
        }

        if (rangeExpr.empty())
        {
            llvm::errs() << "Unable to deduce range expression for loop.\n";
            return "";
        }

        std::string newLoop = "for (auto& elem : " + rangeExpr + ") {\n";

        if (body)
        {
            auto bodyText = clang::Lexer::getSourceText(clang::CharSourceRange::getTokenRange(body->getSourceRange()), rewriter.getSourceMgr(), rewriter.getLangOpts());
            newLoop += bodyText.str() + "\n";
        }

        newLoop += "}\n";

        rewriter.ReplaceText(forStmt->getSourceRange(), newLoop);

        return newLoop;
    }

    std::string ReplaceRawPointers(clang::Type* type, clang::SourceLocation location, clang::Rewriter& rewriter)
    {
        if (!type) return "";

        auto typeStr = type->getCanonicalTypeInternal().getAsString();

        if (typeStr.find('*') != std::string::npos)
        {
            std::string smartPointerType = "std::unique_ptr<" + typeStr.substr(0, typeStr.size() - 1) + ">";

            rewriter.ReplaceText(location, typeStr.size(), smartPointerType);

            return smartPointerType;
        }

        return typeStr;
    }

    std::string RefactorTypedefs(clang::TypedefDecl* typedefDecl, clang::Rewriter& rewriter)
    {
        if (!typedefDecl) return "";

        auto underlyingType = typedefDecl->getUnderlyingType();
        std::string newDecl = "using " + typedefDecl->getNameAsString() + " = " +
            underlyingType.getAsString() + ";";

        rewriter.ReplaceText(typedefDecl->getSourceRange(), newDecl);

        return newDecl;
    }

} // namespace modernizer
