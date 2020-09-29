#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Node{
  char *letter;
  struct Node* next;
};

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


void insertHere(struct Node* current, char* character){
  struct Node* new = malloc(sizeof(struct Node));
  new->letter=malloc(20);
  strcpy(new->letter,character);
  new->next = current->next;
  current->next=new;
}

void addNode(struct Node** graph, int vertices, struct Node* current, char* to) {
  struct Node* head=current->next;
  if(head==0){
    current->next=malloc(sizeof(struct Node));
    current->next->letter=malloc(20);
    strcpy(current->next->letter,to);
    current->next->next=0;
    return;
  }

  while(current->next!=0) {
    if (compareString(current->next->letter,to)==1) {
      insertHere(current,to);
      return;
    }
    current = current->next;
  }
  current->next=malloc(sizeof(struct Node));
  current->next->letter=malloc(20);
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
  printf("\n");
  for (size_t i = 0; i < vertices; i++) {
    printf("%s", graph[i]->letter);
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
    char content[20];
    fscanf(f,"%s\n",content);
    head->letter=malloc(20);
    strcpy(head->letter,content);
    head->next=0;
    graph[i]=head;
  }
  //read(graph,vertices);


  //add nodes
  char source[20],to[20];
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

  char decision[20];
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
