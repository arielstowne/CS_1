// Ariel Towne
//ar217413

#include <stdio.h>
#include <stdlib.h>
#include "LonelyPartyArray.h"

LonelyPartyArray *createLonelyPartyArray(int num_fragments, int fragment_length)
{
    int i;
    LonelyPartyArray *party;

    // Checks if the arguments are invalid.
    if (num_fragments <= 0 || fragment_length <= 0)
        return NULL;

    party = malloc(sizeof(LonelyPartyArray));

    // Malloc error precaution.
    if (party == NULL)
    {
        return NULL;
    }

    // Initializes the integer LPA variables.
    party->num_fragments = num_fragments;
    party->fragment_length = fragment_length;
    party->num_active_fragments = 0;
    party->size = 0;

    party->fragments = malloc(sizeof(int *) * party->num_fragments);

    // Malloc error precaution.
    if (party->fragments == NULL)
    {
        free(party);
        return NULL;
    }

    // Initializes the fragment pointers.
    for (i = 0; i < party->num_fragments; i++)
        party->fragments[i] = NULL;

    party->fragment_sizes = malloc(sizeof(int) * party->num_fragments);

    // Malloc error precaution.
    if (party->fragment_sizes == NULL)
    {
        free(party->fragments);
        free(party);
        return NULL;
    }

    // Initializes the sizes of the fragments.
    for (i = 0; i < party->num_fragments; i++)
        party->fragment_sizes[i] = 0;

    printf("-> A new LonelyPartyArray has emerged from the void. (capacity: %d, fragments: %d)\n",
           (party->num_fragments * party->fragment_length), party->num_fragments);

    return party;
}

LonelyPartyArray *destroyLonelyPartyArray(LonelyPartyArray *party)
{
    int i;

    if (party == NULL)
        return NULL;

    // Deallocates any memory allocated in set().
    for (i = 0; i < party->num_fragments; i++)
    {
        if (party->fragments[i] != NULL)
            free(party->fragments[i]);
    }

    // Deallocates memory allocated in createLonelyPartyArray().
    free(party->fragment_sizes);
    free(party->fragments);
    free(party);

    printf("-> The LonelyPartyArray has returned to the void.\n");

    return NULL;
}

int set(LonelyPartyArray *party, int index, int key)
{
    int i, j, k;

    if (party == NULL)
    {
        printf("-> Bloop! NULL pointer detected in set().\n");
        return LPA_FAILURE;
    }

    // Identifies which fragment (i) and which cell (j) the index maps to.
    i = index / party->fragment_length;
    j = index % party->fragment_length;

    // Checks if index is invalid.
    if (index < 0 || index >= getCapacity(party))
    {
        printf("-> Bloop! Invalid access in set(). (index: %d, fragment: %d, offset: %d)\n", index, i, j);
        return LPA_FAILURE;
    }

    // Creates fragment, initializes values, and increments num_active_fragments as necessary.
    if (party->fragments[i] == NULL)
    {
        party->fragments[i] = malloc(sizeof(int) * party->fragment_length);

        // Malloc error precaution.
        if (party->fragments[i] == NULL)
            return LPA_FAILURE;

        for (k = 0; k < party->fragment_length; k++)
            party->fragments[i][k] = UNUSED;

        printf("-> Spawned fragment %d. (capacity: %d, indices: %d..%d)\n",
               i, party->fragment_length, (party->fragment_length * i), ((party->fragment_length * (i + 1)) - 1));

        party->num_active_fragments++;
    }

    // Updates the count of the number of cells being used when it enters the key.
    if (party->fragments[i][j] == UNUSED)
    {
        party->size++;
        party->fragment_sizes[i]++;
    }

    party->fragments[i][j] = key;

    return LPA_SUCCESS;
}

int get(LonelyPartyArray *party, int index)
{
    int i, j;

    if (party == NULL)
    {
        printf("-> Bloop! NULL pointer detected in get().\n");
        return LPA_FAILURE;
    }

    // Identifies which fragment (i) and which cell (j) the index maps to.
    i = (index / party->fragment_length);
    j = (index % party->fragment_length);

    // Checks if index is invalid.
    if (index < 0 || index >= getCapacity(party))
    {
        printf("-> Bloop! Invalid access in get(). (index: %d, fragment: %d, offset: %d)\n", index, i, j);
        return LPA_FAILURE;
    }

    // If the fragment was never allocated, then the index was not being used.
    if (party->fragments[i] == NULL)
    {
        return UNUSED;
    }

    return party->fragments[i][j];
}

int delete(LonelyPartyArray *party, int index)
{
    int i, j;

    if (party == NULL)
    {
        printf("-> Bloop! NULL pointer detected in delete().\n");
        return LPA_FAILURE;
    }

    // Identifies which fragment (i) and which cell (j) the index maps to.
    i = (index / party->fragment_length);
    j = (index % party->fragment_length);

    // Checks if index is invalid.
    if (index < 0 || index >= getCapacity(party))
    {
        printf("-> Bloop! Invalid access in delete(). (index: %d, fragment: %d, offset: %d)\n", index, i, j);

        return LPA_FAILURE;
    }

    // Checks if index is being used.
    if (party->fragments[i] == NULL || party->fragments[i][j] == UNUSED)
    {
        return LPA_FAILURE;
    }

    // Updates the count of the number of cells being used when it removes the key.
    party->fragments[i][j] = UNUSED;
    party->size--;
    party->fragment_sizes[i]--;

    // Deallocates fragment if it is now empty, resets fragment pointer to NULL, and decrements num_active_fragments.
    if (party->fragment_sizes[i] == 0)
    {
        free(party->fragments[i]);

        party->fragments[i] = NULL;

        party->num_active_fragments--;

        printf("-> Deallocated fragment %d. (capacity: %d, indices: %d..%d)\n",
               i, party->fragment_length, (party->fragment_length * i), ((party->fragment_length * (i + 1)) - 1));
    }

    return LPA_SUCCESS;
}

int printIfValid(LonelyPartyArray *party, int index)
{
    int i, j;

    if (party == NULL)
        return LPA_FAILURE;

    // Identifies which fragment (i) and which cell (j) the index maps to.
    i = (index / party->fragment_length);
    j = (index % party->fragment_length);

    // Checks if index is invalid.
    if (index < 0 || index >= getCapacity(party))
        return LPA_FAILURE;

    // Checks if index is being used.
    if (party->fragments[i] == NULL || party->fragments[i][j] == UNUSED)
        return LPA_FAILURE;

    printf("%d\n", party->fragments[i][j]);

    return LPA_SUCCESS;
}

LonelyPartyArray *resetLonelyPartyArray(LonelyPartyArray *party)
{
    int i;

    if (party == NULL)
        return party;

    // Deallocates fragment, resets fragment pointer to NULL, and updates the count of the number of cells being used as necessary.
    for (i = 0; i < party->num_fragments; i++)
    {
        if (party->fragments[i] != NULL)
        {
            free(party->fragments[i]);
            party->fragments[i] = NULL;
            party->fragment_sizes[i] = 0;
        }
    }

    party->size = 0;
    party->num_active_fragments = 0;

    printf("-> The LonelyPartyArray has returned to its nascent state. (capacity: %d, fragments: %d)\n",
           getCapacity(party), party->num_fragments);

    return party;
}

int getSize(LonelyPartyArray *party)
{
    if (party == NULL)
        return -1;

    // Total number of occupied cells in LPA.
    return party->size;
}

int getCapacity(LonelyPartyArray *party)
{
    if (party == NULL)
        return -1;

    // Total number of cells in LPA.
    return (party->num_fragments * party->fragment_length);
}

int getAllocatedCellCount(LonelyPartyArray *party)
{
    if (party == NULL)
        return -1;

    // Total number of cells in all active fragments.
    return (party->num_active_fragments * party->fragment_length);
}

long long unsigned int getArraySizeInBytes(LonelyPartyArray *party)
{
    if (party == NULL)
        return 0;

    // Total size of all cells in LPA.
    return (getCapacity(party) * (long long unsigned int)sizeof(int));
}

long long unsigned int getCurrentSizeInBytes(LonelyPartyArray *party)
{
    if (party == NULL)
        return 0;

    // Total size of the whole LPA struct.
    return ((long long unsigned int)sizeof(LPA*) + (long long unsigned int)sizeof(LPA) +
            ((long long unsigned int)sizeof(int *) * party->num_fragments) + ((long long unsigned int)sizeof(int) * party->num_fragments) +
            ((long long unsigned int)sizeof(int) * getAllocatedCellCount(party)));
}

double difficultyRating(void)
{
    return 2.35;
}

double hoursSpent(void)
{
    return 35.0;
}

LonelyPartyArray *cloneLonelyPartyArray(LonelyPartyArray *party)
{
    int i, j;

    LonelyPartyArray *newParty;

    newParty = malloc(sizeof(LonelyPartyArray));

    // Malloc error precaution.
    if (newParty == NULL)
    {
        free(newParty);

        return NULL;
    }

    // Initializes the integer LPA variables.
    newParty->num_fragments = party->num_fragments;
    newParty->fragment_length = party->fragment_length;
    newParty->num_active_fragments = party->num_active_fragments;
    newParty->size = party->size;

    newParty->fragments = malloc(sizeof(int *) * newParty->num_fragments);

    // Malloc error precaution.
    if (newParty->fragments == NULL)
    {
        free(newParty);
        return NULL;
    }

    newParty->fragment_sizes = malloc(sizeof(int) * newParty->num_fragments);

    // Malloc error precaution.
    if (newParty->fragment_sizes == NULL)
    {
        free(newParty->fragments);
        free(newParty);
        return NULL;
    }

    // Sets the count of the number of cells being used and sets all fragment values.
    for (i = 0; i < newParty->num_fragments; i++)
    {
        newParty->fragment_sizes[i] = party->fragment_sizes[i];

        if (party->fragments[i] == NULL)
        {
            newParty->fragments[i] = NULL;
            continue;
        }

        // Only creates fragments if they are not NULL in the party LPA.
        newParty->fragments[i] = malloc(sizeof(int) * newParty->fragment_length);

        // Malloc error precaution.
        if (newParty->fragments[i] == NULL)
        {
            while(i >= 0)
            {
                if (newParty->fragments[i] != NULL)
                    free(newParty->fragments[i]);
                i--;
            }

            free(newParty->fragment_sizes);
            free(newParty->fragments);
            free(newParty);
            return NULL;
        }

        // Sets the specific values in the cells.
        for (j = 0; j < newParty->fragment_length; j++)
            newParty->fragments[i][j] = party->fragments[i][j];
    }

    printf("-> Successfully cloned the LonelyPartyArray. (capacity: %d, fragments: %d)\n",
           (newParty->num_fragments * newParty->fragment_length), newParty->num_fragments);

    return newParty;
}

