#include "compiler_wrapper.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <array>
#include <sstream>
#include <stdexcept>

namespace compiler
{

    CompilerWrapper::CompilerWrapper(const std::string& compiler)
            : compiler_(compiler) {}

    bool CompilerWrapper::Compile(const std::string& sourceFile, const std::vector<std::string>& flags)
    {
        // Construct the compilation command
        std::stringstream command;
        command << compiler_;
        for (const auto& flag : flags)
        {
            command << " " << flag;
        }
        command << " " << sourceFile;

        // Execute the command
        int result = std::system(command.str().c_str());

        if (result != 0)
        {
            // Compilation failed, capture errors
            errors_ = GetCompilationErrors();
            return false;
        }

        return true;
    }

    std::string CompilerWrapper::GetCompilationErrors() const
    {
        // Capture compilation errors from standard error (stderr)
        std::array<char, 128> buffer;
        std::stringstream errors;
#if defined(__linux__)
        FILE* fp = popen("clang++ -v", "r");
#else if defined(_WIN32)
        FILE* fp = _popen("clang++ -v", "r");
#endif
        if (fp == nullptr)
        {
            errors << "Error opening process to capture errors.";
            return errors.str();
        }

        while (fgets(buffer.data(), buffer.size(), fp) != nullptr)
        {
            errors << buffer.data();
        }
        fclose(fp);

        return errors.str();
    }

} // namespace compiler
