#pragma once
#include <SDL.h>
#include <stdio.h>
#include <string>

namespace ksdl{
    class frame{
    public:
        int tempo_inicial;
        int tempo_final;
        int ultima_duracao;
        int duracao();
    };

    class camera{
    public:
        int posicao_x;
        int posicao_y;
        int range;
    };

    class equacao{
    public:
        float y;
        float resultado(float input);

    };
}


