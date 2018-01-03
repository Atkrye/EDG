#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMain_unmarkOutputs(Morphism *morphism);

void applyMain_unmarkOutputs(Morphism *morphism, bool record_changes);
bool fillpotMain_unmarkOutputs(MorphismPot *pot, Morphism *morphism);

