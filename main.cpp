#include <SFML/Graphics.hpp>
#include "Character.h"

int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    sf::Texture characterTexture;
    characterTexture.loadFromFile("../marioFrames.png");

    Character character(&characterTexture, sf::Vector2u(5, 2), 0.07f, 300.0f);

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
        }

        character.Update(deltaTime);


        window.clear();
        character.Draw(window);
        window.display();
    }

    return 0;
}