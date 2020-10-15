#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

struct Vertex{
  char *letter;
  int distance, degree, visited;
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

void createStack(int vertices){
  stack = malloc(sizeof(struct Stack));
  stack->top=0;
  stack->max=vertices;
  stack->items=malloc(vertices*sizeof(struct Vertex));
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
  while (stack->top!=0) Pop();
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

  for (size_t i = 0; i < vertices; i++) {
    printf("%s: ", graph[i]->letter);
    for (struct Edge* current = graph[i]->next; current!=0; current=current->next) {
      printf("%s%d ", current->letter,current->weight);
    }
    printf("\n");
  }

  for (size_t i = 0; i < vertices; i++) {
    printf("  %s  ", graph[i]->letter);
    for (struct Edge* current = graph[i]->next; current!=0; current=current->next) {
      printf("-->%s ", current->letter);
    }
    printf("\n");
  }

  for (size_t i = 0; i < vertices; i++) {
    printf("%s ",graph[i]->letter);
    if(graph[i]->distance==INT_MAX) {
      printf("INF\n");
      continue;}
    printf("%d\n", graph[i]->distance);
  }
  printf("\n");
}

void addVertex(struct Vertex **graph, int vertices, char* content, int position){
  struct Vertex* vertex=calloc(1, sizeof(struct Vertex));
  vertex->letter=malloc(20*sizeof(char*));
  strcpy(vertex->letter,content);
  vertex->next=0;
  vertex->visited=0;
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


void reset(struct Vertex** graph, int vertices){
  for (size_t i = 0; i < vertices; i++) {
    graph[i]->distance = INT_MAX;
    graph[i]->visited = 0;
  }

}

void visit(struct Vertex* current){
  struct Edge* adjacent = current->next;
  while(adjacent!=0){
    int known, from, distance;
    known = adjacent->vertex->distance;
    from = current->distance;
    distance = adjacent->weight + from;

    if(known==INT_MAX) adjacent->vertex->distance=distance;
    if(distance<known) adjacent->vertex->distance=distance;

    adjacent = adjacent->next;
  }
  current->visited=1;
}

struct Vertex* smallestUnvisited(struct Vertex** graph, struct Vertex* current,int vertices){
  struct Vertex* smallest=0;
  for (size_t j = 0; j < vertices; j++) {
    for (size_t i = 0; i < vertices; i++) {
      if(graph[i]->visited==1) continue;
      if(graph[i]->distance==INT_MAX) continue;
      if(smallest==0) {
        smallest=graph[i];
        continue;
      }
      if(graph[i]->distance<smallest->distance) smallest=graph[i];
    }
  }
  return smallest;
}

void Dijkstra(struct Vertex** graph, int vertices, char* src){
  struct Vertex* current=0;
  //source
  for (size_t i = 0; i < vertices; i++) {
    if(strcmp(graph[i]->letter,src)==0){
      graph[i]->distance=0;
      visit(graph[i]);

      for (size_t j = 0; j < vertices-1; j++) {
        current = smallestUnvisited(graph,current,vertices);
        if(current==0) return;
        //printf(" current: %s\n", current->letter);
        visit(current);
      }
      return;
    }
  }
}

void readAnswer(struct Vertex** graph,int vertices){
  for (size_t i = 0; i < vertices; i++) {
    printf("%s ",graph[i]->letter);
    if(graph[i]->distance==INT_MAX) {
      printf("INF\n");
      continue;}
    printf("%d\n", graph[i]->distance);
  }
}

void dfs(struct Vertex* root,struct Vertex* current){

  while(stack->top!=0){
    struct Edge* temp = current->next;
    if (temp==0){
      Pop();
      current=Peek();
      //readStack();
      dfs(root,current);
      return;}

    while(temp->vertex->visited!=0){
      if(temp->vertex->visited==1 && strcmp(temp->letter,root->letter)==0) {
        //printf("(%s) ", temp->letter);
        printf("CYCLE\n");
        exit(EXIT_FAILURE);
      };
      temp=temp->next;
      if (temp==0){
        Pop();
        current=Peek();
        //readStack();
        dfs(root,current);
        return;}
    }

    Push(temp->vertex);
    current=Peek();
    current->visited=1;
    //printf("%s ", current->letter);
    while(current->next==0){
      Pop();
      current=Peek();
    }
  }
}

void checkLoop(struct Vertex** graph, int vertices){
  createStack(vertices);

  for (size_t i = 0; i < vertices; i++) {
    reset(graph,vertices);
    Push(graph[i]);
    //printf("current: %s ", Peek()->letter);
    Peek()->visited=1;
    dfs(graph[i],graph[i]);
  }
  printf("\n");
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
  //read(graph,vertices);


  //add nodes
  int weight;
  char source[20],to[20];
  while ((fscanf(f,"%s %s %d",source,to,&weight))!=EOF) fill(f,graph,weight,vertices,source,to);

  checkLoop(graph, vertices);


  f = fopen(argv[2],"r");
  if (f==0) {
    printf("error\n");
    return EXIT_SUCCESS;
  }


  while(fscanf(f,"%s",source)!=EOF){
    reset(graph,vertices);
    Dijkstra(graph,vertices,source);
    readAnswer(graph,vertices);
    printf("\n");
  }

  freeEverything(graph,vertices);
  //readAll(graph,vertices);

}
