#include "include/ssort.h"
#include <stdio.h>

// Swap two elements of an array
void swap(int *arr, int i, int j)
{
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

void ssort(int *arr, int lenght)
{
    // Exit if array is empty
    if (sizeof(arr) == 0 || lenght < 2)
        return;
    int i, j;
    for (i = 1; i < lenght; i++)
    {
        for (j = i - 1; j >= 0; j--)
        {
            // Shifts new number read back untilit is placed in the right place
            if (arr[j + 1] < arr[j])
            {
                swap(arr, j + 1, j);
            }
            else
            {
                break;
            }
        }
    }
}