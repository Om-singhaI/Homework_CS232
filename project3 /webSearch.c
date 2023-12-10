#include <math.h>
#include "crawler.h"
#include "indexPage.h"

int main(int argc, char* argv[]) {
    srand(atol(argv[3]));

    FILE* pipe;
    pipe = fopen(argv[1], "r");
    if (pipe == NULL) {
        fprintf(stderr, "ERROR: Could not open file %s\n", argv[1]);
        return 1; // Exit the program with an error code
    }
    
    char *url;
    url = malloc(sizeof(char) * 1000);
    if (url == NULL) {
        fprintf(stderr, "ERROR: Could not allocate memory for url\n");
        fclose(pipe); // Close the file before exiting
        return 1;
    }
    
    int hopsLimit = 0, MAX_N = atoi(argv[2]), n = 0;
    struct listNode *urls = NULL;
    struct listNode *back = NULL;
    

    while ((fscanf(pipe, "%s %d", url, &hopsLimit) != EOF) && (n < MAX_N)) {
        struct listNode* mylist = crawl(url, hopsLimit, &n, MAX_N);
        if (urls == NULL) {
            urls = mylist;
            back = urls;
            while (back->next != NULL) {
                back = back->next;
            }
        } else {
            struct listNode *temp = mylist;
            struct listNode* backup = temp;
            while ((temp != NULL) && (!(contains(urls, temp->addr)))) {
                backup = temp;
                temp = temp->next;
            }

            if (backup == temp) {
                destroyList(backup, &n);
                backup = NULL;
                temp = NULL;
                mylist = NULL;
            } else {
                destroyList(backup->next, &n);
                backup->next = NULL;
            }

            back->next = mylist;
            while (back->next != NULL) {
                back = back->next;
            }
        }
    }
    
    int num_indexed = 0;
    for (struct listNode *current = urls; current != NULL; current = current->next) {
        ++num_indexed;
    }
    
    printf("Indexing...\n");
    struct trienode* tries[num_indexed];
    int q = -1;
    for (struct listNode *current = urls; current != NULL; current = current->next) {
        tries[++q] = indexPage(current->addr, &(current->numTerms));
    }

    free(url);
    url = malloc(sizeof(char) * 1000);
    for (int r = 0; r < 1000; ++r) {
        url[r] = '\0';
    }
    printf("\nEnter a web query: ");
    fgets(url, 1000, stdin);
    while (url[0] != '\n' && url[0] != '\0') {
        int isgood = 1;
        for (int i = 0; (i < 1000) && isgood; ++i) {
            if ((url[i] != 32) && (!((url[i] >= 97) && (url[i] < 123)))) {
                if (url[i] == 0) {
                    break;
                } else if (url[i] == '\n') {
                    url[i] = '\0';
                    continue;
                } else {
                    printf("Please enter a query containing only lower-case letters.\n");
                    isgood = 0;
                }
            }
        }
        if (isgood) {
            printf("Query is %c%s%c.\n", '"', url, '"');
            printf("IDF scores are\n");

            int start = 0, afterend = 0;
            double first_place = -1.0000, second_place = -1.0000, third_place = -1.0000;
            char first_url[1000];
            char second_url[1000];
            char third_url[1000];
            for (int r = 0; r < 1000; ++r) {
                first_url[r] = '\0';
                second_url[r] = '\0';
                third_url[r] = '\0';
            }
            double score_d[num_indexed];

            for (int r = 0; r < num_indexed; ++r) {
                score_d[r] = 0;
            }
            while (url[start] != 0) {
                while ((url[afterend] != 32) && (url[afterend] != 0)) {
                    ++afterend;
                }
                url[afterend] = '\0';
                int i = -1, num_docs_contains = 0;
                double tf_d_t[num_indexed];
                for (struct listNode *current = urls; current != NULL; current = current->next) {
                    tf_d_t[++i] = 0;
                    struct trienode *root = tries[i];
                    for (int index = start; (index < afterend) && (root != NULL); ++index) {
                        root = root->children[url[index] - 97];
                    }
                    if ((root != NULL) && (root->count > 0)) {
                        tf_d_t[i] = ((double)(root->count)) / ((double)(current->numTerms));
                        ++num_docs_contains;
                    }
                }
                double idf_t = log((1.0 + (double)num_indexed) / (1.0 + (double)num_docs_contains));
                printf("IDF(%s): %.7f\n", url + start, idf_t);

                i = -1;
                for (struct listNode *current = urls; current != NULL; current = current->next) {
                    tf_d_t[++i] *= idf_t;
                    score_d[i] += tf_d_t[i];
                }
                start = ++afterend;
            }
            int i = -1;
            for (struct listNode *current = urls; current != NULL; current = current->next) {
                ++i;
                if (num_indexed >= 3) {
                    if (score_d[i] > first_place) {
                        third_place = second_place;
                        second_place = first_place;
                        first_place = score_d[i];

                        strncpy(third_url, second_url, 1000);
                        strncpy(second_url, first_url, 1000);
                        strncpy(first_url, current->addr, 1000);
                    } else if (score_d[i] > second_place) {
                        third_place = second_place;
                        second_place = score_d[i];

                        strncpy(third_url, second_url, 1000);
                        strncpy(second_url, current->addr, 1000);
                    } else if (score_d[i] > third_place) {
                        third_place = score_d[i];

                        strncpy(third_url, current->addr, 1000);
                    }
                } else if (num_indexed == 2) {
                    if (score_d[i] > first_place) {
                        second_place = first_place;
                        first_place = score_d[i];
                    
                        strncpy(second_url, first_url, 1000);
                        strncpy(first_url, current->addr, 1000);
                    } else if (score_d[i] > second_place) {
                        second_place = score_d[i];

                        strncpy(second_url, current->addr, 1000);
                    }
                } else if (num_indexed == 1) {
                    if (score_d[i] > first_place) {
                        first_place = score_d[i];

                        strncpy(first_url, current->addr, 1000);
                    }
                }
                score_d[i] = 0;
            }
            printf("Web pages:\n");
            if (num_indexed >= 3) {
                if (first_place > 0) {
                    printf("1. %s (score: %.4f)\n", first_url, first_place);
                }
                if (second_place > 0) {
                    printf("2. %s (score: %.4f)\n", second_url, second_place);
                }
                if (third_place > 0) {
                    printf("3. %s (score: %.4f)\n", third_url, third_place);
                }
                
            } else if (num_indexed == 2) {
                if (first_place > 0) {
                    printf("1. %s (score: %.4f)\n", first_url, first_place);
                }
                if (second_place > 0) {
                    printf("2. %s (score: %.4f)\n", second_url, second_place);
                }
            } else if (num_indexed == 1) {
                if (first_place > 0) {
                    printf("1. %s (score: %.4f)\n", first_url, first_place);
                }
            }
            first_place = -1.0000;
            second_place = -1.0000;
            third_place = -1.0000;
        } 
        printf("\nEnter a web query: ");
        fgets(url, 1000, stdin);
        size_t len = strlen(url);
    if (len > 0 && url[len - 1] == '\n') {
        url[len - 1] = '\0';
        }
    }
    printf("Exiting the program\n");

    for (int i = 0; i < num_indexed; ++i) {
        freeTrieMemory(tries[i]);
    }
    
    destroyList(urls, &n);
    back = NULL;

    free(url);
    fclose(pipe);
    return 0;
};