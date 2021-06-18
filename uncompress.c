#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//run struct for compression algorithm
typedef struct{
    char c;
    int count;
} run;

//LIST FUNCTIONS BELLOW THIS POINT
//node
typedef struct nodeS *node;
struct nodeS{
    char c;
    struct nodeS *next;
};

//list
typedef struct listS *list;
struct listS{
    struct nodeS *head;
    struct nodeS *tail;
    int size;
};

//create list
list createList() {
    list l = (list)malloc(sizeof(struct listS));
    
    if (l == NULL) {
        printf("Memory allocation error\n");
        exit(1);
    }
    l->head = NULL;    
    l->size = 0;

    return l;  
}

//add node at the end of the list
void addNodeAtEnd(list l, char ch) {
    node newNode = (node)malloc(sizeof(struct nodeS));
    if (newNode == NULL) {
        printf("Memory allocation error\n");
        exit(1);
    }

    newNode->c = ch;
    newNode->next = NULL;
    
    if (l->size == 0) { //in case the list is empty make the list head also point to newNode
        l->head = newNode;
        l->tail = newNode;
        l->size++;
    } else {
        node oldLast = l->tail; //make a pointer to current/old last node
        oldLast->next = newNode; //make current/old last node point to new last node
        l->tail = newNode; //make list tail also point at new last node   
        l->size++; //increase list size by 1
    }   
}

//done
//insert node at a given position. first position is number 1 NOT 0
void insertNode(list l, char ch, int pos) {
    node newNode = (node)malloc(sizeof(struct nodeS));
    if (newNode == NULL) {
        printf("Memory allocation error\n");
        exit(1);
    }

    node position = l->head;
    newNode->c = ch;

    if (pos < 1 || pos > l->size+1) {
        printf("\nError. Operation falls outside compressed file limits.\n");
        exit(1);
    } else if (pos == 1) {     
        l->head = newNode;
        newNode->next = position;
    } else {
        for (int i = 1; i < pos-1; i++) {//position stops at the node before
            position = position->next;
        }        
        newNode->next = position->next;
        position->next = newNode;
    }
    l->size++;    
}

//done
//replace node at given position. first position is number 1 NOT 0
void replaceNode(list l, char ch, int pos) {
    node position = l->head;

    if (pos < 1 || pos > l->size) {
        printf("\nError. Operation falls outside compressed file limits.\n");
        exit(1);
    } else if (pos == 1) {
        l->head->c = ch;
    } else {
        for (int i = 1; i < pos; i++) {
            position = position->next;
        }
        position->c = ch;
    }    
}

//done
//delete given number of nodes from given position. first position is 1 NOT 0
void deleteNode(list l, int pos) {
    node position = l->head;
    node temp;

    if (pos < 1 || pos > l->size) {
        printf("\nError. Operation falls outside compressed file limits.\n");
        exit(1);
    } else if (pos == 1) {
        l->head = position->next;
        free(position);
        l->size--;
    } else {
        for (int i = 1; i < pos-1; i++) {            
            position = position->next;            
        }
        temp = position->next;
        position->next = position->next->next; //LOL  
        free(temp);
        l->size--;       
    }  
}

//print to txt file
void printToFile(list l, FILE *fpu) {
    node current = l->head;

    for(int i = 0; i < l->size; i++) {
        fputc(current->c, fpu);       
        current = current->next;
    }    
}

//print file to screen
void printToScreen(list l, FILE *fpu) {
    node current = l->head;

    for(int i = 0; i < l->size; i++) {
        printf("%c", current->c);       
        current = current->next;
    }    
}

//add characters at the end
void addCharacters(list l) {
    int number;
    char c, b;
    
    printf("Type character to add: ");
    scanf(" %c", &c);   

    printf("Type how many characters to add: ");
    scanf("%d", &number);

    for (int i = 0; i < number; i++) {
        addNodeAtEnd(l, c);
    }
}

//insert characters from a specific location
void insertCharacters(list l) {
    int number, position;
    char c;

    printf("Type character to insert: ");
    scanf(" %c", &c);

    printf("Type how many characters to insert: ");
    scanf("%d", &number);

    printf("Type the position to insert from: ");
    scanf("%d", &position);

    for (int i = 0; i < number; i++) {
        insertNode(l, c, position+i);
    }
}

//replace characters from aspecific location
void replaceCharacters(list l) {
    int number, position;
    char c;

    printf("Type character to replace with: ");
    scanf(" %c", &c);

    printf("Type how many characters to replace: ");
    scanf("%d", &number);

    printf("Type the position to replace from: ");
    scanf("%d", &position);

    for (int i = 0; i < number; i++) {
        replaceNode(l, c, position+i);
    }
}

//delete characters from a specific location
void deleteCharacters(list l) {
    int number, position;
    
    printf("Type how many characters to delete: ");
    scanf("%d", &number);

    printf("Type the position to delete from: ");
    scanf("%d", &position);

    for (int i = 0; i < number; i++) {
        deleteNode(l, position);
    }
}

//print file modification menu options
void menuOptions(list l, FILE *fpu, char *name) {
    int option;

    do {
        printf("\n");
        printf("\"%s\" contents: \n", name);
        printf("---------------------------------------\n");
        printToScreen(l, fpu);
        printf("\n---------------------------------------\n");

        printf("Number of characters: %d\n\n", l->size);
        printf("Choose option:\n");
        printf("1. Add characters at the end.\n");
        printf("2. Insert characters at specific point.\n");
        printf("3. Replace characters at specific point.\n");
        printf("4. Delete characters at specific point.\n");
        printf("5. Finish and save changes.\n");       

        scanf("%d", &option);

        if (option < 1 || option > 5) {
            printf("Wrong input\n");
            exit(1);
        }

        switch(option) {
            case 1:
                addCharacters(l);
                break;
            case 2:
                insertCharacters(l);
                break;
            case 3:
                replaceCharacters(l);
                break;
            case 4:
                deleteCharacters(l);
                break;
            case 5:
                printToFile(l, fpu);
                break;
        }
    } while (option != 5);
}

//open compressed source and uncompressed destination files
void fileOpen(FILE **fpc, FILE **fpu, char *source, char *dest) {
    *fpc = fopen(source, "r");
    if (*fpc == NULL) {
        printf("Error opening file 1 \"%s\"\n", source);
        exit(1);
    }

    *fpu = fopen(dest, "w");
    if (*fpu == NULL) {
        printf("Error opening file 2 \"%s\"\n", dest);
        exit(1);
    }
}

//decompress file and populate list with chars    
int decompress(list charList, FILE *fpc) {       
    run *chunk = (run*)malloc(sizeof(run));

    int run_count = 0;
    while (fread(chunk, sizeof(run), 1, fpc) != 0) {       
        run_count++;
        for (int i = 0; i < chunk->count; i++) {         
            addNodeAtEnd(charList, chunk->c);
        }
    }
    return run_count;
}


int main(int argc, char **argv) {
    list charList = createList();  
    FILE *fpc, *fpu;
    char *source, *dest;

    int run_count = 0;

    if (!strcmp(argv[1], "-e")) {
        source = argv[2];
        dest = argv[3];
        fileOpen(&fpc, &fpu, source, dest);
        run_count = decompress(charList, fpc);
        menuOptions(charList, fpu, source);//open -e menu           
    } else if (argc == 3) {
        source = argv[1];
        dest = argv[2];
        fileOpen(&fpc, &fpu, source, dest);
        run_count = decompress(charList, fpc);
        printToFile(charList, fpu);
    } else {
        printf("Error, wrong arguments\n");
        exit(1);
    }

    printf("\nOperation complete.\n\n");
    printf("File \"%s\" size compressed is %ld bytes\n", source, sizeof(run) * run_count);
    printf("File \"%s\" size uncompressed is %ld bytes\n", dest, sizeof(char) * charList->size);
    
    fclose(fpu);
    fclose(fpc);
    
    return 0;
}