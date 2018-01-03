#include "Main_demorgansB2.h"

bool neutral_b3 = true;

static bool evaluateCondition(void)
{
   return (neutral_b3);
}

static void evaluatePredicate3(Morphism *morphism)
{
   Assignment assignment_8 = getAssignment(morphism, 8);
   /* If the variable is not yet assigned, return. */
   if(assignment_8.type == 'n') return;
   string var_8 = getStringValue(morphism, 8);

   int list_var_length0 = 0;
   int list_length0 = list_var_length0 + 1;
   HostAtom array0[list_length0];
   int index0 = 0;

   array0[index0].type = 's';
   array0[index0++].str = var_8;

   int list_var_length1 = 0;
   int list_length1 = list_var_length1 + 1;
   HostAtom array1[list_length1];
   int index1 = 0;

   array1[index1].type = 's';
   array1[index1++].str = "IN";

   if(!equalHostLists(array0, array1, list_length0, list_length1)) neutral_b3 = true;
   else neutral_b3 = false;
}

static bool match_n0(Morphism *morphism);
static bool match_e1(Morphism *morphism);
static bool match_n2(Morphism *morphism, Edge *host_edge);
static bool match_e3(Morphism *morphism);
static bool match_n4(Morphism *morphism, Edge *host_edge);
static bool match_e0(Morphism *morphism);
static bool match_n1(Morphism *morphism, Edge *host_edge);
static bool match_e2(Morphism *morphism);
static bool match_n3(Morphism *morphism, Edge *host_edge);
static bool match_n5(Morphism *morphism);

bool matchMain_demorgansB2(Morphism *morphism)
{
   if(6 > host->number_of_nodes || 4 > host->number_of_edges) return false;
   if(match_n0(morphism)) return true;
   else
   {
      initialiseMorphism(morphism, host);
      return false;
   }
}

static bool match_n0(Morphism *morphism)
{
   int host_index;
   for(host_index = 0; host_index < host->nodes.size; host_index++)
   {
      Node *host_node = getNode(host, host_index);
      if(host_node == NULL || host_node->index == -1) continue;
      if(host_node->matched) continue;
      if(host_node->label.mark != 3) continue;
      if(host_node->indegree < 0 || host_node->outdegree < 2 ||
         ((host_node->outdegree + host_node->indegree - 2 - 0 - 0) < 0)) continue;

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
         if(item->atom.type != 's') break;
         else if(strcmp(item->atom.str, "AND") != 0) break;
         match = true;
      } while(false);

      if(match)
      {
         addNodeMap(morphism, 0, host_node->index, new_assignments);
         host_node->matched = true;
         if(match_e1(morphism)) return true;
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

static bool match_e1(Morphism *morphism)
{
   /* Start node is the already-matched node from which the candidate
      edges are drawn. End node may or may not have been matched already. */
   int start_index = lookupNode(morphism, 0);
   int end_index = lookupNode(morphism, 2);
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
      /* Match list variable 5 against the whole host list. */
      int result = -1;
      if(label.length == 1)
      {
         if(label.list->first->atom.type == 'i')
            result = addIntegerAssignment(morphism, 5, label.list->first->atom.num);
         else result = addStringAssignment(morphism, 5, label.list->first->atom.str);
      }
      else result = addListAssignment(morphism, 5, label.list);
      if(result >= 0)
      {
         new_assignments += result;
         match = true;
      }
      if(match)
      {
         addEdgeMap(morphism, 1, host_edge->index, new_assignments);
         host_edge->matched = true;
         if(match_n2(morphism, host_edge)) return true;
         else
         {
            removeEdgeMap(morphism, 1);
            host_edge->matched = false;
         }
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

static bool match_n2(Morphism *morphism, Edge *host_edge)
{
   Node *host_node = getTarget(host, host_edge);

   if(host_node->matched) return false;
   if(host_node->label.mark != 3) return false;
      if(host_node->indegree < 1 || host_node->outdegree < 1 ||
         ((host_node->outdegree + host_node->indegree - 1 - 1 - 0) < 0)) return false;;

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
      if(item->atom.type != 's') break;
      else if(strcmp(item->atom.str, "NOT") != 0) break;
      match = true;
   } while(false);

   if(match)
   {
      addNodeMap(morphism, 2, host_node->index, new_assignments);
      host_node->matched = true;
      if(match_e3(morphism)) return true;
      else
      {
      removeNodeMap(morphism, 2);
      host_node->matched = false;
      }
   }
   else removeAssignments(morphism, new_assignments);
   return false;
}

static bool match_e3(Morphism *morphism)
{
   /* Start node is the already-matched node from which the candidate
      edges are drawn. End node may or may not have been matched already. */
   int start_index = lookupNode(morphism, 2);
   int end_index = lookupNode(morphism, 4);
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
      /* Match list variable 7 against the whole host list. */
      int result = -1;
      if(label.length == 1)
      {
         if(label.list->first->atom.type == 'i')
            result = addIntegerAssignment(morphism, 7, label.list->first->atom.num);
         else result = addStringAssignment(morphism, 7, label.list->first->atom.str);
      }
      else result = addListAssignment(morphism, 7, label.list);
      if(result >= 0)
      {
         new_assignments += result;
         match = true;
      }
      if(match)
      {
         addEdgeMap(morphism, 3, host_edge->index, new_assignments);
         host_edge->matched = true;
         if(match_n4(morphism, host_edge)) return true;
         else
         {
            removeEdgeMap(morphism, 3);
            host_edge->matched = false;
         }
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

static bool match_n4(Morphism *morphism, Edge *host_edge)
{
   Node *host_node = getTarget(host, host_edge);

   if(host_node->matched) return false;
   if(host_node->label.mark != 3) return false;
      if(host_node->indegree < 1 || host_node->outdegree < 0 ||
         ((host_node->outdegree + host_node->indegree - 0 - 1 - 0) < 0)) return false;;

   HostLabel label = host_node->label;
   bool match = false;
   /* Label Matching */
   int new_assignments = 0;
   /* Match list variable 1 against the whole host list. */
   int result = -1;
   if(label.length == 1)
   {
      if(label.list->first->atom.type == 'i')
         result = addIntegerAssignment(morphism, 1, label.list->first->atom.num);
      else result = addStringAssignment(morphism, 1, label.list->first->atom.str);
   }
   else result = addListAssignment(morphism, 1, label.list);
   if(result >= 0)
   {
      new_assignments += result;
      match = true;
   }
   if(match)
   {
      addNodeMap(morphism, 4, host_node->index, new_assignments);
      host_node->matched = true;
      if(match_e0(morphism)) return true;
      else
      {
      removeNodeMap(morphism, 4);
      host_node->matched = false;
      }
   }
   else removeAssignments(morphism, new_assignments);
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
      /* Match list variable 4 against the whole host list. */
      int result = -1;
      if(label.length == 1)
      {
         if(label.list->first->atom.type == 'i')
            result = addIntegerAssignment(morphism, 4, label.list->first->atom.num);
         else result = addStringAssignment(morphism, 4, label.list->first->atom.str);
      }
      else result = addListAssignment(morphism, 4, label.list);
      if(result >= 0)
      {
         new_assignments += result;
         match = true;
      }
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
   if(host_node->label.mark != 3) return false;
      if(host_node->indegree < 1 || host_node->outdegree < 1 ||
         ((host_node->outdegree + host_node->indegree - 1 - 1 - 0) < 0)) return false;;

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
      if(item->atom.type != 's') break;
      else if(strcmp(item->atom.str, "NOT") != 0) break;
      match = true;
   } while(false);

   if(match)
   {
      addNodeMap(morphism, 1, host_node->index, new_assignments);
      host_node->matched = true;
      if(match_e2(morphism)) return true;
      else
      {
      removeNodeMap(morphism, 1);
      host_node->matched = false;
      }
   }
   else removeAssignments(morphism, new_assignments);
   return false;
}

static bool match_e2(Morphism *morphism)
{
   /* Start node is the already-matched node from which the candidate
      edges are drawn. End node may or may not have been matched already. */
   int start_index = lookupNode(morphism, 1);
   int end_index = lookupNode(morphism, 3);
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
      /* Match list variable 6 against the whole host list. */
      int result = -1;
      if(label.length == 1)
      {
         if(label.list->first->atom.type == 'i')
            result = addIntegerAssignment(morphism, 6, label.list->first->atom.num);
         else result = addStringAssignment(morphism, 6, label.list->first->atom.str);
      }
      else result = addListAssignment(morphism, 6, label.list);
      if(result >= 0)
      {
         new_assignments += result;
         match = true;
      }
      if(match)
      {
         addEdgeMap(morphism, 2, host_edge->index, new_assignments);
         host_edge->matched = true;
         if(match_n3(morphism, host_edge)) return true;
         else
         {
            removeEdgeMap(morphism, 2);
            host_edge->matched = false;
         }
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

static bool match_n3(Morphism *morphism, Edge *host_edge)
{
   Node *host_node = getTarget(host, host_edge);

   if(host_node->matched) return false;
   if(host_node->label.mark != 3) return false;
      if(host_node->indegree < 1 || host_node->outdegree < 0 ||
         ((host_node->outdegree + host_node->indegree - 0 - 1 - 0) < 0)) return false;;

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
      addNodeMap(morphism, 3, host_node->index, new_assignments);
      host_node->matched = true;
      if(match_n5(morphism)) return true;
      else
      {
      removeNodeMap(morphism, 3);
      host_node->matched = false;
      }
   }
   else removeAssignments(morphism, new_assignments);
   return false;
}

static bool match_n5(Morphism *morphism)
{
   int host_index;
   for(host_index = 0; host_index < host->nodes.size; host_index++)
   {
      Node *host_node = getNode(host, host_index);
      if(host_node == NULL || host_node->index == -1) continue;
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
         /* Matching string variable 8. */
         if(item->atom.type != 's') break;
         result = addStringAssignment(morphism, 8, item->atom.str);
         if(result >= 0)
         {
            new_assignments += result;
            /* Update global booleans for the variable's predicates. */
            evaluatePredicate3(morphism);
            if(!evaluateCondition())
            {
               /* Reset the boolean variables in the predicates of this variable. */
               neutral_b3 = true;
               break;
            }
         }
         else break;
         item = item->prev;
         /* Matching list variable 2. */
         if(item == start->prev) result = addListAssignment(morphism, 2, NULL);
         else if(item == start)
         {
            if(item->atom.type == 'i') result = addIntegerAssignment(morphism, 2, item->atom.num);
            else result = addStringAssignment(morphism, 2, item->atom.str);
         }
         else
         {
            /* Assign to variable 2 the unmatched sublist of the host list. */
            HostAtom sublist[label.length - 1];
            int list_index = 0;
            HostListItem *iterator = start;
            while(iterator != item->next)
            {
               sublist[list_index++] = iterator->atom;
               iterator = iterator->next;
            }
            HostList *list = makeHostList(sublist, label.length - 1, false);
            result = addListAssignment(morphism, 2, list);
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
         addNodeMap(morphism, 5, host_node->index, new_assignments);
         host_node->matched = true;
         /* All items matched! */
            return true;
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

void applyMain_demorgansB2(Morphism *morphism, bool record_changes)
{
   int host_edge_index = lookupEdge(morphism, 0);
   if(record_changes)
   {
      Edge *edge = getEdge(host, host_edge_index);
      /* A hole is created if the edge is not at the right-most index of the array. */
      pushRemovedEdge(edge->label, edge->source, edge->target, edge->index,
                      edge->index < host->edges.size - 1);
   }
   removeEdge(host, host_edge_index);

   host_edge_index = lookupEdge(morphism, 1);
   if(record_changes)
   {
      Edge *edge = getEdge(host, host_edge_index);
      /* A hole is created if the edge is not at the right-most index of the array. */
      pushRemovedEdge(edge->label, edge->source, edge->target, edge->index,
                      edge->index < host->edges.size - 1);
   }
   removeEdge(host, host_edge_index);

   int host_node_index = lookupNode(morphism, 0);
   HostLabel label_n0 = getNodeLabel(host, host_node_index);
   HostLabel label;
   int list_var_length0 = 0;
   int list_length0 = list_var_length0 + 1;
   HostAtom array0[list_length0];
   int index0 = 0;

   array0[index0].type = 's';
   array0[index0++].str = "NOT";
   if(list_length0 > 0)
   {
      HostList *list0 = makeHostList(array0, list_length0, false);
      label = makeHostLabel(3, list_length0, list0);
   }
   else label = makeEmptyLabel(3);

   if(equalHostLabels(label_n0, label)) removeHostList(label.list);
   else
   {
      if(record_changes) pushRelabelledNode(host_node_index, label_n0);
      relabelNode(host, host_node_index, label);
   }
   host_node_index = lookupNode(morphism, 5);
   HostLabel label_n5 = getNodeLabel(host, host_node_index);
   int list_var_length1 = 0;
   int list_length1 = list_var_length1 + 1;
   HostAtom array1[list_length1];
   int index1 = 0;

   array1[index1].type = 's';
   array1[index1++].str = "OR";
   if(list_length1 > 0)
   {
      HostList *list1 = makeHostList(array1, list_length1, false);
      label = makeHostLabel(1, list_length1, list1);
   }
   else label = makeEmptyLabel(1);

   if(equalHostLabels(label_n5, label)) removeHostList(label.list);
   else
   {
      if(record_changes) pushRelabelledNode(host_node_index, label_n5);
      relabelNode(host, host_node_index, label);
   }
   int source, target;
   int edge_array_size0 = host->edges.size;
   source = lookupNode(morphism, 0);
   target = lookupNode(morphism, 5);
   int list_var_length2 = 0;
   int list_length2 = list_var_length2 + 1;
   HostAtom array2[list_length2];
   int index2 = 0;

   array2[index2].type = 'i';
   array2[index2++].num = 0;
   if(list_length2 > 0)
   {
      HostList *list2 = makeHostList(array2, list_length2, false);
      label = makeHostLabel(0, list_length2, list2);
   }
   else label = makeEmptyLabel(0);

   host_edge_index = addEdge(host, label, source, target);
   /* If the edge array size has not increased after the edge addition, then
      the edge was added to a hole in the array. */
   if(record_changes)
      pushAddedEdge(host_edge_index, edge_array_size0 == host->edges.size);
   int edge_array_size3 = host->edges.size;
   source = lookupNode(morphism, 5);
   target = lookupNode(morphism, 3);
   int list_var_length3 = 0;
   int list_length3 = list_var_length3 + 1;
   HostAtom array3[list_length3];
   int index3 = 0;

   array3[index3].type = 'i';
   array3[index3++].num = 0;
   if(list_length3 > 0)
   {
      HostList *list3 = makeHostList(array3, list_length3, false);
      label = makeHostLabel(1, list_length3, list3);
   }
   else label = makeEmptyLabel(1);

   host_edge_index = addEdge(host, label, source, target);
   /* If the edge array size has not increased after the edge addition, then
      the edge was added to a hole in the array. */
   if(record_changes)
      pushAddedEdge(host_edge_index, edge_array_size3 == host->edges.size);
   int edge_array_size4 = host->edges.size;
   source = lookupNode(morphism, 5);
   target = lookupNode(morphism, 4);
   int list_var_length4 = 0;
   int list_length4 = list_var_length4 + 1;
   HostAtom array4[list_length4];
   int index4 = 0;

   array4[index4].type = 'i';
   array4[index4++].num = 1;
   if(list_length4 > 0)
   {
      HostList *list4 = makeHostList(array4, list_length4, false);
      label = makeHostLabel(1, list_length4, list4);
   }
   else label = makeEmptyLabel(1);

   host_edge_index = addEdge(host, label, source, target);
   /* If the edge array size has not increased after the edge addition, then
      the edge was added to a hole in the array. */
   if(record_changes)
      pushAddedEdge(host_edge_index, edge_array_size4 == host->edges.size);
   /* Reset the morphism. */
   initialiseMorphism(morphism, host);
}

static bool fillpot_n0(MorphismPot *pot, Morphism *morphism);
static bool fillpot_e1(MorphismPot *pot, Morphism *morphism);
static bool fillpot_n2(MorphismPot *pot, Morphism *morphism, Edge *host_edge);
static bool fillpot_e3(MorphismPot *pot, Morphism *morphism);
static bool fillpot_n4(MorphismPot *pot, Morphism *morphism, Edge *host_edge);
static bool fillpot_e0(MorphismPot *pot, Morphism *morphism);
static bool fillpot_n1(MorphismPot *pot, Morphism *morphism, Edge *host_edge);
static bool fillpot_e2(MorphismPot *pot, Morphism *morphism);
static bool fillpot_n3(MorphismPot *pot, Morphism *morphism, Edge *host_edge);
static bool fillpot_n5(MorphismPot *pot, Morphism *morphism);

bool fillpotMain_demorgansB2(MorphismPot *pot, Morphism *morphism)
{
   if(6 > host->number_of_nodes || 4 > host->number_of_edges) return false;
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
   int host_index;
   for(host_index = 0; host_index < host->nodes.size; host_index++)
   {
      Node *host_node = getNode(host, host_index);
      if(host_node == NULL || host_node->index == -1) continue;
      if(host_node->matched) continue;
      if(host_node->label.mark != 3) continue;
      if(host_node->indegree < 0 || host_node->outdegree < 2 ||
         ((host_node->outdegree + host_node->indegree - 2 - 0 - 0) < 0)) continue;

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
         if(item->atom.type != 's') break;
         else if(strcmp(item->atom.str, "AND") != 0) break;
         match = true;
      } while(false);

      if(match)
      {
         addNodeMap(morphism, 0, host_node->index, new_assignments);
         host_node->matched = true;
         if(fillpot_e1(pot, morphism)) return true;
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

static bool fillpot_e1(MorphismPot *pot, Morphism *morphism)
{
   /* Start node is the already-matched node from which the candidate
      edges are drawn. End node may or may not have been matched already. */
   int start_index = lookupNode(morphism, 0);
   int end_index = lookupNode(morphism, 2);
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
      /* Match list variable 5 against the whole host list. */
      int result = -1;
      if(label.length == 1)
      {
         if(label.list->first->atom.type == 'i')
            result = addIntegerAssignment(morphism, 5, label.list->first->atom.num);
         else result = addStringAssignment(morphism, 5, label.list->first->atom.str);
      }
      else result = addListAssignment(morphism, 5, label.list);
      if(result >= 0)
      {
         new_assignments += result;
         match = true;
      }
      if(match)
      {
         addEdgeMap(morphism, 1, host_edge->index, new_assignments);
         host_edge->matched = true;
         if(fillpot_n2(pot, morphism, host_edge)) return true;
         else
         {
            removeEdgeMap(morphism, 1);
            host_edge->matched = false;
         }
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

static bool fillpot_n2(MorphismPot *pot, Morphism *morphism, Edge *host_edge)
{
   Node *host_node = getTarget(host, host_edge);

   if(host_node->matched) return false;
   if(host_node->label.mark != 3) return false;
      if(host_node->indegree < 1 || host_node->outdegree < 1 ||
         ((host_node->outdegree + host_node->indegree - 1 - 1 - 0) < 0)) return false;;

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
      if(item->atom.type != 's') break;
      else if(strcmp(item->atom.str, "NOT") != 0) break;
      match = true;
   } while(false);

   if(match)
   {
      addNodeMap(morphism, 2, host_node->index, new_assignments);
      host_node->matched = true;
      if(fillpot_e3(pot, morphism)) return true;
      else
      {
      removeNodeMap(morphism, 2);
      host_node->matched = false;
      }
   }
   else removeAssignments(morphism, new_assignments);
   return false;
}

static bool fillpot_e3(MorphismPot *pot, Morphism *morphism)
{
   /* Start node is the already-matched node from which the candidate
      edges are drawn. End node may or may not have been matched already. */
   int start_index = lookupNode(morphism, 2);
   int end_index = lookupNode(morphism, 4);
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
      /* Match list variable 7 against the whole host list. */
      int result = -1;
      if(label.length == 1)
      {
         if(label.list->first->atom.type == 'i')
            result = addIntegerAssignment(morphism, 7, label.list->first->atom.num);
         else result = addStringAssignment(morphism, 7, label.list->first->atom.str);
      }
      else result = addListAssignment(morphism, 7, label.list);
      if(result >= 0)
      {
         new_assignments += result;
         match = true;
      }
      if(match)
      {
         addEdgeMap(morphism, 3, host_edge->index, new_assignments);
         host_edge->matched = true;
         if(fillpot_n4(pot, morphism, host_edge)) return true;
         else
         {
            removeEdgeMap(morphism, 3);
            host_edge->matched = false;
         }
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

static bool fillpot_n4(MorphismPot *pot, Morphism *morphism, Edge *host_edge)
{
   Node *host_node = getTarget(host, host_edge);

   if(host_node->matched) return false;
   if(host_node->label.mark != 3) return false;
      if(host_node->indegree < 1 || host_node->outdegree < 0 ||
         ((host_node->outdegree + host_node->indegree - 0 - 1 - 0) < 0)) return false;;

   HostLabel label = host_node->label;
   bool match = false;
   /* Label Matching */
   int new_assignments = 0;
   /* Match list variable 1 against the whole host list. */
   int result = -1;
   if(label.length == 1)
   {
      if(label.list->first->atom.type == 'i')
         result = addIntegerAssignment(morphism, 1, label.list->first->atom.num);
      else result = addStringAssignment(morphism, 1, label.list->first->atom.str);
   }
   else result = addListAssignment(morphism, 1, label.list);
   if(result >= 0)
   {
      new_assignments += result;
      match = true;
   }
   if(match)
   {
      addNodeMap(morphism, 4, host_node->index, new_assignments);
      host_node->matched = true;
      if(fillpot_e0(pot, morphism)) return true;
      else
      {
      removeNodeMap(morphism, 4);
      host_node->matched = false;
      }
   }
   else removeAssignments(morphism, new_assignments);
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
      /* Match list variable 4 against the whole host list. */
      int result = -1;
      if(label.length == 1)
      {
         if(label.list->first->atom.type == 'i')
            result = addIntegerAssignment(morphism, 4, label.list->first->atom.num);
         else result = addStringAssignment(morphism, 4, label.list->first->atom.str);
      }
      else result = addListAssignment(morphism, 4, label.list);
      if(result >= 0)
      {
         new_assignments += result;
         match = true;
      }
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
   if(host_node->label.mark != 3) return false;
      if(host_node->indegree < 1 || host_node->outdegree < 1 ||
         ((host_node->outdegree + host_node->indegree - 1 - 1 - 0) < 0)) return false;;

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
      if(item->atom.type != 's') break;
      else if(strcmp(item->atom.str, "NOT") != 0) break;
      match = true;
   } while(false);

   if(match)
   {
      addNodeMap(morphism, 1, host_node->index, new_assignments);
      host_node->matched = true;
      if(fillpot_e2(pot, morphism)) return true;
      else
      {
      removeNodeMap(morphism, 1);
      host_node->matched = false;
      }
   }
   else removeAssignments(morphism, new_assignments);
   return false;
}

static bool fillpot_e2(MorphismPot *pot, Morphism *morphism)
{
   /* Start node is the already-matched node from which the candidate
      edges are drawn. End node may or may not have been matched already. */
   int start_index = lookupNode(morphism, 1);
   int end_index = lookupNode(morphism, 3);
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
      /* Match list variable 6 against the whole host list. */
      int result = -1;
      if(label.length == 1)
      {
         if(label.list->first->atom.type == 'i')
            result = addIntegerAssignment(morphism, 6, label.list->first->atom.num);
         else result = addStringAssignment(morphism, 6, label.list->first->atom.str);
      }
      else result = addListAssignment(morphism, 6, label.list);
      if(result >= 0)
      {
         new_assignments += result;
         match = true;
      }
      if(match)
      {
         addEdgeMap(morphism, 2, host_edge->index, new_assignments);
         host_edge->matched = true;
         if(fillpot_n3(pot, morphism, host_edge)) return true;
         else
         {
            removeEdgeMap(morphism, 2);
            host_edge->matched = false;
         }
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

static bool fillpot_n3(MorphismPot *pot, Morphism *morphism, Edge *host_edge)
{
   Node *host_node = getTarget(host, host_edge);

   if(host_node->matched) return false;
   if(host_node->label.mark != 3) return false;
      if(host_node->indegree < 1 || host_node->outdegree < 0 ||
         ((host_node->outdegree + host_node->indegree - 0 - 1 - 0) < 0)) return false;;

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
      addNodeMap(morphism, 3, host_node->index, new_assignments);
      host_node->matched = true;
      if(fillpot_n5(pot, morphism)) return true;
      else
      {
      removeNodeMap(morphism, 3);
      host_node->matched = false;
      }
   }
   else removeAssignments(morphism, new_assignments);
   return false;
}

static bool fillpot_n5(MorphismPot *pot, Morphism *morphism)
{
   int host_index;
   for(host_index = 0; host_index < host->nodes.size; host_index++)
   {
      Node *host_node = getNode(host, host_index);
      if(host_node == NULL || host_node->index == -1) continue;
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
         /* Matching string variable 8. */
         if(item->atom.type != 's') break;
         result = addStringAssignment(morphism, 8, item->atom.str);
         if(result >= 0)
         {
            new_assignments += result;
            /* Update global booleans for the variable's predicates. */
            evaluatePredicate3(morphism);
            if(!evaluateCondition())
            {
               /* Reset the boolean variables in the predicates of this variable. */
               neutral_b3 = true;
               break;
            }
         }
         else break;
         item = item->prev;
         /* Matching list variable 2. */
         if(item == start->prev) result = addListAssignment(morphism, 2, NULL);
         else if(item == start)
         {
            if(item->atom.type == 'i') result = addIntegerAssignment(morphism, 2, item->atom.num);
            else result = addStringAssignment(morphism, 2, item->atom.str);
         }
         else
         {
            /* Assign to variable 2 the unmatched sublist of the host list. */
            HostAtom sublist[label.length - 1];
            int list_index = 0;
            HostListItem *iterator = start;
            while(iterator != item->next)
            {
               sublist[list_index++] = iterator->atom;
               iterator = iterator->next;
            }
            HostList *list = makeHostList(sublist, label.length - 1, false);
            result = addListAssignment(morphism, 2, list);
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
         addNodeMap(morphism, 5, host_node->index, new_assignments);
         host_node->matched = true;
         /* All items matched! */
            putMorphism(pot, morphism, "Main_demorgansB2", host);
            removeNodeMap(morphism, 5);
            host_node->matched = false;
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}
