#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Node{
  char *letter;
  int order;
  struct Node* next;
};

int giveOrderNum(struct Node** graph, int vertices,char* character){
  for (size_t i = 0; i < vertices; i++) {
    if(strcmp(graph[i]->letter,character)==0)  return graph[i]->order;
  }
  return EXIT_SUCCESS;
}

void insertHere(struct Node* current, int order, char* character){
  struct Node* new = calloc(1,sizeof(struct Node));
  new->letter=malloc(10);
  strcpy(new->letter,character);
  new->order=order;
  new->next = current->next;
  current->next=new;
  return;
}

void addNode(struct Node** graph, int vertices, struct Node* current, char* to) {
  int order = giveOrderNum(graph,vertices,to); //order number for "to" char
  if(current->next==0){
    current->next=calloc(1,sizeof(struct Node));
    current->next->letter=malloc(10);
    strcpy(current->next->letter,to);
    current->next->order=order;
    current->next->next=0;
    return;
  }

  //dealing with inserting at head
  if (current->next->order>order) {
    insertHere(current,order,to);
    return;
  }

  while(current->next!=0) {
    current = current->next;
    if (current->order>order) {
      insertHere(current,order,to);
      return;
    }
  }
  current->next=calloc(1,sizeof(struct Node));
  current->next->letter=malloc(10);
  strcpy(current->next->letter,to);
  current->next->next=0;
}

void freeLinkedList(struct Node* x){
  if (x==0) return;
  freeLinkedList(x->next);
  free(x->letter);
  free(x);
  return;
}

void freeEverything(struct Node** graph,int vertices){
  for (size_t i = 0; i < vertices; i++) {
    freeLinkedList(graph[i]);
  }
  free(graph);
}

void read(struct Node **graph, int vertices){
  for (size_t i = 0; i < vertices; i++) {
    printf("%s ", graph[i]->letter);
    printf("\n");
  }
  return;
}

void degree(struct Node** graph,int vertices,char* letter){
  for (size_t i = 0; i < vertices; i++) {
    if(strcmp(letter,graph[i]->letter)==0){
      int count=0;
      for (struct Node* current = graph[i]->next; current!=0; current=current->next) {
        count++;
      }
      printf("%d\n",count);
      return;
    }
  }
}

void adjacent(struct Node** graph,int vertices,char* letter){
  for (size_t i = 0; i < vertices; i++) {
    if(strcmp(letter,graph[i]->letter)==0){
      for (struct Node* current = graph[i]->next; current!=0; current=current->next) {
        printf("%s ", current->letter);
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
  struct Node **graph = calloc(vertices,sizeof(struct Node));
  for (size_t i = 0; i < vertices; i++) {
    struct Node* head=calloc(1, sizeof(struct Node));
    char content[10];
    fscanf(f,"%s\n",content);
    head->letter=malloc(10);
    strcpy(head->letter,content);
    head->next=0;
    head->order=i;
    graph[i]=head;
  }
  //read(graph,vertices);


  //add nodes
  char source[10],to[10];
  while ((fscanf(f,"%s %s",source,to))!=EOF){
    for (size_t i = 0; i < vertices; i++) {
      if (strcmp(source,graph[i]->letter)==0) {
        struct Node* current = graph[i];
        addNode(graph,vertices,current,to);
      }
      //for undirection
      if (strcmp(to,graph[i]->letter)==0) {
        struct Node* current = graph[i];
        addNode(graph,vertices,current,source);
      }
    }
  }

  f = fopen(argv[2],"r");
  if (f==0) {
    printf("error\n");
    return EXIT_SUCCESS;
  }

  char decision[10];
  while((fscanf(f,"%s %s", decision,source))!=EOF) {
    if(strcmp(decision,"d")==0) {
      degree(graph,vertices,source);
    } else{ if(strcmp(decision,"a")==0){
      adjacent(graph,vertices,source);
    }}
  }

  freeEverything(graph,vertices);


  return EXIT_SUCCESS;
}
