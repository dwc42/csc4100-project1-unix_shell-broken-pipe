#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"
// Define a node structure
// chat gpt made the structure and some of the code for this.
// https://chatgpt.com/share/68d817e2-cea8-8004-86e2-c2ec6cb1d8a1

struct Node *createNode(char *optionName, int intData, char *stringData)
{
	struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
	newNode->optionName = optionName;
	newNode->intData = intData;
	newNode->stringData = stringData;
	return newNode;
}
void insertAtBeginning(struct Node **head, struct Node *value)
{
	struct Node *newNode = value;
	newNode->next = *head;
	*head = newNode;
}
void printList(struct Node *head)
{
	struct Node *temp = head;
	int i = 0;
	while (temp != NULL)
	{
		printf("{\n");
		printf("  %d:\n", i++);
		printf("  optionName: %s,\n", temp->optionName);
		printf("  intData: %d,\n", temp->intData);
		printf("  stringData: %s\n", temp->stringData);
		printf("} -->\n");
		temp = temp->next;
	}
	printf("NULL\n");
}