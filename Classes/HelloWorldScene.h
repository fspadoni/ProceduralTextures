#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "physics-render/CCRenderTextureLayer.h"



class ShaderRenderPass;



class HelloWorld : public cocos2d::RenderTextureLayer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    virtual void update(float delta) override;
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;
    virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) override;
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override;

    
    void setProcShader(ShaderRenderPass* procShader) { _procShader = procShader; }
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    
private:
    
    void afterCaptured(bool succeed, const std::string& outputFile);
    
    static unsigned int _screenShotCounter;
    
    ShaderRenderPass* _procShader;
    
    int _numberScreenShotToCapture;
    
    bool _startCapture;
    bool _startCaptureOneCycle;
    
};

#endif // __HELLOWORLD_SCENE_H__
