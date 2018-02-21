#ifndef functions_h
#define functions_h

/*****************************************************************************/
/* This structure stores one node in the linked list.                        */
/*****************************************************************************/
struct Node {
    unsigned short int ID;
    char action[31];
    unsigned short int level;
    unsigned short int parentID;
    unsigned short int childNumber;
    char prefix[11];
    struct Node* child;
    struct Node* sibling;
};
struct Node* head; // Global variable pointing to the head

/*****************************************************************************/
/* This structure stores node information given by the file                  */
/*****************************************************************************/
struct node {
    unsigned short int ID;
    char action[31];
};

/*****************************************************************************/
/* This structure stores link information given by the file                  */
/*****************************************************************************/
struct link {
    unsigned short int ID;
    unsigned short int parentID;
};

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
char* findAction(unsigned short int ID, unsigned short int length, struct node *nodes);
char* itoa(int val, int base);
void printTree(struct Node* head);
struct Node* GetNewNode(unsigned short int ID, unsigned short int parentID, unsigned short length, struct node *nodes);
struct Node* checkTree(struct Node* current, unsigned short int targetID);
void makeTree(char *arg);

#endif
