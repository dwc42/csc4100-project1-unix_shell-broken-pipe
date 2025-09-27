#ifndef __LINKED_LIST_H
#define __LINKED_LIST_H

struct Node
{
	char *optionName;
	int intData;
	char *stringData;
	struct Node *next;
};
struct Node *createNode(char *optionName, int intData, char *stringData);
void insertAtBeginning(struct Node **head, struct Node *value);
void printList(struct Node *head);
#endif