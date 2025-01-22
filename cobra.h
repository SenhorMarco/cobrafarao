#pragma once
#include <SDL.h>
#include <stdio.h>
#include <string>
#include <vector>

class frame
{
public:
    int tempo_inicial;
    int tempo_final;
    int ultima_duracao;
    int duracao();
};

class textura
{
public:
    textura(SDL_Renderer* renderer, char* caminho);
    void mudar_cor(int r, int g, int b);
    SDL_Texture* p_textura;

};

class setor_cobra{
public:
    SDL_Rect setor_rect;
    setor_cobra(float x, float y);
    char direcao = 'b', ultima_direcao = 'b';
    setor_cobra *antecessor;
    setor_cobra *sucessor;
};

class cobra{
public:
    std::vector<setor_cobra*> setores_cobra;
    int tamanho = 1;
    setor_cobra *cabeca;
    setor_cobra *rabo;
};

enum estados{
    VIVO,
    MORTO,
    PAUSADO
};
