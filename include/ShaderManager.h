#pragma once

#include "Shader.h"
#include "Compiler.h"
#include "ResourceHolder.hpp"

#include <map> 
#include <string> 
#include <memory>

namespace star{
    namespace shadermanager{
        class ShaderManager{
        public: 

            ///add shader to manager, will return handle to compiled resource
            int AddShader(std::string pathToFile); 
        protected: 

        private:
            uint8_t shaderCounter = 0; //keep track of how many shaders are in manager
            common::ResourceHolder<common::Shader, std::string> holder; 
        };
    }
}
