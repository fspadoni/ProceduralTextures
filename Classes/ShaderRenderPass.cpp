//
//  ShadersSpriteRenderPass.cpp
//  ProcTextures
//
//  Created by Federico Spadoni on 10/06/16.
//
//

#include "ShaderRenderPass.h"




ShaderRenderPass* ShaderRenderPass::create(int width, int height, cocos2d::Texture2D::PixelFormat eFormat)
{
	ShaderRenderPass* p = new (std::nothrow) ShaderRenderPass();
	if (p && p->init(width, height, eFormat)) {
		p->autorelease();
		return p;
	}
	else {
		CC_SAFE_DELETE(p);
		return nullptr;
	}
}


ShaderRenderPass* ShaderRenderPass::create(cocos2d::Texture2D* texture)
{
	ShaderRenderPass* p = new (std::nothrow) ShaderRenderPass();
	if (p && p->initWithTexture(texture)) {
        p->setAnchorPoint(cocos2d::Point::ZERO);
        p->setPosition(cocos2d::Point::ZERO);
        p->setFlippedY(true);
		p->autorelease();
		return p;
	}
	else {
		CC_SAFE_DELETE(p);
		return nullptr;
	}
}

ShaderRenderPass::ShaderRenderPass()
: SpriteRenderPass()
, _time(0.0)
, _timeElapsed(0.0)
, _frequency(1.0)
, _mousePosition(cocos2d::Vec2::ZERO)
, _needToUpdate(true)
{
    scheduleUpdate();
    
    _resolution = cocos2d::Director::getInstance()->getVisibleSize();
    _noiseFrequency = 1.0;
    _noiseAmplitude = 1.0;
    _noiseNumberOctave = 1;
}

ShaderRenderPass::~ShaderRenderPass()
{
}


void ShaderRenderPass::update(float delta)
{
    if ( !_needToUpdate )
        return;
    
    
    _timeElapsed += cocos2d::Director::getInstance()->getAnimationInterval();
    _time = _timeElapsed*_frequency;

    
    if ( _time > 1.0)
        _timeElapsed = 0;
    
}


void ShaderRenderPass::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
    _customCommand.init(_globalZOrder, transform, flags);
    _customCommand.func = CC_CALLBACK_0(ShaderRenderPass::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
    
    //    Sprite::draw(renderer, transform, flags);
}

void ShaderRenderPass::onDraw(const cocos2d::Mat4 &transform, uint32_t flags)
{
    auto glProgram = getGLProgram();
    glProgram->use();
    //    glProgram->setUniformsForBuiltins(_modelViewTransform);
    
    _glProgramState->apply(transform);
    
    
    for ( auto tex : _inputTextures )
    {
        cocos2d::GL::bindTexture2DN( tex.first, tex.second->getName());
    }
    
    
    cocos2d::GL
    ::blendFunc(_blendFunc.src, _blendFunc.dst);
    
    
    
    if (cocos2d::Configuration::getInstance()->supportsShareableVAO())
    {
        cocos2d::GL::bindVAO(_vao);
    }
    else
    {
        cocos2d::GL::enableVertexAttribs(cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION | cocos2d::GL::VERTEX_ATTRIB_FLAG_TEX_COORD);
        
        glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
        // vertex
        glVertexAttribPointer(cocos2d::GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(cocos2d::V3F_T2F), (GLvoid *)offsetof(cocos2d::V3F_T2F, vertices));
        
        // texcood
        glVertexAttribPointer(cocos2d::GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(cocos2d::V3F_T2F), (GLvoid *)offsetof(cocos2d::V3F_T2F, texCoords));
        
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo[1]);
        
    }
    
    _glProgramState->setUniformFloatv("iGlobalTime", 1, &_timeElapsed);
    _glProgramState->setUniformVec2("iResolution", _resolution);
    _glProgramState->setUniformFloatv("iFreq", 1, &_noiseFrequency);
    _glProgramState->setUniformFloatv("iAmp", 1, &_noiseAmplitude);
    _glProgramState->setUniformInt("iNumberOctave", _noiseNumberOctave);
    _glProgramState->setUniformVec2("iMouse", _mousePosition);
    

    
    
    glDrawElements(GL_TRIANGLES, (GLsizei) 6, GL_UNSIGNED_SHORT, _quadIndices );
    
    
    
    CC_INCREMENT_GL_DRAWS(1);
    CHECK_GL_ERROR_DEBUG();
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    if (cocos2d::Configuration::getInstance()->supportsShareableVAO())
    {
        //Unbind VAO
        cocos2d::GL::bindVAO(0);
    }
    
    
}


