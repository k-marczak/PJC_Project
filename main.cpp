#include <SFML/Graphics.hpp>
#include "Animation.h"

int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    sf::RectangleShape player(sf::Vector2f(100.0f, 150.0f));
    player.setPosition(206.0f, 206.0f);
    sf::Texture playerTexture;
    playerTexture.loadFromFile("../marioFrames.png");
    player.setTexture(&playerTexture);

    Animation animation(&playerTexture, sf::Vector2u(5, 2), 0.07f);

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

        animation.Update(0, deltaTime);
        player.setTextureRect(animation.uvRect);

        window.clear();
        window.draw(player);
        window.display();
    }

    return 0;
}