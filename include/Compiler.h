#pragma once

#include "shaderc/shaderc.hpp"

#include <vector> 
#include <string>
#include <iostream>

namespace star{
    namespace shadermanager{
        class Compiler{
        public:   
            //compile provided shader to spirv
            static void Compile(const std::string& pathToFile); 

        private:
            std::string CompileToAssembly(const std::vector<const char> sourceCode); 
        }; 
    }
}