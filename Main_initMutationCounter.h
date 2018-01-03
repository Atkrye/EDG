#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMain_initMutationCounter(Morphism *morphism);

void applyMain_initMutationCounter(Morphism *morphism, bool record_changes);
bool fillpotMain_initMutationCounter(MorphismPot *pot, Morphism *morphism);

