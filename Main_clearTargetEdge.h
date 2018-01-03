#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMain_clearTargetEdge(Morphism *morphism);

void applyMain_clearTargetEdge(Morphism *morphism, bool record_changes);
bool fillpotMain_clearTargetEdge(MorphismPot *pot, Morphism *morphism);

