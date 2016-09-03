#include "HelloWorldScene.h"

#include "ShaderRenderPass.h"

//#include "physics-render/CCRenderPass.h"
#include "physics-shaders/CCPhysicsShaders.h"

USING_NS_CC;

unsigned int HelloWorld::_screenShotCounter = 0;


Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer, 0);
    
    Size visibleSize = Director::getInstance()->getWinSize();
    
    ShaderRenderPass* shaderRenderPass = ShaderRenderPass::create(visibleSize.width, visibleSize.height);
    shaderRenderPass->setGLShader(std::string("shaders/default.vsh"), std::string("shaders/utility.fsh"));
    shaderRenderPass->setFrequency(0.25);
    shaderRenderPass->setNoiseFrequency( 0.006 );
    shaderRenderPass->setNoiseAmplitude( 5.0 );
    shaderRenderPass->setNoiseNumberOctave( 1 );
    
//    shaderRenderPass->setGLShader(ccShader_2D_def_vert, ccShader_2D_def_frag);
    shaderRenderPass->addTexture( layer->getOutputTexture(), 0 );
    scene->addChild((Node*)shaderRenderPass, 2);
    layer->setProcShader( shaderRenderPass );
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !RenderTextureLayer::init() )
    {
        return false;
    }
    
    Node::scheduleUpdate();
    
    _numberScreenShotToCapture = 0;
    _startCapture = false;
    _startCaptureOneCycle = false;
    
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    
    // Register Keyboard Event
    //    setKeyboardEnabled(true);
    EventListenerKeyboard* _keyboardListener = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
    _keyboardListener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboardListener, this);

    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void HelloWorld::update(float delta)
{
    
    Node::update(delta);
    

    if ( _startCapture )
    {
        std::ostringstream fileName;
        fileName << "ScreenShot";
        fileName.width(2);
        fileName.fill('_');
        fileName << _screenShotCounter++ << ".png";
        utils::captureScreen(CC_CALLBACK_2(HelloWorld::afterCaptured, this), fileName.str());
        
        if ( --_numberScreenShotToCapture <= 0 )
            _startCapture = false;
    }
    
    if ( _startCaptureOneCycle )
    {
        
    }
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* unused_event)
{

    
    if ( keyCode == EventKeyboard::KeyCode::KEY_P )
    {
        if ( Director::getInstance()->isPaused() )
        {
            Director::getInstance()->resume();
//            _oneStep = false;
        }
        else
            Director::getInstance()->pause();
    }
    if (keyCode == EventKeyboard::KeyCode::KEY_S )
    {
        std::ostringstream fileName;
        fileName << "ScreenShot";
        fileName.width(2);
        fileName.fill('_');
        fileName << _screenShotCounter++ << ".png";
        utils::captureScreen(CC_CALLBACK_2(HelloWorld::afterCaptured, this), fileName.str());
    }
    if ( keyCode == EventKeyboard::KeyCode::KEY_C )
    {
        _startCapture = true;
    
        _numberScreenShotToCapture = _procShader->getFrequency() / cocos2d::Director::getInstance()->getAnimationInterval() + 1;
    
    }
    if ( keyCode == EventKeyboard::KeyCode::KEY_A )
    {
        _startCaptureOneCycle = true;
    }
}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* unused_event)
{

    if ( keyCode == EventKeyboard::KeyCode::KEY_P )
    {
        
    }
    if (keyCode == EventKeyboard::KeyCode::KEY_S )
    {

    }
    if ( keyCode == EventKeyboard::KeyCode::KEY_C )
    {
 
    }
    if ( keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW )
    {
        
    }
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
    
    _procShader->setMousePosition( touch->getLocation() );
    
    return true;
}


void HelloWorld::onTouchMoved(Touch* touch, Event* event)
{
    _procShader->setMousePosition( touch->getLocation() );
    
}



void HelloWorld::onTouchEnded(Touch* touch, Event* event)
{

}


void HelloWorld::afterCaptured(bool succeed, const std::string& outputFile)
{
    if (succeed)
    {
        // show screenshot
        log("\nCapture screen succeeded: %s", outputFile.c_str() );
    }
    else
    {
        log("Capture screen failed.");
    }
}

