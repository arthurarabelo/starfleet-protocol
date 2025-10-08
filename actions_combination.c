#include "actions_combination.h"

Combination combinations[NUMBER_OF_ACTIONS][NUMBER_OF_ACTIONS] = {
    {
        {-20, -20, false, "Ambos sofreram 20 de dano!"}, 
        {-20, 0, false, "Você sofreu 20 de dano!"}, 
        {0, 0, false, "Ataque bloqueado pelo inimigo!"}, 
        {0, -20, false, "Nave inimiga sofreu 20 de dano!"}, 
        {0, 0, true, "Inimigo fugiu!"}
    },
    {
        {0, -20, false, "Nave inimiga sofreu 20 de dano!"},
        {-20, -20, false, "Ambos sofreram 20 de dano!"}, 
        {0, 0, false, "Ataque bloqueado pelo inimigo!"}, 
        {0, 0, false, "Ataque bloqueado pelo inimigo!"}, 
        {0, 0, true, "Inimigo fugiu!"}
    },
    {
        {0, 0, false, "Ataque inimigo bloqueado!"}, 
        {0, 0, false, "Ataque inimigo bloqueado!"}, 
        {0, 0, false, "Ataque inimigo bloqueado!"}, 
        {0, 0, false, "Ataque inimigo bloqueado!"}, 
        {0, 0, true, "Inimigo fugiu!"}
    },
    {
        {-20, 0, false, "Você sofreu 20 de dano!"}, 
        {0, 0, false, "Ataque inimigo bloqueado!"}, 
        {0, 0, false, "Ataque inimigo bloqueado!"}, 
        {0, 0, false, "Ataque inimigo bloqueado!"}, 
        {0, 0, true, "Inimigo fugiu!"}
    },
    {
        {0, 0, true, "Você fugiu para o hiperespaço"}, 
        {0, 0, true, "Você fugiu para o hiperespaço"}, 
        {0, 0, true, "Você fugiu para o hiperespaço"}, 
        {0, 0, true, "Você fugiu para o hiperespaço"}, 
        {0, 0, true, "Você fugiu para o hiperespaço"}
    }
};
