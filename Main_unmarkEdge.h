#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMain_unmarkEdge(Morphism *morphism);

void applyMain_unmarkEdge(Morphism *morphism, bool record_changes);
bool fillpotMain_unmarkEdge(MorphismPot *pot, Morphism *morphism);

