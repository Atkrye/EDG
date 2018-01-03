#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMain_deleteMutationCounter(Morphism *morphism);

void applyMain_deleteMutationCounter(Morphism *morphism, bool record_changes);
bool fillpotMain_deleteMutationCounter(MorphismPot *pot, Morphism *morphism);

