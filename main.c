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

list NIL;
list* T_NIL = &NIL;


void changeText(int fromLine, int toLine, char **doc, char **text, list **head, int ind, int *s);
void deleteText(int fromLine, int toLine, char **doc, list **head, int ind, int *s);
void printText(int fromLine, int toLine, char **doc);


void insertHead(list **head, char **doc, COMMAND c, int ind, int *s);
void insertTail(list **head, char **doc, COMMAND c, int ind, int *s);
//void removeHead(list **head, list *node);
//void removeTail(list **head, list *node);
void removeHead(list **head);
void removeTail(list **head);

list* findNodeById(list **head, int id);


int main() {

    int size = N;
    int *s = &size;

    char comando[M];
    char com;
    char *p;
    char *punt[3];
    //char *text;


    T_NIL->id = 0;
    T_NIL->prec = T_NIL;
    T_NIL->next = T_NIL;

    list *head;
    head = T_NIL;

    int index = 0;


    char **document = malloc((*s)*sizeof(char*));


    do {

        strcpy(comando, "");
        char *result = fgets(comando, M, stdin);
        int firstLine, secondLine, line;

        if(sscanf(comando, "%d,%d%c\n", &firstLine, &secondLine, &com) == 3) {


            if(com == 'c') {

                index++;

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

                changeText(firstLine, secondLine, document, text, &head, index, s);

                /*for(int k = grandezza - 1; k >= 0; k--) {
                    free(text[k]);
                }*/

                for(int k = 0; k < grandezza; k++) {
                    free(text[k]);
                }
                //free(text);

            }
            else if(com == 'd') {

                index++;

                deleteText(firstLine, secondLine, document, &head, index, s);

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




void changeText(int fromLine, int toLine, char **doc, char **text, list **head, int ind, int *s) {

    for(int i = fromLine; i <= toLine; i++) {

        if(doc[i-1] != NULL) {
            free(doc[i-1]);
            doc[i-1] = malloc(strlen(text[i-fromLine])*sizeof(char));
            strcpy(doc[i-1], text[i-fromLine]);
        }
        else {

            if(i - 1 == *s + 1) {
                doc = realloc(doc, 2*(*s)*sizeof(char*));
                *s = (*s)*2;
            }

            doc[i-1] = malloc(strlen(text[i-fromLine])*sizeof(char));
            strcpy(doc[i-1], text[i-fromLine]);
        }

    }

    insertTail(head, doc, c, ind, s);

}


void deleteText(int fromLine, int toLine, char **doc, list **head, int ind, int *s) {

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



    insertTail(head, doc, c, ind, s);

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



void insertHead(list **head, char **doc, COMMAND c, int ind, int *s) {

    list *n = malloc((sizeof(list)));
    n->id = ind;
    n->com = c;
    n->document = malloc((*s)*sizeof(char*));
    for(int i = 0; doc[i] != NULL; i++) {
        n->document[i] = malloc(strlen(doc[i])*sizeof(char));
        strcpy(n->document[i], doc[i]);
    }

    if(ind == 1) {
        n->prec = T_NIL;
    }
    else {
        n->prec = findNodeById(head, ind-1);
        n->prec->next = n;
    }
    n->next = *head;
    *head = n;
    n->next->prec = n;

}

void insertTail(list **head, char **doc, COMMAND c, int ind, int *s) {

    if(*head == T_NIL) {
        insertHead(head, doc, c, ind, s);
    }
    else {
        insertTail(&(*head)->next, doc, c, ind, s);
    }

}

void removeHead(list **head) {

    if(*head != T_NIL) {
        list *old = *head;
        *head = old->next;
        free(old);
    }

}

void removeTail(list **head) {

    if(*head != T_NIL) {
        if((*head)->next == T_NIL) {
            free(*head);
            *head = T_NIL;
        }
        else {
            removeTail(&(*head)->next);
        }
    }

}

list* findNodeById(list **head, int id) {
    if(*head == T_NIL || (*head)->id == id) {
        return *head;
    }
    else {
        return findNodeById(&(*head)->next, id);
    }
}





