#pragma once
#include <SFML/Graphics.hpp>

class Animation
{
    //Métodos e variáveis públicas:
  public:
    Animation(sf::Texture *texture, sf::Vector2u imageCount, float switchTime);
    ~Animation();

    void Update(int row, float deltaTime, bool faceRight);

  public:
    sf::IntRect uvRect;

    //Métodos e variáveis privadas:
  private:
    sf::Vector2u imageCount;
    sf::Vector2u currentImage;

  private:
    float totalTime;  //tempo total de uma animação em questão
    float switchTime; //tempo de cada parte de uma animação
};