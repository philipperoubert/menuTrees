#include "functions.h"

/*****************************************************************************/
/* Function: findAction                                                      */
/*    Reads the contents of the file (pairs of monarch name and start year)  */
/*    and creates a linked list containing the monarchs in year order.       */
/* Parameters:                                                               */
/*    ID         Represents the the target ID to be matched                  */
/*    length     Represents the length of the array in which the nodes       */
/*               information are stored                                      */
/*    nodes      Is the array containing all the nodes                       */
/* Returns:                                                                  */
/*    A string containing the action string                                  */
/*****************************************************************************/
char* findAction(unsigned short int ID, unsigned short int length, struct node *nodes){
    for (int i=0; i<length; i++){
        if(nodes[i].ID == ID) return nodes[i].action;
    }
    return "";
}


/*****************************************************************************/
/* Function: itoa (Reference: http://www.strudel.org.uk/itoa/)               */
/*    Converts an int value to string                                        */
/* Parameters:                                                               */
/*    val        Is the value to be converted                                */
/*    base       The radix system to be used to convert                      */
/* Returns:                                                                  */
/*    A string containing the int value                                      */
/*****************************************************************************/
char* itoa(int val, int base){
    static char buf[32] = {0};
    int i = 30;
    for(; val && i ; --i, val /= base)
        buf[i] = "0123456789abcdef"[val % base];
    return &buf[i+1];
}


/*****************************************************************************/
/* Function: printTree                                                       */
/*    Prints a given tree node structure                                     */
/* Parameters:                                                               */
/*    head       The very first node of the tree                             */
/* Returns:                                                                  */
/*    void                                                                   */
/*****************************************************************************/
void printTree(struct Node* head){
    char spacing[21];
    strcpy(spacing, "%");
    strcat(spacing, itoa(head->level*3, 10));
    strcat(spacing, "s%s %s\n");

    fprintf(stdout, spacing, "", head->prefix, head->action, head->level); fflush(stdout);
    if (head->child != NULL) printTree(head->child);
    if (head->sibling != NULL) printTree(head->sibling);
}


/*****************************************************************************/
/* Function: GetNewNode                                                      */
/*    Creates a new node                                                     */
/* Parameters:                                                               */
/*    ID         Is the ID of the new node                                   */
/*    parentID   Is the ID of the parent node                                */
/*    length     Is the length of the array containing all the nodes         */
/*    nodes      Is the array containing all the nodes                       */
/* Returns:                                                                  */
/*    A string containing the int value                                      */
/*****************************************************************************/
struct Node* GetNewNode(unsigned short int ID, unsigned short int parentID, unsigned short length, struct node *nodes) {
    struct Node* newNode;
    if(!( newNode = (struct Node*)malloc(sizeof(struct Node)) ))
       {
           fprintf(stderr, "Error: %s\n", "Memory allocation failed.");
           exit(1);
       }
    newNode->ID = ID;
    newNode->parentID = parentID;
    strcpy(newNode->action, findAction(ID, length, nodes));
    newNode->childNumber = 0;
    newNode->child = NULL;
    newNode->sibling = NULL;
    return newNode;

}


/*****************************************************************************/
/* Function: checkTree                                                       */
/*    Finds if a node matches the requirements to be a sibling               */
/* Parameters:                                                               */
/*    current    Represents the current node being checked                   */
/*    targetID   Is the parent ID that needs to be matched with              */
/* Returns:                                                                  */
/*    A node pointer if a node has been found                                */
/*****************************************************************************/
struct Node* checkTree(struct Node* current, unsigned short int targetID){
    if (current->parentID == targetID && current->sibling == NULL){
        return current;
    }
    else{
        if (current->sibling != NULL){
            if(checkTree(current->sibling, targetID)!= NULL) return (checkTree(current->sibling, targetID));
        }
        if (current->child != NULL) {
            if(checkTree(current->child, targetID) != NULL) return (checkTree(current->child, targetID));
        }
        return NULL;
    }
    return current;
}


void freeNodes(struct Node **nodeArray, int nodesAmount){
  for(int i =0; i<nodesAmount;i++){
    //printf("%d\n",nodeArray[i]->ID );
    //if (nodeArray[i]->child != NULL) printf("Child information: ->%s\n", nodeArray[i]->child->ID);
    if (!(!nodeArray[i])) free(nodeArray[i]);
    //printf("%i\n", nodeArray[i]->ID);
    //free(nodeArray[i]);
  }
}

/*****************************************************************************/
/* Function: makeTree                                                        */
/*    Builds the tree from a given file                                      */
/* Parameters:                                                               */
/*    arg        Is the filename                                             */
/* Returns:                                                                  */
/*    void                                                                   */
/*****************************************************************************/
void makeTree(char *arg){

    FILE *fp ;
    char fileContent[51];
    char ch;
    int number;
    char actionName[21];
    int child;
    int parent;

    unsigned short int nodesAmount=0;
    unsigned short int linksAmount=0;
    fp = fopen(arg, "r");

    if( fp == NULL )
    {
        fprintf( stderr, "Error: %s\n", "Cannot open file for read access");
        fclose(fp);
        exit(1);
    }

    while(!feof(fp)){
        fgets(fileContent, 51, fp);
        if (fileContent[0] == 'A'){
            nodesAmount++;
        }
        else{
            linksAmount++;
        }
    }


    struct node nodes[nodesAmount];
    struct link links[linksAmount];

    fclose( fp );
    fp = fopen(arg, "r");
    fprintf(stderr, "\nData values read from file:\n");
    unsigned short int i = 0;
    unsigned short int j = 0;
    while (!feof(fp)) {
        fgets(fileContent, 51, fp);
        if (fileContent[0] == 'A'){
            sscanf(fileContent, " %1c%4d%21[^\n]", &ch, &number, &actionName);

            fprintf(stderr, " %4d: %s\n", number, actionName);

            nodes[i].ID = number;
            strcpy(nodes[i].action, actionName);
            i++;
        }
        else if (fileContent[0] == 'B') {
            sscanf(fileContent, " %1c%4d%4d", &ch, &child, &parent);
            fprintf(stderr, "%8s %d, parent %d\n", "child" , child, parent);
            links[j].ID = child;
            links[j].parentID = parent;
            j++;
        }
        else {
            fprintf(stderr, "Error: %s\n", "wrong file format");
            fclose( fp );
            exit(1);
        }

    }

    fclose( fp );

    struct Node *temp = head;
    struct Node *nodesArray[nodesAmount];

    for (int i = 0; i<nodesAmount; i++){
        struct Node* newNode = GetNewNode(links[i].ID, links[i].parentID, nodesAmount, nodes);
        nodesArray[i] = newNode;

        if(temp == NULL){
            newNode->level = 0;
            strcpy(newNode->prefix, "1");
            head = newNode;
            temp = head;
        }
        else
        {
            if (newNode->parentID == temp->ID){
                newNode->level = temp->level + 1;
                newNode->childNumber = 1;
                strcpy(newNode->prefix, temp->prefix);
                strcat(newNode->prefix, ".1");
                temp->child = newNode;
            }
            else if(newNode->parentID == temp->parentID && temp->sibling == NULL){
                newNode->childNumber = temp->childNumber + 1;

                strcpy(newNode->prefix, temp->prefix);
                unsigned short int length = strlen(newNode->prefix)-1;
                while (newNode->prefix[length-1] != '.'){
                    length--;
                }
                newNode->prefix[length] = *itoa(newNode->childNumber,10);

                newNode->level = temp->level;
                temp->sibling = newNode;
            }
            else{

                temp = checkTree(head, newNode->parentID);
                newNode->childNumber = temp->childNumber + 1;

                if (temp->level != 0){
                    strcpy(newNode->prefix, temp->prefix);
                    unsigned short int length = strlen(newNode->prefix)-1;
                    while (newNode->prefix[length-1] != '.'){
                        length--;
                    }
                    newNode->prefix[length] = *itoa(newNode->childNumber,10);
                }
                else strcpy(newNode->prefix, itoa(newNode->childNumber+1, 10));

                newNode->level = temp->level;
                temp->sibling = newNode;
            }
            temp = newNode;
        }
    }
    fprintf(stdout, "\n"); fflush(stdout);
    printTree(head);
    freeNodes(nodesArray, nodesAmount);
    head = NULL;

    //free(findAction);
}
