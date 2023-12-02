#include <omp.h>
#include "include/ssort.h"
#include <stdio.h>

static const int NUM_THREADS = 5;

void ssort(int *arr, int lenght);
void PrintArray(int *arr, int lenght);
int find_max_pos(int *arr, int lenght, int offset);
int find_min_pos(int *arr, int lenght, int offset);
void configure(int num_threads);
void swap_sole(int *arr, int pos_a, int pos_b);

int find_min_pos(int *arr, int lenght, int offset)
{
    int min_pos = offset;
    for (int i = offset; i < (lenght - (offset == 0 ? 1 : offset)); i++)
    {
        if (*(arr + i) < arr[min_pos])
            min_pos = i;
    }

    return min_pos;
}

int find_max_pos(int *arr, int lenght, int offset)
{
    int max_pos = offset;
   
    for (int i = offset; i < (lenght - (offset == 0 ? 1 : offset)); i++)
    {
        if (arr[i] > arr[max_pos])
            max_pos = i;
    }

    return max_pos;
}

void configure(int num_threads)
{
    omp_set_num_threads(num_threads);
}

void PrintArray(int *arr, int lenght)
{
    for (int i = 0; i < lenght; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void swap_sole(int *arr, int pos_a, int pos_b)
{
    if (pos_a == pos_b)
        return;
    int temp = arr[pos_a];
    arr[pos_a] = arr[pos_b];
    arr[pos_b] = temp;
}

/**
 * This function sorts the array using 2 threads, one from start and one from end
 */
void ssort(int *arr, int lenght)
{

    omp_set_num_threads(2); // One searches maximun, one minimum
    #pragma omp parallel /*shared(elements)*/
    {
        int thread_id = omp_get_thread_num();
        int is_searching_min = thread_id; // 0 --> searches min, 1 --> searches max
        int min_pos = -1, max_pos = -1;
        for (int i = 0; i <= (lenght / 2); i++)
        {
            if (!is_searching_min)
            {
                max_pos = find_max_pos(arr, lenght, i);
                //printf("Thread %d found max value %d in iteration %d\n", omp_get_thread_num(), arr[max_pos], i);
            }
            else
            {
                min_pos = find_min_pos(arr, lenght, i);
                //printf("Thread %d found min value %d in iteration %d\n", omp_get_thread_num(), arr[min_pos], i);
            }

            #pragma omp barrier
            {
                if (is_searching_min)
                {
                    #pragma omp critical
                    {
                        if (arr[i] > arr[min_pos])
                            swap_sole(arr, i, min_pos);
                    }
                }

                else
                {
                    #pragma omp critical
                    {
                        if (arr[lenght - i - 1] < arr[max_pos])
                            swap_sole(arr, lenght - i - 1, max_pos);
                    }
                }
            }
        }
    }
}
