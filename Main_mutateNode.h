#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMain_mutateNode(Morphism *morphism);

void applyMain_mutateNode(Morphism *morphism, bool record_changes);
bool fillpotMain_mutateNode(MorphismPot *pot, Morphism *morphism);

