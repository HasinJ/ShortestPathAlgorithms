#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Vertex{
  char *letter;
  int visited;
  struct Edge* next;

};

struct Edge{
  int weight;
  char* letter;
  struct Edge* next;
  struct Vertex* vertex; //vertex it corresponds with, to make traversal easier
};

struct Stack{
  int top;
  int max;
  struct Vertex **items;
};

struct Vertex* first;
struct Stack* stack;

struct Vertex* Peek(){
  return stack->items[stack->top];
}

void Pop(){
  if(stack->top==0) return;
  stack->top--;
  return;
}

void Push(struct Vertex* vertex){
  if (stack->top>stack->max){
    printf("stack overflow (visited node probably in stack or repitition)\n");
    return;
  }
  stack->items[++stack->top]=vertex;
}

//////////////////////////////////
///* NEEDS TO TEST FOR ISLANDS*///
//////////////////////////////////

void dfs(struct Vertex **graph, int vertices){
  //initialize stack
  stack = malloc(sizeof(struct Stack));
  stack->top=0;
  stack->max=vertices;
  stack->items=calloc(vertices,sizeof(struct Vertex));
  Push(first);
  printf("top %d\n", stack->top);
  //printf("top:%s\n", Peek()->letter); //reads top
  printf("output: %s ", Peek()->letter);
  Peek()->visited=1;
  struct Vertex* current = Peek();
  //printf("\nvisited: %d\n", current->next->vertex->visited);

  while(stack->top!=0){
    struct Edge* temp = current->next;
    while(temp->vertex->visited!=0){
      temp=temp->next;
      if (temp==0) {
        printf("returning\n");
        return;
      }
    }
    Push(temp->vertex);
    current=Peek();
    current->visited=1;
    printf("%s ", current->letter);
    if(current->next==0){
      Pop();
      current=Peek();
    }
  }
}

void insertHere(struct Edge* current, int weight, char* character, struct Vertex* correspondingVertex){
  struct Edge* new = malloc(sizeof(struct Edge));
  new->letter=malloc(20*sizeof(char*));
  strcpy(new->letter,character);
  new->vertex=correspondingVertex;
  new->weight=weight;
  new->next=current->next;
  current->next=new;
}

void insertHead(struct Vertex* current, int weight, char* character, struct Vertex* correspondingVertex){
  struct Edge* new = malloc(sizeof(struct Edge));
  new->letter=malloc(20*sizeof(char*));
  strcpy(new->letter,character);
  new->vertex=correspondingVertex;
  new->weight=weight;
  new->next = current->next;
  current->next=new;
}

void addEdge(struct Vertex* vertex, int weight, char* to, struct Vertex* correspondingVertex) {
  if(vertex->next==0){ //head
    vertex->next=malloc(sizeof(struct Edge));
    vertex->next->letter=malloc(20*sizeof(char*));
    strcpy(vertex->next->letter,to);
    vertex->next->vertex=correspondingVertex;
    vertex->next->weight=weight;
    vertex->next->next=0;
    return;
  } else{ if(strcmp(vertex->next->letter,to)>0){
    if (strcmp(vertex->next->letter,to)==0) return;
    insertHead(vertex,weight,to,correspondingVertex);
    return;
  }}

  struct Edge* current=vertex->next; //set current as head
  if (strcmp(current->letter,to)==0) return;
  while(current->next!=0) {
    if (strcmp(current->next->letter,to)==0) return;
    if (strcmp(current->next->letter,to)>0) { //flipping this swaps the list around
      insertHere(current,weight,to,correspondingVertex);
      return;
    }
    current = current->next;
  }
  current->next=malloc(sizeof(struct Edge));
  current->next->letter=malloc(20*sizeof(char*));
  strcpy(current->next->letter,to);
  current->next->vertex=correspondingVertex;
  current->next->weight=weight;
  current->next->next=0;
}

void fill(FILE* f, struct Vertex** graph, int weight, int vertices, char* source, char* to){
  //add edges
  struct Vertex* corr=0; //corresponding vertex
  for (size_t i = 0; i < vertices; i++) {

    if (strcmp(to,graph[i]->letter)==0) corr = graph[i];
    if (strcmp(source,graph[i]->letter)==0) {
      if(corr==0)  {
        for (size_t j = i; j < vertices; j++) if (strcmp(to,graph[j]->letter)==0) corr = graph[j];
      }
      addEdge(graph[i],weight,to,corr);
    }
  }
}

void freeEdges(struct Edge* x){
  if (x==0) return;
  freeEdges(x->next);
  free(x->letter);
  free(x);
  return;
}

void freeVertex(struct Vertex* x){
  if (x==0) return;
  freeEdges(x->next);
  free(x->letter);
  free(x);
  return;
}

void freeEverything(struct Vertex** graph,int vertices){
  for (size_t i = 0; i < vertices; i++) {
    freeVertex(graph[i]);
  }
  free(graph);
  free(stack->items);
  free(stack);
}

void read(struct Vertex **graph, int vertices){
  printf("\n");
  for (size_t i = 0; i < vertices; i++) {
    printf("%s", graph[i]->letter);
    printf("\n");
  }
  return;
}

void readAll(struct Vertex **graph, int vertices){
  printf("\n");
  printf("first: %s\n", first->letter);
  for (size_t i = 0; i < vertices; i++) {
    printf("%s: ", graph[i]->letter);
    for (struct Edge* current = graph[i]->next; current!=0; current=current->next) {
      printf("%s%d ", current->letter,current->weight);
    }
    printf("\n");
  }
  return;
}

struct Vertex* addVertex(char* content){

}

int main(int argc, char *argv[argc+1]) {
  FILE *f;
  f = fopen(argv[1],"r");
  if (f==0) {
    printf("error\n");
    return EXIT_SUCCESS;
  }

  int vertices;
  fscanf(f,"%d\n",&vertices);

  //intiliaze graph
  struct Vertex **graph = calloc(vertices,sizeof(struct Vertex));
  for (size_t i = 0; i < vertices; i++) {
    char content[20];
    fscanf(f,"%s\n",content);
    graph[i]=addVertex(content); 


  }
  read(graph,vertices);

  //add edges
  char source[20],to[20];
  int weight;
  while ((fscanf(f,"%s %s %d",source,to,&weight))!=EOF){
    fill(f,graph,weight,vertices,source,to);
  }
  readAll(graph,vertices);
  printf("\n");

  dfs(graph,vertices);
  printf("EXITED\n");

  freeEverything(graph,vertices);
  return EXIT_SUCCESS;
}
