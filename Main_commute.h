#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMain_commute(Morphism *morphism);

void applyMain_commute(Morphism *morphism, bool record_changes);
bool fillpotMain_commute(MorphismPot *pot, Morphism *morphism);

