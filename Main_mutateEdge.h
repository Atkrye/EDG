#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMain_mutateEdge(Morphism *morphism);

void applyMain_mutateEdge(Morphism *morphism, bool record_changes);
bool fillpotMain_mutateEdge(MorphismPot *pot, Morphism *morphism);

