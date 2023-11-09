//
// Created by k_mar on 09.11.2023.
//

#include <SFML/Window/Keyboard.hpp>
#include "Character.h"

Character::Character(sf::Texture *texture, sf::Vector2u imageCount, float switchTime, float speed) :
        animation(texture, imageCount, switchTime)
{
    this->speed = speed;
    row = 0;
    faceRight = true;

    body.setSize(sf::Vector2f(100.0f, 150.0f));
    body.setPosition(206.0f, 206.0f);
    body.setTexture(texture);

}

Character::~Character() {

}

void Character::Update(float deltaTime) {
    sf::Vector2f movement(0.0f, 0.0f);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        movement.x -= speed * deltaTime;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        movement.x += speed * deltaTime;

    if(movement.x == 0.0f){
        row = 0;
    }
    else
    {
        row =1;
        if(movement.x > 0.0f)
            faceRight = true;
        else
            faceRight = false;
    }

    animation.Update(row, deltaTime, faceRight);
    body.setTextureRect(animation.uvRect);
    body.move(movement);
}

void Character::Draw(sf::RenderWindow& window) {
    window.draw(body);
}

