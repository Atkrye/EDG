#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMain_markActiveNode(Morphism *morphism);

void applyMain_markActiveNode(Morphism *morphism, bool record_changes);
bool fillpotMain_markActiveNode(MorphismPot *pot, Morphism *morphism);

