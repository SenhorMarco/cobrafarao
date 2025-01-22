#include "cobra.h"
#include <string>

int frame::duracao(){
    ultima_duracao = tempo_final - tempo_inicial;
    return tempo_final - tempo_inicial;
}

textura::textura(SDL_Renderer* renderer, char* caminho){
    SDL_Surface* superficie = SDL_LoadBMP(caminho);
	if(!superficie){
        printf("Erro ao carregar textura.");
	}
	this->p_textura = SDL_CreateTextureFromSurface(renderer, superficie);
	SDL_FreeSurface(superficie);
	superficie = NULL;

}

void textura::mudar_cor(int r, int g, int b){
    SDL_SetTextureColorMod(this->p_textura, r, g, b);
}

setor_cobra::setor_cobra(float x, float y){
    this->setor_rect.h = 20;
    this->setor_rect.w = 20;
    this->setor_rect.x = x;
    this->setor_rect.y = y;
    this->antecessor = NULL;
    this->sucessor = NULL;
}

