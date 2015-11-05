#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    void loadPlistSprite(std::string plistname);
    
    
    void saveImageFromSprite(cocos2d::Sprite* sp,std::string saveName);
    
    cocos2d::Vec2 tpoint;
    
    std::vector<std::string> m_imgnames;
    std::map<std::string,cocos2d::Sprite*> m_sprites;
    
    bool isConvent;
    void callSaveImageToFile(float dt);
    
    
    
    int plistNumber;
};

#endif // __HELLOWORLD_SCENE_H__
