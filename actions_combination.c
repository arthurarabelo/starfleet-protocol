#include "actions_combination.h"

Combination combinations[NUMBER_OF_ACTIONS][NUMBER_OF_ACTIONS] = {
    {{-20, -20, false}, {-20, 0, false}, {0, 0, false}, {0, -20, false}, {0, 0, true}},
    {{0, -20, false}, {-20, -20, false}, {0, 0, false}, {0, 0, false}, {0, 0, true}},
    {{0, 0, false}, {0, 0, false}, {0, 0, false}, {0, 0, false}, {0, 0, true}},
    {{-20, 0, false}, {0, 0, false}, {0, 0, false}, {0, 0, false}, {0, 0, true}},
    {{0, 0, true}, {0, 0, true}, {0, 0, true}, {0, 0, true}, {0, 0, true}}
};
