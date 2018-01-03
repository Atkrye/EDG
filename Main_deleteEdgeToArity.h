#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMain_deleteEdgeToArity(Morphism *morphism);

void applyMain_deleteEdgeToArity(Morphism *morphism, bool record_changes);
bool fillpotMain_deleteEdgeToArity(MorphismPot *pot, Morphism *morphism);

