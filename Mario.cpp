#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <valarray>
#include "Mario.h"
#include "Map.h"

// MAP
Map map;

Mario::Mario(sf::Texture *texture, sf::Vector2u imageCount, float switchTime, float speed, float gravity, float jumpHeight) :
        animation(texture, imageCount, switchTime)
{


    this->speed = speed;
    this->jumpHeight = jumpHeight;
    this->gravity = gravity;
    faceRight = true;
    row = 0;
    body.setSize(sf::Vector2f(80.0f, 120.0f));
    body.setPosition(30.0f, 530.0f);
    body.setTexture(texture);
}

Mario::~Mario() {

}

void Mario::Update(float deltaTime) {

    velocity.x = 0.0f;


    // RUCH LEWO - PRAWO
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        velocity.x -= speed;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        velocity.x += speed;



    // SKAKANIE, COŚ NIE DZIAŁA...
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && canJump){
        canJump = false;
        std::cout << "Tuu ";
        velocity.y = -sqrt(2.0f * 981.0f * jumpHeight);
    }
    velocity.y += 981.0f * deltaTime;





    // Gravity Logic
    if(body.getPosition().y <= map.floorHeight && canJump == false){
        body.move({0, gravity});
        canJump = false;
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




    // Collision screen left side.
    if(body.getPosition().x < 0)
        body.setPosition(0, body.getPosition().y);


//     check the floor
    if(body.getPosition().y > map.floorHeight){
        body.setPosition(body.getPosition().x, 820 - body.getSize().y);
        canJump = true;
        std::cout << "bottom_true";
    }



    animation.Update(row, deltaTime, faceRight);
    body.setTextureRect(animation.uvRect);
    body.move(velocity * deltaTime);



}

void Mario::Draw(sf::RenderWindow& window) {
    window.draw(body);
}