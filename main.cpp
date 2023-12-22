#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <sstream>

using namespace std;

namespace Assets
{
    class Font : public sf::Font
    {
    public:
        Font(string path)
        {
            loadFromFile(path);
        }
    };
    class Texture : public sf::Texture
    {
    public:
        Texture(string path)
        {
            loadFromFile(path);
        }
    };
};
namespace Assets::Fonts
{
    Font Arial("../arial.ttf");
};
namespace Assets::SpriteSheets
{
    //Texture abc("");
    Texture LeverLeft("../leverLeft.png");
    Texture LeverRight("../leverRight.png");
    Texture DoorOpen("../DoorOpen.png");
    Texture DoorClose("../DoorClose.png");
};
class Player {
public:
    sf::RectangleShape body;
    int health = 3;
    unsigned int row = 0;
    bool faceRight = true;
    sf::Clock timer;
    bool delay = false;
    float speedY = 0;

    Player() {
        faceRight = true;
        body.setSize(sf::Vector2f(40.0f, 60.0f));
        body.setFillColor(sf::Color::Green);
    }

};

class Enemy {
public:
    sf::RectangleShape body;
    int posMin = 0;
    int posMax = 0;
    sf::Vector2f startingPos;
    bool kierunek = true; //true = prawo


    Enemy(sf::Vector2f pos, int min = 0, int max = 0) {
        body.setPosition(pos);
        startingPos = pos;
        body.setSize(sf::Vector2f(40.0f, 60.0f));
        body.setFillColor(sf::Color::Red);
        posMin = min;
        posMax = max;
    }

    Enemy() {
        body.setSize(sf::Vector2f(40.0f, 60.0f));
        body.setFillColor(sf::Color::Red);
    }

    virtual sf::RectangleShape &getBody() {                 // Czemu zatem klasa nie jest abstrakcyjna? czy jest?
        return body;
    }

    void setmovement(int min, int max) {
        posMin = min;
        posMax = max;
    }

    void draw(sf::RenderWindow &window) {
        window.draw(body);
    }
};

class Animation {
public:
    vector<sf::Texture *> textures;
};

class Animations {
public:
    vector<Animation> animations;
    vector<int> animationLength;
    Player *player = NULL;
    int frame = 0;
    int animationNr = -1;
    bool pause = true;
    int animationTime = 500;
    sf::Clock timer;

    Animations(string path, int y, vector<int> x, sf::Vector2f size, sf::Vector2f offset, Player *pl) {
        player = pl;
        sf::Image im;
        im.loadFromFile(path);

        for (int i = 0; i < y; i++) {
            Animation temp;
            for (int j = 0; j < x[i]; j++) {
                sf::Texture *texture = new sf::Texture;
                texture->loadFromImage(im, sf::IntRect(offset.x + j * size.x, offset.y + i * size.y, size.x, size.y));
                temp.textures.emplace_back(texture);
            }
            animationLength.emplace_back(x[i]);
        }
    }

    void setAnimation(int nr) {
        if (animationNr != -1) {
            animationNr = nr;
            frame = 0;
            timer.restart();
        }
    }

    void playAnimation() {
        if (animationNr != -1) {
            pause = false;
            timer.restart();
        }
    }

    void pauseAnimation() {
        if (animationNr != -1) pause = true;
    }

    void update() {
        if (animationNr != -1) {
            if (timer.getElapsedTime().asMilliseconds() >= animationTime && !pause) {
                if (frame >= animationLength[animationNr]) {
                    frame = 0;
                } else
                    frame++;
                //settexture
                player->body.setTexture(animations[animationNr].textures[frame]);
                timer.restart();
            }
        }

    }
};

class CollisionObjects { //abstrakcyjna

public:
    bool leftCollision = true;
    bool rightCollision = true;
    bool upCollision = true;
    bool downCollision = true;
    bool isSticky = false;
    bool isTransparent = false;

    virtual sf::RectangleShape &getBody() = 0;

    virtual void collisionActionUp() = 0;

    virtual void collisionActionDown() = 0;

    virtual void collisionActionLeft() = 0;

    virtual void collisionActionRight() = 0;

    virtual void draw(sf::RenderWindow &window) = 0;

    CollisionObjects(bool left, bool right, bool up, bool down) : leftCollision(left), rightCollision(right),
                                                                  upCollision(up), downCollision(down) {}

    CollisionObjects() {}
};

class StickyFloor : public CollisionObjects {
public:
    sf::RectangleShape body;

    StickyFloor(sf::Vector2f pos, sf::Vector2f size) {
        body.setSize(size);
        body.setPosition(pos);
        body.setFillColor(sf::Color::Green);
        isSticky = true;
    }

    virtual sf::RectangleShape &getBody() {
        return body;
    }

    virtual void collisionActionUp() {

    }

    virtual void collisionActionDown() {

    }

    virtual void collisionActionLeft() {

    }

    virtual void collisionActionRight() {

    }

    virtual void draw(sf::RenderWindow &window) {
        window.draw(body);
    }
};

class HidedRoomDoor : public CollisionObjects{
public:
    sf::RectangleShape body;

    HidedRoomDoor(sf::Vector2f pos, sf::Vector2f size){
        body.setSize(size);
        body.setPosition(pos);
        body.setFillColor(sf::Color::Red);
        isTransparent = true;
    }

    virtual sf::RectangleShape &getBody() {
        return body;
    }

    virtual void collisionActionUp() {

    }

    virtual void collisionActionDown() {

    }

    virtual void collisionActionLeft() {

    }

    virtual void collisionActionRight() {

    }

    virtual void draw(sf::RenderWindow &window) {
        window.draw(body);
    }

};

class Floor : public CollisionObjects {
public:
    sf::RectangleShape body;

    Floor(sf::Vector2f pos, sf::Vector2f size) {
        body.setSize(size);
        body.setPosition(pos);
    }

    virtual sf::RectangleShape &getBody() {
        return body;
    }

    virtual void collisionActionUp() {

    }

    virtual void collisionActionDown() {

    }

    virtual void collisionActionLeft() {

    }

    virtual void collisionActionRight() {

    }

    virtual void draw(sf::RenderWindow &window) {
        window.draw(body);
    }
};

class Coin : public CollisionObjects {
public:
    sf::RectangleShape body;
    bool isBroken = false;


    Coin(sf::Vector2f pos, sf::Vector2f size) {
        body.setSize(size);
        body.setPosition(pos);
        body.setFillColor(sf::Color::Yellow);
    }

    virtual sf::RectangleShape &getBody() {
        return body;
    }

    virtual void collisionActionUp() {
        isBroken = true;
        leftCollision = false;
        rightCollision = false;
        upCollision = false;
        downCollision = false;
    }

    virtual void collisionActionDown() {
        isBroken = true;
        leftCollision = false;
        rightCollision = false;
        upCollision = false;
        downCollision = false;
    }

    virtual void collisionActionLeft() {
        isBroken = true;
        leftCollision = false;
        rightCollision = false;
        upCollision = false;
        downCollision = false;
    }

    virtual void collisionActionRight() {
        isBroken = true;
        leftCollision = false;
        rightCollision = false;
        upCollision = false;
        downCollision = false;
    }

    virtual void draw(sf::RenderWindow &window) {
        if (!isBroken) {
            window.draw(body);
        }
    }

};

class Tile : public CollisionObjects {
public:
    sf::RectangleShape body;
    bool isBroken = false;

    Tile(sf::Vector2f pos, sf::Vector2f size) {
        body.setSize(size);
        body.setPosition(pos);
    }

    virtual sf::RectangleShape &getBody() {
        return body;
    }

    virtual void collisionActionUp() {

    }

    virtual void collisionActionDown() {
        leftCollision = false;
        rightCollision = false;
        upCollision = false;
        downCollision = false;
        isBroken = true;
        cout << "bob" << endl;
    }

    virtual void collisionActionLeft() {

    }

    virtual void collisionActionRight() {

    }

    void draw(sf::RenderWindow &window) {
        if (!isBroken) {
            window.draw(body);
        }
    }
};

class Aktuator
{
public:
    bool isOn = false;
    sf::Sprite body;
    Aktuator(bool val) : isOn(val) {}
    Aktuator() {}
    virtual ~Aktuator() {}
    virtual void draw(sf::RenderWindow &window) = 0;
    virtual void activate() = 0;
};

class Door : public Aktuator
{
public:

    Door(bool val, sf::Vector2f pos)
    {
        isOn = val;
        body.setTexture(Assets::SpriteSheets::DoorClose);
        body.setPosition(pos);
        body.setScale(0.2f, 0.3f);
    }
    Door() {body.setTexture(Assets::SpriteSheets::DoorClose);}

    virtual void activate()
    {
        if (isOn)
            body.setTexture(Assets::SpriteSheets::DoorOpen);
        else
            body.setTexture(Assets::SpriteSheets::DoorClose);
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(body);
    }
};

class Platform : public Aktuator
{
public:
    Platform(bool val)
    {
        isOn = val;
    }
    Platform() {}

    virtual void activate()
    {
        cout << "magda" << endl;
    }

    void draw(sf::RenderWindow &window)
    {

    }
};

class Sensor
{
public:
    Aktuator *akt = NULL;
    sf::Sprite body;
    Sensor(Aktuator *ak) : akt(ak) {}
    ~Sensor()
    {
        delete akt;
    }

    void on()
    {
        akt->isOn = true;
    }

    void off()
    {
        akt->isOn = false;
    }

    bool status()
    {
        return akt->isOn;
    }

    virtual void activate() = 0;
    virtual void draw(sf::RenderWindow &window) = 0;
};

class Lever : public Sensor
{
public:

    Lever(Aktuator *ak, sf::Vector2f pos) : Sensor(ak)
    {
        if (akt->isOn)
            body.setTexture(Assets::SpriteSheets::LeverLeft);
        else
            body.setTexture(Assets::SpriteSheets::LeverRight);
        body.setPosition(pos);
        body.setScale(0.3f, 0.2f);
    }

    virtual void activate()
    {
        akt->isOn = !akt->isOn;
        if (akt->isOn)
            body.setTexture(Assets::SpriteSheets::LeverLeft);
        else
            body.setTexture(Assets::SpriteSheets::LeverRight);
        akt->activate();
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(body);
    }
};

class InterButton : public Sensor
{
public:
    InterButton(Aktuator *ak) : Sensor(ak) {}
    virtual void activate()
    {
        cout << "magda" << endl;
    }
    void draw(sf::RenderWindow &window)
    {

    }
};

class Level
{
public:
    vector<CollisionObjects*> floors;
    vector<Sensor*> sensory;
    vector<Enemy*> enemies;
    Player pl;

};

class LevelManager
{
public:
    vector<Level> levels;

    LevelManager()
    {
        //Level1
        levels.emplace_back(Level());
        levels[0].pl.body.setPosition(10.0f, 130.0f);

        // COIN 1
        levels[0].floors.emplace_back(new Coin({300, 100}, {40, 40}));   // COIN 1

        levels[0].floors.emplace_back(new Floor({0, 850}, {200, 50}));   // podłoga 1
        levels[0].floors.emplace_back(new Floor({200, 770}, {30, 130})); // przeszkoda 1
        levels[0].floors.emplace_back(new Floor({250, 540}, {20, 20}));  // przeszkoda 2 (kafelek)
        levels[0].floors.emplace_back(new Floor({20, 540}, {150, 20}));  // floor for lever

        levels[0].floors.emplace_back(new Floor({270, 700}, {30, 200})); // przeszkoda 3
        levels[0].floors.emplace_back(new Floor({340, 630}, {30, 270})); // przeszkoda 4
        levels[0].floors.emplace_back(new Floor({410, 500}, {30, 400})); // przeszkoda 5


        levels[0].floors.emplace_back(new Floor({550, 500}, {30, 100})); // przeszkoda 6
        levels[0].floors.emplace_back(new Floor({550, 700}, {30, 100})); // przeszkoda 7
        levels[0].floors.emplace_back(new Floor({550, 850}, {200, 50})); // podłoga 2


        levels[0].floors.emplace_back(new StickyFloor({950, 700}, {30, 100})); // Sticky Schody 1
        levels[0].floors.emplace_back(new StickyFloor({850, 550}, {30, 100})); // Sticky Schody 2
        levels[0].floors.emplace_back(new StickyFloor({950, 450}, {30, 100})); // Sticky Schody 3

        levels[0].floors.emplace_back(new Floor({0, 350}, {900, 50})); // podłoga 3 top

        // Kolizja od dołu kafelki //
        levels[0].floors.emplace_back(new Tile({600, 230}, {40, 40}));  // przeszkoda 3 (kafelek)
        levels[0].floors.emplace_back(new Tile({650, 230}, {40, 40}));  // przeszkoda 4 (kafelek)
        levels[0].floors.emplace_back(new Tile({700, 230}, {40, 40}));  // przeszkoda 5 (kafelek)


        levels[0].floors.emplace_back(new Floor({0, 0}, {30, 400})); // ściana 1 top

        levels[0].floors.emplace_back(new Floor({50, 250}, {20, 50})); // przeszkoda 8
        levels[0].floors.emplace_back(new Floor({200, 150}, {1330, 50})); // podłoga 4 top


        // WALL Door to Hided Room...
        levels[0].floors.emplace_back(new HidedRoomDoor({1100, 200}, {30, 600})); // ściana 2 top (room)


        levels[0].floors.emplace_back(new Floor({1500, 200}, {30, 600})); // ściana 3 top (room)

        levels[0].floors.emplace_back(new Floor({1080, 350}, {450, 50})); // podłoga 5 top
        levels[0].floors.emplace_back(new Floor({1600, 550}, {320, 50})); // podłoga 6 top
        levels[0].floors.emplace_back(new Floor({1100, 750}, {350, 50})); // podłoga 7
        levels[0].floors.emplace_back(new Floor({1600, 850}, {320, 50})); // podłoga 8

        levels[0].enemies.emplace_back(new Enemy({300, 290}, -150, 50));
        levels[0].enemies.emplace_back(new Enemy({800, 290}, -50, 50));


        levels[0].sensory.emplace_back(new Lever(new Door(false, {1100,270}), {20,515}));


    }
};

class Gra {
public:

    LevelManager levelManager;
    Level *currentLevel = NULL;
    //Animations animations;
    sf::Text text;


    Gra()// : animations("../source.png", 3, {3, 3, 3}, {114, 119}, {0, 0}, &pl)
    {
        text.setFont(Assets::Fonts::Arial);
        text.setFillColor(sf::Color::Red);
        text.setPosition(10, 10);
    }

    void setLevel(int nr)
    {
        if (nr >= 0 && nr < levelManager.levels.size())
        {
            deleteLevel();
            currentLevel = new Level(levelManager.levels[nr]);
            text.setString(to_string(currentLevel->pl.health));
        }
        else
            cout << "Level does not exist" << endl;
    }

    void deleteLevel()
    {
        if (currentLevel != NULL)
            delete currentLevel;
    }

    void collisions() {
        if (currentLevel != NULL)
        {
            int offsetY = 5;
            int offsetX = -10;
            bool isCollision = false;

            bool canCollide = false;
            if (!currentLevel->pl.delay) {
                if (currentLevel->pl.timer.getElapsedTime().asMilliseconds() > 100)
                    currentLevel->pl.delay = true;
            }




            // CHECK Collision Objects
            for (int i = 0; i <
                            (int) currentLevel->floors.size(); i++) {//podzielic kolizje na 4 strony obiektu i dla kazdego zrobic set pos na konkretny element

                currentLevel->floors[i]->getBody().setFillColor(sf::Color(255,255,255,255));
                if (currentLevel->pl.body.getGlobalBounds().intersects(currentLevel->floors[i]->getBody().getGlobalBounds())
                    //currentLevel->pl od góry (stoi)
                    &&
                    (currentLevel->pl.body.getPosition().y + currentLevel->pl.body.getGlobalBounds().height) <
                    currentLevel->floors[i]->getBody().getPosition().y + offsetY
                    &&
                    (currentLevel->pl.body.getPosition().y + currentLevel->pl.body.getGlobalBounds().height) >
                    currentLevel->floors[i]->getBody().getPosition().y - offsetY
                    &&
                    (currentLevel->pl.body.getPosition().x + currentLevel->pl.body.getGlobalBounds().width) >
                    currentLevel->floors[i]->getBody().getPosition().x + offsetX
                    && (currentLevel->pl.body.getPosition().x) <
                       currentLevel->floors[i]->getBody().getPosition().x + currentLevel->floors[i]->getBody().getSize().x - offsetX
                    && currentLevel->floors[i]->upCollision) {
                    isCollision = true;
                    currentLevel->pl.body.setPosition(currentLevel->pl.body.getPosition().x,
                                                      currentLevel->floors[i]->getBody().getPosition().y - currentLevel->pl.body.getGlobalBounds().height);
                    currentLevel->floors[i]->collisionActionUp();
                    //cout<<"gora"<<endl;
                    //przyklad przezroczystosci:
                    //currentLevel->floors[i]->getBody().setFillColor(sf::Color(0,0,0,122));

                    break;
                } else if (currentLevel->pl.body.getGlobalBounds().intersects(currentLevel->floors[i]->getBody().getGlobalBounds())
                           //od dołu (zatrzymuje się na spodzie i spada)
                           && (currentLevel->pl.body.getPosition().y) <
                              currentLevel->floors[i]->getBody().getPosition().y + currentLevel->floors[i]->getBody().getSize().y
                           && (currentLevel->pl.body.getPosition().y) >
                              currentLevel->floors[i]->getBody().getPosition().y + currentLevel->floors[i]->getBody().getSize().y - offsetY
                           && (currentLevel->pl.body.getPosition().x + currentLevel->pl.body.getGlobalBounds().width) >
                              currentLevel->floors[i]->getBody().getPosition().x + offsetX
                           && (currentLevel->pl.body.getPosition().x) <
                              currentLevel->floors[i]->getBody().getPosition().x + currentLevel->floors[i]->getBody().getSize().x - offsetX
                           && currentLevel->floors[i]->downCollision) {
                    //isCollision = true;
                    currentLevel->pl.body.setPosition(currentLevel->pl.body.getPosition().x,
                                                      currentLevel->floors[i]->getBody().getPosition().y + currentLevel->floors[i]->getBody().getSize().y);
                    currentLevel->pl.speedY = 0.2;
                    currentLevel->floors[i]->collisionActionDown();
                    //cout<<"dol"<<endl;
                    break;
                } else if (currentLevel->pl.body.getGlobalBounds().intersects(currentLevel->floors[i]->getBody().getGlobalBounds())
                           //od prawej (zatrzymuje się na ścianie i spada
                           && (currentLevel->pl.body.getPosition().y + currentLevel->pl.body.getGlobalBounds().height) >
                              currentLevel->floors[i]->getBody().getPosition().y
                           && (currentLevel->pl.body.getPosition().y) <
                              currentLevel->floors[i]->getBody().getPosition().y + currentLevel->floors[i]->getBody().getSize().y
                           &&
                           (currentLevel->pl.body.getPosition().x) <
                           currentLevel->floors[i]->getBody().getPosition().x + currentLevel->floors[i]->getBody().getSize().x - offsetX
                           &&
                           (currentLevel->pl.body.getPosition().x) >
                           currentLevel->floors[i]->getBody().getPosition().x + currentLevel->floors[i]->getBody().getSize().x + offsetX
                           && currentLevel->floors[i]->rightCollision) {
                    currentLevel->pl.body.setPosition(currentLevel->floors[i]->getBody().getPosition().x + currentLevel->floors[i]->getBody().getSize().x + 1,
                                                      currentLevel->pl.body.getPosition().y);
                    //cout<<"prawy"<<endl;
                    if (currentLevel->floors[i]->isSticky) isCollision = true;
                    if(currentLevel -> floors[i] -> isTransparent) isCollision = false;
                    currentLevel->floors[i]->collisionActionRight();
                    break;
                } else if (currentLevel->pl.body.getGlobalBounds().intersects(currentLevel->floors[i]->getBody().getGlobalBounds())
                           //od lewej (zatrzymuje się na ścianie i spada
                           && (currentLevel->pl.body.getPosition().y + currentLevel->pl.body.getGlobalBounds().height) >
                              currentLevel->floors[i]->getBody().getPosition().y
                           && (currentLevel->pl.body.getPosition().y) <
                              currentLevel->floors[i]->getBody().getPosition().y + currentLevel->floors[i]->getBody().getSize().y
                           && (currentLevel->pl.body.getPosition().x + currentLevel->pl.body.getGlobalBounds().width) <
                              currentLevel->floors[i]->getBody().getPosition().x - offsetX
                           && (currentLevel->pl.body.getPosition().x + currentLevel->pl.body.getGlobalBounds().width) >
                              currentLevel->floors[i]->getBody().getPosition().x + offsetX
                           && currentLevel->floors[i]->leftCollision) {
                    currentLevel->pl.body.setPosition(currentLevel->floors[i]->getBody().getPosition().x - currentLevel->pl.body.getGlobalBounds().width - 1,
                                                      currentLevel->pl.body.getPosition().y);
                    //cout<<"lewy"<<endl;
                    if (currentLevel->floors[i]->isSticky) isCollision = true;
                    if(currentLevel -> floors[i] -> isTransparent) isCollision = false;
                    currentLevel->floors[i]->collisionActionLeft();
                    break;
                }
            }


            // CHECK ENEMY COLLISION
            for (int i = 0; i < (int) currentLevel->enemies.size(); i++) {
                if (currentLevel->pl.body.getGlobalBounds().intersects(currentLevel->enemies[i]->getBody().getGlobalBounds())
                    //currentLevel->pl od góry (stoi)
                    &&
                    (currentLevel->pl.body.getPosition().y + currentLevel->pl.body.getGlobalBounds().height) <
                    currentLevel->enemies[i]->getBody().getPosition().y + offsetY
                    &&
                    (currentLevel->pl.body.getPosition().y + currentLevel->pl.body.getGlobalBounds().height) >
                    currentLevel->enemies[i]->getBody().getPosition().y - offsetY
                    &&
                    (currentLevel->pl.body.getPosition().x + currentLevel->pl.body.getGlobalBounds().width) >
                    currentLevel->enemies[i]->getBody().getPosition().x + offsetX
                    && (currentLevel->pl.body.getPosition().x) <
                       currentLevel->enemies[i]->getBody().getPosition().x + currentLevel->enemies[i]->getBody().getSize().x - offsetX
                        ) {
                    currentLevel->pl.body.setPosition(currentLevel->pl.body.getPosition().x,
                                                      currentLevel->enemies[i]->getBody().getPosition().y - currentLevel->pl.body.getGlobalBounds().height);
                    cout << "gora" << endl;
                    currentLevel->pl.speedY = -1;
                    currentLevel->enemies.erase(currentLevel->enemies.begin() + i);
                    i--;
                    break;
                } else if (currentLevel->pl.body.getGlobalBounds().intersects(currentLevel->enemies[i]->getBody().getGlobalBounds())
                           //od prawej (zatrzymuje się na ścianie i spada
                           && (currentLevel->pl.body.getPosition().y + currentLevel->pl.body.getGlobalBounds().height) >
                              currentLevel->enemies[i]->getBody().getPosition().y
                           && (currentLevel->pl.body.getPosition().y) <
                              currentLevel->enemies[i]->getBody().getPosition().y + currentLevel->enemies[i]->getBody().getSize().y
                           &&
                           (currentLevel->pl.body.getPosition().x) <
                           currentLevel->enemies[i]->getBody().getPosition().x + currentLevel->enemies[i]->getBody().getSize().x - offsetX
                           &&
                           (currentLevel->pl.body.getPosition().x) >
                           currentLevel->enemies[i]->getBody().getPosition().x + currentLevel->enemies[i]->getBody().getSize().x + offsetX
                        ) {
                    currentLevel->pl.body.setPosition(currentLevel->enemies[i]->getBody().getPosition().x + currentLevel->pl.body.getGlobalBounds().width + 35,
                                                      currentLevel->pl.body.getPosition().y);
                    std::cout << "Kontakt prawo" << endl;

                    currentLevel->pl.health -= 1;
                    text.setString(to_string(currentLevel->pl.health));


                    std::cout << currentLevel->pl.health << endl;
                    break;
                } else if (currentLevel->pl.body.getGlobalBounds().intersects(currentLevel->enemies[i]->getBody().getGlobalBounds())
                           //od lewej (zatrzymuje się na ścianie i spada
                           && (currentLevel->pl.body.getPosition().y + currentLevel->pl.body.getGlobalBounds().height) >
                              currentLevel->enemies[i]->getBody().getPosition().y
                           && (currentLevel->pl.body.getPosition().y) <
                              currentLevel->enemies[i]->getBody().getPosition().y + currentLevel->enemies[i]->getBody().getSize().y
                           && (currentLevel->pl.body.getPosition().x + currentLevel->pl.body.getGlobalBounds().width) <
                              currentLevel->enemies[i]->getBody().getPosition().x - offsetX
                           && (currentLevel->pl.body.getPosition().x + currentLevel->pl.body.getGlobalBounds().width) >
                              currentLevel->enemies[i]->getBody().getPosition().x + offsetX
                        ) {
                    currentLevel->pl.body.setPosition(currentLevel->enemies[i]->getBody().getPosition().x - currentLevel->pl.body.getGlobalBounds().width - 35,
                                                      currentLevel->pl.body.getPosition().y);
                    cout << "Kontakt lewo" << endl;

                    currentLevel->pl.health -= 1;
                    cout << currentLevel->pl.health << endl;
                    text.setString(to_string(currentLevel->pl.health));
                    break;
                }
            }


            if (isCollision && currentLevel->pl.delay) {
                currentLevel->pl.speedY = 0;
            } else {
                if (currentLevel->pl.speedY >= 1) currentLevel->pl.speedY = 1;
                else currentLevel->pl.speedY += 0.01;
                currentLevel->pl.delay = false;
            }
        }
    }

    void enemyMove() {
        if (currentLevel != NULL)
        {
            for (int i = 0; i < currentLevel->enemies.size(); i++) {
                if (!(currentLevel->enemies[i]->posMax == currentLevel->enemies[i]->posMin)) {
                    if (currentLevel->enemies[i]->kierunek) {
                        if (currentLevel->enemies[i]->body.getPosition().x + 1 >= currentLevel->enemies[i]->startingPos.x + currentLevel->enemies[i]->posMax) {
                            currentLevel->enemies[i]->kierunek = false;
                        } else
                            currentLevel->enemies[i]->body.move(0.1, 0);
                    } else {
                        if (currentLevel->enemies[i]->body.getPosition().x - 1 <= currentLevel->enemies[i]->startingPos.x + currentLevel->enemies[i]->posMin) {
                            currentLevel->enemies[i]->kierunek = true;
                        } else
                            currentLevel->enemies[i]->body.move(-0.1, 0);
                    }
                }
            }
        }
    }

    void gravity() {
        if (currentLevel != NULL)
        {
            collisions();
            currentLevel->pl.body.move(0, currentLevel->pl.speedY);
        }
    }

    void draw(sf::RenderWindow &window) {
        if (currentLevel != NULL)
        {
            for (int i = 0; i < (int) currentLevel->floors.size(); i++) {
                currentLevel->floors[i]->draw(window);
            }
            for (int i = 0; i < (int) currentLevel->enemies.size(); i++) {
                currentLevel->enemies[i]->draw(window);
            }
            for(int i = 0; i < (int)currentLevel->sensory.size(); i++)
            {
                currentLevel->sensory[i]->draw(window);
                currentLevel->sensory[i]->akt->draw(window);
            }
            window.draw(currentLevel->pl.body);
            window.draw(text);
        }
    }
};

class Button {
public:
    sf::RectangleShape body;
    sf::Text text;
    string textstring;

    Button(sf::Vector2f size, sf::Vector2f pos, string textStr) {
        body.setSize(size);
        body.setPosition(pos);

        text.setFont(Assets::Fonts::Arial);
        text.setCharacterSize(30);
        text.setString(textStr);
        //        textstring = textStr;
        text.setFillColor(sf::Color::Black);
        text.setPosition(pos.x + body.getSize().x / 2 - text.getGlobalBounds().width / 2,
                         pos.y + body.getSize().y / 2 - text.getGlobalBounds().height / 2 - 10);
    }


    Button() {
        body.setSize({150, 75});
        body.setPosition(0, 0);

        text.setFont(Assets::Fonts::Arial);
        text.setCharacterSize(30);
        text.setString("Button");
        textstring = "Button";
        text.setFillColor(sf::Color::Black);
        text.setPosition(body.getSize().x / 2 - text.getGlobalBounds().width / 2,
                         body.getSize().y / 2 - text.getGlobalBounds().height / 2 - 10);
    }


    bool isHover(sf::RenderWindow &window) {
        if (body.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
            return true;
        else return false;
    }


    void drawButton(sf::RenderWindow &window) {
        window.draw(body);
        window.draw(text);
    }
};

class Screen {
public:
    bool active = false;
};

class StartScreen : public Screen {
public:
    Button button1;
    Button button2;
    Button button3;
    Button button4;

    StartScreen() : button1({350, 125}, {550, 200}, "Wznow"),
                    button2({350, 125}, {950, 200}, "Nowa gra"),
                    button3({350, 125}, {550, 350}, "Opcje"),
                    button4({350, 125}, {950, 350}, "Wyjdz") {
        active = true;
    }

    void drawScreen(sf::RenderWindow &window) {
        cout << button1.isHover(window) << endl;
        button1.drawButton(window);
        button2.drawButton(window);
        button3.drawButton(window);
        button4.drawButton(window);
    }
};

class MainScreen : public Screen {
public:
    Gra gra;

    MainScreen() {
        //        active = true;
    }

    void drawScreen(sf::RenderWindow &window) {
        gra.draw(window);
    }
};

class EndScreen : public Screen {
public:

    EndScreen() {

    }

    void drawScreen(sf::RenderWindow &window) {

    }
};

class OptionScreen : public Screen{
public:
    Button button1;

    OptionScreen() : button1({350, 125}, {550, 200}, "Opcja 1: ") {

    }

    void drawScreen(sf::RenderWindow &window) {
        button1.drawButton(window);
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "My Window");
    window.setFramerateLimit(120);
    StartScreen start;
    MainScreen main;
    EndScreen end;
    OptionScreen options;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (start.active) {
                if (event.type == sf::Event::MouseButtonPressed && start.button1.isHover(window)) {
                    start.active = false;
                    main.active = true;
                }
                if (event.type == sf::Event::MouseButtonPressed && start.button4.isHover(window)) {
                    window.close();
                }
                if (event.type == sf::Event::MouseButtonPressed && start.button3.isHover(window)) {
                    options.active = true;
                    start.active = false;
                }
                if (event.type == sf::Event::MouseButtonPressed && start.button2.isHover(window)) {
                    main.gra.setLevel(0);
                    start.active = false;
                    main.active = true;
                }
            }
            else if (main.active)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                {
                    main.active = false;
                    start.active = true;
                }
                for(int i = 0; i < main.gra.currentLevel->sensory.size(); i++)
                {
                    if (main.gra.currentLevel->sensory[i]->body.getGlobalBounds().intersects(main.gra.currentLevel->pl.body.getGlobalBounds()) && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::R)
                    {
                        main.gra.currentLevel->sensory[i]->activate();
                    }
                }
            }
            else if (end.active) {
                if (event.type == sf::Event::MouseButtonPressed)
                {
                    end.active = false;
                    start.active = true;
                }
            }
            else if (options.active) {
                if (event.type == sf::Event::MouseButtonPressed)
                {
                    options.active = false;
                    start.active = true;
                }
            }
        }
        if (main.gra.currentLevel != NULL)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                main.gra.currentLevel->pl.body.move(-1, 0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                main.gra.currentLevel->pl.body.move(1, 0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                main.gra.currentLevel->pl.timer.restart();
                if (main.gra.currentLevel->pl.speedY == 0 || main.gra.currentLevel->pl.speedY == 0.01f) main.gra.currentLevel->pl.speedY = -1.4;
                main.gra.currentLevel->pl.delay = false;
            }

            main.gra.gravity();
            main.gra.enemyMove();
        }




        window.clear(sf::Color::Black);
        if (start.active) start.drawScreen(window);
        else if (main.active) main.drawScreen(window);
        else if (end.active) end.drawScreen(window);
        else if (options.active) options.drawScreen(window);
        window.display();
    }

    return 0;
}


























int main3()
{
    vector<Sensor*> sensory;
//    sensory.emplace_back(new Lever(new Door(false)));
//    sensory.emplace_back(new InterButton(new Door(false)));

    for(int i = 0; i < (int)sensory.size(); i++)
    {
        sensory[i]->activate();
//        sensory[i]->draw(window);
//        sensory[i]->akt->draw(window);
    }



    return 0;
}

//int main1()
//{
//    sf::RenderWindow window(sf::VideoMode(1920, 1080), "My Window");
//    window.setFramerateLimit(120);
//    Gra gra;
//    sf::Sprite man;
//    man.setTexture(*gra.animations.animations[0].textures[0]);
//    while (window.isOpen())
//    {
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            if(event.type == sf::Event::Closed)
//            {
//                window.close();
//            }
//        }
//        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
//            gra.pl.body.move(-1,0);
//        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
//            gra.pl.body.move(1,0);
//        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
//        {
//            gra.pl.timer.restart();
//            if (gra.pl.speedY == 0 || gra.pl.speedY == 0.01f) gra.pl.speedY = -2;
//            gra.pl.delay = false;
//        }
//        gra.gravity();
//        window.clear(sf::Color::Black);
//        for(int i = 0; i < gra.floors.size(); i++)
//        {
//            window.draw(gra.floors[i].body);
//        }
//        window.draw(gra.pl.body);
//        window.draw(man);
//        window.display();
//    }

//    return 0;
//}
