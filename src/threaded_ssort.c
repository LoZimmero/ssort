#include "include/threadedssort.h"
#include <omp.h>
#include "include/ssort.h"
#include <stdio.h>

void PrintArray(int *arr, int lenght);

static const int NUM_THREADS = 5;

void double_sort(int *arr, int lenght);

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

void thread_ssort(int *arr, int lenght)
{
    omp_set_num_threads(NUM_THREADS);
    int i;
    const int sub_array_max_size = lenght / 4;
    printf("suarraymaxsize = %d\n", sub_array_max_size);
    /*
    #pragma omp parallel for shared(arr)
    for (i = 0; i < num_threads; i++)
    {
        printf("Here's thread number %d\n", omp_get_thread_num());
        int size = sub_array_max_size;
        if (lenght - (i + 1) * sub_array_max_size <= 0)
        {
            size = lenght - i * sub_array_max_size;
        }
        ssort(arr + i * sub_array_max_size, size);
    }
    */
#pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int starting_point = thread_id * sub_array_max_size;
        int ending_point = starting_point + sub_array_max_size;
        if (ending_point > lenght)
        {
            ending_point = lenght;
        }
        printf("Hey, i am thread numer %d\n", thread_id);
        ssort(arr + starting_point, ending_point - starting_point);
        printf("Thread number %d ordered from position %d to %d\n", thread_id, starting_point, ending_point);
        printf("\n\n ARRAY FROM THREAD %d is:\n\n", thread_id);
        PrintArray(arr + starting_point, ending_point - starting_point);
        for (int i = starting_point; i < ending_point; i++)
        {
            printf("Here's thread number %d\n", thread_id);
            int size = sub_array_max_size;
            if (lenght - (i + 1) * sub_array_max_size <= 0)
            {
                size = lenght - i * sub_array_max_size;
            }
            ssort(arr + i * sub_array_max_size, size);
        }
    }
#pragma omp parallel for
    for (int i = 0; i < sub_array_max_size; i++)
    {
        int pieces[NUM_THREADS];
        int j = i, k = 0;
        while (j < lenght)
        {
            pieces[k] = arr[j];
            j += sub_array_max_size;
            k += 1;
        }

        int t = 0;
        ssort(pieces, NUM_THREADS);
        printf("PRINTING PIECES ITERATION NUM %d\n", i);
        PrintArray(pieces, NUM_THREADS);
        for (j = i * sub_array_max_size, t = 0 && j < lenght; t < k; j++, t++)
        {
            printf("Moving element from pieces postion %d to arr position %d. I am iteration num %d\n", t, j, i);
            arr[j] = pieces[t];
        }

        printf("HELLO, %d - i = %d\n", omp_get_thread_num(), i);
    }

    PrintArray(arr, lenght);
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
void double_sort(int *arr, int lenght)
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
                //PrintArray(arr, lenght);
            }
        }
    }
}
