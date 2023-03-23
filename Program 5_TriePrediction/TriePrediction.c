// Ariel Towne
// ar217413

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "TriePrediction.h"

TrieNode *createTrieNode(void)
{
    return calloc(1, sizeof(TrieNode));
}

TrieNode *insertString(TrieNode *root, char *str)
{
    int i, len, index;
    TrieNode *temp;

    if (root == NULL)
        return NULL;

    len = strlen(str);

    for (i = 0, temp = root; i < len; i++)
    {
        // Travels to each node that represents next letter in string.
        index = tolower(str[i]) - 'a';

        if (temp->children[index] == NULL)
            temp->children[index] = createTrieNode();

        temp = temp->children[index];
    }

    temp->count++;

    // Returns the last node that represents the last char in the string.
    return temp;
}

char stripPunctuators(char *str)
{
    char *temp, *buffer, retval;
    int len, index, i;

    len = strlen(str);

    retval = str[len - 1];

    buffer = malloc(sizeof(char) * (len + 1));

    // Keeps only letters from original string.
    for (i = 0, index = 0; i < len; i++)
        if (isalpha(str[i]))
            buffer[index++] = str[i];

    buffer[index] = '\0';
    strcpy(str, buffer);

    free(buffer);

    // Returns last char in original string.
    return retval;
}

// Helper function called by printTrie(). (Credit: Dr. S.)
void printTrieHelper(TrieNode *root, char *buffer, int k)
{
	int i;

	if (root == NULL)
		return;

	if (root->count > 0)
		printf("%s (%d)\n", buffer, root->count);

	buffer[k + 1] = '\0';

	for (i = 0; i < 26; i++)
	{
		buffer[k] = 'a' + i;

		printTrieHelper(root->children[i], buffer, k + 1);
	}

	buffer[k] = '\0';
}

// If printing a subtrie, the second parameter should be 1; otherwise, if
// printing the main trie, the second parameter should be 0. (Credit: Dr. S.)
void printTrie(TrieNode *root, int useSubtrieFormatting)
{
	char buffer[1026];

	if (useSubtrieFormatting)
	{
		strcpy(buffer, "- ");
		printTrieHelper(root, buffer, 2);
	}
	else
	{
		strcpy(buffer, "");
		printTrieHelper(root, buffer, 0);
	}
}

TrieNode *buildTrie(char *filename)
{
    TrieNode *trieOfWords, *subtrieOfWords;
    char *word, punctuationCheck;
    FILE *ifp;

    // File-opening precaution.
    if ((ifp = fopen(filename, "r")) == NULL)
        return NULL;

    word = malloc(sizeof(char) * (MAX_CHARACTERS_PER_WORD + 1));

    trieOfWords = createTrieNode();

    // Ths loops through each sentence.
    while (fscanf(ifp, "%s", word) != EOF)
    {
        punctuationCheck = stripPunctuators(word);

        // This inserts the first word into the trie, but not as a subtrie.
        subtrieOfWords = insertString(trieOfWords, word);

        // If there's one 1 word in the sentence, move on to the next sentence.
        if (punctuationCheck == '.' || punctuationCheck == '!' || punctuationCheck == '?')
            continue;

        // This loops through all other words in the sentence.
        while (fscanf(ifp, "%s", word) != EOF)
        {
            if (subtrieOfWords->subtrie == NULL)
                subtrieOfWords->subtrie = createTrieNode();

            subtrieOfWords = subtrieOfWords->subtrie;

            punctuationCheck = stripPunctuators(word);

            // Inserts the current word in the subtrie of the previous word.
            insertString(subtrieOfWords, word);

            // Any word that is in a subtrie must also be in the trie itself.
            subtrieOfWords = insertString(trieOfWords, word);

            // If this is the end of the sentence, move on to the next sentence.
            if (punctuationCheck == '.' || punctuationCheck == '!' || punctuationCheck == '?')
                break;
        }
    }

    fclose(ifp);
    free(word);

	return trieOfWords;
}

TrieNode *destroyTrie(TrieNode *root)
{
    int i;
    if (root == NULL)
        return NULL;

    destroyTrie(root->subtrie);

    // Recursively goes to all nodes.
    for (i = 0; i < 26; i++)
        destroyTrie(root->children[i]);

    free(root);
}

TrieNode *getNodeHelper(TrieNode *root, char *str)
{
    int i, len;
    TrieNode *temp, *letter;

    if (root == NULL)
        return NULL;

    len = strlen(str);

    for (i = 0, temp = root; i < len; i++)
    {
        // Travels to each node that represents next letter in string.
        letter = temp->children[tolower(str[i]) - 'a'];

        // The string might not be represented in the trie.
        if (letter == NULL)
            return NULL;

        temp = letter;
    }

    // Returns the last node that represents the last char in the string.
    return temp;
}

TrieNode *getNode(TrieNode *root, char *str)
{
    TrieNode *lastNode;

    lastNode = getNodeHelper(root, str);

    // The string might not be in the trie.
    if (lastNode == NULL || lastNode->count <= 0)
        return NULL;

    return lastNode;
}

void getMostFrequentWordHelper(TrieNode *root, char *str, int *mostFrequentCnt, char *buffer, int index)
{
    int i;

    if (root == NULL)
        return;

    // Updates the most frequent word and it's count.
    if (root->count > *mostFrequentCnt)
    {
        strcpy(str, buffer);
        *mostFrequentCnt = root->count;
    }

    buffer[index + 1] = '\0';

    for (i = 0; i < 26; i++)
    {
        // Adds each letter represented by next node to the string.
        buffer[index] = 'a' + i;
        getMostFrequentWordHelper(root->children[i], str, mostFrequentCnt, buffer, index + 1);
    }

    buffer[index] = '\0';
}

void getMostFrequentWord(TrieNode *root, char *str)
{
    int *mostFrequentCnt;
    char *buffer;

    str[0] = '\0';

    if (root == NULL)
        return;

    // Pointer ensures the correct value is consistent in all recursive calls.
    mostFrequentCnt = calloc(1, sizeof(int));

    buffer = malloc(sizeof(char) * (MAX_CHARACTERS_PER_WORD + 1));
    buffer[0] = '\0';

    getMostFrequentWordHelper(root, str, mostFrequentCnt, buffer, 0);

    free(buffer);
    free(mostFrequentCnt);
}

int containsWord(TrieNode *root, char *str)
{
    // The string might not be in the trie.
    if (getNode(root, str) == NULL)
        return 0;

    return 1;
}

int prefixCountHelper(TrieNode *root)
{
	int i, sum = 0;

	if (root == NULL)
        return 0;

    for (i = 0; i < 26; i++)
        // Adds up counts from all nodes.
        sum += prefixCountHelper(root->children[i]);

    return root->count + sum;
}

int prefixCount(TrieNode *root, char *str)
{
    TrieNode *prefix;

    // Find the prefix in the trie.
    prefix = getNodeHelper(root, str);

	return prefixCountHelper(prefix);
}

int processInputFile(TrieNode *root, char *filename)
{
    TrieNode *subTrie;
    int i, n;
    char *str;
    FILE *ifp;

    // File-opening precaution.
    if ((ifp = fopen(filename, "r")) == NULL)
        return 1;

    str = malloc(sizeof(char) * (MAX_CHARACTERS_PER_WORD + 1));

    while (fscanf(ifp, "%s", str) != EOF)
    {
        if (!strcmp(str, "@"))
        {
            fscanf(ifp, "%s %d", str, &n);
            printf("%s", str);

            // Maximum of n words can be printed.
            for (i = 1; i <= n; i++)
            {
                // Search for the string in the trie.
                subTrie = getNodeHelper(root, str);

                if (subTrie == NULL || subTrie->subtrie == NULL)
                    break;

                // Finds the most frequent word in subtrie,
                // which is also the next word to search for in the trie.
                getMostFrequentWord(subTrie->subtrie, str);

                printf(" %s", str);
            }

            printf("\n");
            continue;
        }

        if (!strcmp(str, "!"))
        {
            printTrie(root, 0);
            continue;
        }


        // If the command wasn't @ or !,
        // then it is a word I should search for in the trie.
        printf("%s\n", str);

        // The string might not be in the trie.
        if ((subTrie = getNode(root, str)) == NULL)
        {
            printf("(INVALID STRING)\n");
            continue;
        }

        if (subTrie->subtrie == NULL)
        {
            printf("(EMPTY)\n");
            continue;
        }

        printTrie(subTrie->subtrie, 1);
    }

    fclose(ifp);
    free(str);

	return 0;
}

double difficultyRating(void)
{
	return 3.30;
}

double hoursSpent(void)
{
	return 46.0;
}

int main(int argc, char **argv)
{
    TrieNode *trieOfWords = NULL;
    int fileExistence;

    trieOfWords = buildTrie(argv[1]);

    fileExistence = processInputFile(trieOfWords, argv[2]);

    trieOfWords = destroyTrie(trieOfWords);

	return 0;
}
