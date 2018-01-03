#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMain_checkMutationCounter(Morphism *morphism);

void applyMain_checkMutationCounter(Morphism *morphism, bool record_changes);
bool fillpotMain_checkMutationCounter(MorphismPot *pot, Morphism *morphism);

