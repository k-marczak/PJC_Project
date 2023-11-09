#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include "Mario.h"

Mario::Mario(sf::Texture *texture, sf::Vector2u imageCount, float switchTime, float speed, float gravity) :
        animation(texture, imageCount, switchTime)
{

    this->speed = speed;

    this->gravity = gravity;
    faceRight = true;
    body.setSize(sf::Vector2f(80.0f, 120.0f));
    body.setPosition(30.0f, 30.0f);
    body.setTexture(texture);
    bool canJump = false;
}

Mario::~Mario() {

}

void Mario::Update(float deltaTime) {
    sf::Vector2f velocity(0.0f, 0.0f);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        velocity.x -= speed * deltaTime;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        velocity.x += speed * deltaTime;



    // SKAKANIE, COŚ NIE DZIAŁA...

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && canJump == true){
        std::cout << "Naciskasz!";
        velocity.y = -150;


        canJump = false;

    }




    // Gravity Logic
    if(body.getPosition().y <= 700 && canJump == false){
        body.move({0, gravity});
        canJump = false;
    }

    if(body.getPosition().y == 700){
        canJump = true;
    }


    // Animation change
    if(velocity.x == 0.0f){
        row = 0;
    }
    else{
        row =1;
        if(velocity.x > 0.0f)
            faceRight = true;
        else
            faceRight = false;
    }






    animation.Update(row, deltaTime, faceRight);
    body.setTextureRect(animation.uvRect);
    body.move(velocity);






    // Collision screen left side.
    if(body.getPosition().x < 0)
        body.setPosition(0, body.getPosition().y);

    // check the floor
    if(body.getPosition().y > 700){
        body.setPosition(body.getPosition().x, 820 - body.getSize().y);
        canJump = true;
        std::cout << "bottom_true";
    }


}

void Mario::Draw(sf::RenderWindow& window) {
    window.draw(body);
}


