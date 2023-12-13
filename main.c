#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct names {
    char *n;
    struct names *next, *before;
} names;

names *addlist(names *list, char *name) {

    names *move = list;
    names *new = malloc(sizeof(names));
    new->n = malloc((strlen(name) + 1) * sizeof(char));
    strcpy(new->n, name);

    if(list == NULL) {

        list = new;
        new->next = NULL;
        new->before = NULL;
        return list;

    }

    while(move != NULL) {

        if(strcmp(move->n, new->n) >= 0 && move->next == NULL) {

            move->next = new;
            new->before = move;
            new->next = NULL;

            return list;

        } else if(strcmp(move->n, new->n) >= 0 && strcmp(move->next->n, new->n) < 0) {

            new->next = move->next;
            new->before = move;
            move->next->before = new;
            move->next = new;

            return list;

        } else if(strcmp(move->n, new->n) < 0) {

            new->next = move;
            new->before = NULL;
            list = new;

            return list;

        }

        move = move->next;

    }

    return NULL;

}

char *createFileName(char *str) {

    char c = '\0';
    str = malloc(sizeof(char));
    int i = 1;

    while(c != '\n') {

        c = getc(stdin);
        str = realloc(str, i * sizeof(char));
        str[i-1] = c;

        if(str[i-1] == '\n' && strstr(str, ".txt") == NULL) {

            str[i-1] = '\0';
            str = realloc(str, (i + 4) * sizeof(char));
            strcat(str, ".txt");

        } else if(str[i-1] == '\n' && strstr(str, ".txt") != NULL)
            str[i-1] = '\0';

        i++;

    }

    return str;

}

char *createName(char *str, FILE *ptr) {

    char c = '\0';
    str = malloc(sizeof(char));
    int i = 1;

    while(c != '\n') {

        c = fgetc(ptr);

        str = realloc(str, i * sizeof(char));
        str[i-1] = c;

        if(feof(ptr) > 0) {

            str[i-1] = '\0';

            if(strlen(str) <= 1) {

                free(str);
                return NULL;

            }

            return str;

        }

        if(str[i-1] == '\n')
            str[i-1] = '\0';

        i++;

    }

    return str;

}

names *createList(char *file, names *list) {

    char *name = NULL;

    FILE *ptr = fopen(file,"r");
    if(ptr == NULL) {
        printf("Cannot open file!");
        exit(0);
    }

    do {

        name = createName(name, ptr);
        if(name == NULL) {
            fclose(ptr);
            return list;
        }
        list = addlist(list, name);
        free(name);

    } while(name != NULL);

    fclose(ptr);

    return list;

}

void printList(names *list) {

    names *move = list;

    if(move == NULL)
        return;

    printList(move->next);
    printf("%s\n", move->n);

}

void freeList(names *list) {

    names *move = list;
    names *temp = NULL;

    while(move != NULL) {

        temp = move->next;
        free(move->n);
        free(move);
        move = temp;

    }

}

int main() {

    char *filename = NULL;
    names *list = NULL;

    printf("Please insert the file name: ");
    filename = createFileName(filename);

    FILE *ptr = fopen(filename,"r");
    if(ptr == NULL) {
        free(filename);
        printf("Cannot open file!");
        exit(0);
    }
    fclose(ptr);

    list = createList(filename, list);

    printList(list);

    freeList(list);
    free(filename);

    return 0;

}
