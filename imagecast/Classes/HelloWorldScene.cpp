#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ZipUtils.h"
USING_NS_CC;

//a7844c70e6bc4e24c4858730d3b14c2a
#define ImageJiaMi0 0xa7844c70
#define ImageJiaMi1 0xe6bc4e24
#define ImageJiaMi2 0xc4858730
#define ImageJiaMi3 0xd3b14c2a

using namespace cocostudio::timeline;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    ZipUtils::setPvrEncryptionKey(ImageJiaMi0, ImageJiaMi1, ImageJiaMi2, ImageJiaMi3);
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("res/parketHeroTouxiang.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("res/parketItem.plist");
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto closeItem = MenuItemFont::create("netTest", CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(200,0));
    closeItem->setAnchorPoint(Vec2(0,0));
    closeItem->setTag(1);
    
    tpoint = Vec2(visibleSize.width/2, visibleSize.height/2);
    Sprite *tsp = Sprite::createWithSpriteFrameName("ahuinan.jpg");
    tsp->setPosition(visibleSize.width/2, visibleSize.height/2);
    tsp->setScale(20.0f, 20.0f);
    this->addChild(tsp);
    
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    this->loadPlistSprite("res/parketHeroTouxiang.plist");
    isConvent = false;
    plistNumber = 0;
    
    return true;
}
void HelloWorld::loadPlistSprite(std::string plistname)
{
    std::string pname = plistname;
    __Dictionary *dic = __Dictionary::createWithContentsOfFile(pname.c_str());
    CCLOG("dic size= %d",dic->count());
    __Dictionary *frames = (__Dictionary*)(dic->objectForKey("frames"));
    CCLOG("frames size= %d",frames->count());
    __Array* keys =  frames->allKeys();
    
    for(int i = 0; i < keys->count();i++)
    {
        
        std::string strtmp = ((__String*)(keys->getObjectAtIndex(i)))->getCString();
        CCLOG("image name %d=%s",i,strtmp.c_str());
        
        Sprite* sprtmp = Sprite::createWithSpriteFrameName(strtmp);
        sprtmp->setPosition(tpoint);
        sprtmp->setAnchorPoint(Vec2::ZERO);
        //sprtmp->setVisible(false);
        this->addChild(sprtmp);
        
        m_sprites[strtmp] = sprtmp;
        m_imgnames.push_back(strtmp);
        
    }
}

void HelloWorld::saveImageFromSprite(cocos2d::Sprite* sp,std::string saveName)
{
    Sprite* pNewSpr = Sprite::createWithSpriteFrame(sp->displayFrame());
    RenderTexture* pRender = nullptr;
    
    pNewSpr->setAnchorPoint(Vec2::ZERO);
    pRender = RenderTexture::create(pNewSpr->getContentSize().width, pNewSpr->getContentSize().height, Texture2D::PixelFormat::RGBA8888);
    
    do{
        pRender->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
        pNewSpr->visit();
        pRender->end();
    }while (0);
    pRender->retain();
    Director::getInstance()->getRenderer()->render();
    Image *finalImage = pRender->newImage();
    
    std::string strtmp = saveName;
    std::string wpath = FileUtils::getInstance()->getWritablePath();
    std::string imgpath = wpath + "/" + strtmp;
    finalImage->saveToFile(imgpath);
    
}
void HelloWorld::callSaveImageToFile(float dt)
{
    if (m_imgnames.empty()) {
        Director::getInstance()->getScheduler()->unschedule(schedule_selector(HelloWorld::callSaveImageToFile), this);
        if (plistNumber == 0) {
            m_imgnames.clear();
            m_sprites.clear();
            this->loadPlistSprite("res/parketItem.plist");
            plistNumber = 1;
            Director::getInstance()->getScheduler()->schedule(schedule_selector(HelloWorld::callSaveImageToFile), this, 0.00f, false);
            
        }else if(plistNumber == 1)
        {
            return;
        }
        return;
    }
    
    std::string imgname = m_imgnames.back();
    m_imgnames.pop_back();
    Sprite* sprtmp = m_sprites[imgname];
    
    if (plistNumber == 0) {
        imgname = "hero/" + imgname;
    }else if(plistNumber == 1)
    {
        imgname = "item/" + imgname;
    }
    
    this->saveImageFromSprite(sprtmp,imgname);
    sprtmp->removeFromParent();
}

void HelloWorld::menuCloseCallback(cocos2d::Ref* pSender)
{
    MenuItem* itemtmp = dynamic_cast<MenuItem*>(pSender);
    if(itemtmp->getTag() == 1)
    {
        CCLOG("button touched.");
        
        if(isConvent)
        {
            return;
        }
        Director::getInstance()->getScheduler()->schedule(schedule_selector(HelloWorld::callSaveImageToFile), this, 0.00f, false);
        
    }
}