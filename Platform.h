#include <SFML/Graphics.hpp>
#include "Collider.h"

#ifndef PLATFORM_H
#define PLATFORM_H

class Platform
{
  public:
    Platform(sf::Texture *texture, sf::Vector2f size, sf::Vector2f position);
    ~Platform();

    void Draw(sf::RenderWindow &window);
    Collider GetCollider() { return Collider(body); }
	sf::RectangleShape getBody(void) { return body; }

  private:
    sf::RectangleShape body;
};

#endif