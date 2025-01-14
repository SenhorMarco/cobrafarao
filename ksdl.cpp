#include "ksdl.h"

int ksdl::frame::duracao(){
    ultima_duracao = tempo_final - tempo_inicial;
    return tempo_final - tempo_inicial;
}

/*float ksdl::equacao::resultado(float input){
    y= input;
    return input;
}*/
