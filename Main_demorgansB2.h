#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMain_demorgansB2(Morphism *morphism);

void applyMain_demorgansB2(Morphism *morphism, bool record_changes);
bool fillpotMain_demorgansB2(MorphismPot *pot, Morphism *morphism);

