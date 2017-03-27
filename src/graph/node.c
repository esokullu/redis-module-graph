#include "node.h"
#include "edge.h"

Node* NewNode(const char* alias, const char* id) {
	Node* node = (Node*)malloc(sizeof(Node));
	
	node->id = NULL;
	node->alias = NULL;
	node->outgoingEdges = NewVector(Edge*, 0);
	node->incomingEdges = 0;
	node->internalId = mkrndstr();

	if(id != NULL) {
		node->id = strdup(id);
	}

	if(alias != NULL) {
		node->alias = strdup(alias);
	}

	return node;
}


char *mkrndstr() { // const size_t length, supra
size_t length = 32;
static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!"; // could be const
char *randomString;

if (length) {
    randomString = malloc(length +1); // sizeof(char) == 1, cf. C99

    if (randomString) {
        int l = (int) (sizeof(charset) -1); // (static/global, could be const or #define SZ, would be even better)
        int key;  // one-time instantiation (static/global would be even better)
        for (int n = 0;n < length;n++) {        
            key = rand() % l;   // no instantiation, just assignment, no overhead from sizeof
            randomString[n] = charset[key];
        }

        randomString[length] = '\0';
    }
}

return randomString;
}



Node* Node_Clone(const Node *node) {
	Node *clone = NewNode(node->alias, node->id);
	memcpy(&clone->internalId, &node->internalId, sizeof(node->internalId));
	return clone;
}

int Node_Compare(const Node *a, const Node *b) {
	return a->internalId == b->internalId;
}

void ConnectNode(Node* src, Node* dest, const char* connection) {
	Edge* e = NewEdge(src, dest, connection);
	Vector_Push(src->outgoingEdges, e);
	dest->incomingEdges++;
}

void FreeNode(Node* node) {
	if(node->id) {
		free(node->id);
	}
	if(node->alias) {
		free(node->alias);
	}

	for(int i = 0; i < Vector_Size(node->outgoingEdges); i++) {
		Edge* e;
		Vector_Get(node->outgoingEdges, i, &e);
		FreeEdge(e);
	}

	Vector_Free(node->outgoingEdges);

	free(node);
}
