#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMain_clearTargetMark(Morphism *morphism);

void applyMain_clearTargetMark(Morphism *morphism, bool record_changes);
bool fillpotMain_clearTargetMark(MorphismPot *pot, Morphism *morphism);

