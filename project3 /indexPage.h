#ifndef INDEXPAGE_H
#define INDEXPAGE_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct trienode {
    char ch;    
    int count;      
    struct trienode* children[26];    //26 children for each letter
};

struct trienode* initializeTrieNode();

struct trienode* indexPage(const char* url, int *totalNumTerms);

int addWordOccurrence(const char* word, const int wordLength, struct trienode* root, int *totalNumTerms);

void printTrieContents(struct trienode* root, char* buffer_beginning, char* buffer_end);

void freeTrieMemory(struct trienode* root);

int getText(const char* srcAddr, char* buffer, const int bufSize);

void printTrieContentsHelper(struct trienode* current, char* buffer_beginning, char* buffer_end);
#endif