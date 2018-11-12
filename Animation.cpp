#include <iostream>
#include "Animation.h"

Animation::Animation(sf::Texture *texture, sf::Vector2u imageCount, float switchTime)
{
    this->imageCount = imageCount; //numero total de sprites em uma textura
    this->switchTime = switchTime;
    totalTime = 0.0f;
    currentImage.x = 0;

    //Dimensões de cada sprite individual (tamanho da textura/numero de sprites)
    uvRect.width = texture->getSize().x / float(imageCount.x);
    uvRect.height = texture->getSize().y / float(imageCount.y) - 15;
}

Animation::~Animation()
{
}

void Animation::Update(int row, float deltaTime, bool faceRight)
{
    currentImage.y = row;
    totalTime += deltaTime;

    if (totalTime >= switchTime)
    {
        totalTime -= switchTime;
        currentImage.x++;

        if (currentImage.x >= imageCount.x) //Se chegarmos no final da linha
        {
            currentImage.x = 0;
        }
    }

    uvRect.top = currentImage.y * (uvRect.height + 15) + 15;

    if (faceRight)
    {
        uvRect.left = currentImage.x * uvRect.width;
        uvRect.width = abs(uvRect.width);
    }
    else
    {
        uvRect.left = (currentImage.x) * abs(uvRect.width) + abs(uvRect.width);
        uvRect.width = -abs(uvRect.width);
    }
}
