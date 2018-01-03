#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMain_pickNode(Morphism *morphism);

void applyMain_pickNode(Morphism *morphism, bool record_changes);
bool fillpotMain_pickNode(MorphismPot *pot, Morphism *morphism);

