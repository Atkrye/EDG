#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMain_deleteOldNEdge(Morphism *morphism);

void applyMain_deleteOldNEdge(Morphism *morphism, bool record_changes);
bool fillpotMain_deleteOldNEdge(MorphismPot *pot, Morphism *morphism);

