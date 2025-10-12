#include "actions_combination.h"

Combination combinations[NUMBER_OF_ACTIONS][NUMBER_OF_ACTIONS] = {
    {
        {-20, -20, false, "Ambos sofreram 20 de dano!"}, // 00
        {-20, 0, false, "Você sofreu 20 de dano!"}, // 01
        {0, 0, false, "Ataque bloqueado pelo inimigo!"}, // 02
        {0, -20, false, "Nave inimiga sofreu 20 de dano!"}, // 03
        {0, 0, true, "Inimigo fugiu!"} // 04
    },
    {
        {0, -20, false, "Nave inimiga sofreu 20 de dano!"}, // 10
        {-20, -20, false, "Ambos sofreram 20 de dano!"}, // 11
        {0, 0, false, "Ataque bloqueado pelo inimigo!"}, // 12
        {0, 0, false, "Ataque bloqueado pelo inimigo!"}, // 13
        {0, 0, true, "Inimigo fugiu!"} // 14
    },
    {
        {0, 0, false, "Ataque inimigo bloqueado!"}, // 20
        {0, 0, false, "Ataque inimigo bloqueado!"}, // 21
        {0, 0, false, "Ataque inimigo bloqueado!"}, // 22
        {0, 0, false, "Ataque inimigo bloqueado!"}, // 23
        {0, 0, true, "Inimigo fugiu!"} // 24
    },
    {
        {-20, 0, false, "Você sofreu 20 de dano!"}, // 30
        {0, 0, false, "Ataque inimigo bloqueado!"}, // 31
        {0, 0, false, "Ataque inimigo bloqueado!"}, // 32
        {0, 0, false, "Ataque inimigo bloqueado!"}, // 33
        {0, 0, true, "Inimigo fugiu!"} // 34
    },
    {
        {0, 0, true, "Você fugiu para o hiperespaço"}, // 40
        {0, 0, true, "Você fugiu para o hiperespaço"}, // 41
        {0, 0, true, "Você fugiu para o hiperespaço"}, // 42
        {0, 0, true, "Você fugiu para o hiperespaço"}, // 43
        {0, 0, true, "Você fugiu para o hiperespaço"} // 44
    }
};
