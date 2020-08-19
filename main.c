#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define M 1024
#define N 100


typedef enum{c, d}COMMAND;

struct commandsList {
    int id;
    COMMAND com;
    char **document;
    struct commandsList *prec;
    struct commandsList *next;
};

typedef struct commandsList list;


void changeText(int fromLine, int toLine, char **doc, char **text);
void deleteText(int fromLine, int toLine, char **doc);
void printText(int fromLine, int toLine, char **doc);


void insertHead(list *node, char **doc, COMMAND c);
void insertTail(list *node, char **doc, COMMAND c);
void removeHead(list *node);
void removeTail(list *node);


int main() {

    int i;
    char comando[M];
    char com;
    char *p;
    char *punt[3];
    //char *text;

    list node;
    node.id = 0;
    node.prec = NULL;
    node.next = NULL;


    char **document = malloc(N*sizeof(char*));


    do {

        strcpy(comando, "");
        char *result = fgets(comando, M, stdin);
        int firstLine, secondLine, line;

        if(sscanf(comando, "%d,%d%c\n", &firstLine, &secondLine, &com) == 3) {


            if(com == 'c') {


                int grandezza = (secondLine - firstLine) + 2;
                //char text[grandezza][M];
                char *text[grandezza];


                for(int k = 0; k < grandezza; k++) {

                    strcpy(comando, "");
                    text[k] = malloc(strlen(comando)*sizeof(char));
                    p = fgets(comando, M, stdin);
                    strcpy(text[k], comando);
                }
                //text[grandezza - 1] = malloc(sizeof(char[M]));
                //strcpy(text[grandezza - 1], ".");

                //TODO: il punto va messo sempre con fgets?

                /*printf("%d", firstLine);
                printf("\n");
                printf("%d", secondLine);
                printf("\n");

                for(int l = 0; l < (secondLine - firstLine) + 2; l++) {
                    printf(textSupport[l]);
                    printf("\n");
                }*/

                changeText(firstLine, secondLine, document, text);

                /*for(int k = grandezza - 1; k >= 0; k--) {
                    free(text[k]);
                }*/

                for(int k = 0; k < grandezza; k++) {
                    free(text[k]);
                }
                //free(text);

            }
            else if(com == 'd') {

                deleteText(firstLine, secondLine, document);

            }
            else if(com == 'p') {

                printText(firstLine, secondLine, document);

            }

        }
        else if(sscanf(comando, "%d%c\n", &line, &com) == 2) {

            if(com == 'u') {

            }
            else if(com == 'r') {

            }

        }


    } while(strncmp(comando, "q", 1) != 0);




    return 0;
}




void changeText(int fromLine, int toLine, char **doc, char **text) {

    for(int i = fromLine; i <= toLine; i++) {

        if(doc[i-1] != NULL) {
            free(doc[i-1]);
            doc[i-1] = malloc(strlen(text[i-1])*sizeof(char));
            strcpy(doc[i-1], text[i-fromLine]);
        }
        else {

            if(i - 1 == N + 1) {
                doc = realloc(doc, 2*N*sizeof(char*));
            }

            doc[i-1] = malloc(strlen(text[i-1])*sizeof(char));
            strcpy(doc[i-1], text[i-fromLine]);
        }

    }

}


void deleteText(int fromLine, int toLine, char **doc) {

    int deletedLines = 0;
    //size_t sizeBefore = sizeof(doc)/sizeof(doc[0]);

    if(doc[toLine + 1] == NULL) {
        for(int i = fromLine; i <= toLine; i++) {

            if(doc[i-1] == NULL) {
                continue;
            }
            else {
                deletedLines++;
                free(doc[i-1]);
                doc[i-1] = NULL;
            }

        }
    }
    else{
        for(int i = fromLine; i <= toLine; i++) {
            deletedLines++;
            free(doc[i-1]);
            doc[i-1] = NULL;
        }

        for(int i = toLine; doc[i] != NULL; i++) {

            if(doc[i - deletedLines] == NULL) {
                doc[i - deletedLines] = malloc(strlen(doc[i])*sizeof(char));
                strcpy(doc[i - deletedLines], doc[i]);
                free(doc[i]);
                doc[i] = NULL;
            }
            else {
                free(doc[i - 1 - deletedLines]);
                doc[i - 1 - deletedLines] = malloc(strlen(doc[i - 1])*sizeof(char));
                strcpy(doc[i - 1 - deletedLines], doc[i - 1]);
                free(doc[i - 1]);
                doc[i - 1] = NULL;
            }

        }
    }

}

void printText(int fromLine, int toLine, char **doc) {

    for (int i = fromLine; i <= toLine; i++) {
        if(doc[i - 1] == NULL) {
            fputs(".\n", stdout);
        }
        else {
            fputs(doc[i - 1], stdout);
        }
    }

}



void insertHead(list *node, char **doc, COMMAND c) {

    list *n = malloc((sizeof(list)));
    n->com = c;
    for(int i = 0; doc[i] != NULL; i++) {
        n->document[i] = malloc(strlen(doc[i])*sizeof(char));
        strcpy(n->document[i], doc[i]);
    }
    n->prec = NULL;
    n->next = node;
    *node = *n;

}

void insertTail(list *node, char **doc, COMMAND c) {

    if(node == NULL) {
        insertHead(node, doc, c);
    }
    else {
        insertTail(node->next, doc, c);
    }

}

void removeHead(list *node) {

    if(node != NULL) {
        list *old = node;
        *node = *(old->next);
        free(old);
    }

}

void removeTail(list *node) {

    if(node != NULL) {
        if(node->next == NULL) {
            free(node);
            node = NULL;
        }
        else {
            removeTail(node->next);
        }
    }

}







