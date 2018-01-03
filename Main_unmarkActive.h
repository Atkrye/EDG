#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMain_unmarkActive(Morphism *morphism);

void applyMain_unmarkActive(Morphism *morphism, bool record_changes);
bool fillpotMain_unmarkActive(MorphismPot *pot, Morphism *morphism);

