#include "Main_addEdgeCounter.h"

static bool match_n0(Morphism *morphism);

bool matchMain_addEdgeCounter(Morphism *morphism)
{
   if(1 > host->number_of_nodes || 0 > host->number_of_edges) return false;
   if(match_n0(morphism)) return true;
   else
   {
      initialiseMorphism(morphism, host);
      return false;
   }
}

static bool match_n0(Morphism *morphism)
{
   RootNodes *nodes;
   for(nodes = getRootNodeList(host); nodes != NULL; nodes = nodes->next)
   {
      Node *host_node = getNode(host, nodes->index);
      if(host_node == NULL) continue;
      if(host_node->matched) continue;
      if(host_node->label.mark != 3) continue;
      if(host_node->indegree < 0 || host_node->outdegree < 0 ||
         ((host_node->outdegree + host_node->indegree - 0 - 0 - 0) < 0)) continue;

      HostLabel label = host_node->label;
      bool match = false;
      /* Label Matching */
      int new_assignments = 0;
      /* Match list variable 0 against the whole host list. */
      int result = -1;
      if(label.length == 1)
      {
         if(label.list->first->atom.type == 'i')
            result = addIntegerAssignment(morphism, 0, label.list->first->atom.num);
         else result = addStringAssignment(morphism, 0, label.list->first->atom.str);
      }
      else result = addListAssignment(morphism, 0, label.list);
      if(result >= 0)
      {
         new_assignments += result;
         match = true;
      }
      if(match)
      {
         addNodeMap(morphism, 0, host_node->index, new_assignments);
         host_node->matched = true;
         /* All items matched! */
            return true;
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

void applyMain_addEdgeCounter(Morphism *morphism, bool record_changes)
{
   int node_index = lookupNode(morphism, 0);
   int outdegree0 = getOutdegree(host, node_index);
   int host_node_index;
   int node_array_size1 = host->nodes.size;
   HostLabel label;
   int list_var_length0 = 0;
   int list_length0 = list_var_length0 + 1;
   HostAtom array0[list_length0];
   int index0 = 0;

   array0[index0].type = 'i';
   array0[index0++].num = outdegree0 - 1;
   if(list_length0 > 0)
   {
      HostList *list0 = makeHostList(array0, list_length0, false);
      label = makeHostLabel(2, list_length0, list0);
   }
   else label = makeEmptyLabel(2);

   host_node_index = addNode(host, 1, label);
   /* If the node array size has not increased after the node addition, then
      the node was added to a hole in the array. */
   if(record_changes)
      pushAddedNode(host_node_index, node_array_size1 == host->nodes.size);
   /* Reset the morphism. */
   initialiseMorphism(morphism, host);
}

static bool fillpot_n0(MorphismPot *pot, Morphism *morphism);

bool fillpotMain_addEdgeCounter(MorphismPot *pot, Morphism *morphism)
{
   if(1 > host->number_of_nodes || 0 > host->number_of_edges) return false;
   int oldPotsize = potSize(pot);
   fillpot_n0(pot, morphism);
   if(potSize(pot) > oldPotsize){
      initialiseMorphism(morphism, host);
      return true;
   }
   else
   {
      initialiseMorphism(morphism, host);
      return false;
   }
}

static bool fillpot_n0(MorphismPot *pot, Morphism *morphism)
{
   RootNodes *nodes;
   for(nodes = getRootNodeList(host); nodes != NULL; nodes = nodes->next)
   {
      Node *host_node = getNode(host, nodes->index);
      if(host_node == NULL) continue;
      if(host_node->matched) continue;
      if(host_node->label.mark != 3) continue;
      if(host_node->indegree < 0 || host_node->outdegree < 0 ||
         ((host_node->outdegree + host_node->indegree - 0 - 0 - 0) < 0)) continue;

      HostLabel label = host_node->label;
      bool match = false;
      /* Label Matching */
      int new_assignments = 0;
      /* Match list variable 0 against the whole host list. */
      int result = -1;
      if(label.length == 1)
      {
         if(label.list->first->atom.type == 'i')
            result = addIntegerAssignment(morphism, 0, label.list->first->atom.num);
         else result = addStringAssignment(morphism, 0, label.list->first->atom.str);
      }
      else result = addListAssignment(morphism, 0, label.list);
      if(result >= 0)
      {
         new_assignments += result;
         match = true;
      }
      if(match)
      {
         addNodeMap(morphism, 0, host_node->index, new_assignments);
         host_node->matched = true;
         /* All items matched! */
            putMorphism(pot, morphism, "Main_addEdgeCounter", host);
            removeNodeMap(morphism, 0);
            host_node->matched = false;
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

