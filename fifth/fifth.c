#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Vertex{
  char *letter;
  int visited;
  int degree;
  struct Edge* next;
  struct Edge* prev;
};

struct Edge{
  int weight;
  char* letter;
  struct Edge* next;
  struct Edge* prev;
  struct Vertex* vertex; //vertex it corresponds with, to make traversal easier
};

struct Stack{
  int top;
  int max;
  struct Vertex **items;
};

struct Stack* IndegreeStack;

void Pop(struct Stack* stack){
  if(stack->top==0) return;
  stack->top--;
  return;
}

void Push(struct Stack* stack,struct Vertex* vertex){
  if (stack->top>stack->max){
    printf("stack overflow (visited node probably in stack or repitition)\n");
    return;
  }
  stack->items[++stack->top]=vertex;
}

struct Stack* createStack(struct Stack* stack, int vertices){
  stack = malloc(sizeof(struct Stack));
  stack->top=0;
  stack->max=vertices;
  stack->items=malloc(vertices*sizeof(struct Vertex));
  return stack;
}

void insertHere(struct Edge* current, struct Vertex* correspondingVertex, char* character, int weight){
  struct Edge* new = malloc(sizeof(struct Edge));
  new->letter=malloc(20*sizeof(char*));
  strcpy(new->letter,character);
  new->weight=weight;
  new->vertex=correspondingVertex;
  new->next = current->next;
  current->next=new;
}

void insertHead(struct Vertex* current, struct Vertex* correspondingVertex, char* character, int weight){
  struct Edge* new = malloc(sizeof(struct Edge));
  new->letter=malloc(20*sizeof(char*));
  strcpy(new->letter,character);
  new->weight=weight;
  new->vertex=correspondingVertex;
  new->next = current->next;
  current->next=new;
}

void addNext(struct Vertex* vertex, struct Vertex* correspondingVertex, char* to, int weight) {
  if(vertex->next==0){ //head
    vertex->next=malloc(sizeof(struct Edge));
    vertex->next->letter=malloc(20*sizeof(char*));
    strcpy(vertex->next->letter,to);
    vertex->next->vertex=correspondingVertex;
    vertex->next->next=0;
    return;
  } else{ if(strcmp(vertex->next->letter,to)>0){
    if (strcmp(vertex->next->letter,to)==0) return;
    insertHead(vertex,correspondingVertex,to,weight);
    return;
  }}

  struct Edge* current=vertex->next; //set current as head
  if (strcmp(current->letter,to)==0) return;
  while(current->next!=0) {
    if (strcmp(current->next->letter,to)==0) return;
    if (strcmp(current->next->letter,to)>0) { //flipping this swaps the list around
      insertHere(current,correspondingVertex,to,weight);
      return;
    }
    current = current->next;
  }
  current->next=malloc(sizeof(struct Edge));
  current->next->letter=malloc(20*sizeof(char*));
  strcpy(current->next->letter,to);
  current->next->vertex=correspondingVertex;
  current->next->next=0;
}

void addPrev(struct Vertex* vertex,struct Vertex* correspondingVertex,char* to,int weight) {
  if(vertex->prev==0){ //head
    vertex->prev=malloc(sizeof(struct Edge));
    vertex->prev->letter=malloc(20*sizeof(char*));
    strcpy(vertex->prev->letter,to);
    vertex->prev->weight=weight;
    vertex->prev->vertex=correspondingVertex;
    vertex->prev->prev=0;
    return;
  }

  struct Edge* current=vertex->prev; //set current as head
  while(current->prev!=0) {
    current = current->prev;
  }
  current->prev=malloc(sizeof(struct Edge));
  current->prev->letter=malloc(20*sizeof(char*));
  strcpy(current->prev->letter,to);
  current->prev->weight=weight;
  current->prev->vertex=correspondingVertex;
  current->prev->prev=0;
}

void freeNextEdges(struct Edge* x){
  if (x==0) return;
  freeNextEdges(x->next);
  free(x->letter);
  free(x);
  return;
}

void freePrevEdges(struct Edge* x){
  if (x==0) return;
  freePrevEdges(x->prev);
  free(x->letter);
  free(x);
  return;
}

void freeVertex(struct Vertex* x){
  if (x==0) return;
  freeNextEdges(x->next);
  freePrevEdges(x->prev);
  free(x->letter);
  free(x);
  return;
}

void freeEverything(struct Vertex** graph,int vertices){
  for (size_t i = 0; i < vertices; i++) {
    freeVertex(graph[i]);
  }
  free(graph);
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
  for (size_t i = 0; i < vertices; i++) {
    for (struct Edge* current = graph[i]->prev; current!=0; current=current->prev) {
      printf(" %s<--", current->letter);
    }
    printf("  %s  ", graph[i]->letter);
    for (struct Edge* current = graph[i]->next; current!=0; current=current->next) {
      printf("-->%s ", current->letter);
    }
    printf("degree: %d\n", graph[i]->degree);
  }
}

void addVertex(struct Vertex **graph, int vertices, char* content, int position){
  struct Vertex* vertex=calloc(1, sizeof(struct Vertex));
  vertex->letter=malloc(20*sizeof(char*));
  strcpy(vertex->letter,content);
  vertex->next=0;
  vertex->visited=0;
  vertex->degree=0;
  if (graph[0]==0){
    graph[position]=vertex;
    return;
  }else if(strcmp(graph[position-1]->letter,content)>0){
    for (size_t i = position; i!=-1; i--) {
      if(i==0) {
        graph[0]=vertex;
        return;
      }
      if(strcmp(graph[i-1]->letter,content)<0){
        graph[i]=vertex;
        return;
      }
      graph[i]=graph[i-1];
    }
  }

  graph[position]=vertex;
}

void degree(struct Vertex** graph,int vertices){
  for (size_t i = 0; i < vertices; i++) {
    //in degree
    int count=0;
    for (struct Edge* current = graph[i]->prev; current!=0; current=current->prev) {
      count++;
    }
    graph[i]->degree=count;
  }
}

void fill(struct Vertex **graph, int vertices, char* source, char* to, int weight){

  //add edges
  struct Vertex* corr=0; //corresponding vertex
  for (size_t i = 0; i < vertices; i++) {

    if (strcmp(to,graph[i]->letter)==0) corr = graph[i];
    if (strcmp(source,graph[i]->letter)==0) {
      if(corr==0)  {
        for (size_t j = i; j < vertices; j++) if (strcmp(to,graph[j]->letter)==0) corr = graph[j];
      }
      addNext(graph[i],corr,to,weight);

      //undirected (does the same for the "to" vertex)
      for (size_t j = 0; j < vertices; j++){
        if (strcmp(to,graph[j]->letter)==0) {
          addPrev(graph[j],graph[i],source,weight);
          return;}}
    }
  }
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
    addVertex(graph,vertices,content,i);
  }
  read(graph,vertices);

  //add nodes
  int weight;
  char source[20],to[20];
  while ((fscanf(f,"%s %s %d",source,to,&weight))!=EOF){
    fill(graph,vertices,source,to,weight);
  }
  degree(graph,vertices);
  readAll(graph,vertices);

  IndegreeStack = createStack(IndegreeStack,vertices);

  freeEverything(graph,vertices);
  return EXIT_SUCCESS;
}
