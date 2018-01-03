#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMain_demorgansB1(Morphism *morphism);

void applyMain_demorgansB1(Morphism *morphism, bool record_changes);
bool fillpotMain_demorgansB1(MorphismPot *pot, Morphism *morphism);

