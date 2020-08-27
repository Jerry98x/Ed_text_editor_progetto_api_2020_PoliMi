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


void changeText(int fromLine, int toLine, char **doc, char **text, list **head, list **h, int ind, int *s);
void deleteText(int fromLine, int toLine, char **doc, list **head, list **h, int ind, int *s);
void undo(int numCommands, char **doc, list **head, int ind);
void redo(int numCommands, char **doc, list **head, int ind);
void printText(int fromLine, int toLine, char **doc);


void insertHead(list **head, list **h, list *n, int ind);
void insertTail(list **head, list **h, list *n, int ind);
//void removeHead(list **head, list *node);
//void removeTail(list **head, list *node);
void removeHead(list **head);
void removeTail(list **head);
//void removeElement(list **head, list *node);

list* findNodeById(list *node, int id);


int main() {

    int size = N;
    int *s = &size;

    char comando[M];
    char com;
    char *p;
    //char *punt[3];
    //char *text;


    T_NIL->id = 0;
    T_NIL->prec = T_NIL;
    T_NIL->next = T_NIL;

    list *head;
    head = T_NIL;

    list *h;
    h = T_NIL;

    int index = 0;
    int possibleRedoLines = 0;


    char **document = malloc((*s)*sizeof(char*));


    do {

        strcpy(comando, "");
        char *result = fgets(comando, M, stdin);
        int firstLine, secondLine, line;

        if(sscanf(comando, "%d,%d%c\n", &firstLine, &secondLine, &com) == 3) {


            if(com == 'c') {

                possibleRedoLines = 0;
                index++;

                int grandezza = (secondLine - firstLine) + 2;
                char *text[grandezza];


                for(int k = 0; k < grandezza; k++) {

                    strcpy(comando, "");
                    text[k] = malloc(strlen(comando)*sizeof(char));
                    p = fgets(comando, M, stdin);
                    strcpy(text[k], comando);
                }


                //TODO: il punto va messo sempre con fgets?

                changeText(firstLine, secondLine, document, text, &head, &h, index, s);


                for(int k = 0; k < grandezza; k++) {
                    free(text[k]);
                }

            }
            else if(com == 'd') {

                possibleRedoLines = 0;
                index++;

                deleteText(firstLine, secondLine, document, &head, &h, index, s);

            }
            else if(com == 'p') {

                printText(firstLine, secondLine, document);

            }

        }
        else if(sscanf(comando, "%d%c\n", &line, &com) == 2) {

            if(com == 'u') {

                int indTemp = index;

                for(int i = 0; i < line; i++) {
                    if(i < indTemp) {
                        index--;
                        possibleRedoLines++;
                    }
                }

                undo(possibleRedoLines, document, &head, indTemp);

            }
            else if(com == 'r') {

                int indTemp = index;
                int redoLineTemp = possibleRedoLines;

                for(int i = 0; i < line; i++) {
                    if(possibleRedoLines > 0) {
                        index++;
                        possibleRedoLines--;
                    }

                }

                redo((redoLineTemp-possibleRedoLines), document, &head, indTemp);

            }

        }


    } while(strncmp(comando, "q", 1) != 0);

    for(int i = 0; document[i] != NULL; i++) {
        free(document[i]);
    }
    free(document);


    return 0;
}




void changeText(int fromLine, int toLine, char **doc, char **text, list **head, list **h, int ind, int *s) {

    list *indexNode = findNodeById(*h, ind);

    if(indexNode->next != T_NIL) {

        while(indexNode->next != T_NIL) {
            removeHead(&(indexNode->next));
            //indexNode = indexNode->next;
        }

        indexNode->next = T_NIL;

    }

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

    list *n = malloc((sizeof(list)));
    n->id = ind;
    n->com = c;
    n->document = malloc((*s)*sizeof(char*));
    for(int i = 0; doc[i] != NULL; i++) {
        n->document[i] = malloc(strlen(doc[i])*sizeof(char));
        strcpy(n->document[i], doc[i]);
    }

    insertTail(head, h, n, ind);

}


void deleteText(int fromLine, int toLine, char **doc, list **head, list **h, int ind, int *s) {

    list *indexNode = findNodeById(*h, ind);

    if(indexNode->next != T_NIL) {

        while(indexNode->next != T_NIL) {
            removeHead(&(indexNode->next));
            //indexNode = indexNode->next;
        }

        indexNode->next = T_NIL;

    }


    int deletedLines = 0;

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

    list *n = malloc((sizeof(list)));
    n->id = ind;
    n->com = c;
    n->document = malloc((*s)*sizeof(char*));
    for(int i = 0; doc[i] != NULL; i++) {
        n->document[i] = malloc(strlen(doc[i])*sizeof(char));
        strcpy(n->document[i], doc[i]);
    }

    insertTail(head, h, n, ind);

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

void undo(int numCommands, char **doc, list **head, int ind) {

    list *backToNode = findNodeById(*head, ind-numCommands);

    for(int i = 0; doc[i] != NULL; i++) {
        free(doc[i]);
        doc[i] = NULL;
    }
    for(int i = 0; backToNode->document[i] != NULL; i++) {
        doc[i] = malloc(strlen(backToNode->document[i])*sizeof(char));
        strcpy(doc[i], backToNode->document[i]);
    }
}

void redo(int numCommands, char **doc, list **head, int ind) {

    list *restoreToNode = findNodeById(*head, ind+numCommands);

    for(int i = 0; doc[i] != NULL; i++) {
        free(doc[i]);
        doc[i] = NULL;
    }
    for(int i = 0; restoreToNode->document[i] != NULL; i++) {
        doc[i] = malloc(strlen(restoreToNode->document[i])*sizeof(char));
        strcpy(doc[i], restoreToNode->document[i]);
    }

}



void insertHead(list **head, list **h, list *n, int ind) {

    /*list *n = malloc((sizeof(list)));
    n->id = ind;
    n->com = c;
    n->document = malloc((*s)*sizeof(char*));
    for(int i = 0; doc[i] != NULL; i++) {
        n->document[i] = malloc(strlen(doc[i])*sizeof(char));
        strcpy(n->document[i], doc[i]);
    }*/

    if(ind == 1) {
        n->prec = T_NIL;

        n->next = *head;
        *head = n;
        n->next->prec = n;
        *h = n;
    }
    else {
        n->prec = findNodeById(*h, ind-1);
        n->next = *head;
        *head = n;
        n->next->prec = n;
        n->prec->next = n;

    }

}

//provare a dare due parametri uguali che indicano la testa **head.
//uno lo tengo fisso, così da riuscire ad utilizzarlo (dereferenziato) come parametro per la findNodeById

void insertTail(list **head, list **h, list *n, int ind) {

    if(*head == T_NIL) {
        insertHead(head, h, n, ind);
    }
    else {
        insertTail(&(*head)->next, h, n, ind);
    }

}

void removeHead(list **head) {

    if(*head != T_NIL) {
        list *old = *head;
        *head = old->next;
        old->next->prec = *head;
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

/*void removeElement(list **head, list *node) {
    if(*head != T_NIL) {

    }
}*/


list* findNodeById(list *node, int id) {
    if(node == T_NIL || node->id == id) {
        return node;
    }
    else {
        return findNodeById(node->next, id);
    }
}





