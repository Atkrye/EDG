#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMain_unmarkNode(Morphism *morphism);

void applyMain_unmarkNode(Morphism *morphism, bool record_changes);
bool fillpotMain_unmarkNode(MorphismPot *pot, Morphism *morphism);

