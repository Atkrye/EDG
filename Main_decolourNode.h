#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMain_decolourNode(Morphism *morphism);

void applyMain_decolourNode(Morphism *morphism, bool record_changes);
bool fillpotMain_decolourNode(MorphismPot *pot, Morphism *morphism);

