#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define M 1024
#define N 100


int main() {

    int i;
    char comando[M];
    char com;
    char *p;
    char *punt[3];
    //char *text;



    do {

        strcpy(comando, "");
        char *result = fgets(comando, M, stdin);
        int firstLine, secondLine, line;

        if(sscanf(comando, "%d,%d%c\n", &firstLine, &secondLine, &com) == 3) { //%*[^0123456789]%*c%*[^0123456789]%c\n"


            if(com == 'c') {


                int grandezza = (secondLine - firstLine) + 2;
                //char text[grandezza][M];
                char *text[grandezza];


                for(int k = 0; k < grandezza - 1; k++) {

                    strcpy(comando, "");
                    text[k] = malloc(sizeof(char[M]));
                    p = fgets(comando, M, stdin);
                    strcpy(text[k], comando);
                }
                text[grandezza - 1] = malloc(sizeof(char[M]));
                strcpy(text[grandezza - 1], ".");

                /*printf("%d", firstLine);
                printf("\n");
                printf("%d", secondLine);
                printf("\n");

                for(int l = 0; l < (secondLine - firstLine) + 2; l++) {
                    printf(textSupport[l]);
                    printf("\n");
                }*/

            }
            else if(com == 'd') {

            }
            else if(com == 'p') {

            }

        }
        else if(sscanf(comando, "%d%c\n", &line, &com) == 2) {

            if(com == 'u') {

            }
            else if(com == 'r') {

            }

        }


    } while(strncmp(comando, "q", 1)!=0);




    return 0;
}