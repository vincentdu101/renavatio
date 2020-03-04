//
//  Texture.h
//  Game-mac
//
//  Created by Vincent Du on 2/26/20.
//  Copyright © 2020 Sanjay Madhav. All rights reserved.
//

#include <string>

class Texture
{
public:
    Texture();
    ~Texture();
    
    bool Load(const std::string& fileName);
    void Unload();
    
    void SetActive();
    
    int GetWidth() const { return mWidth; }
    int GetHeight() const { return mHeight; }
private:
    unsigned int mTextureID;
    int mWidth;
    int mHeight;
};
