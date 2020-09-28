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
  for (size_t i = 0; i < vertices; i++) {
    printf("%s\t", graph[i]->letter);
    printf("\n");
  }
  return;
}

void degree(struct Node** graph,int vertices,char* letter){
  for (size_t i = 0; i < vertices; i++) {
    if(strcmp(letter,graph[i]->letter)==0){
      int count=0;
      for (struct Node* current = graph[i]; current!=0; current=current->next) count++;
      printf("%d\n",count);
      return;
    }
  }
}

void adjacent(struct Node** graph,int vertices,char* letter){
  for (size_t i = 0; i < vertices; i++) {
    if(strcmp(letter,graph[i]->letter)==0){
      for (struct Node* current = graph[i]; current!=0; current=current->next) printf("%s\t", current->letter);
      printf("\n");
      return;
    }
  }
}

int main(int argc, char *argv[argc+1]) {
  if (argc>3 || argc==1) {
    printf("Please enter 1 file name.\n");
    return EXIT_SUCCESS;
  }

  FILE *f;
  f = fopen(argv[1],"r");
  if (f==0) {
    printf("error\n");
    return EXIT_SUCCESS;
  }

  int vertices;
  fscanf(f,"%d\n",&vertices);

  struct Node **graph = calloc(vertices,sizeof(struct Node));
  for (size_t i = 0; i < vertices; i++) {
    struct Node* head=calloc(1, sizeof(struct Node));
    char content[2];
    fscanf(f,"%s\n",content);
    head->letter=malloc(2);
    strcpy(head->letter,content);
    head->next=0;
    graph[i]=head;
  }
  //read(graph,vertices);


  char source[2],to[2];
  while ((fscanf(f,"%s %s",source,to))!=EOF){
    for (size_t i = 0; i < vertices; i++) {
      if (strcmp(source,graph[i]->letter)==0) {
        struct Node* current = graph[i];
        while(current->next!=0) current = current->next;
        current->next=calloc(1,sizeof(struct Node));
        current->next->letter=malloc(2);
        strcpy(current->next->letter,to);
        current->next->next=0;
      }
    }
  }

  f = fopen(argv[2],"r");
  if (f==0) {
    printf("error\n");
    return EXIT_SUCCESS;
  }

  char decision[2];
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
