#include "Main_commute.h"

bool b10 = true;

static bool evaluateCondition(void)
{
   return (b10);
}

static void evaluatePredicate10(Morphism *morphism)
{
   Assignment assignment_5 = getAssignment(morphism, 5);
   /* If the variable is not yet assigned, return. */
   if(assignment_5.type == 'n') return;
   string var_5 = getStringValue(morphism, 5);

   int list_var_length0 = 0;
   int list_length0 = list_var_length0 + 1;
   HostAtom array0[list_length0];
   int index0 = 0;

   array0[index0].type = 's';
   array0[index0++].str = var_5;

   int list_var_length1 = 0;
   int list_length1 = list_var_length1 + 1;
   HostAtom array1[list_length1];
   int index1 = 0;

   array1[index1].type = 's';
   b10 = false;

   array1[index1++].str = "AND";
   if(equalHostLists(array0, array1, list_length0, list_length1)) b10 = true;
   array1[index1 - 1].str = "OR";
   if(equalHostLists(array0, array1, list_length0, list_length1)) b10 = true;
   array1[index1 - 1].str = "NAND";
   if(equalHostLists(array0, array1, list_length0, list_length1)) b10 = true;
   array1[index1 - 1].str = "NOR";
   if(equalHostLists(array0, array1, list_length0, list_length1)) b10 = true;
   array1[index1 - 1].str = "XOR";
   if(equalHostLists(array0, array1, list_length0, list_length1)) b10 = true;
   array1[index1 - 1].str = "ADD";
   if(equalHostLists(array0, array1, list_length0, list_length1)) b10 = true;
   array1[index1 - 1].str = "MUL";
   if(equalHostLists(array0, array1, list_length0, list_length1)) b10 = true;
}

static bool match_n0(Morphism *morphism);
static bool match_e1(Morphism *morphism);
static bool match_n2(Morphism *morphism, Edge *host_edge);
static bool match_e0(Morphism *morphism);
static bool match_n1(Morphism *morphism, Edge *host_edge);

bool matchMain_commute(Morphism *morphism)
{
   if(3 > host->number_of_nodes || 2 > host->number_of_edges) return false;
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
         /* Matching string variable 5. */
         if(item->atom.type != 's') break;
         result = addStringAssignment(morphism, 5, item->atom.str);
         if(result >= 0)
         {
            new_assignments += result;
            /* Update global booleans for the variable's predicates. */
            evaluatePredicate10(morphism);
            if(!evaluateCondition())
            {
               /* Reset the boolean variables in the predicates of this variable. */
               b10 = true;
               break;
            }
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
      if(host_node->indegree < 1 || host_node->outdegree < 0 ||
         ((host_node->outdegree + host_node->indegree - 0 - 1 - 0) < 0)) return false;;

   HostLabel label = host_node->label;
   bool match = false;
   /* Label Matching */
   int new_assignments = 0;
   /* Match list variable 2 against the whole host list. */
   int result = -1;
   if(label.length == 1)
   {
      if(label.list->first->atom.type == 'i')
         result = addIntegerAssignment(morphism, 2, label.list->first->atom.num);
      else result = addStringAssignment(morphism, 2, label.list->first->atom.str);
   }
   else result = addListAssignment(morphism, 2, label.list);
   if(result >= 0)
   {
      new_assignments += result;
      match = true;
   }
   if(match)
   {
      addNodeMap(morphism, 2, host_node->index, new_assignments);
      host_node->matched = true;
      if(match_e0(morphism)) return true;
      else
      {
      removeNodeMap(morphism, 2);
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
      /* Match list variable 3 against the whole host list. */
      int result = -1;
      if(label.length == 1)
      {
         if(label.list->first->atom.type == 'i')
            result = addIntegerAssignment(morphism, 3, label.list->first->atom.num);
         else result = addStringAssignment(morphism, 3, label.list->first->atom.str);
      }
      else result = addListAssignment(morphism, 3, label.list);
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
      addNodeMap(morphism, 1, host_node->index, new_assignments);
      host_node->matched = true;
      /* All items matched! */
         return true;
   }
   else removeAssignments(morphism, new_assignments);
   return false;
}

void applyMain_commute(Morphism *morphism, bool record_changes)
{
   Assignment var_3 = getAssignment(morphism, 3);
   Assignment var_4 = getAssignment(morphism, 4);
   int host_edge_index = lookupEdge(morphism, 0);
   HostLabel label_e0 = getEdgeLabel(host, host_edge_index);
   HostLabel label;
   int list_var_length0 = 0;
   list_var_length0 += getAssignmentLength(var_4);
   int list_length0 = list_var_length0 + 0;
   HostAtom array0[list_length0];
   int index0 = 0;

   if(var_4.type == 'l' && var_4.list != NULL)
   {
      HostListItem *item0 = var_4.list->first;
      while(item0 != NULL)
      {
         array0[index0++] = item0->atom;
         item0 = item0->next;
      }
   }
   else if(var_4.type == 'i')
   {
      array0[index0].type = 'i';
      array0[index0++].num = var_4.num;
   }
   else if(var_4.type == 's')
   {
      array0[index0].type = 's';
      array0[index0++].str = var_4.str;
   }

   if(list_length0 > 0)
   {
      HostList *list0 = makeHostList(array0, list_length0, false);
      label = makeHostLabel(0, list_length0, list0);
   }
   else label = makeEmptyLabel(0);

   /* Relabel the edge if its label is not equal to the RHS label. */
   if(equalHostLabels(label_e0, label)) removeHostList(label.list);
   else
   {
      if(record_changes) pushRelabelledEdge(host_edge_index, label_e0);
      relabelEdge(host, host_edge_index, label);
   }
   host_edge_index = lookupEdge(morphism, 1);
   HostLabel label_e1 = getEdgeLabel(host, host_edge_index);
   int list_var_length1 = 0;
   list_var_length1 += getAssignmentLength(var_3);
   int list_length1 = list_var_length1 + 0;
   HostAtom array1[list_length1];
   int index1 = 0;

   if(var_3.type == 'l' && var_3.list != NULL)
   {
      HostListItem *item1 = var_3.list->first;
      while(item1 != NULL)
      {
         array1[index1++] = item1->atom;
         item1 = item1->next;
      }
   }
   else if(var_3.type == 'i')
   {
      array1[index1].type = 'i';
      array1[index1++].num = var_3.num;
   }
   else if(var_3.type == 's')
   {
      array1[index1].type = 's';
      array1[index1++].str = var_3.str;
   }

   if(list_length1 > 0)
   {
      HostList *list1 = makeHostList(array1, list_length1, false);
      label = makeHostLabel(0, list_length1, list1);
   }
   else label = makeEmptyLabel(0);

   /* Relabel the edge if its label is not equal to the RHS label. */
   if(equalHostLabels(label_e1, label)) removeHostList(label.list);
   else
   {
      if(record_changes) pushRelabelledEdge(host_edge_index, label_e1);
      relabelEdge(host, host_edge_index, label);
   }
   /* Reset the morphism. */
   initialiseMorphism(morphism, host);
}

static bool fillpot_n0(MorphismPot *pot, Morphism *morphism);
static bool fillpot_e1(MorphismPot *pot, Morphism *morphism);
static bool fillpot_n2(MorphismPot *pot, Morphism *morphism, Edge *host_edge);
static bool fillpot_e0(MorphismPot *pot, Morphism *morphism);
static bool fillpot_n1(MorphismPot *pot, Morphism *morphism, Edge *host_edge);

bool fillpotMain_commute(MorphismPot *pot, Morphism *morphism)
{
   if(3 > host->number_of_nodes || 2 > host->number_of_edges) return false;
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
         /* Matching string variable 5. */
         if(item->atom.type != 's') break;
         result = addStringAssignment(morphism, 5, item->atom.str);
         if(result >= 0)
         {
            new_assignments += result;
            /* Update global booleans for the variable's predicates. */
            evaluatePredicate10(morphism);
            if(!evaluateCondition())
            {
               /* Reset the boolean variables in the predicates of this variable. */
               b10 = true;
               break;
            }
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
      if(host_node->indegree < 1 || host_node->outdegree < 0 ||
         ((host_node->outdegree + host_node->indegree - 0 - 1 - 0) < 0)) return false;;

   HostLabel label = host_node->label;
   bool match = false;
   /* Label Matching */
   int new_assignments = 0;
   /* Match list variable 2 against the whole host list. */
   int result = -1;
   if(label.length == 1)
   {
      if(label.list->first->atom.type == 'i')
         result = addIntegerAssignment(morphism, 2, label.list->first->atom.num);
      else result = addStringAssignment(morphism, 2, label.list->first->atom.str);
   }
   else result = addListAssignment(morphism, 2, label.list);
   if(result >= 0)
   {
      new_assignments += result;
      match = true;
   }
   if(match)
   {
      addNodeMap(morphism, 2, host_node->index, new_assignments);
      host_node->matched = true;
      if(fillpot_e0(pot, morphism)) return true;
      else
      {
      removeNodeMap(morphism, 2);
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
      /* Match list variable 3 against the whole host list. */
      int result = -1;
      if(label.length == 1)
      {
         if(label.list->first->atom.type == 'i')
            result = addIntegerAssignment(morphism, 3, label.list->first->atom.num);
         else result = addStringAssignment(morphism, 3, label.list->first->atom.str);
      }
      else result = addListAssignment(morphism, 3, label.list);
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
      addNodeMap(morphism, 1, host_node->index, new_assignments);
      host_node->matched = true;
      /* All items matched! */
         putMorphism(pot, morphism, "Main_commute", host);
         removeNodeMap(morphism, 1);
         host_node->matched = false;
   }
   else removeAssignments(morphism, new_assignments);
   return false;
}
