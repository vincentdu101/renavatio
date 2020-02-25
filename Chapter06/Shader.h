//
//  Shader.h
//  Game-mac
//
//  Created by Vincent Du on 2/25/20.
//  Copyright © 2020 Sanjay Madhav. All rights reserved.
//

#pragma once
#include <GL/glew.h>
#include <string>
#include "Math.h"

class Shader
{
public:
    Shader();
    ~Shader();
    
    // load the vertex/fragment shaders with the given names
    bool Load(const std::string& vertName, const std::string& fragName);
    void Unload();
    
    // set this as the active shader program
    void SetActive();
    
    // sets a matrix uniform
    void SetMatrixUniform(const char* name, const Matrix4& matrix);
    
    // sets a vector uniform
    void SetVectorUniform(const char* name, const Vector3& vector);
    
    // sets a float uniform
    void SetFloatUniform(const char* name, float value);
    
private:
    // tries to compile the specified shader
    bool CompileShader(const std::string& fileName,
                       GLenum shaderType,
                       GLuint& outShader);
    
    // tests whether shader compiled successfully
    bool IsCompiled(GLuint shader);
    
    // tests whether vertex/fragment programs link
    bool IsValidProgram();
private:
    // store the shader object ids
    GLuint mVertexShader;
    GLuint mFragShader;
    GLuint mShaderProgram;
};
