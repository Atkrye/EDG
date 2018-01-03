#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMain_deleteNodeCounter(Morphism *morphism);

void applyMain_deleteNodeCounter(Morphism *morphism, bool record_changes);
bool fillpotMain_deleteNodeCounter(MorphismPot *pot, Morphism *morphism);

