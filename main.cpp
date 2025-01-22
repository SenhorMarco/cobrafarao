#include <iostream>
#include <SDL.h>
#include <stdio.h>
#include <cmath>
#include <vector>
#include <ctime>
#include <random>
#include <bits/stdc++.h>
#include "cobra.h"

#define T_FRAME 75
#define ALTURA 800
#define LARGURA 800
#define CASAS 40

bool iniciar();
void sair();
bool loop_jogo(frame *frame_atual);
int tamanho_casa = ALTURA/CASAS;

SDL_Window* janela;
SDL_Renderer* renderer;
textura* verde;
textura* cinza;
SDL_Rect comida;
cobra *minha_cobra = new cobra;
enum estados estado = VIVO;

int main(int argc, char** args) {
    frame frame_atual;
    std::srand(time(0));
	if ( !iniciar() ){
        return 1;
	}

	minha_cobra->cabeca = new setor_cobra(400, 400);
	minha_cobra->setores_cobra.push_back(minha_cobra->cabeca);
	minha_cobra->rabo = minha_cobra->cabeca;
	comida.h = tamanho_casa;
	comida.w = tamanho_casa;
    comida.x = (rand() % CASAS) * tamanho_casa;
    comida.y = (rand() % CASAS) * tamanho_casa;

	while ( loop_jogo(&frame_atual)) {
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

bool loop_jogo(frame *frame_atual) {
    frame_atual->tempo_inicial = SDL_GetTicks();
	SDL_Event evento;

	SDL_SetRenderDrawColor( renderer, 6, 62, 92, 0 );
	SDL_RenderClear( renderer );

	//Animação de morte da cobra
    if(estado == MORTO){
        static int timer_morte = minha_cobra->tamanho;

        //Caso a animação esteja sendo repetida
        if(timer_morte == 0){
            timer_morte = minha_cobra->tamanho;
        }
        for(int i = 0; i < timer_morte; i++){
            SDL_RenderCopyEx(renderer, verde->p_textura, NULL, &(minha_cobra->setores_cobra.at(i)->setor_rect), 0, NULL, SDL_FLIP_NONE);
        }
        timer_morte--;
        if(timer_morte == 0){
            for(setor_cobra* setor : minha_cobra->setores_cobra){
                delete setor;
            }
            delete minha_cobra;
            //Reinicia o jogo depois de morrer
            minha_cobra = new cobra;
            minha_cobra->cabeca = new setor_cobra(400, 400);
            minha_cobra->setores_cobra.push_back(minha_cobra->cabeca);
            minha_cobra->rabo = minha_cobra->cabeca;
            comida.x = (rand() % CASAS) * tamanho_casa;
            comida.y = (rand() % CASAS) * tamanho_casa;
            estado = VIVO;
        }
        SDL_RenderPresent( renderer );
        frame_atual->tempo_final = SDL_GetTicks();
        return true;
    }

    //Pausa
    if(estado == PAUSADO){
        while ( SDL_PollEvent( &evento ) != 0) {
            switch ( evento.type ) {
                case SDL_QUIT:
                    return false;
                case SDL_KEYDOWN:
                    switch( evento.key.keysym.sym ){
                        case SDLK_ESCAPE:
                            estado = VIVO;
                            break;
                    }
            }
        }
        frame_atual->tempo_final = SDL_GetTicks();
        return true;
    }

    //Desenhar os segmentos da cobra
	for(setor_cobra* setor : minha_cobra->setores_cobra){
        SDL_RenderCopyEx(renderer, verde->p_textura, NULL, &(setor->setor_rect), 0, NULL, SDL_FLIP_NONE);
	}

	//Desenhar a comida da cobra
	SDL_RenderCopyEx(renderer, cinza->p_textura, NULL, &comida, 0, NULL, SDL_FLIP_NONE);

	//Checar se o jogador comeu a comida
    if(minha_cobra->cabeca->setor_rect.x == comida.x && minha_cobra->cabeca->setor_rect.y == comida.y){
        //Criar nova comida
        minha_cobra->tamanho++;
        comida.x = (rand() % CASAS) * tamanho_casa;
        comida.y = (rand() % CASAS) * tamanho_casa;
        setor_cobra *novo_setor;

        //Decidir a posição do novo segmento da cobra
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

    //Atribui à cabeça da cobra a direção decidida pelo jogador
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
                    case SDLK_ESCAPE:
                        estado = PAUSADO;
                        break;
            }
        }
    }

    //Atribui aos demais segmentos da cobra suas respectivas direções
    for(setor_cobra* setor : minha_cobra->setores_cobra){
        if(setor->sucessor != NULL){
            setor->direcao = setor->sucessor->ultima_direcao;
        }
    }

    //Movimenta todos os segmentos da cobra baseado em suas direções
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


    //Guarda a ultima direção usada para cada segmento
    for(setor_cobra* setor : minha_cobra->setores_cobra){
        setor->ultima_direcao = setor->direcao;
    }

	SDL_RenderPresent( renderer );

	//Checa se a cobra tocou nas boras da tela
	if(minha_cobra->cabeca->setor_rect.x < 0 || minha_cobra->cabeca->setor_rect.x >= LARGURA || minha_cobra->cabeca->setor_rect.y >= ALTURA || minha_cobra->cabeca->setor_rect.y < 0){
        estado = MORTO;
    }

    //Checa se a cobra tocou a si mesma (:freaky:)
    for(setor_cobra* setor : minha_cobra->setores_cobra){
        if(setor->setor_rect.x == minha_cobra->cabeca->setor_rect.x && setor->setor_rect.y == minha_cobra->cabeca->setor_rect.y && setor != minha_cobra->cabeca){
            estado = MORTO;
        }
    }

    frame_atual->tempo_final = SDL_GetTicks();
	return true;
}

bool iniciar() {
	if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
		printf("Erro na inicializacao!\n");
		return false;
	}

	janela = SDL_CreateWindow( "maldicao da minhoca louca do farao", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, LARGURA, ALTURA, SDL_WINDOW_SHOWN );
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
	verde = new textura(renderer, "branco.bmp");
	cinza = new textura(renderer, "branco.bmp");
	verde->mudar_cor(43,153,43);
	cinza->mudar_cor(125, 135, 125);
	return true;
}

void sair() {
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( janela );
	for(setor_cobra* setor : minha_cobra->setores_cobra){
        delete setor;
    }
    delete minha_cobra;
	delete verde;
	delete cinza;
	SDL_Quit();
}
