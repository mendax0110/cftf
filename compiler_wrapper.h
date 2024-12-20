#ifndef COMPILER_WRAPPER_H
#define COMPILER_WRAPPER_H

#include <string>
#include <vector>

namespace compiler
{

    class CompilerWrapper
    {
    public:
        CompilerWrapper(std::string  compiler = "clang++");

        // Method to invoke the system compiler with given flags
        bool Compile(const std::string& sourceFile, const std::vector<std::string>& flags);

        // Get compilation errors if any
        static std::string GetCompilationErrors() ;

    private:
        std::string compiler_;  // Compiler command (clang++)
        std::string errors_;    // Compilation errors string
    };

} // namespace compiler

#endif // COMPILER_WRAPPER_HPP
