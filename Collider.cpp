#include <iostream>
#include "Collider.h"

Collider::Collider(sf::RectangleShape &body) : //Utiliza-se uma lista de inicialização*
                                               body(body)
{
}

Collider::~Collider()
{
}

bool Collider::CheckCollision(Collider &otherElement, float push)
{
    sf::Vector2f otherElementPosition = otherElement.GetElementPosition();
    sf::Vector2f otherElementHalfSize = otherElement.GetElementHalfSize();

    sf::Vector2f thisElementPosition = GetElementPosition();
    sf::Vector2f thisElementHalfSize = GetElementHalfSize();

    float deltaX = otherElementPosition.x - thisElementPosition.x;
    float deltaY = otherElementPosition.y - thisElementPosition.y;

    //De acordo com os cálculos do documento:
    float intersectX = abs(deltaX) - (otherElementHalfSize.x + thisElementHalfSize.x);
    float intersectY = abs(deltaY) - (otherElementHalfSize.y + thisElementHalfSize.y);

    if (intersectX < 0.0f && intersectY < 0.0f)
    {

        push = std::min(std::max(push, 0.0f), 1.0f); //Clamping
        if (abs(intersectX) < abs(intersectY))
        {
            if (deltaX > 0.0f)
            {
                MoveElement(intersectX * (1.0f - push), 0.0f);
                otherElement.MoveElement(-intersectX * push, 0.0f);
            }
            else
            {
                MoveElement(-intersectX * (1.0f - push), 0.0f);
                otherElement.MoveElement(intersectX * push, 0.0f);
            }
        }
        else
        {
            if (deltaY > 0.0f)
            {
                MoveElement(0.0f, intersectY * (1.0f - push));
                otherElement.MoveElement(0.0f, -intersectY * push);
            }
            else
            {
                MoveElement(0.0f, -intersectY * (1.0f - push));
                otherElement.MoveElement(0.0f, intersectY * push);
            }
        }
        return true;
    }

    return false; // Deve retornar falso ao final, por que o final da interação é quando não tem contato
}