#pragma once
#include <SFML/Graphics.hpp>

class Collider
{
  public:
    Collider(sf::RectangleShape &body);
    ~Collider();
    bool CheckCollision(Collider &otherElement, float push);
    /*
            Push define o quanto um objeto pode ser empurrado. Se for 0,
            O player não conseguirá o mover.
        */
    sf::Vector2f GetElementPosition() { return body.getPosition(); };
    sf::Vector2f GetElementHalfSize() { return body.getSize() / 2.0f; };
    void MoveElement(float dx, float dy) { body.move(dx, dy); };

  private:
    sf::RectangleShape &body;
};