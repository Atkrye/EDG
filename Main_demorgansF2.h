#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMain_demorgansF2(Morphism *morphism);

void applyMain_demorgansF2(Morphism *morphism, bool record_changes);
bool fillpotMain_demorgansF2(MorphismPot *pot, Morphism *morphism);

