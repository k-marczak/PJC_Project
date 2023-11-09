

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Animation.h"

class Character {

public:
    Character(sf::Texture* texture, sf::Vector2u imageCount, float switchTime, float speed);
    ~Character();

    void Update(float deltaTime);
    void Draw(sf::RenderWindow& window);

private:
    sf::RectangleShape body;
    Animation animation;
    unsigned int row;
    float speed;
    bool faceRight;

};

