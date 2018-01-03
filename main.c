#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "common.h"
#include "debug.h"
#include "graph.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

#include "Main_initNodeCounter.h"
Morphism *M_Main_initNodeCounter = NULL;
#include "Main_addNode.h"
Morphism *M_Main_addNode = NULL;
#include "Main_addEdgeToArity.h"
Morphism *M_Main_addEdgeToArity = NULL;
#include "Main_decolourNode.h"
Morphism *M_Main_decolourNode = NULL;
#include "Main_connectOutput.h"
Morphism *M_Main_connectOutput = NULL;
#include "Main_deleteNodeCounter.h"
Morphism *M_Main_deleteNodeCounter = NULL;

#include "Main_clearTargetMark.h"
Morphism *M_Main_clearTargetMark = NULL;
#include "Main_clearTargetEdge.h"
Morphism *M_Main_clearTargetEdge = NULL;
#include "Main_initMutationCounter.h"
Morphism *M_Main_initMutationCounter = NULL;
#include "Main_checkMutationCounter.h"
Morphism *M_Main_checkMutationCounter = NULL;
#include "Main_deleteMutationCounter.h"
Morphism *M_Main_deleteMutationCounter = NULL;
#include "Main_pickEdgeMutate.h"
Morphism *M_Main_pickEdgeMutate = NULL;
#include "Main_pickFuncMutate.h"
Morphism *M_Main_pickFuncMutate = NULL;
#include "Main_pickedEdge.h"
Morphism *M_Main_pickedEdge = NULL;
#include "Main_pickedNode.h"
Morphism *M_Main_pickedNode = NULL;
#include "Main_pickEdge.h"
Morphism *M_Main_pickEdge = NULL;
#include "Main_markOutputs.h"
Morphism *M_Main_markOutputs = NULL;
#include "Main_pickNewTarget.h"
Morphism *M_Main_pickNewTarget = NULL;
#include "Main_mutateEdge.h"
Morphism *M_Main_mutateEdge = NULL;
#include "Main_unmarkOutputs.h"
Morphism *M_Main_unmarkOutputs = NULL;
#include "Main_pickNode.h"
Morphism *M_Main_pickNode = NULL;
#include "Main_mutateNode.h"
Morphism *M_Main_mutateNode = NULL;
#include "Main_addEdgeToArityBlue.h"
Morphism *M_Main_addEdgeToArityBlue = NULL;
#include "Main_addEdgeCounter.h"
Morphism *M_Main_addEdgeCounter = NULL;
#include "Main_deleteEdgeToArity.h"
Morphism *M_Main_deleteEdgeToArity = NULL;
#include "Main_deleteEdgeCounter.h"
Morphism *M_Main_deleteEdgeCounter = NULL;
#include "Main_unmarkActive.h"
Morphism *M_Main_unmarkActive = NULL;


#include "Main_markOutputNode.h"
Morphism *M_Main_markOutputNode = NULL;
#include "Main_markActiveNode.h"
Morphism *M_Main_markActiveNode = NULL;
#include "Main_demorgansF1.h"
Morphism *M_Main_demorgansF1 = NULL;
#include "Main_demorgansB1.h"
Morphism *M_Main_demorgansB1 = NULL;
#include "Main_demorgansB2.h"
Morphism *M_Main_demorgansB2 = NULL;
#include "Main_demorgansF2.h"
Morphism *M_Main_demorgansF2 = NULL;
#include "Main_doubNegF.h"
Morphism *M_Main_doubNegF = NULL;
#include "Main_doubNegB.h"
Morphism *M_Main_doubNegB = NULL;
#include "Main_commute.h"
Morphism *M_Main_commute = NULL;
#include "Main_unmarkNode.h"
Morphism *M_Main_unmarkNode = NULL;
#include "Main_deleteOldAEdge.h"
Morphism *M_Main_deleteOldAEdge = NULL;
#include "Main_deleteOldNEdge.h"
Morphism *M_Main_deleteOldNEdge = NULL;
#include "Main_unmarkEdge.h"
Morphism *M_Main_unmarkEdge = NULL;

#include <time.h>

static double totalTime = 0.0;
static long evals = 0;
static double totalPopTime = 0.0;
static long repops = 0;
static double totalMutateTime = 0.0;
static long mutates = 0;
static struct timespec before, after;

static Graph* generateIndividual(char* functionSet[], int functionArities[], int functions, int nodes, int inputs, int outputs);
static Graph* mutateIndividual(Graph* host, char* functionSet[], int functionArities[], int functions, double mutation_rate);
static Graph* neutralIndividual(Graph* host);
static Graph* buildEmptyHostGraph();
static void freeMorphisms_GenerateIndividual(void);
static void garbageCollect_GenerateIndividual(void);
static void freeMorphisms_MutateIndividual(void);
static void garbageCollect_MutateIndividual(void);
static void freeMorphisms_Neutral(void);
static void garbageCollect_Neutral(void);
static void evaluate_population(Graph* population[], double scores[], int lambda, int winnerIndex, int inputs, int outputs, int rows, double data[rows][inputs + outputs]);
static double evaluate(Graph* host, int inputs, int outputs, int rows, double data[rows][inputs + outputs]);
static double worstCase(Graph* host, int inputs, int outputs, int rows, double data[rows][inputs + outputs]);
static Graph* duplicateGraph(Graph* host);
static void repopulate(Graph* population[], int lambda, int winnerIndex, char* functionSet[], int functionArities[], int functions, double mutation_rate);
static void loadDataSet(char* file, int inputs, int outputs, int rows, double data[rows][inputs + outputs]);
static void printFunction(FILE *file, Graph* hostG, Node *node, bool top);
static void printfFunction(Graph* hostG, Node *node, bool top);
static void printFunctionFlat(Graph* hostG, Node *node, bool top);
static void markActiveBlue(Graph* hostG);
static void markActiveChildrenBlue(Graph* hostG, Node *node);
static void markParentsBlue(Graph* hostG, Node *node);
static void unmarkGraph(Graph* hostG);
static int countActiveNodes(Graph* hostG);
static void loadFunctionSet(char* fsetV, char* functionSet[]);
static int countFunctions(char* fsetV);
static void loadFunctionArities(char* fsetV, int functionArities[]);

static Graph* duplicateGraph(Graph *graph)
{
	clock_gettime(CLOCK_REALTIME, &before);
   Graph *graph_copy = newGraph(graph->nodes.capacity, graph->edges.capacity);
	 int node_mapping[graph->nodes.size];
	 //Copy nodes
	 for(int i = 0; i < graph->nodes.size; i++){
			Node *host_node = getNode(graph, i);
			if(host_node == NULL || host_node->index == -1) continue;
			node_mapping[i] = addNode(graph_copy, host_node->root, makeHostLabel(host_node->label.mark, host_node->label.length, copyHostList(host_node->label.list)));
	 }
	 //Copy edges
	 for(int i = 0; i < graph->edges.size; i++){
			Edge *host_edge = getEdge(graph, i);
			if(host_edge == NULL || host_edge->index == -1) continue;
			addEdge(graph_copy, makeHostLabel(host_edge->label.mark, host_edge->label.length, copyHostList(host_edge->label.list)), node_mapping[host_edge->source], node_mapping[host_edge->target]);
	 }
 	clock_gettime(CLOCK_REALTIME, &after);
 	totalPopTime += (double)(((after.tv_sec - before.tv_sec) * 1000) + ((double)(after.tv_nsec - before.tv_nsec) / (double)1000000)) / (double)1000;
 	repops = repops + 1.0;
	 return graph_copy;
}

static void freeMorphisms_GenerateIndividual(void)
{
   freeMorphism(M_Main_initNodeCounter);
   freeMorphism(M_Main_addNode);
   freeMorphism(M_Main_addEdgeToArity);
   freeMorphism(M_Main_decolourNode);
   freeMorphism(M_Main_connectOutput);
   freeMorphism(M_Main_deleteNodeCounter);
}


static void freeMorphisms_MutateIndividual(void)
{
   freeMorphism(M_Main_clearTargetMark);
   freeMorphism(M_Main_clearTargetEdge);
   freeMorphism(M_Main_initMutationCounter);
   freeMorphism(M_Main_checkMutationCounter);
   freeMorphism(M_Main_deleteMutationCounter);
   freeMorphism(M_Main_pickEdgeMutate);
   freeMorphism(M_Main_pickFuncMutate);
   freeMorphism(M_Main_pickedEdge);
   freeMorphism(M_Main_pickedNode);
   freeMorphism(M_Main_pickEdge);
   freeMorphism(M_Main_markOutputs);
   freeMorphism(M_Main_pickNewTarget);
   freeMorphism(M_Main_mutateEdge);
   freeMorphism(M_Main_unmarkOutputs);
   freeMorphism(M_Main_pickNode);
   freeMorphism(M_Main_mutateNode);
   freeMorphism(M_Main_unmarkActive);
   freeMorphism(M_Main_addEdgeToArityBlue);
   freeMorphism(M_Main_addEdgeCounter);
   freeMorphism(M_Main_deleteEdgeToArity);
   freeMorphism(M_Main_deleteEdgeCounter);
}

static void freeMorphisms_Neutral(void)
{
   freeMorphism(M_Main_markOutputNode);
   freeMorphism(M_Main_markActiveNode);
   freeMorphism(M_Main_demorgansF1);
   freeMorphism(M_Main_demorgansB1);
   freeMorphism(M_Main_demorgansB2);
   freeMorphism(M_Main_demorgansF2);
   freeMorphism(M_Main_doubNegF);
   freeMorphism(M_Main_doubNegB);
   freeMorphism(M_Main_commute);
   freeMorphism(M_Main_unmarkNode);
   freeMorphism(M_Main_deleteOldAEdge);
   freeMorphism(M_Main_deleteOldNEdge);
   freeMorphism(M_Main_unmarkEdge);
}

Graph *host = NULL;
int *node_map = NULL;

MorphismPot *pot = NULL;
static void garbageCollect_GenerateIndividual(void)
{
   freeMorphisms_GenerateIndividual();
   freePot(pot);
}


static void garbageCollect_MutateIndividual(void)
{
   freeMorphisms_MutateIndividual();
   freePot(pot);
}


static void garbageCollect_Neutral(void)
{
   freeMorphisms_Neutral();
   freePot(pot);
}

static void loadDataSet(char* file, int inputs, int outputs, int rows, double data[rows][inputs + outputs]){
  FILE* fp = fopen(file, "r");
  if(fp == NULL){
      printf("Error: file %s cannot be found.\n", file);
      exit(0);
  }
  for (int i = 0; i < rows; i++){
    for(int j = 0; j < (inputs + outputs); j++){
      int r = fscanf(fp," %lf %*c",&data[i][j]);
      if(r != 1){
        printf("Error: file %s does not match the given row and columns dimensions.\n", file);
        exit(0);
      }
    }
  }
}

static Graph *buildEmptyHostGraph()
{
   Graph* new_host = newGraph(100000, 100000);
   node_map = calloc(100000, sizeof(int));
   if(node_map == NULL)
   {
      freeGraph(new_host);
      return NULL;
   }
   return new_host;
}

bool success = true;

int main(int argc, char **argv)
{
    srand(time(NULL));
    int lambda = 4;
    int nodes = atoi(argv[1]);
    int inputs = atoi(argv[2]);
    int outputs = atoi(argv[3]);
    int rows = atoi(argv[4]);
    int functions = 4;
    double mutation_rate = atof(argv[5]);
		int generations = atoi(argv[6]);

    char* fsetV = argv[7];
    int fCount = countFunctions(fsetV);
    char* functionSet[fCount];
    int functionArities[fCount];
    loadFunctionSet(fsetV, functionSet);
    loadFunctionArities(fsetV, functionArities);

		int update = 1;
    double data[rows][inputs + outputs];
    loadDataSet(argv[8], inputs, outputs, rows, data);
    struct Graph* population[lambda + 1];
    for(int i = 0; i < lambda + 1; i++){
      population[i] = NULL;
    }
    int winnerIndex = 0;
    population[0] = generateIndividual(functionSet, functionArities, fCount, nodes, inputs, outputs);
    double initScore = evaluate(population[0], inputs, outputs, rows, data);
    printf("Initial score %lf\n", initScore);
		double scores[lambda + 1];
		scores[0] = initScore;
		int gen = 0;
		double targetScore = 0.01;
		double winningscore = initScore;
    bool finished = false;
		while(gen < generations && !finished){
	    repopulate(population, lambda, winnerIndex, functionSet, functionArities, fCount, mutation_rate);
			evaluate_population(population, scores, lambda, winnerIndex, inputs, outputs, rows, data);
			int oldWinner = winnerIndex;
			double neutrals = 0.0;
			// Graph* neutral = neutralIndividual(duplicateGraph(population[winnerIndex]));
			// printf("Evaluating neutral...");
			// double neutral_score = evaluate(neutral, inputs, outputs, rows, data);
			// if (neutral_score != winningscore){
			// 	printf("Failed neutral mutation\n");
			// 	printf("Old host\n");
			// 	printfGraph(population[winnerIndex]);
			// 	printf("New host\n");
			// 	printfGraph(neutral);
			// 	freeGraph(neutral);
			// }
			// else{
			// 	freeGraph(population[winnerIndex]);
			// 	population[winnerIndex] = neutral;
			// }
			neutralIndividual(population[winnerIndex]);
			printf("Evaluating parent... \n");
			double pscore = evaluate(population[winnerIndex], inputs, outputs, rows, data);
			printf("%lf\n", pscore);
		  for(int i = 0; i < lambda + 1; i++){
					if(i != oldWinner){
						if(scores[i] < winningscore){
								winningscore = scores[i];
								winnerIndex = i;
								neutrals = 0.0;
						}
						else if (scores[i] == winningscore){
							double r = ((double)rand() / (double)RAND_MAX);
							neutrals = neutrals + 1.0;
							if(neutrals == 0.0 || r <= (1.0 / neutrals)){
								winningscore = scores[i];
								winnerIndex = i;
							}
						}
					}
			}
      double worst_case = worstCase(population[winnerIndex], inputs, outputs, rows, data);
      if(worst_case <= targetScore){
        finished = true;

      }
			if(gen%update == 0 || finished){
							int active = countActiveNodes(population[winnerIndex]);
							printf("Re-valuate %d: %lf\n", winnerIndex, evaluate(population[winnerIndex], inputs, outputs, rows, data));
							printf("Generation %d score %lf (%lf worst example) winner %d of size %d\n", gen, winningscore, worst_case, winnerIndex, active);
			}
			gen++;
		}

		int active = countActiveNodes(population[winnerIndex]);
		host = population[winnerIndex];
		FILE *output_file = fopen("edg.output", "w");

		fprintf(output_file, "%d, %lf, %d~\n", gen, winningscore, active);

		printGraph(host, output_file);
		fprintf(output_file, "~\n");

		printf("Individual %d score %lf\n", winnerIndex, winningscore);
		printf("Size %d\n", active);
		for(int host_index = 0; host_index < host->nodes.size; host_index++)
		{
			 Node *host_node = getNode(host, host_index);
			 if(host_node == NULL || host_node->index == -1) continue;

			 HostLabel label = host_node->label;
			 if(label.length != 2) break;

			 HostListItem *item = label.list->first;
			 HostListItem *item2 = item->next;
			 if(item->atom.type != 'i') break;
			 if(item2->atom.type != 's') break;
			 if(strcmp(item2->atom.str, "OUT") == 0){
				 	fprintf(output_file, "o%d: ", label.list->first->atom.num);
					printFunction(output_file, host, host_node, true);
					fprintf(output_file, "~\n");
			 }
		}
	  for(int i = 0; i < lambda + 1; i++){
	    freeGraph(population[i]);
	    population[i] = NULL;
	  }
    freeGraphChangeStack();
		printf("%lfs\n", totalTime / (double)evals);
		printf("%lfs\n", totalPopTime / (double)repops);
		printf("%lfs\n", totalMutateTime / (double)mutates);
}

static void loadFunctionSet(char* fsetV, char* functionSet[]){
    int f = 0;
    int i = 0;
    int o = 0;
    while(o < (int)strlen(fsetV)){
      if(fsetV[o] == ',' || o == (int)strlen(fsetV) - 1){
        char* name;
        if(o == (int)strlen(fsetV) - 1){
          name = malloc((o + 1 - i) * sizeof(char));
          for(int j = i; j <= o; j++){
            name[j - i] = fsetV[j];
          }
        }
        else{
          name = malloc((o - i) * sizeof(char));
          for(int j = i; j < o; j++){
            name[j - i] = fsetV[j];
          }
        }
        if(strcmp(name, "and") == 0){
          functionSet[f] = "AND";
          printf("Function set: AND\n");
        }
        else if(strcmp(name, "or") == 0){
          functionSet[f] = "OR";
          printf("Function set: OR\n");
        }
        else if(strcmp(name, "nand") == 0){
          functionSet[f] = "NAND";
          printf("Function set: NAND\n");
        }
        else if(strcmp(name, "nor") == 0){
          functionSet[f] = "NOR";
          printf("Function set: NOR\n");
        }
        else if(strcmp(name, "not") == 0){
          functionSet[f] = "NOT";
          printf("Function set: NOT\n");
        }
        else if(strcmp(name, "xor") == 0){
          functionSet[f] = "XOR";
          printf("Function set: XOR\n");
        }
        else if(strcmp(name, "andi") == 0){
          functionSet[f] = "ANDI";
          printf("Function set: ANDI\n");
        }
        else if(strcmp(name, "add") == 0){
          functionSet[f] = "ADD";
          printf("Function set: ADD\n");
        }
        else if(strcmp(name, "sub") == 0){
          functionSet[f] = "SUB";
          printf("Function set: SUB\n");
        }
        else if(strcmp(name, "div") == 0){
          functionSet[f] = "DIV";
          printf("Function set: DIV\n");
        }
        else if(strcmp(name, "mul") == 0){
          functionSet[f] = "MUL";
          printf("Function set: MUL\n");
        }
        else if(strcmp(name, "log") == 0){
          functionSet[f] = "LOG";
          printf("Function set: LOG\n");
        }
        else if(strcmp(name, "sin") == 0){
          functionSet[f] = "SIN";
          printf("Function set: SIN\n");
        }
        else if(strcmp(name, "cos") == 0){
          functionSet[f] = "COS";
          printf("Function set: COS\n");
        }
        else if(strcmp(name, "exp") == 0){
          functionSet[f] = "EXP";
          printf("Function set: EXP\n");
        }
        else if(strcmp(name, "id") == 0){
          functionSet[f] = "ID";
          printf("Function set: ID\n");
        }
        i = o + 1;
        f++;
      }
      o++;
    }
}
static int countFunctions(char* fsetV){
  int o = 0;
  int f = 0;
  while(o < (int)strlen(fsetV)){
    if(fsetV[o] == ',' || o == (int)strlen(fsetV) - 1){
      f++;
    }
    o++;
  }
  return f;
}
static void loadFunctionArities(char* fsetV, int functionArities[]){
    int f = 0;
    int i = 0;
    int o = 0;
    while(o < (int)strlen(fsetV)){
      if(fsetV[o] == ',' || o == (int)strlen(fsetV) - 1){
        char* name;
        if(o == (int)strlen(fsetV) - 1){
          name = malloc((o + 1 - i) * sizeof(char));
          for(int j = i; j <= o; j++){
            name[j - i] = fsetV[j];
          }
        }
        else{
          name = malloc((o - i) * sizeof(char));
          for(int j = i; j < o; j++){
            name[j - i] = fsetV[j];
          }
        }
        if(strcmp(name, "and") == 0){
          functionArities[f] = 2;
        }
        else if(strcmp(name, "or") == 0){
          functionArities[f] = 2;
        }
        else if(strcmp(name, "nand") == 0){
          functionArities[f] = 2;
        }
        else if(strcmp(name, "nor") == 0){
          functionArities[f] = 2;
        }
        else if(strcmp(name, "not") == 0){
          functionArities[f] = 1;
        }
        else if(strcmp(name, "xor") == 0){
          functionArities[f] = 2;
        }
        else if(strcmp(name, "andi") == 0){
          functionArities[f] = 2;
        }
        else if(strcmp(name, "add") == 0){
          functionArities[f] = 2;
        }
        else if(strcmp(name, "sub") == 0){
          functionArities[f] = 2;
        }
        else if(strcmp(name, "div") == 0){
          functionArities[f] = 2;
        }
        else if(strcmp(name, "mul") == 0){
          functionArities[f] = 2;
        }
        else if(strcmp(name, "log") == 0){
          functionArities[f] = 1;
        }
        else if(strcmp(name, "sin") == 0){
          functionArities[f] = 1;
        }
        else if(strcmp(name, "cos") == 0){
          functionArities[f] = 1;
        }
        else if(strcmp(name, "exp") == 0){
          functionArities[f] = 1;
        }
        else if(strcmp(name, "id") == 0){
          functionArities[f] = 1;
        }
        i = o + 1;
        f++;
      }
      o++;
    }
}

static void markActiveBlue(Graph* hostG){

	for(int host_index = 0; host_index < hostG->nodes.size; host_index++)
	{
		 Node *host_node = getNode(hostG, host_index);
		 if(host_node == NULL || host_node->index == -1) continue;

		 HostLabel label = host_node->label;
		 if(label.length != 2) break;

		 HostListItem *item = label.list->first;
		 HostListItem *item2 = item->next;
		 if(item->atom.type != 'i') break;
		 if(item2->atom.type != 's') break;
		 if(strcmp(item2->atom.str, "OUT") == 0){
	   	 changeNodeMark(hostG, host_index, 3);
			 markActiveChildrenBlue(hostG, host_node);
		 }
	}
}

static void markActiveChildrenBlue(Graph* hostG, Node *node){

	int counter;
	for(counter = 0; counter < node->out_edges.size + 2; counter++)
	{
		Edge *host_edge = getNthOutEdge(hostG, node, counter);
		if(host_edge == NULL || host_edge->index == -1) continue;
		Node *target = getNode(hostG, host_edge->target);
		HostLabel label = target->label;
		if(label.mark == 0){
 	   	changeNodeMark(hostG, target->index, 3);
			markActiveChildrenBlue(hostG, target);
		}
	}
}


static void markParentsBlue(Graph* hostG, Node *node){

	int counter;
	for(counter = 0; counter < node->in_edges.size + 2; counter++)
	{
		Edge *host_edge = getNthInEdge(hostG, node, counter);
		if(host_edge == NULL || host_edge->index == -1) continue;
		Node *source = getNode(hostG, host_edge->source);
		HostLabel label = source->label;
		if(label.mark == 0){
 	   	changeNodeMark(hostG, source->index, 3);
			markParentsBlue(hostG, source);
		}
	}
}

static void unmarkGraph(Graph* hostG){

		for(int host_index = 0; host_index < hostG->nodes.size; host_index++)
		{
			 Node *host_node = getNode(hostG, host_index);
			 if(host_node == NULL || host_node->index == -1) continue;
			 if(host_node->label.mark != 0){
		   	changeNodeMark(hostG, host_index, 0);
				if(host_node->root){
					host_node->root = false;
				}
			}
		}
}

static int countActiveNodes(Graph* hostG){
	markActiveBlue(hostG);
	int count = 0;
	for(int host_index = 0; host_index < hostG->nodes.size; host_index++)
	{
		 Node *host_node = getNode(hostG, host_index);
		 if(host_node == NULL || host_node->index == -1) continue;
		 HostLabel label = host_node->label;
		 if(label.mark > 0){
		 	count++;
		 }
	}
	unmarkGraph(hostG);
	return count;
}

static void printfFunction(Graph* hostG, Node *node, bool top){
	      HostLabel label = node->label;
	      HostListItem *item = label.list->last;
	      char* function = item->atom.str;
				if(strcmp(item->atom.str, "IN") == 0){
	      	HostListItem *item2 = label.list->first;
					printf("%s-%d", function, item2->atom.num);
				}
				else{
								printf("%s( ", function);
		      int counter;
					bool first = true;
		      for(counter = 0; counter < node->out_edges.size + 2; counter++)
		      {
					  Edge *host_edge = getNthOutEdge(host, node, counter);
						if(host_edge == NULL || host_edge->index == -1) continue;
						if(!first){
							printf(", ");
						}
						else{
							first = false;
						}
						printfFunction(host, getNode(hostG, host_edge->target), false);
					}
					if(top){
						printf(")\n");
					}
					else{
						printf(")");
					}
			}
}


static void printFunction(FILE *file, Graph* hostG, Node *node, bool top){
	      HostLabel label = node->label;
	      HostListItem *item = label.list->last;
	      char* function = item->atom.str;
				if(strcmp(item->atom.str, "IN") == 0){
	      	HostListItem *item2 = label.list->first;
					fprintf(file, "i%d", item2->atom.num);
				}
				else{
								fprintf(file, "%s( ", function);
		      int counter;
					bool first = true;
		      for(counter = 0; counter < node->out_edges.size + 2; counter++)
		      {
					  Edge *host_edge = getNthOutEdge(host, node, counter);
						if(host_edge == NULL || host_edge->index == -1) continue;
						if(!first){
							fprintf(file, ", ");
						}
						else{
							first = false;
						}
						printFunction(file, host, getNode(hostG, host_edge->target), false);
					}
					if(top){
						fprintf(file, ")\n");
					}
					else{
						fprintf(file, ")");
					}
			}
}



static void printFunctionFlat(Graph* hostG, Node *node, bool top){
	      HostLabel label = node->label;
	      HostListItem *item = label.list->last;
	      char* function = item->atom.str;
				if(strcmp(item->atom.str, "IN") == 0){
	      	HostListItem *item2 = label.list->first;
					printf("%s-%d", function, item2->atom.num);
				}
				else{
								printf("( ");
		      int counter;
					bool first = true;
		      for(counter = 0; counter < node->out_edges.size + 2; counter++)
		      {
					  Edge *host_edge = getNthOutEdge(host, node, counter);
						if(host_edge == NULL || host_edge->index == -1) continue;
						printFunctionFlat(host, getNode(hostG, host_edge->target), false);
						if(first){
							first = false;
							if(strcmp(item->atom.str, "AND") == 0){
								printf(" & ");
							}
							if(strcmp(item->atom.str, "OR") == 0){
								printf(" + ");
							}
							if(strcmp(item->atom.str, "NOR") == 0){
								printf(" - ");
							}
							if(strcmp(item->atom.str, "NOT") == 0){
								printf(" ! ");
							}
							if(strcmp(item->atom.str, "NAND") == 0){
								printf(" | ");
							}
						}
					}
					if(top){
						printf(")\n");
					}
					else{
						printf(")");
					}
			}
}

static void repopulate(Graph* population[], int lambda, int winnerIndex, char* functionSet[], int functionArities[], int functions, double mutation_rate){
    for(int i = 0; i < lambda + 1; i++){
      if(population[i] != NULL && i != winnerIndex){
        freeGraph(population[i]);
      }
      if(i != winnerIndex){
        population[i] = duplicateGraph(population[winnerIndex]);
        mutateIndividual(population[i], functionSet, functionArities, functions, mutation_rate);
      }
    }
}

static void evaluate_population(Graph* population[], double scores[], int lambda, int winnerIndex, int inputs, int outputs, int rows, double data[rows][inputs + outputs]){
	for(int i = 0; i < lambda + 1; i++){
		if(i != winnerIndex){
			scores[i] = evaluate(population[i], inputs, outputs, rows, data);
		}
	}
}

static double evaluate(Graph* host_graph, int inputs, int outputs, int rows, double data[rows][inputs + outputs]){
	clock_gettime(CLOCK_REALTIME, &before);
	markActiveBlue(host_graph);

  double values[rows][host->nodes.size];
  int ready[host->nodes.size];
  double totalError = 0.0;
  //Find inputs and outputs
  int inputIndex[inputs];
  for(int i = 0; i<inputs; i++){
    inputIndex[i] = -1;
  }
  int outputIndex[outputs];
  for(int j = 0; j<outputs; j++){
    outputIndex[j] = -1;
  }

  for(int host_index = 0; host_index < host_graph->nodes.size; host_index++)
  {
     Node *host_node = getNode(host_graph, host_index);
     if(host_node == NULL || host_node->index == -1) continue;

     HostLabel label = host_node->label;
     if(label.length != 2) break;

     HostListItem *item = label.list->first;
     HostListItem *item2 = item->next;
     if(item->atom.type != 'i') break;
     if(item2->atom.type != 's') break;
     if(strcmp(item2->atom.str, "IN") == 0){
       inputIndex[item->atom.num] = host_index;
     }
     if(strcmp(item2->atom.str, "OUT") == 0){
       outputIndex[item->atom.num] = host_index;
     }
  }

  int queue[host_graph->nodes.size];
  int head = 0;
  int tail = 0;

	for(int x = 0; x < host_graph->nodes.size; x++){
      ready[x] = 0;
	}
  for(int r = 0; r < rows; r++){
    for(int x = 0; x < host_graph->nodes.size; x++){
      values[r][x] = -1.0;
    }
	}
  for(int q = 0; q < host_graph->nodes.size; q++){
    queue[q] = 0;
  }

  //Load inputs
  for(int i = 0; i < inputs; i++){
		for(int r = 0; r < rows; r++){
    	values[r][inputIndex[i]] = data[r][i];
		}
    ready[inputIndex[i]] = 1;
    Node *host_node = getNode(host_graph, inputIndex[i]);
    int counter;
    for(counter = 0; counter < host_node->in_edges.size + 2; counter++)
    {
      Edge *host_edge = getNthInEdge(host_graph, host_node, counter);
      if(host_edge == NULL) continue;
      int source = host_edge->source;
      ready[source]++;
			Node* source_node = getNode(host_graph, source);
			if(source_node->label.mark == 0) continue;
       if(ready[source] >= source_node->outdegree){
        queue[tail] = source;
        tail++;
      }
    }
  }



  while(head < tail){
    int node_index = queue[head];
    head++;
    Node *host_node = getNode(host_graph, node_index);

    HostLabel label = host_node->label;
    HostListItem *item = label.list->last;
    char* function = item->atom.str;

    double in_values[rows][host_node->outdegree];
    int counter;
    for(counter = 0; counter < host_node->out_edges.size + 2; counter++)
    {
      Edge *host_edge = getNthOutEdge(host_graph, host_node, counter);
      if(host_edge == NULL) continue;
      int target = host_edge->target;

			HostLabel label = host_edge->label;
			int vIndex = label.list->first->atom.num;
			for(int r = 0; r < rows; r++){
      	in_values[r][vIndex] = values[r][target];
			}
    }
    if(strcmp(function, "NAND") == 0){
			for(int r = 0; r < rows; r++){
				if(in_values[r][0] == -1.0 || in_values[r][1] == -1.0){
					printf("Broken NAND %d\n", host_node->index);
				}
        if((int)in_values[r][0] == 0 || (int)in_values[r][1] == 0){
          values[r][node_index] = 1.0;
        }
        else {
          values[r][node_index] = 0.0;
        }
			}
    }
    else if(strcmp(function, "NOR") == 0){
			for(int r = 0; r < rows; r++){
				if(in_values[r][0] == -1.0 || in_values[r][1] == -1.0){
					printf("Broken NOR %d\n",host_node->index);
				}
        if((int)in_values[r][0] == 0 && (int)in_values[r][1] == 0){
          values[r][node_index] = 1.0;
        }
        else{
          values[r][node_index] = 0.0;
        }
			}
    }
    else if(strcmp(function, "NOT") == 0){
			for(int r = 0; r < rows; r++){
				if(in_values[r][0] == -1.0){
					printf("Broken NOT %d\n", host_node->index);
				}
        if((int)in_values[r][0] == 0){
          values[r][node_index] = 1.0;
        }
        else{
          values[r][node_index] = 0.0;
        }
			}
    }
    else if(strcmp(function, "OR") == 0){
			for(int r = 0; r < rows; r++){
				if(in_values[r][0] == -1.0 || in_values[r][1] == -1.0){
					printf("Broken OR %d\n", host_node->index);
				}
        if((int)in_values[r][0] == 1 || (int)in_values[r][1] == 1){
          values[r][node_index] = 1.0;
        }
        else{
          values[r][node_index] = 0.0;
        }
			}
    }
    else if(strcmp(function, "AND") == 0){
			for(int r = 0; r < rows; r++){
				if(in_values[r][0] == -1.0 || in_values[r][1] == -1.0){
					printf("Broken AND %d\n", host_node->index);
				}
        if((int)in_values[r][0] == 1 && (int)in_values[r][1] == 1){
          values[r][node_index] = 1.0;
        }
        else{
          values[r][node_index] = 0.0;
        }
			}
    }
    else if(strcmp(function, "ANDI") == 0){
			for(int r = 0; r < rows; r++){
        if((int)in_values[r][0] == 1 && (int)in_values[r][1] == 0){
          values[r][node_index] = 1.0;
        }
        else{
          values[r][node_index] = 0.0;
        }
			}
    }
    else if(strcmp(function, "XOR") == 0){
			for(int r = 0; r < rows; r++){
        if(((int)in_values[r][0] == 1 && (int)in_values[r][1] == 0) || ((int)in_values[r][0] == 0 && (int)in_values[r][1] == 1)){
          values[r][node_index] = 1.0;
        }
        else{
          values[r][node_index] = 0.0;
        }
			}
    }
    else if(strcmp(function, "ADD") == 0){
			//printf("Add function\n");
			for(int r = 0; r < rows; r++){
	        values[r][node_index] = in_values[r][0] + in_values[r][1];
					//printf("%d: %lf\n", node_index, values[r][node_index]);
				}
    }
    else if(strcmp(function, "SUB") == 0){
			//printf("Sub function\n");
			for(int r = 0; r < rows; r++){
				values[r][node_index] = in_values[r][0] - in_values[r][1];
				//printf("%d: %lf\n", node_index, values[r][node_index]);
			}
    }
    else if(strcmp(function, "MUL") == 0){
			//printf("Mul function\n");
			for(int r = 0; r < rows; r++){
				values[r][node_index] = in_values[r][0] * in_values[r][1];
				//printf("%d: %lf\n", node_index, values[r][node_index]);
			}
    }
    else if(strcmp(function, "DIV") == 0){
			//printf("Div function\n");
			for(int r = 0; r < rows; r++){
				if(in_values[r][1] == 0.0){
					values[r][node_index] = 1.0;
					//printf("%d: %lf\n", node_index, values[r][node_index]);
				}
				else{
		        values[r][node_index] = in_values[r][0] / in_values[r][1];
						//printf("%d: %lf\n", node_index, values[r][node_index]);
				}
			}
    }
    else if(strcmp(function, "LOG") == 0){
			//printf("Log function\n");
			for(int r = 0; r < rows; r++){
				if(in_values[r][0] <= 0.0){
					values[r][node_index] = 1.0;
					//printf("%d: %lf\n", node_index, values[r][node_index]);
				}
				else{
		        values[r][node_index] = log(in_values[r][0]);
						//printf("%d: %lf\n", node_index, values[r][node_index]);
				}
			}
    }
    else if(strcmp(function, "SIN") == 0){
			//printf("Sin function\n");
		    for(int r = 0; r < rows; r++){
	        values[r][node_index] = sin(in_values[r][0]);
					//printf("%d: %lf\n", node_index, values[r][node_index]);
			}
    }
    else if(strcmp(function, "COS") == 0){
			//printf("Cos function\n");
		    for(int r = 0; r < rows; r++){
	        values[r][node_index] = cos(in_values[r][0]);
					//printf("%d: %lf\n", node_index, values[r][node_index]);
			}
    }
    else if(strcmp(function, "EXP") == 0){
			//printf("Exp function\n");
		    for(int r = 0; r < rows; r++){
	        values[r][node_index] = exp(in_values[r][0]);
					//printf("%d: %lf\n", node_index, values[r][node_index]);
			}
    }
    else if(strcmp(function, "ID") == 0){
			//printf("id function\n");
		    for(int r = 0; r < rows; r++){
	        values[r][node_index] = in_values[r][0];
					//printf("%d: %lf\n", node_index, values[r][node_index]);
			}
    }
    else if(strcmp(function, "OUT") == 0){
			//printf("OUT function\n");
			for(int r = 0; r < rows; r++){
      	values[r][node_index] = in_values[r][0];
				//printf("%d: %lf\n", node_index, values[r][node_index]);
			}
    }

    for(counter = 0; counter < host_node->in_edges.size + 2; counter++)
    {
      Edge *host_edge = getNthInEdge(host_graph, host_node, counter);
      if(host_edge == NULL) continue;
      int source = host_edge->source;
      ready[source]++;
			Node* source_node = getNode(host_graph, source);
			if(source_node->label.mark == 0) continue;
      if(ready[source] >= source_node->outdegree){
        queue[tail] = source;
        tail++;
      }
    }
  }

  for(int o = 0; o < outputs; o++){
		for(int r = 0; r < rows; r++){
			//printf("Expected %lf got %lf diff %lf\n", data[r][inputs + o], values[r][outputIndex[o]], fabs(values[r][outputIndex[o]] - data[r][inputs + o]));
    	totalError = totalError + pow(fabs(values[r][outputIndex[o]] - data[r][inputs + o]), 2);
		}
  }
	clock_gettime(CLOCK_REALTIME, &after);
	totalTime += (double)(((after.tv_sec - before.tv_sec) * 1000) + ((double)(after.tv_nsec - before.tv_nsec) / (double)1000000)) / (double)1000;
	evals = evals + 1.0;
	unmarkGraph(host_graph);
  return totalError;
}

static double worstCase(Graph* host_graph, int inputs, int outputs, int rows, double data[rows][inputs + outputs]){
	clock_gettime(CLOCK_REALTIME, &before);

	markActiveBlue(host_graph);

  double values[rows][host->nodes.size];
  int ready[host->nodes.size];
  double worstError = 0.0;
  //Find inputs and outputs
  int inputIndex[inputs];
  for(int i = 0; i<inputs; i++){
    inputIndex[i] = -1;
  }
  int outputIndex[outputs];
  for(int j = 0; j<outputs; j++){
    outputIndex[j] = -1;
  }
  for(int host_index = 0; host_index < host_graph->nodes.size; host_index++)
  {
     Node *host_node = getNode(host_graph, host_index);
     if(host_node == NULL || host_node->index == -1) continue;

     HostLabel label = host_node->label;
     if(label.length != 2) break;

     HostListItem *item = label.list->first;
     HostListItem *item2 = item->next;
     if(item->atom.type != 'i') break;
     if(item2->atom.type != 's') break;
     if(strcmp(item2->atom.str, "IN") == 0){
       inputIndex[item->atom.num] = host_index;
     }
     if(strcmp(item2->atom.str, "OUT") == 0){
       outputIndex[item->atom.num] = host_index;
     }
  }

  int queue[host_graph->nodes.size];
  int head = 0;
  int tail = 0;

	for(int x = 0; x < host_graph->nodes.size; x++){
      ready[x] = 0;
	}
  for(int r = 0; r < rows; r++){
    for(int x = 0; x < host_graph->nodes.size; x++){
      values[r][x] = -1.0;
    }
	}
  for(int q = 0; q < host_graph->nodes.size; q++){
    queue[q] = 0;
  }

  //Load inputs
  for(int i = 0; i < inputs; i++){
		for(int r = 0; r < rows; r++){
    	values[r][inputIndex[i]] = data[r][i];
		}
    ready[inputIndex[i]] = 1;
    Node *host_node = getNode(host_graph, inputIndex[i]);
    int counter;
    for(counter = 0; counter < host_node->in_edges.size + 2; counter++)
    {
      Edge *host_edge = getNthInEdge(host_graph, host_node, counter);
      if(host_edge == NULL) continue;
      int source = host_edge->source;
      ready[source]++;
			Node* source_node = getNode(host_graph, source);
			if(source_node->label.mark == 0) continue;
       if(ready[source] >= source_node->outdegree){
        queue[tail] = source;
        tail++;
      }
    }
  }


  while(head < tail){
    int node_index = queue[head];
    head++;
    Node *host_node = getNode(host_graph, node_index);

    HostLabel label = host_node->label;
    HostListItem *item = label.list->last;
    char* function = item->atom.str;

    double in_values[rows][host_node->outdegree];
    int counter;
    for(counter = 0; counter < host_node->out_edges.size + 2; counter++)
    {
      Edge *host_edge = getNthOutEdge(host_graph, host_node, counter);
      if(host_edge == NULL) continue;
      int target = host_edge->target;

			HostLabel label = host_edge->label;
			int vIndex = label.list->first->atom.num;
			for(int r = 0; r < rows; r++){
      	in_values[r][vIndex] = values[r][target];
			}
    }
    if(strcmp(function, "NAND") == 0){
			for(int r = 0; r < rows; r++){
        if((int)in_values[r][0] == 0 || (int)in_values[r][1] == 0){
          values[r][node_index] = 1.0;
        }
        else{
          values[r][node_index] = 0.0;
        }
			}
    }
    else if(strcmp(function, "NOR") == 0){
			for(int r = 0; r < rows; r++){
        if((int)in_values[r][0] == 0 && (int)in_values[r][1] == 0){
          values[r][node_index] = 1.0;
        }
        else{
          values[r][node_index] = 0.0;
        }
			}
    }
    else if(strcmp(function, "NOT") == 0){
			for(int r = 0; r < rows; r++){
        if((int)in_values[r][0] == 0){
          values[r][node_index] = 1.0;
        }
        else{
          values[r][node_index] = 0.0;
        }
			}
    }
    else if(strcmp(function, "OR") == 0){
			for(int r = 0; r < rows; r++){
        if((int)in_values[r][0] == 1 || (int)in_values[r][1] == 1){
          values[r][node_index] = 1.0;
        }
        else{
          values[r][node_index] = 0.0;
        }
			}
    }
    else if(strcmp(function, "AND") == 0){
			for(int r = 0; r < rows; r++){
        if((int)in_values[r][0] == 1 && (int)in_values[r][1] == 1){
          values[r][node_index] = 1.0;
        }
        else{
          values[r][node_index] = 0.0;
        }
			}
    }
    else if(strcmp(function, "ANDI") == 0){
			for(int r = 0; r < rows; r++){
        if((int)in_values[r][0] == 1 && (int)in_values[r][1] == 0){
          values[r][node_index] = 1.0;
        }
        else{
          values[r][node_index] = 0.0;
        }
			}
    }
    else if(strcmp(function, "XOR") == 0){
			for(int r = 0; r < rows; r++){
        if(((int)in_values[r][0] == 1 && (int)in_values[r][1] == 0) || ((int)in_values[r][0] == 0 && (int)in_values[r][1] == 1)){
          values[r][node_index] = 1.0;
        }
        else{
          values[r][node_index] = 0.0;
        }
			}
    }
    else if(strcmp(function, "ADD") == 0){
			//printf("Add function\n");
			for(int r = 0; r < rows; r++){
	        values[r][node_index] = in_values[r][0] + in_values[r][1];
					//printf("%d: %lf\n", node_index, values[r][node_index]);
				}
    }
    else if(strcmp(function, "SUB") == 0){
			//printf("Sub function\n");
			for(int r = 0; r < rows; r++){
				values[r][node_index] = in_values[r][0] - in_values[r][1];
				//printf("%d: %lf\n", node_index, values[r][node_index]);
			}
    }
    else if(strcmp(function, "MUL") == 0){
			//printf("Mul function\n");
			for(int r = 0; r < rows; r++){
				values[r][node_index] = in_values[r][0] * in_values[r][1];
				//printf("%d: %lf\n", node_index, values[r][node_index]);
			}
    }
    else if(strcmp(function, "DIV") == 0){
			//printf("Div function\n");
			for(int r = 0; r < rows; r++){
				if(in_values[r][1] == 0.0){
					values[r][node_index] = 1.0;
					//printf("%d: %lf\n", node_index, values[r][node_index]);
				}
				else{
		        values[r][node_index] = in_values[r][0] / in_values[r][1];
						//printf("%d: %lf\n", node_index, values[r][node_index]);
				}
			}
    }
    else if(strcmp(function, "LOG") == 0){
			//printf("Log function\n");
			for(int r = 0; r < rows; r++){
				if(in_values[r][0] <= 0.0){
					values[r][node_index] = 1.0;
					//printf("%d: %lf\n", node_index, values[r][node_index]);
				}
				else{
		        values[r][node_index] = log(in_values[r][0]);
						//printf("%d: %lf\n", node_index, values[r][node_index]);
				}
			}
    }
    else if(strcmp(function, "SIN") == 0){
			//printf("Sin function\n");
		    for(int r = 0; r < rows; r++){
	        values[r][node_index] = sin(in_values[r][0]);
					//printf("%d: %lf\n", node_index, values[r][node_index]);
			}
    }
    else if(strcmp(function, "COS") == 0){
			//printf("Cos function\n");
		    for(int r = 0; r < rows; r++){
	        values[r][node_index] = cos(in_values[r][0]);
					//printf("%d: %lf\n", node_index, values[r][node_index]);
			}
    }
    else if(strcmp(function, "EXP") == 0){
			//printf("Exp function\n");
		    for(int r = 0; r < rows; r++){
	        values[r][node_index] = exp(in_values[r][0]);
					//printf("%d: %lf\n", node_index, values[r][node_index]);
			}
    }
    else if(strcmp(function, "ID") == 0){
			//printf("id function\n");
		    for(int r = 0; r < rows; r++){
	        values[r][node_index] = in_values[r][0];
					//printf("%d: %lf\n", node_index, values[r][node_index]);
			}
    }
    else if(strcmp(function, "OUT") == 0){
			//printf("OUT function\n");
			for(int r = 0; r < rows; r++){
      	values[r][node_index] = in_values[r][0];
				//printf("%d: %lf\n", node_index, values[r][node_index]);
			}
    }

    for(counter = 0; counter < host_node->in_edges.size + 2; counter++)
    {
      Edge *host_edge = getNthInEdge(host_graph, host_node, counter);
      if(host_edge == NULL) continue;
      int source = host_edge->source;
      ready[source]++;
			Node* source_node = getNode(host_graph, source);
			if(source_node->label.mark == 0) continue;
      if(ready[source] >= source_node->outdegree){
        queue[tail] = source;
        tail++;
      }
    }
  }
  for(int o = 0; o < outputs; o++){
		for(int r = 0; r < rows; r++){
			//printf("Expected %lf got %lf diff %lf\n", data[r][inputs + o], values[r][outputIndex[o]], fabs(values[r][outputIndex[o]] - data[r][inputs + o]));

      double error = fabs(values[r][outputIndex[o]] - data[r][inputs + o]);
      if(error > worstError){
        worstError = error;
      }
		}
  }
	clock_gettime(CLOCK_REALTIME, &after);
	totalTime += (double)(((after.tv_sec - before.tv_sec) * 1000) + ((double)(after.tv_nsec - before.tv_nsec) / (double)1000000)) / (double)1000;
	evals = evals + 1.0;
	unmarkGraph(host_graph);
  return worstError;
}


/*
static double evaluate(Graph* host_graph, int inputs, int outputs, int rows, double data[rows][inputs + outputs]){
	clock_gettime(CLOCK_REALTIME, &before);
  double values[host->nodes.size];
  int ready[host->nodes.size];
  double totalError = 0.0;
  //Find inputs and outputs
  int inputIndex[inputs];
  for(int i = 0; i<inputs; i++){
    inputIndex[i] = -1;
  }
  int outputIndex[outputs];
  for(int j = 0; j<outputs; j++){
    outputIndex[j] = -1;
  }
  for(int host_index = 0; host_index < host_graph->nodes.size; host_index++)
  {
     Node *host_node = getNode(host_graph, host_index);
     if(host_node == NULL || host_node->index == -1) continue;

     HostLabel label = host_node->label;
     if(label.length != 2) break;

     HostListItem *item = label.list->first;
     HostListItem *item2 = item->next;
     if(item->atom.type != 'i') break;
     if(item2->atom.type != 's') break;
     if(strcmp(item2->atom.str, "IN") == 0){
       inputIndex[item->atom.num] = host_index;
     }
     if(strcmp(item2->atom.str, "OUT") == 0){
       outputIndex[item->atom.num] = host_index;
     }
  }

  int queue[host_graph->nodes.size];
  int head;
  int tail;

  for(int r = 0; r < rows; r++){
    double error = 0.0;
    for(int x = 0; x < host_graph->nodes.size; x++){
      ready[x] = 0;
      values[x] = -1.0;

    }

    head = 0;
    tail = 0;
    for(int q = 0; q < host_graph->nodes.size; q++){
      queue[q] = 0;
    }

    //Load inputs
    for(int i = 0; i < inputs; i++){
      values[inputIndex[i]] = data[r][i];
      ready[inputIndex[i]] = 1;
      Node *host_node = getNode(host_graph, inputIndex[i]);
      int counter;
      for(counter = 0; counter < host_node->in_edges.size + 2; counter++)
      {
        Edge *host_edge = getNthInEdge(host_graph, host_node, counter);
        if(host_edge == NULL) continue;
        int source = host_edge->source;
        ready[source]++;
        if(ready[source] >= getNode(host_graph, source)->outdegree){
          queue[tail] = source;
          tail++;
        }
      }
    }


    while(head < tail){
      int node_index = queue[head];
      head++;
      Node *host_node = getNode(host_graph, node_index);

      HostLabel label = host_node->label;
      HostListItem *item = label.list->last;
      char* function = item->atom.str;

      double in_values[host_node->outdegree];
      int counter;
      int edge_count = 0;
      for(counter = 0; counter < host_node->out_edges.size + 2; counter++)
      {
        Edge *host_edge = getNthOutEdge(host_graph, host_node, counter);
        if(host_edge == NULL) continue;
        int target = host_edge->target;
        in_values[edge_count] = values[target];
        edge_count++;
      }

      if(strcmp(function, "NAND") == 0){
        if((int)in_values[0] == 0 || (int)in_values[1] == 0){
          values[node_index] = 1.0;
        }
        else{
          values[node_index] = 0.0;
        }
      }
      else if(strcmp(function, "NOR") == 0){
        if((int)in_values[0] == 0 && (int)in_values[1] == 0){
          values[node_index] = 1.0;
        }
        else{
          values[node_index] = 0.0;
        }
      }
      else if(strcmp(function, "OR") == 0){
        if((int)in_values[0] == 1 || (int)in_values[1] == 1){
          values[node_index] = 1.0;
        }
        else{
          values[node_index] = 0.0;
        }
      }
      else if(strcmp(function, "AND") == 0){
        if((int)in_values[0] == 1 && (int)in_values[1] == 1){
          values[node_index] = 1.0;
        }
        else{
          values[node_index] = 0.0;
        }
      }
      else if(strcmp(function, "OUT") == 0){
        values[node_index] = in_values[0];
      }

      for(counter = 0; counter < host_node->in_edges.size + 2; counter++)
      {
        Edge *host_edge = getNthInEdge(host_graph, host_node, counter);
        if(host_edge == NULL) continue;
        int source = host_edge->source;
        ready[source]++;
        if(ready[source] == getNode(host_graph, source)->outdegree){
          queue[tail] = source;
          tail++;
        }
      }
    }

    for(int o = 0; o < outputs; o++){
      error = error + abs(values[outputIndex[o]] - data[r][inputs + o]);
    }
    totalError = totalError + error;
  }
	clock_gettime(CLOCK_REALTIME, &after);
	totalTime += (double)(((after.tv_sec - before.tv_sec) * 1000) + ((double)(after.tv_nsec - before.tv_nsec) / (double)1000000)) / (double)1000;
	evals = evals + 1.0;
  return totalError;
}
*/

static Graph* generateIndividual(char* functionSet[], int functionArities[], int functions, int nodes, int inputs, int outputs)
{
   host = buildEmptyHostGraph();

   /* Initialise host graph */
   /* INPUTS */
   for(int i = 0; i < inputs; i++){
     HostAtom array[2];
     HostLabel label;
     array[0].type = 'i';
     array[0].num = i;
     array[1].type = 's';
     array[1].str = "IN";
     HostList *list = makeHostList(array, 2, false);
     label = makeHostLabel(0, 2, list);
     addNode(host, 0, label);
   }

   /* OUTPUTS */
   for(int i = 0; i < outputs; i++){
     HostAtom array[2];
     HostLabel label;
     array[0].type = 'i';
     array[0].num = i;
     array[1].type = 's';
     array[1].str = "OUT";
     HostList *list = makeHostList(array, 2, false);
     label = makeHostLabel(0, 2, list);
     addNode(host, 0, label);
   }

   /* FUNCTIONSET */
   for(int i = 0; i < functions; i++){
      HostAtom array[3];
      HostLabel label;
      array[0].type = 's';
      array[0].str = "FunctionSet";
      array[1].type = 's';
      array[1].str = functionSet[i];
      array[2].type = 'i';
      array[2].num = functionArities[i];
      HostList *list = makeHostList(array, 3, false);
      label = makeHostLabel(0, 3, list);
      addNode(host, 1, label);
   }

   /* NODES */
    HostAtom array[2];
    HostLabel label;
    array[0].type = 's';
    array[0].str = "Nodes";
    array[1].type = 'i';
    array[1].num = nodes;
    HostList *list = makeHostList(array, 2, false);
    label = makeHostLabel(0, 2, list);
    addNode(host, 1, label);

    /* Run Individual Generation */
   pot = makeMorphismPot();
   emptyPot(pot);
   printfGraph(host);
   M_Main_initNodeCounter = makeMorphism(1, 0, 1);
   M_Main_addNode = makeMorphism(2, 0, 3);
   M_Main_addEdgeToArity = makeMorphism(3, 0, 5);
   M_Main_decolourNode = makeMorphism(1, 0, 1);
   M_Main_connectOutput = makeMorphism(2, 0, 3);
   M_Main_deleteNodeCounter = makeMorphism(1, 0, 1);

   /* Rule Call */
   if(matchMain_initNodeCounter(M_Main_initNodeCounter))
   {
      applyMain_initNodeCounter(M_Main_initNodeCounter, false);
      success = true;
   }
   else
   {
      garbageCollect_GenerateIndividual();
      return NULL;
   }
   /* Loop Statement */
   int restore_point0 = graph_change_stack == NULL ? 0 : topOfGraphChangeStack();
   while(success)
   {
      /* Rule Call */
      emptyPot(pot);
      fillpotMain_addNode(pot, M_Main_addNode);
      if(potSize(pot) > 0){
         MorphismHolder *holder = drawFromPot(pot);
         duplicateMorphism(holder->morphism, M_Main_addNode, host);
         freeMorphism(holder->morphism);
         free(holder);
         applyMain_addNode(M_Main_addNode, true);
         success = true;
      }
      else
      {
         success = false;
         undoChanges(host, restore_point0);
      }
      emptyPot(pot);
      if(!success) break;

      /* Loop Statement */
      while(success)
      {
         /* Rule Call */
         emptyPot(pot);
         fillpotMain_addEdgeToArity(pot, M_Main_addEdgeToArity);
         if(potSize(pot) > 0){
            MorphismHolder *holder = drawFromPot(pot);
            duplicateMorphism(holder->morphism, M_Main_addEdgeToArity, host);
            freeMorphism(holder->morphism);
            free(holder);
            applyMain_addEdgeToArity(M_Main_addEdgeToArity, true);
            success = true;
         }
         else
         {
            success = false;
         }
         emptyPot(pot);
      }
      success = true;
      if(!success) break;

      /* Rule Call */
      if(matchMain_decolourNode(M_Main_decolourNode))
      {
         applyMain_decolourNode(M_Main_decolourNode, true);
         success = true;
      }
      else
      {
         success = false;
         undoChanges(host, restore_point0);
      }
      /* Graph changes from loop body may not have been used.
         Discard them so that future graph roll backs are uncorrupted. */
      if(success) discardChanges(restore_point0);
   }
   success = true;
   /* Loop Statement */
   while(success)
   {
      /* Rule Call */
      emptyPot(pot);
      fillpotMain_connectOutput(pot, M_Main_connectOutput);
      if(potSize(pot) > 0){
         MorphismHolder *holder = drawFromPot(pot);
         duplicateMorphism(holder->morphism, M_Main_connectOutput, host);
         freeMorphism(holder->morphism);
         free(holder);
         applyMain_connectOutput(M_Main_connectOutput, false);
         success = true;
      }
      else
      {
         success = false;
      }
      emptyPot(pot);
   }

	/* Rule Call */
	if(matchMain_deleteNodeCounter(M_Main_deleteNodeCounter))
	{
	 applyMain_deleteNodeCounter(M_Main_deleteNodeCounter, false);
	 success = true;
	}
	else
	{
	 garbageCollect_GenerateIndividual();
	 return host;
	}

   success = true;
   garbageCollect_GenerateIndividual();

   /* REMOVE FUNCTION SET AND NODES COUNTER. These are the only rooted nodes. */
	 for(int i = 0; i < host->nodes.size; i++){
	 	 Node *host_node = getNode(host, i);
		 if(host_node == NULL) continue;
		 if(!host_node->root) continue;
     removeNode(host, i);
   }
   return host;
}


static Graph* mutateIndividual(Graph* hostGraph, char* functionSet[], int functionArities[], int functions, double mutation_rate)
{
	clock_gettime(CLOCK_REALTIME, &before);
   pot = makeMorphismPot();
   emptyPot(pot);

   host = hostGraph;

   int nodes = host->number_of_nodes;
   int edges = host->number_of_edges;
   int mutations = 0;
   double r;
   for(int i = 0; i < nodes + edges; i++){
      r = ((double)rand() / (double)RAND_MAX);
      if(r <= mutation_rate){
        mutations++;
      }
   }
   if(mutations == 0){
     mutations = 1;
   }

    /* FUNCTIONSET */
    for(int i = 0; i < functions; i++){
     HostAtom array[3];
     HostLabel label;
     array[0].type = 's';
     array[0].str = "FunctionSet";
     array[1].type = 's';
     array[1].str = functionSet[i];
     array[2].type = 'i';
     array[2].num = functionArities[i];
     HostList *list = makeHostList(array, 3, false);
     label = makeHostLabel(0, 3, list);
     addNode(host, 1, label);
    }

    /* Mutations */
    HostAtom array[2];
    HostLabel label;
    array[0].type = 's';
    array[0].str = "Mutations";
    array[1].type = 'i';
    array[1].num = mutations;
    HostList *list = makeHostList(array, 2, false);
    label = makeHostLabel(0, 2, list);
    addNode(host, 1, label);
    M_Main_clearTargetMark = makeMorphism(2, 1, 2);
    M_Main_clearTargetEdge = makeMorphism(3, 3, 3);
    M_Main_initMutationCounter = makeMorphism(1, 0, 1);
    M_Main_checkMutationCounter = makeMorphism(2, 0, 2);
    M_Main_deleteMutationCounter = makeMorphism(1, 0, 1);
    M_Main_pickEdgeMutate = makeMorphism(1, 0, 1);
    M_Main_pickFuncMutate = makeMorphism(4, 1, 3);
    M_Main_pickedEdge = makeMorphism(1, 0, 0);
    M_Main_pickedNode = makeMorphism(1, 0, 0);
    M_Main_pickEdge = makeMorphism(4, 3, 3);
    M_Main_markOutputs = makeMorphism(3, 3, 3);
    M_Main_pickNewTarget = makeMorphism(2, 1, 2);
    M_Main_mutateEdge = makeMorphism(4, 4, 5);
    M_Main_unmarkOutputs = makeMorphism(2, 1, 1);
    M_Main_pickNode = makeMorphism(3, 1, 2);
		M_Main_mutateNode = makeMorphism(2, 0, 4);
		M_Main_addEdgeToArityBlue = makeMorphism(3, 0, 5);
		M_Main_addEdgeCounter = makeMorphism(1, 0, 1);
		M_Main_deleteEdgeToArity = makeMorphism(4, 1, 6);
		M_Main_deleteEdgeCounter = makeMorphism(1, 0, 1);
    M_Main_unmarkActive = makeMorphism(2, 1, 1);

		/* Rule Call */
    if(matchMain_initMutationCounter(M_Main_initMutationCounter))
    {
       applyMain_initMutationCounter(M_Main_initMutationCounter, false);
       success = true;
    }
    else
    {
       garbageCollect_MutateIndividual();
       return 0;
    }
    /* Loop Statement */
    int restore_point0 = graph_change_stack == NULL ? 0 : topOfGraphChangeStack();while(success)
   {
      /* Rule Call */
      if(matchMain_checkMutationCounter(M_Main_checkMutationCounter))
      {
         applyMain_checkMutationCounter(M_Main_checkMutationCounter, true);
         success = true;
      }
      else
      {
         success = false;
         undoChanges(host, restore_point0);
      }
      if(!success) break;

    /* Loop Statement */
    while(success)
    {
       /* Rule Set Call */
       do
       {
          if(matchMain_clearTargetMark(M_Main_clearTargetMark))
          {
             applyMain_clearTargetMark(M_Main_clearTargetMark, true);
             success = true;
             break;
          }
          if(matchMain_clearTargetEdge(M_Main_clearTargetEdge))
          {
             applyMain_clearTargetEdge(M_Main_clearTargetEdge, true);
             success = true;
          }
          else
          {
             success = false;
          }
       } while(false);
    }
    success = true;
    if(!success) break;

		      /* Rule Set Call */
		      do
		      {
		      bool valid[2];
		      double weight[2];
		      double totalWeight = 0.0;
		      bool someValid = false;
		      valid[0] = false;
					weight[0] = ((double)edges)/((double)(edges + nodes));
	        valid[1] = false;
	        weight[1] = ((double)nodes)/((double)(edges + nodes));
		      if(matchMain_pickEdgeMutate(M_Main_pickEdgeMutate)){
		         valid[0] = true;
		         totalWeight = totalWeight + weight[0];
		         someValid = true;
		      }
		      initialiseMorphism(M_Main_pickEdgeMutate, host);
		      if(matchMain_pickFuncMutate(M_Main_pickFuncMutate)){
		         valid[1] = true;
		         totalWeight = totalWeight + weight[1];
		         someValid = true;
		      }
		      initialiseMorphism(M_Main_pickFuncMutate, host);
		      if (someValid){
		         double r = ((double)rand() / (double)RAND_MAX);
		         r = r * totalWeight;
		         double sum = 0.0;
		         if(valid[0]){
		            sum = sum + weight[0];
		            if(r <= sum){
		               emptyPot(pot);
		               fillpotMain_pickEdgeMutate(pot, M_Main_pickEdgeMutate);
		               if(potSize(pot) > 0){
		                  MorphismHolder *holder = drawFromPot(pot);
		                  duplicateMorphism(holder->morphism, M_Main_pickEdgeMutate, host);
		                  freeMorphism(holder->morphism);
		                  free(holder);
		                  applyMain_pickEdgeMutate(M_Main_pickEdgeMutate, true);
		                  success = true;
		                  break;
		               }
		               emptyPot(pot);
		            }
		         }
		         if(valid[1]){
		            sum = sum + weight[1];
		            if(r <= sum && r > sum - weight[1]){
		               emptyPot(pot);
		               fillpotMain_pickFuncMutate(pot, M_Main_pickFuncMutate);
		               if(potSize(pot) > 0){
		                  MorphismHolder *holder = drawFromPot(pot);
		                  duplicateMorphism(holder->morphism, M_Main_pickFuncMutate, host);
		                  freeMorphism(holder->morphism);
		                  free(holder);
		                  applyMain_pickFuncMutate(M_Main_pickFuncMutate, true);
		                  success = true;
		                  break;
		               }
		               emptyPot(pot);
		            }
		         }
		      } else {
		      success = false;
		      undoChanges(host, restore_point0);
		      }
		      } while(false);
		      if(!success) break;

		      /* Try Statement */
		      /* Condition */
		      int restore_point1 = graph_change_stack == NULL ? 0 : topOfGraphChangeStack();
		      do
		      {
		         /* Rule Call */
		         if(matchMain_pickedEdge(M_Main_pickedEdge))
		         {
		            applyMain_pickedEdge(M_Main_pickedEdge, true);
		            success = true;
		         }
		         else
		         {
		            success = false;
		            break;
		         }
		      } while(false);

		      /* Then Branch */
		      if(success)
		      {
		         discardChanges(restore_point1);
		         /* Try Statement */
		         /* Condition */
		         int restore_point2 = graph_change_stack == NULL ? 0 : topOfGraphChangeStack();
		         do
		         {
		            /* Rule Call */
		            emptyPot(pot);
		            fillpotMain_pickEdge(pot, M_Main_pickEdge);
		            if(potSize(pot) > 0){
		               MorphismHolder *holder = drawFromPot(pot);
		               duplicateMorphism(holder->morphism, M_Main_pickEdge, host);
		               freeMorphism(holder->morphism);
		               free(holder);
		               applyMain_pickEdge(M_Main_pickEdge, true);
		               success = true;
		            }
		            else
		            {
		               success = false;
		               break;
		            }
		            emptyPot(pot);
		            /* Loop Statement */
		            while(success)
		            {
		               /* Rule Call */
		               if(matchMain_unmarkActive(M_Main_unmarkActive))
		               {
		                  applyMain_unmarkActive(M_Main_unmarkActive, true);
		                  success = true;
		               }
		               else
		               {
		                  success = false;
		               }
	               }
                success = true;
                while(success)
                {
                   /* Rule Call */
                   if(matchMain_markOutputs(M_Main_markOutputs))
                   {
                      applyMain_markOutputs(M_Main_markOutputs, true);
                      success = true;
                   }
                   else
                   {
                      success = false;
                   }
                }
		            success = true;
		            /* Rule Call */
		            emptyPot(pot);
		            fillpotMain_pickNewTarget(pot, M_Main_pickNewTarget);
		            if(potSize(pot) > 0){
		               MorphismHolder *holder = drawFromPot(pot);
		               duplicateMorphism(holder->morphism, M_Main_pickNewTarget, host);
		               freeMorphism(holder->morphism);
		               free(holder);
		               applyMain_pickNewTarget(M_Main_pickNewTarget, true);
		               success = true;
		            }
		            else
		            {
		               success = false;
		               break;
		            }
		            emptyPot(pot);
		            /* Rule Call */
		            if(matchMain_mutateEdge(M_Main_mutateEdge))
		            {
		               applyMain_mutateEdge(M_Main_mutateEdge, true);
		               success = true;
		            }
		            else
		            {
		               success = false;
		               break;
		            }
                unmarkGraph(host);
		            success = true;
		         } while(false);

		         /* Then Branch */
		         if(success)
		         {
		            discardChanges(restore_point2);
		            /* Skip Statement */
		            success = true;
		         }
		         /* Else Branch */
		         else
		         {
		            undoChanges(host, restore_point2);
		            success = true;
		            /* Skip Statement */
		            success = true;
		         }
		      }
		      /* Else Branch */
		      else
		      {
		         undoChanges(host, restore_point1);
		         success = true;
		         /* Skip Statement */
		         success = true;
		      }
		      if(!success) break;

		      /* Try Statement */
		      /* Condition */
		      int restore_point3 = graph_change_stack == NULL ? 0 : topOfGraphChangeStack();
		      do
		      {
		         /* Rule Call */
		         if(matchMain_pickedNode(M_Main_pickedNode))
		         {
		            applyMain_pickedNode(M_Main_pickedNode, true);
		            success = true;
		         }
		         else
		         {
		            success = false;
		            break;
		         }
		      } while(false);

		      /* Then Branch */
		      if(success)
		      {
		         discardChanges(restore_point3);
		         /* Rule Call */
		         emptyPot(pot);
		         fillpotMain_pickNode(pot, M_Main_pickNode);
		         if(potSize(pot) > 0){
		            MorphismHolder *holder = drawFromPot(pot);
		            duplicateMorphism(holder->morphism, M_Main_pickNode, host);
		            freeMorphism(holder->morphism);
		            free(holder);
		            applyMain_pickNode(M_Main_pickNode, true);
		            success = true;
		         }
		         else
		         {
		            success = false;
		            undoChanges(host, restore_point0);
		         }
		         emptyPot(pot);
		         if(!success) break;
						 /* Rule Call */
		         emptyPot(pot);
		         fillpotMain_mutateNode(pot, M_Main_mutateNode);
		         if(potSize(pot) > 0){
		            MorphismHolder *holder = drawFromPot(pot);
		            duplicateMorphism(holder->morphism, M_Main_mutateNode, host);
		            freeMorphism(holder->morphism);
		            free(holder);
		            applyMain_mutateNode(M_Main_mutateNode, true);
		            success = true;
		         }
		         else
		         {
		            success = false;
		            undoChanges(host, restore_point0);
		         }
		         emptyPot(pot);
		         if(!success) break;

		         /* Loop Statement */
		         while(success)
		         {
		            /* Rule Call */
		            if(matchMain_markOutputs(M_Main_markOutputs))
		            {
		               applyMain_markOutputs(M_Main_markOutputs, true);
		               success = true;
		            }
		            else
		            {
		               success = false;
		            }
		         }
		         success = true;
		         if(!success) break;

		         /* Loop Statement */
		         while(success)
		         {
		            /* Rule Call */
		            if(matchMain_addEdgeToArityBlue(M_Main_addEdgeToArityBlue))
		            {
		               applyMain_addEdgeToArityBlue(M_Main_addEdgeToArityBlue, true);
		               success = true;
		            }
		            else
		            {
		               success = false;
		            }
		         }
		         success = true;
		         if(!success) break;

		         /* Rule Call */
		         if(matchMain_addEdgeCounter(M_Main_addEdgeCounter))
		         {
		            applyMain_addEdgeCounter(M_Main_addEdgeCounter, true);
		            success = true;
		         }
		         else
		         {
		            success = false;
		            undoChanges(host, restore_point0);
		         }
		         if(!success) break;

		         /* Loop Statement */
		         while(success)
		         {
		            /* Rule Call */
		            if(matchMain_deleteEdgeToArity(M_Main_deleteEdgeToArity))
		            {
		               applyMain_deleteEdgeToArity(M_Main_deleteEdgeToArity, true);
		               success = true;
		            }
		            else
		            {
		               success = false;
		            }
		         }
		         success = true;
		         if(!success) break;

		         /* Rule Call */
		         if(matchMain_deleteEdgeCounter(M_Main_deleteEdgeCounter))
		         {
		            applyMain_deleteEdgeCounter(M_Main_deleteEdgeCounter, true);
		            success = true;
		         }
		         else
		         {
		            success = false;
		            undoChanges(host, restore_point0);
		         }
		         if(!success) break;

						 unmarkGraph(host);
		      }
		      /* Else Branch */
		      else
		      {
		         undoChanges(host, restore_point3);
		         success = true;
		         /* Skip Statement */
		         success = true;
		      }
		      /* Graph changes from loop body may not have been used.
		         Discard them so that future graph roll backs are uncorrupted. */
		      if(success) discardChanges(restore_point0);
		   }
		   success = true;
		   /* Rule Call */
		   if(matchMain_deleteMutationCounter(M_Main_deleteMutationCounter))
		   {
		      applyMain_deleteMutationCounter(M_Main_deleteMutationCounter, false);
		      success = true;
		   }
		   else
		   {
		      garbageCollect_MutateIndividual();
		      return 0;
		   }

   success = true;
   garbageCollect_MutateIndividual();
		/* REMOVE FUNCTION SET AND NODES COUNTER. These are the only rooted nodes. */
		for(int i = 0; i < host->nodes.size; i++){
			 Node *host_node = getNode(host, i);
		 if(host_node == NULL) continue;
		 if(!host_node->root) continue;
		 removeNode(host, i);
		}
    clock_gettime(CLOCK_REALTIME, &after);
    totalMutateTime += (double)(((after.tv_sec - before.tv_sec) * 1000) + ((double)(after.tv_nsec - before.tv_nsec) / (double)1000000)) / (double)1000;
    mutates = mutates + 1.0;
   return 0;
}

static Graph* neutralIndividual(Graph* hostGraph)
{
    pot = makeMorphismPot();
    emptyPot(pot);

    host = hostGraph;
		M_Main_markOutputNode = makeMorphism(1, 0, 1);
    M_Main_markActiveNode = makeMorphism(2, 1, 3);
    M_Main_demorgansF1 = makeMorphism(6, 3, 9);
    M_Main_demorgansB1 = makeMorphism(6, 4, 9);
    M_Main_demorgansB2 = makeMorphism(6, 4, 9);
    M_Main_demorgansF2 = makeMorphism(6, 3, 9);
    M_Main_doubNegF = makeMorphism(4, 1, 7);
    M_Main_doubNegB = makeMorphism(4, 3, 7);
    M_Main_commute = makeMorphism(3, 2, 6);
    M_Main_unmarkNode = makeMorphism(1, 0, 1);
    M_Main_deleteOldAEdge = makeMorphism(2, 1, 3);
    M_Main_deleteOldNEdge = makeMorphism(2, 1, 3);
    M_Main_unmarkEdge = makeMorphism(2, 1, 3);

   /* Loop Statement */
   while(success)
   {
      /* Rule Call */
      if(matchMain_markOutputNode(M_Main_markOutputNode))
      {
         applyMain_markOutputNode(M_Main_markOutputNode, false);
         success = true;
      }
      else
      {
         success = false;
      }
   }
   success = true;
   /* Loop Statement */
   while(success)
   {
      /* Rule Call */
      if(matchMain_markActiveNode(M_Main_markActiveNode))
      {
         applyMain_markActiveNode(M_Main_markActiveNode, false);
         success = true;
      }
      else
      {
         success = false;
      }
   }
   success = true;
   /* Try Statement */
   /* Condition */
   do
   {
      /* Rule Set Call */
      do
      {
      bool valid[6];
      double weight[6];
      double totalWeight = 0.0;
      bool someValid = false;
      valid[0] = false;
      weight[0] = 1.0;
      valid[1] = false;
      weight[1] = 1.0;
      valid[2] = false;
      weight[2] = 1.0;
      valid[3] = false;
      weight[3] = 1.0;
      valid[4] = false;
      weight[4] = 1.0;
      valid[5] = false;
      weight[5] = 1.0;
      if(matchMain_demorgansF1(M_Main_demorgansF1)){
         valid[0] = true;
         totalWeight = totalWeight + weight[0];
         someValid = true;
      }
      initialiseMorphism(M_Main_demorgansF1, host);
      if(matchMain_demorgansF2(M_Main_demorgansF2)){
         valid[1] = true;
         totalWeight = totalWeight + weight[1];
         someValid = true;
      }
      initialiseMorphism(M_Main_demorgansF2, host);
      if(matchMain_demorgansB1(M_Main_demorgansB1)){
         valid[2] = true;
         totalWeight = totalWeight + weight[2];
         someValid = true;
      }
      initialiseMorphism(M_Main_demorgansB1, host);
      if(matchMain_demorgansB2(M_Main_demorgansB2)){
         valid[3] = true;
         totalWeight = totalWeight + weight[3];
         someValid = true;
      }
      initialiseMorphism(M_Main_demorgansB2, host);
      if(matchMain_doubNegB(M_Main_doubNegB)){
         valid[4] = true;
         totalWeight = totalWeight + weight[4];
         someValid = true;
      }
      initialiseMorphism(M_Main_doubNegB, host);
      if(matchMain_commute(M_Main_commute)){
         valid[5] = true;
         totalWeight = totalWeight + weight[5];
         someValid = true;
      }
      initialiseMorphism(M_Main_commute, host);
      if (someValid){
         double r = ((double)rand() / (double)RAND_MAX);
         r = r * totalWeight;
         double sum = 0.0;
         if(valid[0]){
            sum = sum + weight[0];
            if(r <= sum){
               emptyPot(pot);
               fillpotMain_demorgansF1(pot, M_Main_demorgansF1);
               if(potSize(pot) > 0){
                  MorphismHolder *holder = drawFromPot(pot);
                  duplicateMorphism(holder->morphism, M_Main_demorgansF1, host);
                  freeMorphism(holder->morphism);
                  free(holder);
                  applyMain_demorgansF1(M_Main_demorgansF1, false);
                  success = true;
									printf("Demorgans F1\n");
                  break;
               }
               emptyPot(pot);
            }
         }
         if(valid[1]){
            sum = sum + weight[1];
            if(r <= sum && r > sum - weight[1]){
               emptyPot(pot);
               fillpotMain_demorgansF2(pot, M_Main_demorgansF2);
               if(potSize(pot) > 0){
                  MorphismHolder *holder = drawFromPot(pot);
                  duplicateMorphism(holder->morphism, M_Main_demorgansF2, host);
                  freeMorphism(holder->morphism);
                  free(holder);
                  applyMain_demorgansF2(M_Main_demorgansF2, false);
                  success = true;
									printf("Demorgans F2\n");
                  break;
               }
               emptyPot(pot);
            }
         }
         if(valid[2]){
            sum = sum + weight[2];
            if(r <= sum && r > sum - weight[2]){
               emptyPot(pot);
               fillpotMain_demorgansB1(pot, M_Main_demorgansB1);
               if(potSize(pot) > 0){
                  MorphismHolder *holder = drawFromPot(pot);
                  duplicateMorphism(holder->morphism, M_Main_demorgansB1, host);
                  freeMorphism(holder->morphism);
                  free(holder);
                  applyMain_demorgansB1(M_Main_demorgansB1, false);
                  success = true;
									printf("Demorgans B1\n");
                  break;
               }
               emptyPot(pot);
            }
         }
         if(valid[3]){
            sum = sum + weight[3];
            if(r <= sum && r > sum - weight[3]){
               emptyPot(pot);
               fillpotMain_demorgansB2(pot, M_Main_demorgansB2);
               if(potSize(pot) > 0){
                  MorphismHolder *holder = drawFromPot(pot);
                  duplicateMorphism(holder->morphism, M_Main_demorgansB2, host);
                  freeMorphism(holder->morphism);
                  free(holder);
                  applyMain_demorgansB2(M_Main_demorgansB2, false);
                  success = true;
									printf("Demorgans B2\n");
                  break;
               }
               emptyPot(pot);
            }
         }
         if(valid[4]){
            sum = sum + weight[4];
            if(r <= sum && r > sum - weight[4]){
               printf("Main_doubNegB chosen\n");
               emptyPot(pot);
               fillpotMain_doubNegB(pot, M_Main_doubNegB);
               if(potSize(pot) > 0){
                  MorphismHolder *holder = drawFromPot(pot);
                  duplicateMorphism(holder->morphism, M_Main_doubNegB, host);
                  freeMorphism(holder->morphism);
                  free(holder);
                  applyMain_doubNegB(M_Main_doubNegB, false);
                  success = true;
                  break;
               }
               emptyPot(pot);
            }
         }
         if(valid[5]){
            sum = sum + weight[5];
            if(r <= sum && r > sum - weight[5]){
               printf("Main_commute chosen\n");
               emptyPot(pot);
               fillpotMain_commute(pot, M_Main_commute);
               if(potSize(pot) > 0){
                  MorphismHolder *holder = drawFromPot(pot);
                  duplicateMorphism(holder->morphism, M_Main_commute, host);
                  freeMorphism(holder->morphism);
                  free(holder);
                  applyMain_commute(M_Main_commute, false);
                  success = true;
                  break;
               }
               emptyPot(pot);
            }
         }
      } else {
      success = false;
      break;
      }
      } while(false);
   } while(false);

   /* Then Branch */
   if(success)
   {
      /* Skip Statement */
      success = true;
   }
   /* Else Branch */
   else
   {
      success = true;
      /* Skip Statement */
      success = true;
   }
   /* Loop Statement */
   while(success)
   {
      /* Rule Call */
      if(matchMain_deleteOldNEdge(M_Main_deleteOldNEdge))
      {
         applyMain_deleteOldNEdge(M_Main_deleteOldNEdge, false);
         success = true;
      }
      else
      {
         success = false;
      }
   }
   success = true;
   /* Loop Statement */
   while(success)
   {
      /* Rule Call */
      if(matchMain_deleteOldAEdge(M_Main_deleteOldAEdge))
      {
         applyMain_deleteOldAEdge(M_Main_deleteOldAEdge, false);
         success = true;
      }
      else
      {
         success = false;
      }
   }
   success = true;
   /* Loop Statement */
   while(success)
   {
      /* Rule Call */
      if(matchMain_unmarkEdge(M_Main_unmarkEdge))
      {
         applyMain_unmarkEdge(M_Main_unmarkEdge, false);
         success = true;
      }
      else
      {
         success = false;
      }
   }
   success = true;
   /* Loop Statement */
   while(success)
   {
      /* Rule Call */
      if(matchMain_unmarkNode(M_Main_unmarkNode))
      {
         applyMain_unmarkNode(M_Main_unmarkNode, false);
         success = true;
      }
      else
      {
         success = false;
      }
   }
   success = true;
   garbageCollect_Neutral();
   return host;
}
