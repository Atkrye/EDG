#include "Main_pickFuncMutate.h"

static bool match_n0(Morphism *morphism);

bool matchMain_pickFuncMutate(Morphism *morphism)
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
      if(host_node->label.mark != 0) continue;
      if(host_node->indegree < 0 || host_node->outdegree < 0 ||
         ((host_node->outdegree + host_node->indegree - 0 - 0 - 0) < 0)) continue;

      HostLabel label = host_node->label;
      bool match = false;
      /* Label Matching */
      int new_assignments = 0;
      do
      {
         if(label.length < 2) break;
         /* Matching from the start of the host list. */
         HostListItem *item = label.list->first;
         /* Check if the end of the host list has been reached. */
         if(item == NULL) break;
         /* Matching rule atom 1. */
         if(item->atom.type != 's') break;
         else if(strcmp(item->atom.str, "FunctionSet") != 0) break;
         item = item->next;

         /* Check if the end of the host list has been reached. */
         if(item == NULL) break;
         /* Matching rule atom 2. */
         int result = -1;
         /* Matching string variable 2. */
         if(item->atom.type != 's') break;
         result = addStringAssignment(morphism, 2, item->atom.str);
         if(result >= 0)
         {
            new_assignments += result;
         }
         else break;
         item = item->next;

         HostListItem *start = item;
         /* Matching list variable 1. */
         if(start == NULL) result = addListAssignment(morphism, 1, NULL);
         else if(item == start)
         {
            if(item->atom.type == 'i') result = addIntegerAssignment(morphism, 1, item->atom.num);
            else result = addStringAssignment(morphism, 1, item->atom.str);
         }
         else
         {
            /* Assign to variable 1 the unmatched sublist of the host list. */
            HostAtom sublist[label.length - 2];
            int list_index = 0;
            HostListItem *iterator = start;
            while(iterator != item->next)
            {
               sublist[list_index++] = iterator->atom;
               iterator = iterator->next;
            }
            HostList *list = makeHostList(sublist, label.length - 2, false);
            result = addListAssignment(morphism, 1, list);
            freeHostList(list);
         }
         if(result >= 0)
         {
            new_assignments += result;
            match = true;
         }
      } while(false);

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

void applyMain_pickFuncMutate(Morphism *morphism, bool record_changes)
{
   int host_node_index;
   int node_array_size1 = host->nodes.size;
   HostLabel label;
   int list_var_length0 = 0;
   int list_length0 = list_var_length0 + 1;
   HostAtom array0[list_length0];
   int index0 = 0;

   array0[index0].type = 's';
   array0[index0++].str = "MutateNode";
   if(list_length0 > 0)
   {
      HostList *list0 = makeHostList(array0, list_length0, false);
      label = makeHostLabel(0, list_length0, list0);
   }
   else label = makeEmptyLabel(0);

   host_node_index = addNode(host, 1, label);
   /* If the node array size has not increased after the node addition, then
      the node was added to a hole in the array. */
   if(record_changes)
      pushAddedNode(host_node_index, node_array_size1 == host->nodes.size);
   /* Reset the morphism. */
   initialiseMorphism(morphism, host);
}

static bool fillpot_n0(MorphismPot *pot, Morphism *morphism);

bool fillpotMain_pickFuncMutate(MorphismPot *pot, Morphism *morphism)
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
      if(host_node->label.mark != 0) continue;
      if(host_node->indegree < 0 || host_node->outdegree < 0 ||
         ((host_node->outdegree + host_node->indegree - 0 - 0 - 0) < 0)) continue;

      HostLabel label = host_node->label;
      bool match = false;
      /* Label Matching */
      int new_assignments = 0;
      do
      {
         if(label.length < 2) break;
         /* Matching from the start of the host list. */
         HostListItem *item = label.list->first;
         /* Check if the end of the host list has been reached. */
         if(item == NULL) break;
         /* Matching rule atom 1. */
         if(item->atom.type != 's') break;
         else if(strcmp(item->atom.str, "FunctionSet") != 0) break;
         item = item->next;

         /* Check if the end of the host list has been reached. */
         if(item == NULL) break;
         /* Matching rule atom 2. */
         int result = -1;
         /* Matching string variable 2. */
         if(item->atom.type != 's') break;
         result = addStringAssignment(morphism, 2, item->atom.str);
         if(result >= 0)
         {
            new_assignments += result;
         }
         else break;
         item = item->next;

         HostListItem *start = item;
         /* Matching list variable 1. */
         if(start == NULL) result = addListAssignment(morphism, 1, NULL);
         else if(item == start)
         {
            if(item->atom.type == 'i') result = addIntegerAssignment(morphism, 1, item->atom.num);
            else result = addStringAssignment(morphism, 1, item->atom.str);
         }
         else
         {
            /* Assign to variable 1 the unmatched sublist of the host list. */
            HostAtom sublist[label.length - 2];
            int list_index = 0;
            HostListItem *iterator = start;
            while(iterator != item->next)
            {
               sublist[list_index++] = iterator->atom;
               iterator = iterator->next;
            }
            HostList *list = makeHostList(sublist, label.length - 2, false);
            result = addListAssignment(morphism, 1, list);
            freeHostList(list);
         }
         if(result >= 0)
         {
            new_assignments += result;
            match = true;
         }
      } while(false);

      if(match)
      {
         addNodeMap(morphism, 0, host_node->index, new_assignments);
         host_node->matched = true;
         /* All items matched! */
            putMorphism(pot, morphism, "Main_pickFuncMutate", host);
            removeNodeMap(morphism, 0);
            host_node->matched = false;
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}