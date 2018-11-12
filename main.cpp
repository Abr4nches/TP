#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Player.h"
#include "Platform.h"

using namespace std;

static const float VIEW_HEIGHT = 1024.0f;
static const float VIEW_WIDTH = 1024.0f;

static const string SUB = "batata";

void ResizeView(const sf::RenderWindow &window, sf::View &view) // Por que const?
{
    //a razão de aspecto  = largura/altura
    float aspectRatio = float(window.getSize().x) / float(window.getSize().y); // O cast é necessário pq getSize() retorna um sf::Vector2u
    view.setSize(VIEW_HEIGHT * aspectRatio, VIEW_WIDTH);
}

void posicionaLetras(sf::Vector2f lpos[], sf::Vector2u backgroundSize)
{
    cout << backgroundSize.x << ", " << backgroundSize.y << endl;
    int distx = backgroundSize.x * 2;
    int disty = backgroundSize.y * 2;
    for(int i = 0; i < 20; i++)
    {
        lpos[i].x = rand() % distx - distx * 0.5;
        lpos[i].y = rand() % disty - disty * 0.5;
    }
}

int main(int argc, char const *argv[])
{
    sf::RenderWindow window(sf::VideoMode(1024, 768), "TUX turma X", sf::Style::Close | sf::Style::Resize);
    sf::View view(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(VIEW_HEIGHT, VIEW_WIDTH));

    sf::Texture playerTexture;
    playerTexture.loadFromFile("tux_from_linux.png");

    sf::Texture backgroundTexture;
    backgroundTexture.loadFromFile("background.png");
/*
    sf::Texture platformTexture;
    platformTexture.loadFromFile("wall.png");
*/

	// Adquirindo palavra chave
	ifstream f;
	string s;
	f.open("palavra-chave.txt");
	if (f.is_open())
		getline(f, s);
	else
		s = SUB;
	f.close();

	int n = s.length();
	if (s.length() > 20)
		s = SUB;
	for (int i = 0; i < n; i++)
	{
		if (s[i] >= 'A' && s[i] <= 'Z')
			s[i] += 32;
		if (s[i] == ' ')
			s[i] = '_';
	}

	srand(time(NULL));
	char l[20];
	vector<char> v;
	for (int i = 0; i < 20; i++)
	{
		if (i < n)
			v.push_back(s[i]);
		else
			v.push_back(s[rand() % n]);
	}
	int r;
	for (int i = 0; i < 20; i++)
	{
		r = rand() % (20 - i);
		l[i] = v[r];
		v.erase(v.begin() + r);
	}

	string t;
	sf::Text d;
	sf::Font font;
	font.loadFromFile("arial.ttf");
	d.setFont(font);

	// Criando o fundo
	float scale = 10.0f;
	sf::Sprite background(backgroundTexture);
	background.setPosition(0, 0);
	background.setOrigin(700, 500);

	sf::Vector2f lpos[20];
	posicionaLetras(lpos, backgroundTexture.getSize());

    //Criando o tux
    Player player(&playerTexture, sf::Vector2u(3, 9), 0.3f, 500.0f, (char*) s.c_str());

    //Criando duas plataformas
/*
    Platform platform1(&platformTexture, sf::Vector2f(400.0f, 200.0f), sf::Vector2f(500.0f, 200.0f));
    Platform platform2(&platformTexture, sf::Vector2f(400.0f, 200.0f), sf::Vector2f(500.0f, -10.0f));
	player.addPlatform(platform1);
	player.addPlatform(platform2);
*/

    float x, y, w, deltaTime = 0.0f;
    sf::Clock clock;

	float ws = s.length() * 20;

    while (window.isOpen())
    {
        deltaTime = clock.restart().asSeconds();

        sf::Event evnt;
        while (window.pollEvent(evnt))
        {
            switch (evnt.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized:
                ResizeView(window, view); //Deve-se criar uma função pra isso por que n da pra criar variáveis dentro de um Switch
                break;
            }
        }

        player.MovimentoAutomatizado(deltaTime); // devemos setar a view depois de atualizar o sprite, por motivos de lag
/*
        // Atribuição necessária para consertar erro de ponteiros lvalue:
        Collider col1 = player.GetCollider();
        Collider col2 = platform1.GetCollider();
        Collider col3 = platform2.GetCollider();
        // Checa-se a colisão entre cada plataforma e o jogador e, em seguida, entre ambas
        platform1.GetCollider().CheckCollision(col1, 0.5f);
        platform1.GetCollider().CheckCollision(col3, 0.0f);
        platform2.GetCollider().CheckCollision(col1, 1.0f);
        platform2.GetCollider().CheckCollision(col2, 0.0f);
*/

        // Ajustar escala do fundo
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp))
            scale += 0.01f;
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown))
            scale -= 0.01f;
        if(scale < 1)
            scale = 1;
        background.setScale(scale, scale);

		// Limpar palavra atual
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !player.verificaSePalavraVazia())
		{
			t = player.limpaPalavra();
			for (int i = 0; i < 20; i++)
			{
				if (l[i] == ' ')
				{
					l[i] = t[0];
					t = t.substr(1);
				}
			}
		}

		// Colisão com as letras
		x = player.ObtemPosicaoX();
		y = player.ObtemPosicaoY();
		for (int i = 0; i < 20; i++)
		{
			// Distancia entre o personagem e cada letra
			float dist = (x-lpos[i].x)*(x-lpos[i].x) + (y-lpos[i].y)*(y-lpos[i].y);
			//if (pow(x - (-170 + 80 * (i % 10)), 2) + pow(y - (i > 9 ? 400 : 0), 2) < 800)
			if(dist < 800)
			{
				if (l[i] != ' ')
					player.incrementaPalavra(l[i]);
				l[i] = ' ';
			}
		}

        view.setCenter(player.GetViewPosition());

        window.clear(sf::Color(204, 204, 255));
        window.setView(view);
        window.draw(background);

        player.Draw(window);
/*
        platform1.Draw(window);
        platform2.Draw(window);
*/
		sf::RectangleShape textShape(sf::Vector2f(50, 50));
		textShape.setFillColor(sf::Color::White);
		textShape.setOutlineThickness(2);
		textShape.setOutlineColor(sf::Color::Black);
		textShape.setOrigin(20, 0);
		d.setFillColor(sf::Color::Black);

		for (int i = 0; i < 20; i++)
		{
			d.setString(l[i]);
			//d.setPosition(-180 + 80 * (i % 10), i > 9 ? 400 : 0);
			d.setPosition(lpos[i].x, lpos[i].y);
			if(l[i] != ' ')
                window.draw(textShape, d.getTransform());
			window.draw(d);
		}
		t = player.adquirePalavra();
		w = t.length() * 20;

		sf::RectangleShape guessShape(sf::Vector2f(500, 50));
        guessShape.setFillColor(sf::Color::White);
        guessShape.setOutlineThickness(2);
        guessShape.setOrigin(250, 0);
        guessShape.setPosition(x, y + window.getSize().y * 0.44);
		if (player.verificaPalavra((char*) s.c_str()))
        {
            guessShape.setOutlineColor(sf::Color::Green);
            d.setFillColor(sf::Color::Green);
        }
		else
        {
            guessShape.setOutlineColor(sf::Color::Red);
            d.setFillColor(sf::Color::Red);
        }
        window.draw(guessShape);

		for (int i = 0; i < t.length(); i++)
		{
			d.setString(t[i]);
			d.setPosition(x - w / 2 + 20 * i, y + 340);
			window.draw(d);
		}
		d.setFillColor(sf::Color::Blue);

		sf::RectangleShape respostaShape(sf::Vector2f(1000, 50));
        respostaShape.setFillColor(sf::Color::White);
        respostaShape.setOutlineThickness(2);
        respostaShape.setOutlineColor(sf::Color::Blue);
        respostaShape.setOrigin(500, 0);
        respostaShape.setPosition(x, y - window.getSize().y * 0.6);
        window.draw(respostaShape);
		for (int i = 0; i < n; i++)
		{
			d.setString(s[i]);
			d.setPosition(x - ws / 2 + 20 * i, y - 460);
			window.draw(d);
		}
		player.atualizaVetor();
		d.setFillColor(sf::Color::White);

        sf::RectangleShape invenShape(sf::Vector2f(1000, 50));
        invenShape.setFillColor(sf::Color::Black);
        invenShape.setOutlineThickness(2);
        invenShape.setOutlineColor(sf::Color::White);
        invenShape.setOrigin(500, 0);
        invenShape.setPosition(x, y + window.getSize().y * 0.52);
        window.draw(invenShape);
		for (int i = 0; player.retornaCaractere(i) != '\0'; i++)
		{
			stringstream text;
			text << player.retornaCaractere(i);
			text << ":" << player.retornaElementoVetor(i);
			d.setString(text.str());
			d.setPosition(x - 480 + 100 * (i % 10), y + 400 + 50 * (i / 10));
			window.draw(d);
		}

        window.display();
    }

    return 0;
}
