#ifndef CRAWLER_H
#define CRAWLER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "indexPage.h"

struct listNode{
  int numTerms;
  struct listNode *next;
  char addr[1000];
};

int contains(const struct listNode *pNode, const char *addr);

void insertBack(struct listNode *pNode, const char *addr);

void printAddresses(const struct listNode *pNode);

void destroyList(struct listNode *pNode, int *n);
int getLink(const char* srcAddr, char* link, const int maxLinkLength);

struct listNode* crawl(char* start_url, int numHops, int *n, int MAX_N);
#endif