#include <stdio.h>
#include "include/ssort.h"
#include "include/bubble_sort.h"
#include <time.h>

int test_array[] = {44,6, 4, 1, 326, 3, 236, 23, 3, 6, 3, 2, 667, 18, 6, 7, 8, 97, 567, 5, 10, 11};
const int test_array_lenght = 22;
//int b_test_array[] = {6, 4, 1, 326, 3, 236, 23, 3, 6, 3, 2, 667, 6, 7, 8, 97, 567, 5};

void print_test_array();

int normal_main()
{
    time_t start, end;
    printf("Printing initial array:\n");
    print_test_array();
    start = clock();
    bubble_sort(test_array, 18);
    end = clock();
    printf("SSORT took time: %f\n", ((double)(end - start)));
    printf("Printing sorted array:\n");
    print_test_array();
    return 0;
}

int parallel_main()
{
    time_t start, end;
    printf("Printing initial array:\n");
    print_test_array();
    start = clock();
    ssort(test_array, test_array_lenght);
    end = clock();
    printf("SSORT took time: %f ms\n", ((double)((end - start) / CLOCKS_PER_SEC)));
    printf("Printing sorted array:\n");
    print_test_array();

    return 0;
}

int main(int argc, char *argv[])
{
    //normal_main();
    parallel_main();
}

void print_test_array()
{
    for (int i = 0; i < test_array_lenght; i++)
    {
        printf("%d  ", test_array[i]);
    }
    printf("\n");
}
