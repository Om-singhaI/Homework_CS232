#include "crawler.h"

#define MAX_ADDR_LENGTH 1000

struct listNode* crawl(char* start_url, int numHops, int *n, int MAX_N){
  char startAddr[MAX_ADDR_LENGTH];
  char destAddr[MAX_ADDR_LENGTH];
  
  int hopNum;
  
  struct listNode *pListStart;

  strncpy(startAddr, start_url, MAX_ADDR_LENGTH);
  startAddr[MAX_ADDR_LENGTH - 1] = '\0';

  pListStart = malloc(sizeof(struct listNode));
  if(pListStart == NULL){
    fprintf(stderr, "ERROR: could not allocate memory\n");
    return NULL;
  }
  strncpy(pListStart->addr, startAddr, MAX_ADDR_LENGTH);
  pListStart->next = NULL;
  pListStart->numTerms = 0;
  ++(*n);
  for(hopNum=1; hopNum <= numHops ; hopNum++){
    int res = getLink(startAddr, destAddr, MAX_ADDR_LENGTH);
    //printf("Iteration %d: res=%d, startAddr=%s, destAddr=%s\n", hopNum, res, startAddr, destAddr);

    if(!res){
      break;
    }

    if ((!(contains(pListStart, destAddr))) && (*n < MAX_N)) {
      insertBack(pListStart, destAddr);
      strncpy(startAddr, destAddr, MAX_ADDR_LENGTH);
      ++(*n);
    } else {
      break;
    }
  }
  return pListStart;
}

int contains(const struct listNode *pNode, const char *addr){
 
  for (const struct listNode *current = pNode; current != NULL; current = current->next) {
    int equal = 1;
if (strcmp(current->addr, addr) == 0) {
      return 1; // Strings are equal
    }
  }
  return 0; // String not found
}
    
void insertBack(struct listNode *pNode, const char *addr){
  struct listNode *endNode = pNode;
  while (endNode->next != NULL) endNode = endNode->next;

  endNode->next = malloc(sizeof(struct listNode));
  if (endNode->next == NULL) {
    fprintf(stderr, "ERROR: could not allocate memory\n");
    return; // or handle the error in an appropriate way
  }
  endNode = endNode->next;
  endNode->next = NULL;
  endNode->numTerms = 0;

  strncpy(endNode->addr, addr, MAX_ADDR_LENGTH);
}

void printAddresses(const struct listNode *pNode){
  for (const struct listNode *current = pNode; current != NULL; current = current->next) {
    printf("%s\n",current->addr);
    putchar('\n');
  }
}

void destroyList(struct listNode *pNode, int *n){
  while (pNode != NULL) {
    struct listNode* nextNode = pNode->next;
    free(pNode);
    --(*n);
    pNode = nextNode;
  }
}

int getLink(const char* srcAddr, char* link, const int maxLinkLength){
  const int bufSize = 1000;
  char buffer[bufSize];

  int numLinks = 0;

  FILE *pipe;

  snprintf(buffer, bufSize, "curl -s \"%s\" | python3 getLinks.py", srcAddr);

  pipe = popen(buffer, "r");
  if(pipe == NULL){
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n", buffer);
    return 0;
  }

  fscanf(pipe, "%d\n", &numLinks);

  if(numLinks > 0){
    int linkNum;
    double r = (double)rand() / ((double)RAND_MAX + 1.0);

    for(linkNum=0; linkNum<numLinks; linkNum++){
      fgets(buffer, bufSize, pipe);
      
      if(r < (linkNum + 1.0) / numLinks){
		    break;
      }
    }
    strncpy(link, buffer, maxLinkLength);
    link[maxLinkLength-1] = '\0';
    {
      char* pNewline = strchr(link, '\n');
      if(pNewline != NULL){
		    *pNewline = '\0';
      }
    }
  }

  pclose(pipe);

  if(numLinks > 0){
    return 1;
  }
  else{
    return 0;
  }
}
