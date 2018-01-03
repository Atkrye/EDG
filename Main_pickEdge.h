#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMain_pickEdge(Morphism *morphism);

void applyMain_pickEdge(Morphism *morphism, bool record_changes);
bool fillpotMain_pickEdge(MorphismPot *pot, Morphism *morphism);

