#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Animation.h"
#include "Collider.h"
#include "Platform.h"

using namespace std;

class Player
{
  public:
    Player(sf::Texture *texture, sf::Vector2u imageCount, float switchTime, float speed, char chave[]);
    ~Player();

    void Update(float deltaTime);
    void Draw(sf::RenderWindow &window);
    //NOVAS:
    void MovimentoAutomatizado(float tempo);
    float ObtemPosicaoX();
    float ObtemPosicaoY();
    int InputHorizontal();
    int InputVertical();
    bool AtualizaSprite(float x, float y, float tempo);
    sf::Keyboard::Key DefineTecla(char tecla);
    bool VerificaTecla(char tecla);
    //Funções relativas ao uso de views:
    sf::Vector2f GetViewPosition() { return body.getPosition(); }
    //Funções relativas à colisão:
    Collider GetCollider() { return Collider(body); }
	void adquirirDadosPersonagem(float &x, float &y, float &w, float &h);
	void adquirirDadosPlataforma(float &x, float &y, float &w, float &h, int i);
	void addPlatform(Platform &platform) { this->plataformas.push_back(platform); }
	int numeroDePlataformas(void) { return this->plataformas.size(); }
	bool verificaColisao(float dx, float dy, int i);
	int retornaElementoVetor(int i) { return (char) this->vetor[i]; }
	char retornaCaractere(int i) { return this->caracteres[i]; }
	void incrementaPalavra(char c);
	string adquirePalavra();
	string limpaPalavra();
	bool verificaSePalavraVazia() { return this->palavra[0] == '\0'; }
	int id(char c);
	void atualizaVetor();
	void criaListaCaracteres(char chave[]);
	int verificaPalavra(char chave[]);

	void ObtemListaCaracteres(char caracteres[]);
	void SalvaVetor(int vetor[]);
	void ObtemPalavraJogador(char palavra[]);

  private:
	char palavra[20];
	char caracteres[20];
	int vetor[20];
	int lista_caracteres_tam;
    sf::RectangleShape body;
    Animation animation;
    unsigned int row; //unsigned so pra deixar claro que será sempre > 0
    float speed;
    bool faceRight;
	vector<Platform> plataformas;
};
