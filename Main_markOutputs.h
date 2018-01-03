#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMain_markOutputs(Morphism *morphism);

void applyMain_markOutputs(Morphism *morphism, bool record_changes);
bool fillpotMain_markOutputs(MorphismPot *pot, Morphism *morphism);

