
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int* findWordsContaining(char** words, int wordsSize, char x, int* returnSize)
{
    int i = 0, j = 0;
    int *res = malloc(wordsSize * sizeof(int));
    while (i < wordsSize)
    {
        if (strchr(words[i], x))
        {
            res[j] = i;
            j++;
        }
        i++;
    }
    returnSize = &wordsSize;
    return res;
}

int main()
{
    int *returnres;
    int i = 0;
    char *arr[] = {"leet", "code"};
    int *res = findWordsContaining(arr, 2, 'e', returnres);
    while (res[i] < 3)
    {
        printf("[%d]", res[i++]);
    }
}