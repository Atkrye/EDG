#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMain_pickedNode(Morphism *morphism);

void applyMain_pickedNode(Morphism *morphism, bool record_changes);
bool fillpotMain_pickedNode(MorphismPot *pot, Morphism *morphism);

