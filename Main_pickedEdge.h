#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMain_pickedEdge(Morphism *morphism);

void applyMain_pickedEdge(Morphism *morphism, bool record_changes);
bool fillpotMain_pickedEdge(MorphismPot *pot, Morphism *morphism);

