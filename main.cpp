#include <SFML/Graphics.hpp>
#include "Mario.h"

auto checkPath(){

}

int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(1200, 900), "My window");
    sf::Texture marioTexture;

    std::string pathName = "../zzhuj1.png";


    marioTexture.loadFromFile(pathName);



    Mario mario(&marioTexture, sf::Vector2u(3, 3), 0.07f, 300.0f, 0.3);


    float deltaTime = 0.0f;
    sf::Clock clock;

    // run the program as long as the window is open
    while (window.isOpen())
    {
        deltaTime = clock.restart().asSeconds();
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type = sf::Event::KeyReleased){
                mario.isJumping = false;
            }

        }

        mario.Update(deltaTime);


        window.clear();
        mario.Draw(window);
        window.display();
    }

    return 0;
}