#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMain_pickNewTarget(Morphism *morphism);

void applyMain_pickNewTarget(Morphism *morphism, bool record_changes);
bool fillpotMain_pickNewTarget(MorphismPot *pot, Morphism *morphism);

