/**
 * @file abstractSyntaxTree.c
 * @author Mugisho Mpozi s2440547
 * @brief 
 * @version 0.1
 * @date 2023-10-21
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abstractSyntaxTree.h"
#include "errorHandling.h"
#include <tokenString.h>





#include "abstractSyntaxTree.h"
#include <stdlib.h>

// Function prototypes for the methods
static Node *getRootAST(AbstractSyntaxTree *);
static void addAST(Node *, AbstractSyntaxTree *);
static void removeAST(Node *, AbstractSyntaxTree *);
static void cleanUpTreeAST(AbstractSyntaxTree *);
static void reduceAST(AbstractSyntaxTree *);
static bool possibleBetaReductionAST(Node *, AbstractSyntaxTree *);
static void betaReductionAST(Node *, bool *, AbstractSyntaxTree *);
static bool capturedVariableAST(Node *, const char *, AbstractSyntaxTree *);
static bool hasVariableAST(Node *, AbstractSyntaxTree *);
static void alfaConversionAST(Node *, const char *, const char *, AbstractSyntaxTree *);
static void replaceVarAST(Node *, const char *, Node *, AbstractSyntaxTree *);
static Node *copyTreeAST(Node *, Node *, AbstractSyntaxTree *);
static void printAST(Node *, const AbstractSyntaxTree *);
static void typeCheckingAST(Node *, const AbstractSyntaxTree *);
static void typeCheckingASTHelper(Node *, const AbstractSyntaxTree *, char *[], int);

AbstractSyntaxTree *createAbstractSyntaxTree(ParseTree *parseTree) {
    AbstractSyntaxTree *obj = malloc(sizeof(AbstractSyntaxTree));
    if (obj == NULL) {
        longjmp(memoryAllocationException, 1);
        return NULL;
    }

    // Initialize fields
    obj->size = parseTree->getSize(parseTree);
    obj->root = parseTree->getRoot(parseTree);
    obj->nodes = parseTree->getNodes(parseTree);

    // Initialize function pointers
    obj->getRoot = getRootAST;
    obj->add = addAST;
    obj->remove = removeAST;
    obj->cleanUpTree = cleanUpTreeAST;
    obj->reduce = reduceAST;
    obj->typeChecking = typeCheckingAST;
    obj->possibleBetaReduction = possibleBetaReductionAST;
    obj->betaReduction = betaReductionAST;
    obj->capturedVariable = capturedVariableAST;
    obj->hasVariable = hasVariableAST;
    obj->alfaConversion = alfaConversionAST;
    obj->replaceVar = replaceVarAST;
    obj->copyTree = copyTreeAST;
    obj->print = printAST;

    return obj;
}

void destroyAbstractSyntaxTree(AbstractSyntaxTree *obj) {
    if (obj == NULL) {
        return;  // Nothing to free
    }
    
    // Free resources associated with the nodes
    // You need to implement logic to properly free all nodes

    // Free the AbstractSyntaxTree object itself
    free(obj);
}

// Implementations of the methods (currently stubs)
static Node *getRootAST(AbstractSyntaxTree *this) {
    return this->root;
}

static void addAST(Node *node, AbstractSyntaxTree *this) {
    // not needed
}

static void removeAST(Node *node, AbstractSyntaxTree *this) {
    // not needed
}

static void cleanUpTreeAST(AbstractSyntaxTree *this) {
    for (int i = 1; i < this->size; i++) {
        Node * node = this->nodes[i];
        if (node->singleChild != NULL && 
            strcmp(node->info.expression, "BACKSLASH") != 0 && 
            strcmp(node->info.expression, "TYPE1") != 0 &&
            strcmp(node->info.expression, "EMPTY") != 0) {

            node->singleChild->parent = node->parent;
            if (node->parent->leftChild == node){
                node->parent->leftChild = node->singleChild;
                node->info.expression = "EMPTY";
            }
            else if (node->parent->rightChild == node) {
                node->parent->rightChild = node->singleChild;
                node->info.expression = "EMPTY";
            }
            else if (node->parent->singleChild == node) {
                node->parent->singleChild = node->singleChild;
                node->info.expression = "EMPTY";
            }
        } 
        if (strcmp(node->info.expression, "EXPR1") == 0) {
            if (node->leftChild == NULL && node->rightChild == NULL) {
                // because expr is basically empty with no children
                // we only need the left child which is lexpr
                // because expr 1 can be empty
                node->parent->singleChild = node->parent->leftChild;
                node->parent->leftChild = NULL;
                node->parent->rightChild = NULL;
                node->info.expression = "EMPTY";
            }
        }
        if (strcmp(node->info.expression, "TYPE1") == 0) {
            if (node->singleChild == NULL) {
                node->parent->singleChild = node->parent->leftChild;
                node->parent->leftChild = NULL;
                node->parent->rightChild = NULL;
                node->info.expression = "EMPTY";
            }
        }
    }

    for (int i = 1; i < this->size; i++) {
        Node * node = this->nodes[i];
        if (node->singleChild != NULL && 
            strcmp(node->info.expression, "BACKSLASH") != 0 &&
            strcmp(node->info.expression, "TYPE1") != 0 && 
            strcmp(node->info.expression, "EMPTY") != 0) {
            node->singleChild->parent = node->parent;
            if (node->parent->leftChild == node){
                node->parent->leftChild = node->singleChild;
                node->info.expression = "EMPTY";
            }
            else if (node->parent->rightChild == node) {
                node->parent->rightChild = node->singleChild;
                node->info.expression = "EMPTY";
            }
            else if (node->parent->singleChild == node) {
                node->parent->singleChild = node->singleChild;
                node->info.expression = "EMPTY";
            }
        } 
    }
    for (int i = 0; i < this->size; i++) {
        Node * node = this->nodes[i];
        if (strcmp(node->info.expression, "EXPR") == 0 || strcmp(node->info.expression, "EXPR1") == 0) {
            node->info.expression = "SPACE";
            node->info.string = " ";
        } 
    }
}

static void reduceAST(AbstractSyntaxTree *this) {
    int counter = 0;
    while (possibleBetaReductionAST(this->root, this) == true && counter < 1000) {
        // printf("%d\n", counter);
        bool reductionDone = false;
        betaReductionAST(this->root, &reductionDone, this);
        counter++;
    }

    if (possibleBetaReductionAST(this->root, this) == true) {
        printf("Maximum number of reductions steps (1000) reached.\n");
        longjmp(lexerException, 1);
    }
}

static bool possibleBetaReductionAST(Node * node, AbstractSyntaxTree *this) {
    if (node == NULL) return false;

    if (node->leftChild != NULL && strcmp(node->leftChild->info.expression,"BACKSLASH") == 0) {
        
        if (hasVariableAST(node->rightChild, this) == true) {
            return true;
        }
    }

    if (possibleBetaReductionAST(node->leftChild, this) == true || possibleBetaReductionAST(node->rightChild, this) == true || possibleBetaReductionAST(node->singleChild, this) == true)
        return true;
    
    return false;
}

static void betaReductionAST(Node *node, bool *reductionDone, AbstractSyntaxTree *this) {
    if (node == NULL) return;
    if (reductionDone == true) return;

    if (node->leftChild != NULL && strcmp(node->leftChild->info.expression, "BACKSLASH") == 0) {
        if (capturedVariableAST(node->rightChild, node->leftChild->info.string, this) == true) {
            char newCharacter[2] = "a";

            while(capturedVariableAST(node->leftChild, newCharacter, this) == true) newCharacter[0]++;
            alfaConversionAST(node->leftChild, node->leftChild->info.string, newCharacter, this);
        }
    
        replaceVarAST(node->leftChild->singleChild, node->leftChild->info.string, node->rightChild, this);
        
        node->leftChild->singleChild->parent = node->parent;
        if (node->parent != NULL) {
            if (node->parent->leftChild == node)
                node->parent->leftChild = node->leftChild->singleChild;
            else if (node->parent->rightChild == node)
                node->parent->rightChild = node->leftChild->singleChild;
            else if (node->parent->singleChild == node)
                node->parent->singleChild = node->leftChild->singleChild;
        }
        node = node->leftChild->singleChild;

        if (node->parent == NULL) {
            this->root = node;
        }

        reductionDone = true;
        return;
    }
    
    betaReductionAST(node->singleChild, reductionDone, this);
    betaReductionAST(node->leftChild, reductionDone, this);
    betaReductionAST(node->rightChild, reductionDone, this);
}


// Note to self, i am doing the boolean evaluations of true and false explicit because
// i just realised my custom boolean class was implemented in the reverse so
// with 0 is false instaed of true, will cost too much time to fix that class
// and adjust all the other affected classes so will have to do it this way

static bool capturedVariableAST(Node *node, const char *variable, AbstractSyntaxTree *this) {
    if (node == NULL) return false;
    if (strcmp(node->info.expression, "VARIABLE") == 0 && strcmp(node->info.string, variable) == 0) return true;
    if(capturedVariableAST(node->leftChild, variable, this) == true || capturedVariableAST(node->rightChild, variable, this) == true || capturedVariableAST(node->singleChild, variable, this) == true )
        return true;
    
    return false;
}

static bool hasVariableAST(Node *node, AbstractSyntaxTree *this) {
    if (node == NULL) return false;
    if (strcmp(node->info.expression, "VARIABLE") == 0) {
        return true;
    }
    if (hasVariableAST(node->leftChild, this) == true || hasVariableAST(node->rightChild, this) == true || hasVariableAST(node->singleChild, this) == true)
        return true;
    return false;
}

static void alfaConversionAST(Node *node, const char *oldCharacter, const char *newCharacter, AbstractSyntaxTree *this) {
    if (node == NULL) return;
    if ((strcmp(node->info.expression, "VARIABLE") == 0 || 
        strcmp(node->info.expression, "BACKSLASH") == 0) && 
        strcmp(node->info.string, oldCharacter) == 0) 
        node->info.string = newCharacter;

    alfaConversionAST(node->singleChild, oldCharacter, newCharacter, this);
    alfaConversionAST(node->leftChild, oldCharacter, newCharacter, this);
    alfaConversionAST(node->rightChild, oldCharacter, newCharacter, this);
}

static void replaceVarAST(Node *node, const char *variable, Node *substitution, AbstractSyntaxTree *this) {
    if (node == NULL) return;

    if (strcmp(node->info.expression, "VARIABLE") == 0 && strcmp(node->info.string, variable) == 0) {
        Node * tempSubstitution = copyTreeAST(substitution, NULL, this);
        tempSubstitution->parent = node->parent;
        if (node->parent->leftChild == node)
            node->parent->leftChild = tempSubstitution;
        else if (node->parent->rightChild == node)
            node->parent->rightChild = tempSubstitution;
        else if (node->parent->singleChild == node)
            node->parent->singleChild = tempSubstitution;

        node = tempSubstitution;
        return;
    }
    replaceVarAST(node->singleChild, variable, substitution, this);
    replaceVarAST(node->leftChild, variable, substitution, this);
    replaceVarAST(node->rightChild, variable, substitution, this);
}

static Node *copyTreeAST(Node *root, Node *parent, AbstractSyntaxTree *this) {
    if (root == NULL) return NULL;
        
    Node * copy = malloc(sizeof(Node));
    copy->info = root->info;
    copy->index = root->index;
    copy->leftChild = copyTreeAST(root->leftChild, copy, this);
    copy->rightChild = copyTreeAST(root->rightChild, copy, this);
    copy->singleChild = copyTreeAST(root->singleChild, copy, this);
    copy->parent = parent;
    return copy;
}

static void printAST(Node *node, const AbstractSyntaxTree *this) {
    // Implementation
    if (node == NULL) return;

    int parentIndex = (node->parent == NULL) ? -1 : node->parent->index;
    // if (strcmp(node->info.expression, "LVAR") == 0) {
    //     printf("TESTESTEST\n");
    //     printf("%s||||\n", node->info.string);
    // }
    if (node->singleChild != NULL) {
        if (strcmp(node->info.expression, "BACKSLASH") == 0) {
            printf("(");
            printf("\\%s ", node->info.string);
            printf("^");
            printAST(node->singleChild, this);
            printAST(node->rightChild, this);
            printf(")");
        } else if (strcmp(node->info.expression, "TYPE1") == 0) {
            // printf("TEST\n");
            printf("->");
            printAST(node->singleChild, this);
        } else {
            printAST(node->singleChild, this);
            printf("%s", node->info.string);
        }
    } else {
        printf("(");
        printAST(node->leftChild, this);
        printf("%s", node->info.string);
        printAST(node->rightChild, this);
        printf(")");
    }
    if (node == this->root) printf("\n");
}

static bool checkIfItHasAType(Node *node, const char *variable, const AbstractSyntaxTree *this) {
    // Implementation
    if (node == NULL) return false;



    if (strcmp(node->info.expression, "BACKSLASH") == 0) {
        if (strcmp(node->info.string, variable) == 0) return true;
    }

    return checkIfItHasAType(node->parent, variable, this);

}

static bool checkIfTypeIsDerivable(Node *node, char *variable, const AbstractSyntaxTree *this) {
    // Implementation
    if (node == NULL) return false;



    if (strcmp(node->info.expression, "BACKSLASH") == 0) {
        if (strcmp(node->info.string, variable) == 0) return true;
    }

    return checkIfItHasAType(node->parent, variable, this);

}

static void checkIfDerivable(Node *node, const AbstractSyntaxTree *this) {
    // Implementation
    if (node == NULL) return;

    int parentIndex = (node->parent == NULL) ? -1 : node->parent->index;
    if (node->singleChild != NULL) {
        if (strcmp(node->info.expression, "BACKSLASH") == 0) {
            checkIfDerivable(node->singleChild, this);
            checkIfDerivable(node->rightChild, this);
        } else if (strcmp(node->info.expression, "TYPE1") == 0) {
            checkIfDerivable(node->singleChild, this);
        } else {
            checkIfDerivable(node->singleChild, this);
        }
    } else {
        checkIfDerivable(node->leftChild, this);
        // printf("%s", node->info.string);
        if (strcmp(node->info.string, ":") == 0) {
            printf("\n", node->info.string);
        } 
        checkIfDerivable(node->rightChild, this);
    }
    if (node == this->root) printf("\n");
}

static void typeCheckingAST(Node *node, const AbstractSyntaxTree *this) {
    for (int i = 0; i < this->size; i++) {
        Node * node = this->nodes[i];
        if (strcmp(node->info.expression, "LVAR") == 0) {
            // printf("%s", node->info.string);
            
            if (checkIfItHasAType(node, node->info.string, this) == false) {
                printf("Error occured::typeCheckingAST::Variable does not have a type\n");
                longjmp(lexerException, 1);
            }
        } 
    }
    
    for (int i = 0; i < this->size; i++) {
        Node * node = this->nodes[i];
        if (node->parent != NULL && strcmp(node->parent->info.string, ":") == 0 && node->parent->rightChild == node) break;
        // checkIfDerivable(node, this);
        if (strcmp(node->info.expression, "UVAR") == 0) {
            // char type[] = ""; // Initialize an empty character array
            // checkIfTypeIsDerivable(node, type, this);
            printf("%s", node->info.string);
        }
        // if (strcmp(node->info.string, ":") == 0) {
        //     printf("\n", node->info.string);
        // } 
    }
}