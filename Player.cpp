#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include "Player.h"

using namespace std;

Player::Player(sf::Texture *texture, sf::Vector2u imageCount, float switchTime, float speed, char chave[]) : animation(texture, imageCount, switchTime)
{
	this->speed = speed;
	this->palavra[0] = '\0';
	for (int i = 0; i < 20; i++)
		this->vetor[i] = 0;
	criaListaCaracteres(chave);
	row = 0;
	faceRight = true;
	body.setSize(sf::Vector2f(100.0f, 150.0f));
	body.setOrigin(body.getSize() / 2.0f); //Definindo que a origem do Tux é o seu centro
	body.setPosition(0.0f, 0.0f);
	body.setTexture(texture);
}

Player::~Player()
{
}

sf::Keyboard::Key Player::DefineTecla(char tecla)
{
	int key = tecla - ('a' - 'A') * (tecla >= 'a') - 'A';
	sf::Keyboard::Key sfkey = (sf::Keyboard::Key)key;
	return sfkey;
}

bool Player::VerificaTecla(char tecla)
{
	if (sf::Keyboard::isKeyPressed(DefineTecla(tecla)))
		return true;

	return false;
}

bool Player::AtualizaSprite(float x, float y, float tempo)
{
	int direcao = InputHorizontal();
	if (direcao == 0)
	{
		row = 0;
	}
	else
	{
		row = 1;

		if (direcao > 0)
			faceRight = true;
		else
			faceRight = false;
	}

	animation.Update(row, tempo, faceRight);
	body.setTextureRect(animation.uvRect);
	body.setPosition(x, y);

	return faceRight;
}

int Player::InputHorizontal()
{
	if (VerificaTecla('A'))
		return -1;
	else if (VerificaTecla('D'))
		return 1;
	else
		return 0;
}

int Player::InputVertical()
{
	if (VerificaTecla('W'))
		return -1;
	else if (VerificaTecla('S'))
		return 1;
	else
		return 0;
}

float Player::ObtemPosicaoX()
{
	return body.getPosition().x;
}

float Player::ObtemPosicaoY()
{
	return body.getPosition().y;
}

void Player::MovimentoAutomatizado(float tempo)
{

	float x, y, dx, dy;
	bool move = true;

	x = ObtemPosicaoX();
	y = ObtemPosicaoY();
	dx = speed * tempo * InputHorizontal();
	dy = speed * tempo * InputVertical();

	int i, n = numeroDePlataformas();
	for (i = 0; i < n; i++)
	{
		if (verificaColisao(dx, dy, i))
		{
			move = false;
			break;
		}
	}

	if (move)
	{
		x += dx;
		y += dy;
	}

	AtualizaSprite(x, y, tempo);
}

void Player::Update(float deltaTime)
{
	//Se faz necessária uma variável pro movimento, sendo que deseja-se mudar a animação de acordo com a direção do mesmo
	sf::Vector2f movement(0.0f, 0.0f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		movement.x += speed * deltaTime * InputHorizontal();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		movement.x += speed * deltaTime * InputHorizontal();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		movement.y += speed * deltaTime * InputVertical();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		movement.y += speed * deltaTime * InputVertical();

	//AtualizaSprite(movement);

	animation.Update(row, deltaTime, faceRight);
	body.setTextureRect(animation.uvRect);
	body.move(movement);
}

void Player::Draw(sf::RenderWindow &window)
{
	window.draw(body);
}

void Player::adquirirDadosPersonagem(float &x, float &y, float &w, float &h)
{
	x = this->body.getPosition().x;
	y = this->body.getPosition().y;
	w = this->body.getSize().x;
	h = this->body.getSize().y;
}

void Player::adquirirDadosPlataforma(float &x, float &y, float &w, float &h, int i)
{
	if (i < 0 || i >= this->plataformas.size())
		return;
	x = this->plataformas[i].getBody().getPosition().x;
	y = this->plataformas[i].getBody().getPosition().y;
	w = this->plataformas[i].getBody().getSize().x;
	h = this->plataformas[i].getBody().getSize().y;
}

bool Player::verificaColisao(float dx, float dy, int i)
{
	float x1, x2, y1, y2, w1, w2, h1, h2;
	adquirirDadosPersonagem(x1, y1, w1, h1);
	adquirirDadosPlataforma(x2, y2, w2, h2, i);
	x1 += dx;
	y1 += dy;
	if (x1 + w1 / 2 > x2 - w2 / 2 && x1 - w1 / 2 < x2 + w2 / 2)
		if (y1 + h1 / 2 > y2 - h2 / 2 && y1 - h1 / 2 < y2 + h2 / 2)
			return true;
	return false;
}

void Player::incrementaPalavra(char c)
{
	int i = 0;
	while (palavra[i] != '\0')
		i++;
	palavra[i++] = c;
	palavra[i] = '\0';
}

string Player::adquirePalavra()
{
	string s;
	for (int i = 0; palavra[i] != '\0'; i++)
		s += palavra[i];
	return s;
}

string Player::limpaPalavra()
{
	string s = adquirePalavra();
	palavra[0] = '\0';
	for (int i = 0; i < 20; i++)
		vetor[i] = 0;
	return s;
}

void Player::ObtemListaCaracteres(char caracteres[])
{
    memcpy(caracteres, this->caracteres, 20);
}

void Player::SalvaVetor(int vetor[])
{
    memcpy(this->vetor, vetor, 20*sizeof(int));
}

void Player::ObtemPalavraJogador(char palavra[])
{
    memcpy(palavra, this->palavra, 20);
}

void Player::criaListaCaracteres(char chave[])
{
	int ascii[256];
	int j = 0;
	for (int i = 0; i < 256; i++)
		ascii[i] = 0;
	for (int i = 0; chave[i] != '\0'; i++)
		ascii[chave[i]]++;
	for (int i = 0; i < 256; i++)
		if (ascii[i] > 0)
			caracteres[j++] = i;
	caracteres[j] = '\0';
}

// Exercicio
int Player::id(char c)
{
    char caracteres[20];
    ObtemListaCaracteres(caracteres);

    /// COLOQUE SEU CÓDIGO ABAIXO
    /// Você pode mudar o return se achar necessário

	return -1;
}

// Exercicio
void Player::atualizaVetor()
{
    char palavra[20];
    ObtemPalavraJogador(palavra);
    int vetor[20];

    /// COLOQUE SEU CÓDIGO ABAIXO



    /// A função SalvaVetor repassa os valores calculados no seu código acima
    /// para o parte do programa que cuida da atualização dos elementos da tela.
    /// Você não se precisa se preocupar em modificá-la.
    SalvaVetor(vetor);
}

// Exercicio
int Player::verificaPalavra(char chave[])
{
    char palavra[20];
    ObtemPalavraJogador(palavra);

    /// COLOQUE SEU CÓDIGO ABAIXO
    /// Você pode mudar o return se achar necessário

	return 0;
}
