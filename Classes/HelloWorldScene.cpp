/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

namespace InterfaceButtons
{
	int BUTTONS_PARENT_TAG = 0;
	int TITLE_TAG = 1;
	int ATTACK_TAG = 2;
}

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	_myHero = new Hero(this, "hero_2.json", "hero_2.atlas", 1.5f); // 0.2 is a set of scaling images
	if(_myHero)
		_myHero->SetPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	_attacking = false;

	auto _mouseListener = EventListenerMouse::create();

	_mouseListener->onMouseUp = CC_CALLBACK_1(HelloWorld::onMouseUp, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

	auto attackButton = MenuItemImage::create(
												"button.png",
												"button.png",
												CC_CALLBACK_1(HelloWorld::attackCallBack, this));
	if (attackButton)
	{
		float x = origin.x + visibleSize.width / 2;
		float y = origin.y + attackButton->getContentSize().height / 2;
		attackButton->setPosition(Vec2(x, y));
		auto title = Label::createWithSystemFont("Attack", "Arial", 24);
		title->setPosition(Vec2(attackButton->getContentSize().width / 2, attackButton->getContentSize().height / 2));
		attackButton->addChild(title, 1, InterfaceButtons::TITLE_TAG);
	}

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
	menu->addChild(attackButton, 1, InterfaceButtons::ATTACK_TAG);
    this->addChild(menu, 1, InterfaceButtons::BUTTONS_PARENT_TAG);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("Hero Movement", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }

    return true;
}

void HelloWorld::onMouseUp(Event *event)
{
	EventMouse* mouseEvent = (EventMouse*)event;

	if (mouseEvent->getMouseButton() == cocos2d::EventMouse::MouseButton::BUTTON_RIGHT)
	{
		_attacking = false;
		ChangeButtonTitle(InterfaceButtons::ATTACK_TAG, "Attack");
		if(_myHero)
			_myHero->MoveTo(Vec2(mouseEvent->getCursorX(), mouseEvent->getCursorY()));
	}
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

void HelloWorld::attackCallBack(cocos2d::Ref * pSender)
{
	if (_myHero)
	{
		if (_attacking)
		{
			_myHero->Stop();
			_attacking = false;
			ChangeButtonTitle(InterfaceButtons::ATTACK_TAG, "Attack");
		}
		else
		{
			_myHero->Attack();
			_attacking = true;
			ChangeButtonTitle(InterfaceButtons::ATTACK_TAG, "Stop");
		}
	}
}

void HelloWorld::ChangeButtonTitle(const int & buttonTag, const std::string & newTitle)
{
	auto buttonsParent = this->getChildByTag(InterfaceButtons::BUTTONS_PARENT_TAG);
	if (buttonsParent)
	{
		auto button = buttonsParent->getChildByTag(buttonTag);
		if (button && button->getChildrenCount() > 0)
		{
			Label* title = static_cast<Label*>(button->getChildByTag(InterfaceButtons::TITLE_TAG));
			if (title)
			{
				title->setString(newTitle);
			}
		}
	}	
}
