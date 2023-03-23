// Ariel Towne
// ar217413

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ListyString.h"

int main(int argc, char **argv)
{
    processInputFile(argv[1]);

    return 0;
}

int processInputFile(char *filename)
{
    FILE *fp = NULL;
    char *buffer, key, command;
    ListyString *listy;

    // Open-file error precaution.
    if ((fp = fopen(filename, "r")) == NULL)
        return 1;

    buffer = malloc(sizeof(char) * 1024);

    // The first line in the file will become the original listy string.
    fscanf(fp, "%s", buffer);
    listy = createListyString(buffer);

    // Reads all commands in the file; each symbol calls a different function.
    while(!feof(fp))
    {
        do
        {
          command = fgetc(fp);
        } while(!feof(fp) && isspace(command));

        switch(command)
        {
            case '@':
                do
                {
                    key = fgetc(fp);
                } while(!feof(fp) && isspace(key));

                fscanf(fp, "%s", buffer);
                replaceChar(listy, key, buffer);
                break;

            case '+':
                fscanf(fp, "%s", buffer);
                listyCat(listy, buffer);
                break;

            case '-':
                do
                {
                    key = fgetc(fp);
                } while(!feof(fp) && isspace(key));

                replaceChar(listy, key, NULL);
                break;

            case '~':
                reverseListyString(listy);
                break;

            case '?':
                printf("%d\n", listyLength(listy));
                break;

            case '!':
                printListyString(listy);
                break;
        }
    }

    free(buffer);
    destroyListyString(listy);

    fclose(fp);

    return 0;
}

ListyString *createListyString(char *str)
{
    int i;
    ListyNode *temp;
    ListyString *listy = malloc(sizeof(ListyString));

    // Creates a linked-list representation of an empty string.
    if(str == NULL || !(strcmp(str, "")))
    {
        listy->head = NULL;
        listy->length = 0;
        return listy;
    }

    listy->head = malloc(sizeof(ListyNode));
    temp = listy->head;

    // 'i' acts as the index and its final value will be (strlen-1).
    for(i = 0; str[i+1] != '\0'; i++)
    {
        temp->data = str[i];
        temp->next = malloc(sizeof(ListyNode));
        temp = temp->next;
    }

    // Sets the last char in the linked-list and codes defensively.
    temp->data = str[i];
    temp->next = NULL;

    listy->length = i + 1;

    return listy;
}

ListyString *destroyListyString(ListyString *listy)
{
    ListyNode *temp;

    if(listy == NULL)
        return NULL;

    temp = listy->head;

    // listy->head "moves down the line" as
    // temp deallocates the previous link in the chain.
    while(temp != NULL)
    {
        listy->head = listy->head->next;
        free(temp);
        temp = listy->head;
    }

    free(listy);

    return NULL;
}

ListyString *cloneListyString(ListyString *listy)
{
    ListyString *new_listy;
    ListyNode *original_temp, *new_temp;

    if(listy == NULL)
        return NULL;

    if(listy->head == NULL)
        return createListyString(NULL);

    // new_listy will be the clone.
    new_listy = malloc(sizeof(ListyString));
    new_listy->head = malloc(sizeof(ListyNode));

    new_listy->length = listy->length;

    original_temp = listy->head;
    new_temp = new_listy->head;

    // Copies each char into new_listy as it is created.
    while (original_temp->next != NULL)
    {
        new_temp->data = original_temp->data;
        new_temp->next = malloc(sizeof(ListyNode));
        new_temp = new_temp->next;
        original_temp = original_temp->next;
    }

    // Sets the last char in new_listy and codes defensively.
    new_temp->data = original_temp->data;
    new_temp->next = NULL;

    return new_listy;
}

void replaceChar(ListyString *listy, char key, char *str)
{
    int i;
    ListyNode *placeHolder, *temp, *freeKey, *freeHead = NULL;

    if(listy == NULL || listy->head == NULL)
        return;

    temp = listy->head;

    // This will check the first node in the listy string for the key.
    if(listy->head->data == key)
    {
        // Keeps track of the key and the node that the replacement string needs to reattach to.
        freeHead = listy->head;
        placeHolder = temp->next;

        // Creates the replacement string and attaches it to the nodes around the key.
        for (i = 0; (str != NULL && str[i] != '\0'); i++)
        {
            temp->next = malloc(sizeof(ListyNode));
            temp = temp->next;
            temp->data = str[i];
        }

        temp->next = placeHolder;

        // Adds the length of the replacement string and subtracts the length of the key: 1.
        listy->length += (i - 1);

    }

    // This checks all of the nodes after the first one for the key.
    while (temp->next != NULL)
    {
        if(temp->next->data != key)
        {
            temp = temp->next;
            continue;
        }

        // Keeps track of the key and the node that the replacement string needs to reattach to.
        freeKey = temp->next;
        placeHolder = temp->next->next;

        // Creates the replacement string and attaches it to the nodes around the key.
        for (i = 0; (str != NULL && str[i] != '\0'); i++)
        {
            temp->next = malloc(sizeof(ListyNode));
            temp = temp->next;
            temp->data = str[i];
        }

        temp->next = placeHolder;

        // Adds the length of the replacement string and subtracts the length of the key: 1.
        listy->length += (i - 1);

        free(freeKey);
    }

    // If listy->head had the key, then it will be deallocated and there is a new head.
    if (freeHead != NULL)
    {
      listy->head = listy->head->next;
      free(freeHead);
    }
}

void reverseListyString(ListyString *listy)
{
    int i = 0;
    ListyNode *temp;
    char *buffer;

    if(listy == NULL || listy->head == NULL || listy->head->next == NULL)
        return;

    buffer = malloc(sizeof(char) * 1024);

    // Makes the linked-list into a string by incrementing index 'i'.
    temp = listy->head;
    while (temp != NULL)
    {
        buffer[i] = temp->data;
        i++;
        temp = temp->next;
    }

    // Puts the string back into the linked-list by decrementing index 'i'.
    temp = listy->head;
    while (temp != NULL)
    {
        i--;
        temp->data = buffer[i];
        temp = temp->next;
    }

    free(buffer);
}

ListyString *listyCat(ListyString *listy, char *str)
{
    ListyString *new_listy;
    ListyNode *temp;

    // Special cases.
    if (listy == NULL && str == NULL)
        return NULL;
    else if (listy == NULL)
        return (createListyString(str));
    else if (str == NULL || !(strcmp(str, "")))
        return listy;

    new_listy = createListyString(str);

    temp = listy->head;

    if (temp == NULL)
        listy->head = new_listy->head;

    // new_listy is attached to the end of listy.
    else
    {
        while (temp->next != NULL)
            temp = temp->next;

        temp->next = new_listy->head;
    }

    listy->length += new_listy->length;

    // Deallocates the extra ListyString pointer that came from createListyString().
    free(new_listy);

    return listy;
}

int listyCmp(ListyString *listy1, ListyString *listy2)
{
    ListyNode *first_temp, *second_temp;

    // Special cases.
    if(listy1 == NULL && listy2 == NULL)
        return 0;
    else if((listy1 == NULL && listy2 != NULL) || (listy1 != NULL && listy2 == NULL))
        return 1;
    else if(listy1->head == NULL && listy2->head == NULL)
        return 0;
    else if((listy1->head == NULL && listy2->head != NULL) || (listy1->head != NULL && listy2->head == NULL))
        return 1;
    // If the strings are not of the same length, then they are not equal.
    else if(listy1->length != listy2->length)
        return 1;

    first_temp = listy1->head;
    second_temp = listy2->head;

    while(first_temp != NULL && second_temp != NULL)
    {
        // Leaves at the first char that is not the same in both listy strings.
        if(first_temp->data != second_temp->data)
            return 1;

        first_temp = first_temp->next;
        second_temp = second_temp->next;
    }

    // Checks if one listy string ended before the other;
    // should not execute due to the last "special case", but just in case.
    if (!(first_temp == NULL && second_temp == NULL))
        return 1;

    return 0;
}

int listyLength(ListyString *listy)
{
    if(listy == NULL)
        return -1;

    if(listy->head == NULL)
        return 0;

    // The number of chars/ListyNodes in listy.
    return listy->length;
}

void printListyString(ListyString *listy)
{
    ListyNode *temp;

    // These represent that there is no string or it's an empty string.
    if(listy == NULL || listy->head == NULL || listy->length == 0)
    {
        printf("(empty string)\n");
        return;
    }

    temp = listy->head;

    // Prints the string in order.
    while(temp != NULL)
    {
        printf("%c", temp->data);
        temp = temp->next;
    }

    printf("\n");
}

double difficultyRating(void)
{
    return 2.55;
}

double hoursSpent(void)
{
    return 38.5;
}
