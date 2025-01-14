#include <iostream>
#include <SDL.h>
#include <stdio.h>
#include <cmath>
#include <vector>
#include <ctime>
#include <random>
#include <bits/stdc++.h>
#include "ksdl.h"

#define T_FRAME 75
#define ALTURA 800
#define LARGURA 800
bool iniciar(int largura_janela, int altura_janela);
void sair();
bool loop(ksdl::frame *frame_atual, ksdl::camera *camera,SDL_Window *janela, SDL_Renderer *renderer);
int casas = 40, tamanho_casa = ALTURA/casas;

class setor_cobra{
public:
    SDL_Rect setor_rect;
    setor_cobra(float x, float y){
        this->setor_rect.h = 20;
        this->setor_rect.w = 20;
        this->setor_rect.x = x;
        this->setor_rect.y = y;
        this->antecessor = NULL;
        this->sucessor = NULL;
    }
    char direcao = 'b', ultima_direcao = 'b';
    setor_cobra *antecessor;
    setor_cobra *sucessor;
    float x,y;
};

class cobra{
public:
    std::vector<setor_cobra*> setores_cobra;
    int tamanho = 1;
    setor_cobra *cabeca;
    setor_cobra *rabo;
};


SDL_Window* janela;
SDL_Renderer* renderer;
SDL_Texture* branco;
SDL_Texture* amarelo;
SDL_Rect comida;
cobra *minha_cobra = new cobra;

int main(int argc, char** args) {
    std::srand(time(0));
    ksdl::frame frame_atual;
    ksdl::camera camera;
    /*int largura_janela, altura_janela;
    scanf("%d %d", &largura_janela, &altura_janela);*/
	if ( !iniciar(LARGURA, ALTURA) ){
        return 1;
	}
	minha_cobra->cabeca = new setor_cobra(0, 0);
	minha_cobra->setores_cobra.push_back(minha_cobra->cabeca);
	minha_cobra->rabo = minha_cobra->cabeca;
	comida.h = tamanho_casa;
	comida.w = tamanho_casa;
    comida.x = (rand() % casas) * tamanho_casa;
    comida.y = (rand() % casas) * tamanho_casa;

	while ( loop(&frame_atual, &camera, janela, renderer)) {
        if(frame_atual.duracao() > T_FRAME){
            continue;
        }
        else{
           SDL_Delay(T_FRAME-frame_atual.duracao());
        }
	}

	sair();
	return 0;
}

bool loop(ksdl::frame *frame_atual, ksdl::camera *camera, SDL_Window *janela, SDL_Renderer *renderer) {
    frame_atual->tempo_inicial = SDL_GetTicks();

	SDL_Event evento;
	SDL_SetRenderDrawColor( renderer, 0, 0, 0, 0 );
	SDL_RenderClear( renderer );
	for(setor_cobra* setor : minha_cobra->setores_cobra){
        SDL_RenderCopyEx(renderer, branco, NULL, &(setor->setor_rect), 0, NULL, SDL_FLIP_NONE);
	}
	SDL_RenderCopyEx(renderer, amarelo, NULL, &comida, 0, NULL, SDL_FLIP_NONE);
if(minha_cobra->cabeca->setor_rect.x == comida.x && minha_cobra->cabeca->setor_rect.y == comida.y){
        minha_cobra->tamanho++;
        comida.x = (rand() % casas) * tamanho_casa;
        comida.y = (rand() % casas) * tamanho_casa;
        setor_cobra *novo_setor;
        switch(minha_cobra->rabo->ultima_direcao){
            case 'e':
                {
                novo_setor = new setor_cobra(minha_cobra->rabo->setor_rect.x +tamanho_casa, minha_cobra->rabo->setor_rect.y);
                break;
                }
            case 'd':
                {
                novo_setor = new setor_cobra(minha_cobra->rabo->setor_rect.x -tamanho_casa, minha_cobra->rabo->setor_rect.y);
                break;
                }
            case 'c':
                {
                novo_setor = new setor_cobra(minha_cobra->rabo->setor_rect.x, minha_cobra->rabo->setor_rect.y +tamanho_casa);
                break;
                }
            case 'b':
                {
                novo_setor = new setor_cobra(minha_cobra->rabo->setor_rect.x, minha_cobra->rabo->setor_rect.y -tamanho_casa);
                break;
                }
        }
        minha_cobra->setores_cobra.push_back(novo_setor);
        minha_cobra->rabo->antecessor = novo_setor;
        novo_setor->sucessor = minha_cobra->rabo;
        minha_cobra->rabo = novo_setor;

    }

	while ( SDL_PollEvent( &evento ) != 0) {
		switch ( evento.type ) {
			case SDL_QUIT:
				return false;
			case SDL_KEYDOWN:
				switch( evento.key.keysym.sym ){
                    case SDLK_LEFT:
                        minha_cobra->cabeca->direcao = 'e';
                        break;
                    case SDLK_RIGHT:
                        minha_cobra->cabeca->direcao = 'd';
                        break;
                    case SDLK_UP:
                        minha_cobra->cabeca->direcao = 'c';
                        break;
                    case SDLK_DOWN:
                        minha_cobra->cabeca->direcao = 'b';
                        break;
            }
        }
    }
    for(setor_cobra* setor : minha_cobra->setores_cobra){
        if(setor->sucessor != NULL){
            setor->direcao = setor->sucessor->ultima_direcao;
        }
    }

    for(setor_cobra* setor : minha_cobra->setores_cobra){
        switch( setor->direcao ){
            case 'e':
                setor->setor_rect.x -= tamanho_casa;
                break;
            case 'd':
                setor->setor_rect.x += tamanho_casa;
                break;
            case 'c':
                setor->setor_rect.y -= tamanho_casa;
                break;
            case 'b':
                setor->setor_rect.y += tamanho_casa;
                break;
        }
    }
    if(minha_cobra->cabeca->setor_rect.x < 0 || minha_cobra->cabeca->setor_rect.x > 800 || minha_cobra->cabeca->setor_rect.y > 800 || minha_cobra->cabeca->setor_rect.y < 0){
        printf("Tomou gap\nPontos finais: %d", minha_cobra->tamanho);
        return false;
    }
    for(setor_cobra* setor : minha_cobra->setores_cobra){
        if(setor->setor_rect.x == minha_cobra->cabeca->setor_rect.x && setor->setor_rect.y == minha_cobra->cabeca->setor_rect.y && setor != minha_cobra->cabeca){
            printf("Tomou gap\nPontos finais: %d", minha_cobra->tamanho);
            return false;
        }
    }

    for(setor_cobra* setor : minha_cobra->setores_cobra){
        setor->ultima_direcao = setor->direcao;
    }

	SDL_RenderPresent( renderer );
    frame_atual->tempo_final = SDL_GetTicks();
	return true;
}

bool iniciar(int largura_janela, int altura_janela) {
	if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
		printf("Erro na inicializacao!\n");
		return false;
	}

	janela = SDL_CreateWindow( "maldicao da minhoca louca do farao", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, largura_janela, altura_janela, SDL_WINDOW_SHOWN );
	if ( !janela ) {
		printf("Erro ao criar janela : %s\n", SDL_GetError());
		return false;
	}

	renderer = SDL_CreateRenderer( janela, -1, SDL_RENDERER_ACCELERATED );
	if ( !renderer ) {
		printf("Erro ao criar renderer : %s\n", SDL_GetError());
		return false;
	}

	SDL_Surface* superficie = SDL_LoadBMP("branco.bmp");
	if(!superficie){
        return false;
	}
	branco = SDL_CreateTextureFromSurface( renderer, superficie);
	amarelo = SDL_CreateTextureFromSurface( renderer, superficie);
	SDL_SetTextureColorMod(amarelo, 252, 228, 5);
	SDL_FreeSurface(superficie);
	superficie = NULL;
	return true;
}

void sair() {
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( janela );
	SDL_Quit();
}
