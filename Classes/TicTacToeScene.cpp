#include "TicTacToeScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* TicTacToe::createScene() {
    return TicTacToe::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename) {
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool TicTacToe::init() {
    //////////////////////////////
    // 1. super init first
    if (!Scene::init()) {
        return false;
    }
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    initial();
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(TicTacToe::onTouchBegan, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    return true;
}

bool TicTacToe::onTouchBegan(Touch* touch, Event* event) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            Vec2 areaX = Vec2(origin.x + visibleSize.width / 2 - 100 + i * 100 - 50,
                origin.y + visibleSize.height / 2 - 100 + j * 100 - 50);
            Vec2 areaX2 = Vec2(origin.x + visibleSize.width / 2 - 100 + i * 100 + 50,
                origin.y + visibleSize.height / 2 - 100 + j * 100 - 50);
            Vec2 areaX3 = Vec2(origin.x + visibleSize.width / 2 - 100 + i * 100 - 50,
                origin.y + visibleSize.height / 2 - 100 + j * 100 + 50);
            Vec2 areaX4 = Vec2(origin.x + visibleSize.width / 2 - 100 + i * 100 + 50,
                origin.y + visibleSize.height / 2 - 100 + j * 100 + 50);

            if (touch->getLocation().x < areaX4.x && touch->getLocation().x > areaX.x &&
                touch->getLocation().y < areaX4.y && touch->getLocation().y > areaX.y) {
                if (sisa > 0 && post[i][j] == 99 && !stop) {
                    post[i][j] = player % 2;
                    auto sprite = Sprite::create();
                    if (player % 2 == 0) sprite = Sprite::create("res/O.png");
                    else sprite = Sprite::create("res/X.png");
                    sprite->setPosition(areaX.x + 50, areaX.y + 50);
                    sprite->setTag(1);
                    this->addChild(sprite);
                    sisa[player % 2]--;
                    move++;
                    player++;
                    if (Oturn->getOpacity() > 0) {
                        Xturn->setOpacity(255);
                        Oturn->setOpacity(0);
                    }
                    else {
                        Xturn->setOpacity(0);
                        Oturn->setOpacity(255);
                    }
                    checkGameOver(i, j, (player - 1) % 2);
                }
            }
        }
    }
    return true;
}

void TicTacToe::initial() {
    //auto bg = LayerColor::create(Color4B(0, 188, 212, 255));
    auto bg = LayerColor::create(Color4B(50, 143, 96, 255));
    this->addChild(bg);

    //Draw lines
    auto draw = DrawNode::create();
    draw->drawLine(
        Vec2(origin.x + visibleSize.width / 2 - 150, origin.y + visibleSize.height / 2 - 50),
        Vec2(origin.x + visibleSize.width / 2 + 150, origin.y + visibleSize.height / 2 - 50),
        Color4F::WHITE);

    addChild(draw);
    draw->drawLine(
        Vec2(origin.x + visibleSize.width / 2 - 150, origin.y + visibleSize.height / 2 + 50),
        Vec2(origin.x + visibleSize.width / 2 + 150, origin.y + visibleSize.height / 2 + 50),
        Color4F::WHITE);

    addChild(draw);
    draw->drawLine(
        Vec2(origin.x + visibleSize.width / 2 - 50, origin.y + visibleSize.height / 2 - 150),
        Vec2(origin.x + visibleSize.width / 2 - 50, origin.y + visibleSize.height / 2 + 150),
        Color4F::WHITE);

    addChild(draw);
    draw->drawLine(
        Vec2(origin.x + visibleSize.width / 2 + 50, origin.y + visibleSize.height / 2 - 150),
        Vec2(origin.x + visibleSize.width / 2 + 50, origin.y + visibleSize.height / 2 + 150),
        Color4F::WHITE);

    addChild(draw);

    Oturn = Sprite::create("res/O.png");
    Xturn = Sprite::create("res/X.png");
    Oturn->setScale(0.5);
    Xturn->setScale(0.5);
    Oturn->setPosition(
        Vec2(origin.x + visibleSize.width * 3 / 4 + 100, origin.y + visibleSize.height / 2));
    Xturn->setPosition(
        Vec2(origin.x + visibleSize.width * 1 / 4 - 100, origin.y + visibleSize.height / 2));
    this->addChild(Oturn);
    this->addChild(Xturn);

    //Because O first
    Xturn->setOpacity(0);

    stop = false;
    sisa[0] = 5;
    sisa[1] = 4;
    move = 0;
    player = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            post[i][j] = 99;
        }
    }
}

//Checking the victory of one of the players
void TicTacToe::checkGameOver(int x, int y, int player) {
    bool gameOver = true;
    float delay = 2.0f;
    auto delayAction = DelayTime::create(delay);
    auto funcCallback = CallFunc::create([player, gameOver, this]() {
        reset(player, gameOver);
        });

    for (int i = 0; i < 3; i++) {
        if (post[i][y] != player) {
            gameOver = false;
            break;
        }
    }

    if (gameOver) {
        Xturn->setOpacity(0);
        Oturn->setOpacity(0);
        stop = true;
        this->runAction(Sequence::create(delayAction, funcCallback, NULL));
        return;
    }

    gameOver = true;
    for (int i = 0; i < 3; i++) {
        if (post[x][i] != player) {
            gameOver = false;
            break;
        }
    }

    if (gameOver) {
        Xturn->setOpacity(0);
        Oturn->setOpacity(0);
        stop = true;
        this->runAction(Sequence::create(delayAction, funcCallback, NULL));
        return;
    }

    if (x == y) {
        gameOver = true;
        for (int i = 0; i < 3; i++) {
            if (post[i][i] != player) {
                gameOver = false;
                break;
            }
        }
    }

    if (gameOver) {
        Xturn->setOpacity(0);
        Oturn->setOpacity(0);
        stop = true;
        this->runAction(Sequence::create(delayAction, funcCallback, NULL));
        return;
    }

    if (x == y - 2 || y == x - 2 || (x == 1 && y == 1)) {
        gameOver = true;
        for (int i = 0; i < 3; i++) {
            if (post[i][2 - i] != player) {
                gameOver = false;
                break;
            }
        }
    }

    if (move == 9 || gameOver) {
        stop = true;
        Xturn->setOpacity(0);
        Oturn->setOpacity(0);
        auto funcCallback = CallFunc::create([player, gameOver, this]() {
            reset(player, gameOver);
            });
        this->runAction(Sequence::create(delayAction, funcCallback, NULL));
    }
    return;
}

void TicTacToe::reset(int player, bool check) {
    auto bg = LayerColor::create(Color4B(50, 205, 50, 100));
    this->addChild(bg);

    auto label = Label::create();
    if (!check) {
        label = Label::createWithTTF("DRAW!", "fonts/Roboto.ttf", 50);
    }
    else {
        if (player == 0) {
            label = Label::createWithTTF("O WIN!", "fonts/Roboto.ttf", 50);
        }
        else {
            label = Label::createWithTTF("X WIN!", "fonts/Roboto.ttf", 50);
        }
    }
    label->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    this->addChild(label);
    this->scheduleOnce(schedule_selector(TicTacToe::removeAll), 1.7);
}

//Clear table
void TicTacToe::removeAll(float dt) {
    this->removeAllChildren();
    initial();
}