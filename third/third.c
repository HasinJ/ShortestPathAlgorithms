#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Vertex{
  char *letter;
  int visited;
  struct Edge* next;

};

struct Edge{
  //int weight;
  char* letter;
  struct Edge* next;
  struct Vertex* vertex; //vertex it corresponds with, to make traversal easier
};

struct Node{
  char *letter;
  struct Node* next;
};

struct Node* qHead=0;

void Delete(char* x){ //should deal with an already sorted list
  if (qHead==0) return;
  else{
    if (strcmp(qHead->letter,x)==0) {
      struct Node* temp=qHead;
      qHead=qHead->next;
      free(temp->letter);
      free(temp);
      return;
    }
  }

  struct Node* current=qHead;
  while (strcmp(current->next->letter,x)==1) {
    current=current->next;
    if (current->next==0) return;
  }
  struct Node* temp=current->next;
  current->next=current->next->next;
  free(temp->letter);
  free(temp);

}

void Push(char* x){
  if (qHead==0){
    qHead = calloc(1,sizeof(struct Node));
    qHead->letter=malloc(20*sizeof(char*));
    strcpy(qHead->letter,x);
    return;
  }
  struct Node* new = (struct Node*) malloc(sizeof(struct Node));
  new->letter=malloc(20*sizeof(char*));
  strcpy(new->letter,x);
  new->next=qHead;
  qHead=new;
}

void Enqueue(char* x){
  if(qHead==0) {
    Push(x);
    return;
  }
  struct Node* current=qHead;
  while(current->next!=0){
    current=current->next;
  }
  current->next = calloc(1,sizeof(struct Node));
  current->next->letter=malloc(20*sizeof(char*));
  strcpy(current->next->letter,x);
  current->next->next = 0;
}

int compareString(char* a, char* b){
  if (strlen(a)>strlen(b)) return 1;
  if (strlen(a)<strlen(b)) return 0;
  if (a==b) return 1;

  for (size_t i = 0; i < strlen(a); i++) {
    //printf("%c vs %c\n", a[i],b[i]);
    if(a[i]==b[i]) continue;
    if(a[i]>b[i]) return 1;
    if(a[i]<b[i]) return 0;
  }

  return 0;
}

void insertHere(struct Edge* current, char* character, struct Vertex* correspondingVertex){
  struct Edge* new = malloc(sizeof(struct Edge));
  new->letter=malloc(20*sizeof(char*));
  strcpy(new->letter,character);
  new->vertex=correspondingVertex;
  new->next = current->next;
  current->next=new;
}

void insertHead(struct Vertex* current, char* character, struct Vertex* correspondingVertex){
  struct Edge* new = malloc(sizeof(struct Edge));
  new->letter=malloc(20*sizeof(char*));
  strcpy(new->letter,character);
  new->vertex=correspondingVertex;
  new->next = current->next;
  current->next=new;
}

void addEdge(struct Vertex* vertex, char* to, struct Vertex* correspondingVertex) {
  if(vertex->next==0){ //head
    vertex->next=malloc(sizeof(struct Edge));
    vertex->next->letter=malloc(20*sizeof(char*));
    strcpy(vertex->next->letter,to);
    vertex->next->vertex=correspondingVertex;
    vertex->next->next=0;
    return;
  } else{ if(compareString(vertex->next->letter,to)==1){
    insertHead(vertex,to,correspondingVertex);
    return;
  }}

  struct Edge* current=vertex->next; //set current as head
  while(current->next!=0) {
    if (compareString(current->next->letter,to)==1) { //flipping this swaps the list around
      insertHere(current,to,correspondingVertex);
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
  for (size_t i = 0; i < vertices; i++) {
    printf("%s: ", graph[i]->letter);
    for (struct Edge* current = graph[i]->next; current!=0; current=current->next) {
      printf("%s ", current->letter);
    }
    printf("\n");
  }
  return;
}

void correspondingVertexTest(struct Vertex **graph, int vertices, char* source){
  printf("\n");
  for (size_t i = 0; i < vertices; i++) {
    if(strcmp(graph[i]->letter,source)==0){
      struct Edge* current = graph[i]->next->vertex->next;
      while(current!=0){
        printf("%s %d\n", current->vertex->letter,current->vertex->visited);

        current=current->next;
      }
    }
  }
  printf("\n");
}

void fill(FILE* f, struct Vertex** graph, int vertices, char* source, char* to){
  //add edges
  struct Vertex* corr=0; //corresponding vertex
  for (size_t i = 0; i < vertices; i++) {

    if (strcmp(to,graph[i]->letter)==0) corr = graph[i];
    if (strcmp(source,graph[i]->letter)==0) {
      if(corr==0)  {
        for (size_t j = i; j < vertices; j++) if (strcmp(to,graph[j]->letter)==0) corr = graph[j];
      }
      addEdge(graph[i],to,corr);

      //undirected (does the same for the "to" vertex)
      for (size_t j = 0; j < vertices; j++){
        if (strcmp(to,graph[j]->letter)==0) {
          addEdge(graph[j],source,graph[i]);
          return;}}
    }
  }
}

void reset(struct Vertex** graph, int vertices){
  for (size_t i = 0; i < vertices; i++) {
    graph[i]->visited=0;
  }
}

void traverse(struct Vertex** graph, int vertices, char* source){
  for (size_t i = 0; i < vertices; i++) {
    if (strcmp(graph[i]->letter,source)==0) {
      graph[i]->visited=1;
      struct Vertex* root = 0;
      struct Edge* current = graph[i]->next;
      int count=1;
      //printf("\n");
      while(count!=vertices){
        if (current->vertex->visited==0) {
          printf("%s ", current->letter);
          count++;
          //printf("%d ", count);
          current->vertex->visited=1;
        }else{
          current=current->next;
          continue;
        }

        if(root==0) {
          struct Vertex* prevRoot = current->vertex; //find root function that surfs through all vertices
          root=current->vertex;
        }
        if(current->next==0){
          current=root->next;
          root=0;
          continue;
        }
        current=current->next;
      }
      printf("\n");
      return;
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
    struct Vertex* head=calloc(1, sizeof(struct Vertex));
    char content[20];
    //graph
    fscanf(f,"%s\n",content);
    head->letter=malloc(20*sizeof(char*));
    strcpy(head->letter,content);
    head->next=0;
    head->visited=0;
    graph[i]=head;

    //queue
    Enqueue(content);
  }
  //read(graph,vertices);

  //add edges
  char source[20],to[20];
  while ((fscanf(f,"%s %s",source,to))!=EOF){
    fill(f,graph,vertices,source, to);
  }
  readAll(graph,vertices);
  correspondingVertexTest(graph,vertices,"B");

  f = fopen(argv[2],"r");
  if (f==0) {
    printf("error\n");
    return EXIT_SUCCESS;
  }

  while (fscanf(f,"%s",source)!=EOF) {
    printf("%s ",source);
    traverse(graph,vertices,source);
    reset(graph,vertices);
  }
  freeEverything(graph,vertices);
}
