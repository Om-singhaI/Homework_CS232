//Om Singhal
//Project 2 Cs232

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Define the trie node structure
struct trienode {
  char character;    
  int count;      
  struct trienode* children[26]; 
};

// Function prototypes
struct trienode* initializeTrieNode();
struct trienode* indexPage(const char* url);
int addWordOccurrence(const char* word, const int wordLength, struct trienode* root);
void printTrieContents(struct trienode* root, char* buffer_beginning, char* buffer_end);
int freeTrieMemory(struct trienode* root);
int getText(const char* srcAddr, char* buffer, const int bufSize);
void printTrieContentsHelper(struct trienode* current, char* buffer_beginning, char* buffer_end);

// Entry point

int main(int argc, char** argv){
   
  if (argc > 1) {
    struct trienode* root = indexPage(argv[1]);
    char buffer[300001];
    printTrieContents(root, buffer, buffer);    
    int x = freeTrieMemory(root);   
  }
  
  return 0;
}

// Initialize a new trie node
struct trienode* initializeTrieNode() {
    struct trienode* root = malloc(sizeof(struct trienode));
    for (int i = 0; i < 26; ++i) {
        root->children[i] = NULL;  
    }
    root->character = ' ';   
    root->count = 0;  
    return root;    
}

struct trienode* indexPage(const char* url) {
  char buffer[300001];
  int bufferlen = getText(url, buffer, 300001);   
  printf("%s", url);    

  struct trienode* root = initializeTrieNode();  

  int currentIndex = 0;    
  while (currentIndex < bufferlen) {
     int wordStratIndex = currentIndex;
    for (; wordStratIndex < bufferlen; ++wordStratIndex) {
       
      if (!(((buffer[wordStratIndex] >= 97) && (buffer[wordStratIndex] < 123)) || ((buffer[wordStratIndex] >= 65) && (buffer[wordStratIndex] < 91)))) {
        break;
      }
    }
     
    if (wordStratIndex != currentIndex) {
      currentIndex += addWordOccurrence(buffer + currentIndex, wordStratIndex - currentIndex, root);
    } else {
      ++currentIndex;
    }
  }
  putchar('\n');    

  return root;
}

// Add occurrence of a word to the trie
int addWordOccurrence(const char* word, const int wordLength, struct trienode* root) {
  printf("\n\t");
  
  struct trienode* current = root;  
  int charIndex = 0;
  
  for (; charIndex < wordLength; ++charIndex) {
    char toLower = (word[charIndex] >= 97)? word[charIndex] : word[charIndex] + 32;

    if (current->children[toLower - 97] == NULL) {
      current->children[toLower - 97] = initializeTrieNode();
    }

    current = current->children[toLower - 97];
    current->character = toLower;    
    putchar(current->character);   
  }
  current->count++;   
  return charIndex;   

}

// Print the contents of the trie
void printTrieContents(struct trienode* root, char* buffer_beginning, char* buffer_end) {
  if (root != NULL) {
    for (int i = 0; i < 26; ++i) {    
      printTrieContentsHelper(root->children[i], buffer_beginning, buffer_end);
    }
  }
}

// Helper function for printing trie contents
void printTrieContentsHelper(struct trienode* current, char* buffer_beginning, char* buffer_end) {
  if (current != NULL) {
    *buffer_end = current->character;
    if (current->count != 0) {    
      *(buffer_end + 1) = '\0';   
      printf("%s: %d\n", buffer_beginning, current->count);   
    }
    for (int i = 0; i < 26; ++i) {    
      printTrieContentsHelper(current->children[i], buffer_beginning, buffer_end + 1);
    }
  }
}

// Free the memory allocated for the trie
int freeTrieMemory(struct trienode* root) {
  if (root != NULL) {
    for (int i = 0; i < 26; ++i) {
      int x = freeTrieMemory(root->children[i]);
    }
    free(root);
  }
  return 0;
}

 /* You should not need to modify this function */
int getText(const char* srcAddr, char* buffer, const int bufSize){
  FILE *pipe;
  int bytesRead;

  snprintf(buffer, bufSize, "curl -s \"%s\" | python3 getText.py", srcAddr);

  pipe = popen(buffer, "r");
  if(pipe == NULL){
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n",
 	  buffer);
    return 0;
  }

  bytesRead = fread(buffer, sizeof(char), bufSize-1, pipe);
  buffer[bytesRead] = '\0';

  pclose(pipe);

  return bytesRead;
}