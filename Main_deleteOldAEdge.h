#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMain_deleteOldAEdge(Morphism *morphism);

void applyMain_deleteOldAEdge(Morphism *morphism, bool record_changes);
bool fillpotMain_deleteOldAEdge(MorphismPot *pot, Morphism *morphism);

