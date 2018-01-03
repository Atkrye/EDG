#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMain_demorgansF1(Morphism *morphism);

void applyMain_demorgansF1(Morphism *morphism, bool record_changes);
bool fillpotMain_demorgansF1(MorphismPot *pot, Morphism *morphism);

