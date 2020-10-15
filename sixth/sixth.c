#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

struct Vertex{
  char *letter;
  int distance, degree, visited;
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

int visitedCount=0;

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
    vertex->next->weight=weight;
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
  current->next->weight=weight;
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

void freeEverything(struct Vertex** graph, int vertices){
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
    printf("%s: ", graph[i]->letter);
    for (struct Edge* current = graph[i]->next; current!=0; current=current->next) {
      printf("%s%d ", current->letter,current->weight);
    }
    printf("\n");
  }

  for (size_t i = 0; i < vertices; i++) {
    for (struct Edge* current = graph[i]->prev; current!=0; current=current->prev) {
      printf(" %s<--", current->letter);
    }
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
  vertex->degree=0;
  vertex->distance=INT_MAX;
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
  visitedCount++;
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
  while ((fscanf(f,"%s %s %d",source,to,&weight))!=EOF) fill(graph,vertices,source,to,weight);

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
