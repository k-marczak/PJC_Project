

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Animation.h"

class Mario {

public:
    Mario(sf::Texture* texture, sf::Vector2u imageCount, float switchTime, float speed, float gravity);
    ~Mario();

    void Update(float deltaTime);
    void Draw(sf::RenderWindow& window);
    bool isJumping = false;

private:
    sf::RectangleShape body;
    Animation animation;
    unsigned int row;
    float speed;
    float gravity;
    bool faceRight;


};

