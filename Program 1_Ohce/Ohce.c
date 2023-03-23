// Ariel Towne
// ar217413

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "Ohce.h"

double difficultyRating(void);
double hoursSpent(void);

int main(int argc, char **argv)
{
    int i, j, lengthOfArgument, isItEcho = 0;

    // If there is no input, there will be no output.
    if(argc == 1)
        return 0;

    // Loops through each argument from user.
    for(i = 1; i < argc; i++)
    {
        lengthOfArgument = strlen(argv[i]);

        // Loops through each character except the null terminator.
        for(j = (lengthOfArgument - 1); j >= 0; j--)
        {
            // Checks for the word "echo" in each argument without going past the null terminator.
            if(tolower(argv[i][j]) == 'e' && argv[i][j] != '\0')
            {
                if(tolower(argv[i][j+1]) == 'c' && argv[i][j+1] != '\0')
                {
                    if(tolower(argv[i][j+2]) == 'h' && argv[i][j+2] != '\0')
                    {
                        if(tolower(argv[i][j+3]) == 'o' && argv[i][j+3] != '\0')
                        {
                            isItEcho = 1;
                        }
                    }
                }
            }

            // Prints each argument in reverse.
            printf("%c", argv[i][j]);
        }

        // Prints a space after reversing each argument except the last one.
        if( i != (argc - 1) )
            printf(" ");
    }

    // Executes if "echo" was found in any argument.
    if(isItEcho == 1)
        printf("\nEcho, echo, echo!");

    printf("\n");

    return 0;
}

double difficultyRating(void)
{
    return 1.7;
}

double hoursSpent(void)
{
    return 21.0;
}
