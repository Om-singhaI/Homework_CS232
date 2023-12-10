#include "indexPage.h"

struct trienode* initializeTrieNode() {
    struct trienode* root = malloc(sizeof(*root));
    for (int i = 0; i < 26; ++i) {
        root->children[i] = NULL;   
    }
    root->ch = ' ';   
    root->count = 0;  

    return root;   
}

struct trienode* indexPage(const char* url, int *totalNumTerms) {
  char buffer[300001];
  int bufferlen = getText(url, buffer, 300001);   
  printf("%s", url);   

  struct trienode* root = initializeTrieNode();   
  int i = 0;   
  while (i < bufferlen) {
    int j = i;
    for (; j < bufferlen; ++j) {
      if (!(((buffer[j] >= 97) && (buffer[j] < 123)) || ((buffer[j] >= 65) && (buffer[j] < 91)))) {
        break;
      }
    }
    if (j != i) {
      i += addWordOccurrence(buffer + i, j - i, root, totalNumTerms);
    } else {
      ++i;
    }
  }
  putchar('\n');    
  return root;
}

int addWordOccurrence(const char* word, const int wordLength, struct trienode* root, int *totalNumTerms) {
  printf("\n\t");   

  struct trienode* current = root; 
  int i = 0;
  
  for (; i < wordLength; ++i) {
    char toLower = (word[i] >= 97)? word[i] : word[i] + 32;
    if (current->children[toLower - 97] == NULL) {
      current->children[toLower - 97] = initializeTrieNode();
    }

    current = current->children[toLower - 97];
    current->ch = toLower;    
    putchar(current->ch);   
  }
  current->count++;  
  ++(*totalNumTerms);

  return i;}

void printTrieContents(struct trienode* root, char* buffer_beginning, char* buffer_end) {
  if (root != NULL) {
    for (int i = 0; i < 26; ++i) {    //Print for each starting letter
      printTrieContentsHelper(root->children[i], buffer_beginning, buffer_end);
    }}}


void printTrieContentsHelper(struct trienode* current, char* buffer_beginning, char* buffer_end) {
  if (current != NULL) {
    *buffer_end = current->ch;
    if (current->count != 0) {    //At a word, so print
      *(buffer_end + 1) = '\0';   //Add null terminating string at end of word
      printf("%s: %d\n", buffer_beginning, current->count);   //Print word and its count
    }
    for (int i = 0; i < 26; ++i) {    //Recursively print a char and each char following
      printTrieContentsHelper(current->children[i], buffer_beginning, buffer_end + 1);
    }
  }
}

void freeTrieMemory(struct trienode* root) {
  if (root != NULL) {
    for (int i = 0; i < 26; ++i) {
      freeTrieMemory(root->children[i]);
    }
    free(root);
  }
}


int getText(const char* srcAddr, char* buffer, const int bufSize){
  FILE *pipe;
  int bytesRead;

  snprintf(buffer, bufSize, "curl -s \"%s\" | python3 getText.py", srcAddr);

  pipe = popen(buffer, "r");
  if(bytesRead < 0){
     bytesRead = fread(buffer, sizeof(char), bufSize-1, pipe);
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n",
	    buffer);
    return 0;
  }

  bytesRead = fread(buffer, sizeof(char), bufSize-1, pipe);
  buffer[bytesRead] = '\0';

  pclose(pipe);

  return bytesRead;
}
