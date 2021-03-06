//
//  VertexArray.cpp
//  Game-mac
//
//  Created by Vincent Du on 2/26/20.
//  Copyright © 2020 Sanjay Madhav. All rights reserved.
//

#include "VertexArray.h"
#include <GL/glew.h>

VertexArray::VertexArray(const float* verts, unsigned int numVerts,
    const unsigned int* indices, unsigned int numIndices)
    :mNumVerts(numVerts)
    ,mNumIndices(numIndices)
{
    // Create vertex array
    glGenVertexArrays(mVertexObjects, &mVertexArray);
    glBindVertexArray(mVertexArray);

    // Create vertex buffer
    glGenBuffers(mVertexObjects, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, numVerts * 8 * sizeof(float), verts, GL_STATIC_DRAW);

    // Create index buffer
    glGenBuffers(mVertexObjects, &mIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    // Specify the vertex attributes
    // (For now, assume one vertex format)
    // Position is 3 floats
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
    // Normal is 3 floats
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
        reinterpret_cast<void*>(sizeof(float) * 3));
    // Texture coordinates is 2 floats
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
        reinterpret_cast<void*>(sizeof(float) * 6));
}

VertexArray::~VertexArray()
{
    // deleting 1 vertex object because we generated one above
    glDeleteBuffers(mVertexObjects, &mVertexBuffer);
    glDeleteBuffers(mVertexObjects, &mIndexBuffer);
    glDeleteVertexArrays(mVertexObjects, &mVertexArray);
}

void VertexArray::SetActive()
{
    glBindVertexArray(mVertexArray);
}



