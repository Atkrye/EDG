#include "Main_deleteEdgeToArity.h"

bool b8 = true;
bool b9 = true;

static bool evaluateCondition(void)
{
   return (b8 && b9);
}

static void evaluatePredicate8(Morphism *morphism)
{
   Assignment assignment_3 = getAssignment(morphism, 3);
   /* If the variable is not yet assigned, return. */
   if(assignment_3.type == 'n') return;
   string var_3 = getStringValue(morphism, 3);

   int list_var_length0 = 0;
   int list_length0 = list_var_length0 + 1;
   HostAtom array0[list_length0];
   int index0 = 0;

   array0[index0].type = 's';
   array0[index0++].str = var_3;

   int list_var_length1 = 0;
   int list_length1 = list_var_length1 + 1;
   HostAtom array1[list_length1];
   int index1 = 0;

   array1[index1].type = 's';
   array1[index1++].str = "OUT";

   if(!equalHostLists(array0, array1, list_length0, list_length1)) b8 = true;
   else b8 = false;
}

static void evaluatePredicate9(Morphism *morphism)
{
   Assignment assignment_4 = getAssignment(morphism, 4);
   /* If the variable is not yet assigned, return. */
   if(assignment_4.type == 'n') return;
   int var_4 = getIntegerValue(morphism, 4);

   Assignment assignment_5 = getAssignment(morphism, 5);
   /* If the variable is not yet assigned, return. */
   if(assignment_5.type == 'n') return;
   int var_5 = getIntegerValue(morphism, 5);

   if(var_5 >= var_4) b9 = true;
   else b9 = false;
}

static bool match_n0(Morphism *morphism);
static bool match_e0(Morphism *morphism);
static bool match_n1(Morphism *morphism, Edge *host_edge);
static bool match_n2(Morphism *morphism);
static bool match_n3(Morphism *morphism);

bool matchMain_deleteEdgeToArity(Morphism *morphism)
{
   if(4 > host->number_of_nodes || 1 > host->number_of_edges) return false;
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
      if(host_node->indegree < 0 || host_node->outdegree < 1 ||
         ((host_node->outdegree + host_node->indegree - 1 - 0 - 0) < 0)) continue;

      HostLabel label = host_node->label;
      bool match = false;
      /* Label Matching */
      int new_assignments = 0;
      do
      {
         if(label.length < 1) break;
         /* Matching from the start of the host list. */
         HostListItem *item = label.list->first;
         int result = -1;
         HostListItem *start = item;
         /* The current host list position marks the start of the list that is
            assigned to the list variable. */
         /* More rule atoms to match. If the end of the host list is reached, break. */
         if(start == NULL) break;

         /* Matching from the end of the host list. */
         item = label.list->last;
         /* Check if the host list has passed "start". */
         if(item == start->prev) break;
         /* Matching rule atom 2 */
         /* Matching string variable 2. */
         if(item->atom.type != 's') break;
         result = addStringAssignment(morphism, 2, item->atom.str);
         if(result >= 0)
         {
            new_assignments += result;
         }
         else break;
         item = item->prev;
         /* Matching list variable 0. */
         if(item == start->prev) result = addListAssignment(morphism, 0, NULL);
         else if(item == start)
         {
            if(item->atom.type == 'i') result = addIntegerAssignment(morphism, 0, item->atom.num);
            else result = addStringAssignment(morphism, 0, item->atom.str);
         }
         else
         {
            /* Assign to variable 0 the unmatched sublist of the host list. */
            HostAtom sublist[label.length - 1];
            int list_index = 0;
            HostListItem *iterator = start;
            while(iterator != item->next)
            {
               sublist[list_index++] = iterator->atom;
               iterator = iterator->next;
            }
            HostList *list = makeHostList(sublist, label.length - 1, false);
            result = addListAssignment(morphism, 0, list);
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
         if(match_e0(morphism)) return true;
         else
         {
         removeNodeMap(morphism, 0);
         host_node->matched = false;
         }
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

static bool match_e0(Morphism *morphism)
{
   /* Start node is the already-matched node from which the candidate
      edges are drawn. End node may or may not have been matched already. */
   int start_index = lookupNode(morphism, 0);
   int end_index = lookupNode(morphism, 1);
   if(start_index < 0) return false;
   Node *host_node = getNode(host, start_index);

   int counter;
   for(counter = 0; counter < host_node->out_edges.size + 2; counter++)
   {
      Edge *host_edge = getNthOutEdge(host, host_node, counter);
      if(host_edge == NULL) continue;
      if(host_edge->matched) continue;
      if(host_edge->source == host_edge->target) continue;
      if(host_edge->label.mark != 0) continue;

      /* If the end node has been matched, check that the target of the
       * host edge is the image of the end node. */
      if(end_index >= 0)
      {
         if(host_edge->target != end_index) continue;
      }
      /* Otherwise, the target of the host edge should be unmatched. */
      else
      {
         Node *end_node = getNode(host, host_edge->target);
         if(end_node->matched) continue;
      }

      HostLabel label = host_edge->label;
      bool match = false;
      /* Label Matching */
      int new_assignments = 0;
      do
      {
         /* The rule list does not contain a list variable, so there is no
          * match if the host list has a different length. */
         if(label.length != 1) break;
         HostListItem *item = label.list->first;
         /* Matching rule atom 1. */
         int result = -1;
         /* Matching integer variable 5. */
         if(item->atom.type != 'i') break;
         result = addIntegerAssignment(morphism, 5, item->atom.num);
         if(result >= 0)
         {
            new_assignments += result;
            /* Update global booleans for the variable's predicates. */
            evaluatePredicate9(morphism);
            if(!evaluateCondition())
            {
               /* Reset the boolean variables in the predicates of this variable. */
               b9 = true;
               break;
            }
         }
         else break;
         match = true;
      } while(false);

      if(match)
      {
         addEdgeMap(morphism, 0, host_edge->index, new_assignments);
         host_edge->matched = true;
         if(match_n1(morphism, host_edge)) return true;
         else
         {
            removeEdgeMap(morphism, 0);
            host_edge->matched = false;
         }
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

static bool match_n1(Morphism *morphism, Edge *host_edge)
{
   Node *host_node = getTarget(host, host_edge);

   if(host_node->matched) return false;
   if(host_node->label.mark != 0) return false;
      if(host_node->indegree < 1 || host_node->outdegree < 0 ||
         ((host_node->outdegree + host_node->indegree - 0 - 1 - 0) < 0)) return false;;

   HostLabel label = host_node->label;
   bool match = false;
   /* Label Matching */
   int new_assignments = 0;
   do
   {
      if(label.length < 1) break;
      /* Matching from the start of the host list. */
      HostListItem *item = label.list->first;
      int result = -1;
      HostListItem *start = item;
      /* The current host list position marks the start of the list that is
         assigned to the list variable. */
      /* More rule atoms to match. If the end of the host list is reached, break. */
      if(start == NULL) break;

      /* Matching from the end of the host list. */
      item = label.list->last;
      /* Check if the host list has passed "start". */
      if(item == start->prev) break;
      /* Matching rule atom 2 */
      /* Matching string variable 3. */
      if(item->atom.type != 's') break;
      result = addStringAssignment(morphism, 3, item->atom.str);
      if(result >= 0)
      {
         new_assignments += result;
         /* Update global booleans for the variable's predicates. */
         evaluatePredicate8(morphism);
         if(!evaluateCondition())
         {
            /* Reset the boolean variables in the predicates of this variable. */
            b8 = true;
            break;
         }
      }
      else break;
      item = item->prev;
      /* Matching list variable 1. */
      if(item == start->prev) result = addListAssignment(morphism, 1, NULL);
      else if(item == start)
      {
         if(item->atom.type == 'i') result = addIntegerAssignment(morphism, 1, item->atom.num);
         else result = addStringAssignment(morphism, 1, item->atom.str);
      }
      else
      {
         /* Assign to variable 1 the unmatched sublist of the host list. */
         HostAtom sublist[label.length - 1];
         int list_index = 0;
         HostListItem *iterator = start;
         while(iterator != item->next)
         {
            sublist[list_index++] = iterator->atom;
            iterator = iterator->next;
         }
         HostList *list = makeHostList(sublist, label.length - 1, false);
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
      addNodeMap(morphism, 1, host_node->index, new_assignments);
      host_node->matched = true;
      if(match_n2(morphism)) return true;
      else
      {
      removeNodeMap(morphism, 1);
      host_node->matched = false;
      }
   }
   else removeAssignments(morphism, new_assignments);
   return false;
}

static bool match_n2(Morphism *morphism)
{
   RootNodes *nodes;
   for(nodes = getRootNodeList(host); nodes != NULL; nodes = nodes->next)
   {
      Node *host_node = getNode(host, nodes->index);
      if(host_node == NULL) continue;
      if(host_node->matched) continue;
      if(host_node->label.mark != 2) continue;
      if(host_node->indegree < 0 || host_node->outdegree < 0 ||
         ((host_node->outdegree + host_node->indegree - 0 - 0 - 0) < 0)) continue;

      HostLabel label = host_node->label;
      bool match = false;
      /* Label Matching */
      int new_assignments = 0;
      do
      {
         /* The rule list does not contain a list variable, so there is no
          * match if the host list has a different length. */
         if(label.length != 1) break;
         HostListItem *item = label.list->first;
         /* Matching rule atom 1. */
         int result = -1;
         /* Matching integer variable 5. */
         if(item->atom.type != 'i') break;
         result = addIntegerAssignment(morphism, 5, item->atom.num);
         if(result >= 0)
         {
            new_assignments += result;
            /* Update global booleans for the variable's predicates. */
            evaluatePredicate9(morphism);
            if(!evaluateCondition())
            {
               /* Reset the boolean variables in the predicates of this variable. */
               b9 = true;
               break;
            }
         }
         else break;
         match = true;
      } while(false);

      if(match)
      {
         addNodeMap(morphism, 2, host_node->index, new_assignments);
         host_node->matched = true;
         if(match_n3(morphism)) return true;
         else
         {
         removeNodeMap(morphism, 2);
         host_node->matched = false;
         }
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

static bool match_n3(Morphism *morphism)
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
         /* The rule list does not contain a list variable, so there is no
          * match if the host list has a different length. */
         if(label.length != 3) break;
         HostListItem *item = label.list->first;
         /* Matching rule atom 1. */
         if(item->atom.type != 's') break;
         else if(strcmp(item->atom.str, "FunctionSet") != 0) break;
         item = item->next;

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

         /* Matching rule atom 3. */
         /* Matching integer variable 4. */
         if(item->atom.type != 'i') break;
         result = addIntegerAssignment(morphism, 4, item->atom.num);
         if(result >= 0)
         {
            new_assignments += result;
            /* Update global booleans for the variable's predicates. */
            evaluatePredicate9(morphism);
            if(!evaluateCondition())
            {
               /* Reset the boolean variables in the predicates of this variable. */
               b9 = true;
               break;
            }
         }
         else break;
         match = true;
      } while(false);

      if(match)
      {
         addNodeMap(morphism, 3, host_node->index, new_assignments);
         host_node->matched = true;
         /* All items matched! */
            return true;
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

void applyMain_deleteEdgeToArity(Morphism *morphism, bool record_changes)
{
   int var_5 = getIntegerValue(morphism, 5);
   int host_edge_index = lookupEdge(morphism, 0);
   if(record_changes)
   {
      Edge *edge = getEdge(host, host_edge_index);
      /* A hole is created if the edge is not at the right-most index of the array. */
      pushRemovedEdge(edge->label, edge->source, edge->target, edge->index,
                      edge->index < host->edges.size - 1);
   }
   removeEdge(host, host_edge_index);

   int host_node_index = lookupNode(morphism, 2);
   HostLabel label_n2 = getNodeLabel(host, host_node_index);
   HostLabel label;
   int list_var_length0 = 0;
   int list_length0 = list_var_length0 + 1;
   HostAtom array0[list_length0];
   int index0 = 0;

   array0[index0].type = 'i';
   array0[index0++].num = var_5 - 1;
   if(list_length0 > 0)
   {
      HostList *list0 = makeHostList(array0, list_length0, false);
      label = makeHostLabel(2, list_length0, list0);
   }
   else label = makeEmptyLabel(2);

   if(equalHostLabels(label_n2, label)) removeHostList(label.list);
   else
   {
      if(record_changes) pushRelabelledNode(host_node_index, label_n2);
      relabelNode(host, host_node_index, label);
   }
   /* Reset the morphism. */
   initialiseMorphism(morphism, host);
}

static bool fillpot_n0(MorphismPot *pot, Morphism *morphism);
static bool fillpot_e0(MorphismPot *pot, Morphism *morphism);
static bool fillpot_n1(MorphismPot *pot, Morphism *morphism, Edge *host_edge);
static bool fillpot_n2(MorphismPot *pot, Morphism *morphism);
static bool fillpot_n3(MorphismPot *pot, Morphism *morphism);

bool fillpotMain_deleteEdgeToArity(MorphismPot *pot, Morphism *morphism)
{
   if(4 > host->number_of_nodes || 1 > host->number_of_edges) return false;
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
      if(host_node->indegree < 0 || host_node->outdegree < 1 ||
         ((host_node->outdegree + host_node->indegree - 1 - 0 - 0) < 0)) continue;

      HostLabel label = host_node->label;
      bool match = false;
      /* Label Matching */
      int new_assignments = 0;
      do
      {
         if(label.length < 1) break;
         /* Matching from the start of the host list. */
         HostListItem *item = label.list->first;
         int result = -1;
         HostListItem *start = item;
         /* The current host list position marks the start of the list that is
            assigned to the list variable. */
         /* More rule atoms to match. If the end of the host list is reached, break. */
         if(start == NULL) break;

         /* Matching from the end of the host list. */
         item = label.list->last;
         /* Check if the host list has passed "start". */
         if(item == start->prev) break;
         /* Matching rule atom 2 */
         /* Matching string variable 2. */
         if(item->atom.type != 's') break;
         result = addStringAssignment(morphism, 2, item->atom.str);
         if(result >= 0)
         {
            new_assignments += result;
         }
         else break;
         item = item->prev;
         /* Matching list variable 0. */
         if(item == start->prev) result = addListAssignment(morphism, 0, NULL);
         else if(item == start)
         {
            if(item->atom.type == 'i') result = addIntegerAssignment(morphism, 0, item->atom.num);
            else result = addStringAssignment(morphism, 0, item->atom.str);
         }
         else
         {
            /* Assign to variable 0 the unmatched sublist of the host list. */
            HostAtom sublist[label.length - 1];
            int list_index = 0;
            HostListItem *iterator = start;
            while(iterator != item->next)
            {
               sublist[list_index++] = iterator->atom;
               iterator = iterator->next;
            }
            HostList *list = makeHostList(sublist, label.length - 1, false);
            result = addListAssignment(morphism, 0, list);
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
         if(fillpot_e0(pot, morphism)) return true;
         else
         {
         removeNodeMap(morphism, 0);
         host_node->matched = false;
         }
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

static bool fillpot_e0(MorphismPot *pot, Morphism *morphism)
{
   /* Start node is the already-matched node from which the candidate
      edges are drawn. End node may or may not have been matched already. */
   int start_index = lookupNode(morphism, 0);
   int end_index = lookupNode(morphism, 1);
   if(start_index < 0) return false;
   Node *host_node = getNode(host, start_index);

   int counter;
   for(counter = 0; counter < host_node->out_edges.size + 2; counter++)
   {
      Edge *host_edge = getNthOutEdge(host, host_node, counter);
      if(host_edge == NULL) continue;
      if(host_edge->matched) continue;
      if(host_edge->source == host_edge->target) continue;
      if(host_edge->label.mark != 0) continue;

      /* If the end node has been matched, check that the target of the
       * host edge is the image of the end node. */
      if(end_index >= 0)
      {
         if(host_edge->target != end_index) continue;
      }
      /* Otherwise, the target of the host edge should be unmatched. */
      else
      {
         Node *end_node = getNode(host, host_edge->target);
         if(end_node->matched) continue;
      }

      HostLabel label = host_edge->label;
      bool match = false;
      /* Label Matching */
      int new_assignments = 0;
      do
      {
         /* The rule list does not contain a list variable, so there is no
          * match if the host list has a different length. */
         if(label.length != 1) break;
         HostListItem *item = label.list->first;
         /* Matching rule atom 1. */
         int result = -1;
         /* Matching integer variable 5. */
         if(item->atom.type != 'i') break;
         result = addIntegerAssignment(morphism, 5, item->atom.num);
         if(result >= 0)
         {
            new_assignments += result;
            /* Update global booleans for the variable's predicates. */
            evaluatePredicate9(morphism);
            if(!evaluateCondition())
            {
               /* Reset the boolean variables in the predicates of this variable. */
               b9 = true;
               break;
            }
         }
         else break;
         match = true;
      } while(false);

      if(match)
      {
         addEdgeMap(morphism, 0, host_edge->index, new_assignments);
         host_edge->matched = true;
         if(fillpot_n1(pot, morphism, host_edge)) return true;
         else
         {
            removeEdgeMap(morphism, 0);
            host_edge->matched = false;
         }
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

static bool fillpot_n1(MorphismPot *pot, Morphism *morphism, Edge *host_edge)
{
   Node *host_node = getTarget(host, host_edge);

   if(host_node->matched) return false;
   if(host_node->label.mark != 0) return false;
      if(host_node->indegree < 1 || host_node->outdegree < 0 ||
         ((host_node->outdegree + host_node->indegree - 0 - 1 - 0) < 0)) return false;;

   HostLabel label = host_node->label;
   bool match = false;
   /* Label Matching */
   int new_assignments = 0;
   do
   {
      if(label.length < 1) break;
      /* Matching from the start of the host list. */
      HostListItem *item = label.list->first;
      int result = -1;
      HostListItem *start = item;
      /* The current host list position marks the start of the list that is
         assigned to the list variable. */
      /* More rule atoms to match. If the end of the host list is reached, break. */
      if(start == NULL) break;

      /* Matching from the end of the host list. */
      item = label.list->last;
      /* Check if the host list has passed "start". */
      if(item == start->prev) break;
      /* Matching rule atom 2 */
      /* Matching string variable 3. */
      if(item->atom.type != 's') break;
      result = addStringAssignment(morphism, 3, item->atom.str);
      if(result >= 0)
      {
         new_assignments += result;
         /* Update global booleans for the variable's predicates. */
         evaluatePredicate8(morphism);
         if(!evaluateCondition())
         {
            /* Reset the boolean variables in the predicates of this variable. */
            b8 = true;
            break;
         }
      }
      else break;
      item = item->prev;
      /* Matching list variable 1. */
      if(item == start->prev) result = addListAssignment(morphism, 1, NULL);
      else if(item == start)
      {
         if(item->atom.type == 'i') result = addIntegerAssignment(morphism, 1, item->atom.num);
         else result = addStringAssignment(morphism, 1, item->atom.str);
      }
      else
      {
         /* Assign to variable 1 the unmatched sublist of the host list. */
         HostAtom sublist[label.length - 1];
         int list_index = 0;
         HostListItem *iterator = start;
         while(iterator != item->next)
         {
            sublist[list_index++] = iterator->atom;
            iterator = iterator->next;
         }
         HostList *list = makeHostList(sublist, label.length - 1, false);
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
      addNodeMap(morphism, 1, host_node->index, new_assignments);
      host_node->matched = true;
      if(fillpot_n2(pot, morphism)) return true;
      else
      {
      removeNodeMap(morphism, 1);
      host_node->matched = false;
      }
   }
   else removeAssignments(morphism, new_assignments);
   return false;
}

static bool fillpot_n2(MorphismPot *pot, Morphism *morphism)
{
   RootNodes *nodes;
   for(nodes = getRootNodeList(host); nodes != NULL; nodes = nodes->next)
   {
      Node *host_node = getNode(host, nodes->index);
      if(host_node == NULL) continue;
      if(host_node->matched) continue;
      if(host_node->label.mark != 2) continue;
      if(host_node->indegree < 0 || host_node->outdegree < 0 ||
         ((host_node->outdegree + host_node->indegree - 0 - 0 - 0) < 0)) continue;

      HostLabel label = host_node->label;
      bool match = false;
      /* Label Matching */
      int new_assignments = 0;
      do
      {
         /* The rule list does not contain a list variable, so there is no
          * match if the host list has a different length. */
         if(label.length != 1) break;
         HostListItem *item = label.list->first;
         /* Matching rule atom 1. */
         int result = -1;
         /* Matching integer variable 5. */
         if(item->atom.type != 'i') break;
         result = addIntegerAssignment(morphism, 5, item->atom.num);
         if(result >= 0)
         {
            new_assignments += result;
            /* Update global booleans for the variable's predicates. */
            evaluatePredicate9(morphism);
            if(!evaluateCondition())
            {
               /* Reset the boolean variables in the predicates of this variable. */
               b9 = true;
               break;
            }
         }
         else break;
         match = true;
      } while(false);

      if(match)
      {
         addNodeMap(morphism, 2, host_node->index, new_assignments);
         host_node->matched = true;
         if(fillpot_n3(pot, morphism)) return true;
         else
         {
         removeNodeMap(morphism, 2);
         host_node->matched = false;
         }
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

static bool fillpot_n3(MorphismPot *pot, Morphism *morphism)
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
         /* The rule list does not contain a list variable, so there is no
          * match if the host list has a different length. */
         if(label.length != 3) break;
         HostListItem *item = label.list->first;
         /* Matching rule atom 1. */
         if(item->atom.type != 's') break;
         else if(strcmp(item->atom.str, "FunctionSet") != 0) break;
         item = item->next;

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

         /* Matching rule atom 3. */
         /* Matching integer variable 4. */
         if(item->atom.type != 'i') break;
         result = addIntegerAssignment(morphism, 4, item->atom.num);
         if(result >= 0)
         {
            new_assignments += result;
            /* Update global booleans for the variable's predicates. */
            evaluatePredicate9(morphism);
            if(!evaluateCondition())
            {
               /* Reset the boolean variables in the predicates of this variable. */
               b9 = true;
               break;
            }
         }
         else break;
         match = true;
      } while(false);

      if(match)
      {
         addNodeMap(morphism, 3, host_node->index, new_assignments);
         host_node->matched = true;
         /* All items matched! */
            putMorphism(pot, morphism, "Main_deleteEdgeToArity", host);
            removeNodeMap(morphism, 3);
            host_node->matched = false;
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

