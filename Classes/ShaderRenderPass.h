//
//  ShadersSpriteRenderPass.h
//  ProcTextures
//
//  Created by Federico Spadoni on 10/06/16.
//
//

#ifndef __ProcTextures__ShaderRenderPass__
#define __ProcTextures__ShaderRenderPass__


#include "physics-render/CCRenderPass.h"


class ShaderRenderPass : public cocos2d::SpriteRenderPass
{
public:

    static ShaderRenderPass* create(int width, int height, cocos2d::Texture2D::PixelFormat eFormat = cocos2d::Texture2D::PixelFormat::RGBA8888);
    
    static ShaderRenderPass* create(cocos2d::Texture2D* texture);
    
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
    
    void onDraw(const cocos2d::Mat4 &transform, uint32_t flags);
    
    virtual void update(float delta) override;

    
    void setFrequency(const float freq) { _frequency = freq; }
    float getFrequency() { return _frequency; }
    
    
    void setNoiseFrequency(const float noiseFreq) { _noiseFrequency = noiseFreq; }
    float getNoiseFrequency() { return _noiseFrequency; }
    
    void setNoiseAmplitude(const float noiseAmp) { _noiseAmplitude = noiseAmp; }
    float getNoiseAmplitude() { return _noiseAmplitude; }
    
    void setNoiseNumberOctave(const unsigned int noiseNumOct) { _noiseNumberOctave = noiseNumOct; }
    unsigned int getNoiseNumberOctave() { return _noiseNumberOctave; }
    
    void setMousePosition(cocos2d::Vec2 pos) { _mousePosition = pos; }
    cocos2d::Vec2 getMousePosition() { return _mousePosition; }
    
    void play(bool on) { _needToUpdate = on; }

    
private:
    
    ShaderRenderPass();
    
    virtual ~ShaderRenderPass();

    
    float _time;
    float _timeElapsed;
    float _frequency;
    
    cocos2d::Vec2 _resolution;
    float _noiseFrequency;
    float _noiseAmplitude;
    unsigned int _noiseNumberOctave;
    
    cocos2d::Vec2 _mousePosition;
    
    bool _needToUpdate;
    
};

#endif /* defined(__ProcTextures__ShaderRenderPass__) */
