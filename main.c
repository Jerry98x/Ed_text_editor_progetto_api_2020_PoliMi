#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define M 1024
#define N 1000


typedef enum{c, d}COMMAND;

struct commandsList {
    int id;
    COMMAND com;
    int fLine;
    int sLine;
    char **document;
    struct commandsList *prec;
    struct commandsList *next;
};

typedef struct commandsList list;

list NIL;
list* T_NIL = &NIL;


void changeText(int fromLine, int toLine, char **doc, char *text, list **head, int *lc, int ind, int *s);
void deleteText(int fromLine, int toLine, char **doc, list **head, int *lc, int ind, int *s);
void undo(int numCommands, char **doc, list **head, int ind, int *lc);
void redo(int numCommands, char **doc, list **head, int ind, int *lc);
void printText(int fromLine, int toLine, char **doc);


void insertHead(list **head, list **h, list *n, int ind);
void insHead(list **head, list *n);
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
    int lineCounter = 0;
    int *lc = &lineCounter;
    int possibleRedoLines = 0;

    char **document = malloc((*s)*sizeof(char*));


    do {

        strcpy(comando, "");
        char *result = fgets(comando, M, stdin);
        int firstLine, secondLine, line;

        if(sscanf(comando, "%d,%d%c\n", &firstLine, &secondLine, &com) == 3) {


            if(com == 'c') {

                if(firstLine == 0 || secondLine == 0) {
                    continue;
                }

                possibleRedoLines = 0;
                index++;

                while(secondLine > *s) {
                    document = realloc(document, 2*(*s)*sizeof(char*));
                    *s = (*s)*2;
                }

                int grandezza = (secondLine - firstLine) + 2;
                //char *text[grandezza];

                char text[M];


                /*for(int k = 0; k < grandezza; k++) {

                    strcpy(comando, "");
                    text[k] = malloc(M*sizeof(char));
                    p = fgets(comando, M, stdin);
                    strcpy(text[k], comando);
                }*/

                changeText(firstLine, secondLine, document, text, &head, lc, index, s);


                /*for(int k = 0; k < grandezza; k++) {
                    free(text[k]);
                    //text[k] = NULL;
                }*/

            }
            else if(com == 'd') {

                /*if(firstLine == 0 || secondLine == 0) {
                    continue;
                }*/

                possibleRedoLines = 0;
                index++;

                deleteText(firstLine, secondLine, document, &head, lc, index, s);

            }
            else if(com == 'p') {

                printText(firstLine, secondLine, document);

            }

        }
        else if(sscanf(comando, "%d%c\n", &line, &com) == 2) {

            if(com == 'u') {

                if(line == 0) {
                    continue;
                }

                int indTemp = index;
                int thisUndo = 0;

                for(int i = 0; i < line; i++) {
                    if(i < indTemp) {
                        if(index >= 0) {
                            index--;
                            possibleRedoLines++;
                            thisUndo++;
                        }
                        else {
                            continue;
                        }
                    }
                }

                undo(thisUndo, document, &head, indTemp, lc);

            }
            else if(com == 'r') {

                if(line == 0) {
                    continue;
                }

                int indTemp = index;
                int redoLineTemp = possibleRedoLines;

                for(int i = 0; i < line; i++) {
                    if(possibleRedoLines > 0) {
                        index++;
                        possibleRedoLines--;
                    }

                }

                redo((redoLineTemp-possibleRedoLines), document, &head, indTemp, lc);

            }

        }


    } while(strncmp(comando, "q", 1) != 0);


    return 0;
}




void changeText(int fromLine, int toLine, char **doc, char *text, list **head, int *lc, int ind, int *s) {

    /*list *indexNode = findNodeById(*head, ind-1);

    if(indexNode->prec != T_NIL) {

        for(int i = (*head)->id; i > ind; i--) {
            removeHead(head);
        }

        while(indexNode->prec != T_NIL) {
            removeHead(&(indexNode->prec));
        }

        indexNode->prec = T_NIL;

    }*/

    for(int i = (*head)->id; i > ind-1; i--) {
        removeHead(head);
    }


    /*if(fromLine == 0) {
        if(toLine == 0) {
            return;
        }
        else {
            fromLine++;
        }
    }*/



    list *n = malloc((sizeof(list)));
    n->id = ind;
    n->com = c;
    n->fLine = fromLine;
    n->sLine = toLine;
    n->document = malloc((*lc)*sizeof(char*));
    /*for(int i = fromLine; i <= toLine; i++) {
        if(doc[i-1] != NULL) {
            n->document[i-fromLine] = doc[i-1];
        }

    }*/

    for(int i = fromLine; i <= toLine; i++) {
        if(doc[i-1] != NULL) {
            n->document[i-fromLine] = doc[i-1];
        }

        strcpy(text, "");
        if(fgets(text, M, stdin) == NULL) {
          return;
        }
        char *string = malloc((strlen(text)+1)*sizeof(char));
        strcpy(string, text);

        if(i != toLine + 1) {
            doc[i-1] = string;
        }
        else {
            continue;
        }

    }
    if(fgets(text, M, stdin) == NULL) {
        return;
    }

    if(toLine > *lc) {
        *lc = toLine;
    }



    /*list *n = malloc((sizeof(list)));
    n->id = ind;
    n->com = c;
    n->fLine = fromLine;
    n->sLine = toLine;
    n->document = malloc((toLine-fromLine+1)*sizeof(char*));
    for(int i = fromLine; i <= toLine; i++) {
        n->document[i-fromLine] = doc[i-1];
    }*/

    //insertTail(head, h, n, ind);
    insHead(head,n);


}


void deleteText(int fromLine, int toLine, char **doc, list **head, int *lc, int ind, int *s) {

    /*list *indexNode = findNodeById(*h, ind);

    if(indexNode->next != T_NIL) {

        while(indexNode->next != T_NIL) {
            removeHead(&(indexNode->next));
        }

        indexNode->next = T_NIL;

    }*/

    for(int i = (*head)->id; i > ind-1; i--) {
        removeHead(head);
    }


    if(fromLine == 0) {
        if(toLine == 0) {
            list *n = malloc((sizeof(list)));
            n->id = ind;
            n->com = d;
            n->fLine = -1;
            n->sLine = -1;
            n->document = NULL;
            return;
        }
        else {
            fromLine++;
        }
    }
    if(toLine > *lc) {
        toLine = *lc;
    }
    if(doc[fromLine-1] == NULL) {
        list *n = malloc((sizeof(list)));
        n->id = ind;
        n->com = d;
        n->fLine = -1;
        n->sLine = -1;
        n->document = NULL;
        //TODO: ho messo doc[fromLine-1] al posto di doc[fromLine] in questo if
        //TODO: aggiungere questo nodo (e quello sopra nel controllo sugli zeri)
        //n->document = malloc((*lc)*sizeof(char*));
        insHead(head, n);
        return;
    }


    int deletedLines = 0;

    list *n = malloc((sizeof(list)));
    n->id = ind;
    n->com = d;
    n->fLine = fromLine;
    n->sLine = toLine;
    n->document = malloc((*lc)*sizeof(char*));
    for(int i = fromLine; i <= toLine; i++) {
        if(doc[i-1] != NULL) {
            n->document[i-fromLine] = doc[i-1];
        }
    }
    //TODO: mergiare i cicli

    if(doc[toLine] == NULL) {
        for(int i = fromLine; i <= toLine; i++) {

            if(doc[i-1] == NULL) {
                continue;
            }
            else {
                deletedLines++;
                doc[i-1] = NULL;
            }

        }
    }
    else{
        for(int i = fromLine; i <= toLine; i++) {
            deletedLines++;
            doc[i-1] = NULL;
        }

        for(int i = toLine; doc[i] != NULL; i++) {

            if(doc[i - deletedLines] == NULL) {
                doc[i - deletedLines] = doc[i];
                doc[i] = NULL;
            }
            else {
                doc[i - 1 - deletedLines] = doc[i-1];
                doc[i - 1] = NULL;
            }

        }
    }

    *lc = *lc - deletedLines;


    //insertTail(head, h, n, ind);
    insHead(head, n);

}

void printText(int fromLine, int toLine, char **doc) {

    if(fromLine == 0) {
        if(toLine == 0) {
            fputs(".\n", stdout);
        }
        else {
            fputs(".\n", stdout);
            fromLine++;

        }
    }

    if(fromLine != 0) {
        for (int i = fromLine; i <= toLine; i++) {
            if(doc[i - 1] == NULL) {
                fputs(".\n", stdout);
            }
            else {
                fputs(doc[i - 1], stdout);
            }
        }
    }

}


void undo(int numCommands, char **doc, list **head, int ind, int *lc) {

    list *actualNode = findNodeById(*head, ind);
    //TODO: indirizzo maggiore di 1 del massimo ind della testa
    //list *actualNode = *head;

    while((actualNode != T_NIL) && (actualNode->id != ind - numCommands)) {

        if(actualNode->com == c && actualNode->document[0] == NULL) {
            for(int i = actualNode->fLine; i <= actualNode->sLine; i++) {
                actualNode->document[i-(actualNode->fLine)] = doc[i-1];
                doc[i-1] = NULL;
            }
            *lc = *lc - (actualNode->sLine - actualNode->fLine + 1);
        }
        else if(actualNode->com == c && actualNode->document[0] != NULL) {
            int overwrittenLines = 0;
            for(int i = actualNode->fLine; i <= actualNode->sLine; i++) {

                if(actualNode->document[i-(actualNode->fLine)] != NULL) {
                    char *buffer;
                    buffer = doc[i-1];
                    doc[i-1] = actualNode->document[i-(actualNode->fLine)];
                    actualNode->document[i-(actualNode->fLine)] = buffer;
                    overwrittenLines++;
                }
                else {
                    actualNode->document[i-(actualNode->fLine)] = doc[i-1];
                    doc[i-1] = NULL;
                }

            }
            *lc = *lc - ((actualNode->sLine - actualNode->fLine + 1) - overwrittenLines);
        }
        else if(actualNode->com == d) {
            //if(actualNode->document[0] != NULL) {
            if(actualNode->fLine == -1) {
                actualNode = actualNode->next;
                continue;
            }
            else {
                int dLines = actualNode->sLine - actualNode->fLine + 1;
                if(doc[(actualNode->fLine)-1] == NULL) { //cancella dal fondo, non serve shiftare
                    for(int i = actualNode->fLine; actualNode->document[i-(actualNode->fLine)] != NULL; i++) {
                        doc[i-1] = actualNode->document[i-(actualNode->fLine)];
                    }
                }
                else {
                    for(int i = *lc - 1; i >= actualNode->sLine; i--) {
                        doc[i+dLines] = doc[i];
                    }
                    for(int i = actualNode->fLine; actualNode->document[i-(actualNode->fLine)] != NULL; i++) {
                        doc[i-1+dLines] = doc[i-1];
                        doc[i-1] = actualNode->document[i-(actualNode->fLine)];
                    }
                }
                *lc = *lc + dLines;
            }


            //}
        }

        actualNode = actualNode->next;

    }

}

void undoOld(int numCommands, char **doc, list **head, int ind) {

    list *actualNode = findNodeById(*head, ind);

    while((actualNode != T_NIL) && (actualNode->id != ind - numCommands)) {

        if(actualNode->com == c) {
            for(int i = actualNode->fLine; i <= actualNode->sLine; i++) {
                if(actualNode->prec != T_NIL) {
                    //int precLines = actualNode->prec->sLine - actualNode->prec->fLine + 1;
                    if(actualNode->prec->sLine < i) {
                        doc[i-1] = NULL;
                    }
                    else {
                        doc[i-1] = actualNode->prec->document[i-(actualNode->fLine)];
                    }

                }
                else {
                    doc[i-1] = NULL;
                }
            }
            for(int i = actualNode->sLine + 1; doc[i] != NULL; i++) {
                doc[i] = NULL;
            }
        }
        else if(actualNode->com == d) {
            if(actualNode->document[0] != NULL) {
                int dLines = actualNode->sLine - actualNode->fLine + 1;
                for(int i = actualNode->sLine; actualNode->document[(actualNode->sLine)-i] != NULL; i++) {
                    doc[i-2+dLines] = doc[i-2];
                    doc[i-2] = actualNode->document[i];
                }
            }
        }

        actualNode = actualNode->prec;

    }





    /*list *backToNode = findNodeById(*head, ind-numCommands);

    for(int i = 0; doc[i] != NULL; i++) {
        doc[i] = NULL;
    }
    if(backToNode != T_NIL) {
        for(int i = 0; backToNode->document[i] != NULL; i++) {
            doc[i] = backToNode->document[i];
        }
    }*/

}

void redo(int numCommands, char **doc, list **head, int ind, int *lc) {

    list *actualNode = findNodeById(*head, ind + 1);

    while((actualNode != T_NIL) && ((actualNode->id) - 1 != ind + numCommands)) {

        if(actualNode->com == c) {
            for(int i = actualNode->fLine; i <= actualNode->sLine; i++) {

                if(doc[i-1] == NULL) {
                    doc[i-1] = actualNode->document[i-(actualNode->fLine)];
                    actualNode->document[i-(actualNode->fLine)] = NULL;
                }
                else {
                    char *buffer;
                    buffer = doc[i-1];
                    doc[i-1] = actualNode->document[i-(actualNode->fLine)];
                    actualNode->document[i-(actualNode->fLine)] = buffer;

                }

            }

            if(actualNode->sLine > *lc) {
                *lc = actualNode->sLine;
            }
        }
        else if(actualNode->com == d) {

            if(actualNode->fLine == -1) {
                actualNode = actualNode->prec;
                continue;
            }
            else {
                for(int i = actualNode->fLine; i <= actualNode->sLine; i++) {
                    actualNode->document[i-(actualNode->fLine)] = doc[i-1];
                    doc[i-1] = NULL;
                    //TODO: nel caso sistemare la roba copiata in più
                }
                for(int i = actualNode->sLine; doc[i] != NULL; i++) {
                    int dLines = actualNode->sLine - actualNode->fLine + 1;
                    if(doc[i-dLines] == NULL) {
                        doc[i-dLines] = doc[i];
                        //doc[i-dLines] = actualNode->document[i-(actualNode->sLine)];
                        doc[i] = NULL;
                        //TODO: nel caso mergiare con for precedente
                    }
                    else {
                        doc[i-1-dLines] = doc[i-1];
                        doc[i-1] = NULL;
                    }

                    *lc = *lc - dLines;
                }

            }

        }

        actualNode = actualNode->prec;

    }


    /*list *restoreToNode = findNodeById(*head, ind+numCommands);

    for(int i = 0; doc[i] != NULL; i++) {
        doc[i] = NULL;
    }
    for(int i = 0; restoreToNode->document[i] != NULL; i++) {
        doc[i] = restoreToNode->document[i];
    }*/

}


void insHead(list **head, list *n) {
    n->prec = T_NIL;
    n->next = *head;
    n->next->prec = n;
    *head = n;
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
        (*head)->prec = T_NIL;
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

    list *nodeTemp = node;

    while(nodeTemp != T_NIL) {
        if(nodeTemp->id == id) {
            return nodeTemp;
        }
        else {
            nodeTemp = nodeTemp->next;
        }
    }
}


list* findNodeByIdRic(list *node, int id) {
    if(node == T_NIL || node->id == id) {
        return node;
    }
    else {
        return findNodeById(node->next, id);
    }
}



