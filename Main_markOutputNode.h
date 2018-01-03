#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMain_markOutputNode(Morphism *morphism);

void applyMain_markOutputNode(Morphism *morphism, bool record_changes);
bool fillpotMain_markOutputNode(MorphismPot *pot, Morphism *morphism);

