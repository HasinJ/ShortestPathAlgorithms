#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Node{
  char *letter;
  struct Node* next;
};

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
  printf("\n");
  for (size_t i = 0; i < vertices; i++) {
    printf("%s", graph[i]->letter);
    printf("\n");
  }
  return;
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
    char content[20];
    fscanf(f,"%s\n",content);
    head->letter=malloc(20*sizeof(char*));
    strcpy(head->letter,content);
    head->next=0;
    graph[i]=head;
  }
  read(graph,vertices);
  freeEverything(graph,vertices);
}
