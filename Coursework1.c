#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#include "cwk1_extra.h"

void addToSet(int value)
{
    int i;

    if (setSize == maxSetSize) return;

#pragma omp critical
    {
        for (i = 0; i < setSize; i++)
            if (set[i] == value)
                return;

        set[setSize] = value;
        setSize++;
    }
}

void removeFromSet(int value)
{
    int i;
    int index = -1;
    
    #pragma omp parallel for
    for (i = 0; i < setSize; i++)
    {
        if (set[i] == value)
        {
            #pragma omp critical
            {
                if (index == -1)
                {
                    index = i;
                }
            }
        }
    }

    if (index != -1)
    {
        #pragma omp critical
        {
            for (i = index; i < setSize - 1; i++)
                set[i] = set[i + 1];
            setSize--;
        }
    }
}

void swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(int arr[], int low, int high)
{
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j] <= pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void parallelQuickSort(int arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);

        #pragma omp task firstprivate(low, pi)
        {
            parallelQuickSort(arr, low, pi - 1);
        }

        #pragma omp task firstprivate(pi, high)
        {
            parallelQuickSort(arr, pi + 1, high);
        }
    }
}

void sortSet(int arr[], int setSize)
{
    #pragma omp parallel
    {
        #pragma omp single
        {
            parallelQuickSort(arr, 0, setSize - 1);
        }
    }
}

int main(int argc, char** argv)
{
    int i;

    if (argc != 5)
    {
        printf("Need 4 command line arguments: The maximum set size; how many values to add initially; how many");
        printf(" to then remove; and whether or not to sort the set at the end [0/1].\n");
        return EXIT_FAILURE;
    }

    int
        maxSetSize = atoi(argv[1]),
        initSetSize = atoi(argv[2]),
        numToRemove = atoi(argv[3]),
        sortYesNo = atoi(argv[4]);

    if (maxSetSize <= 0 || initSetSize < 0 || numToRemove < 0 || sortYesNo < 0 || sortYesNo>1)
    {
        printf("Invalid arguments. Check that: the max. size > 0; the numbers to add/remove are non-negative;");
        printf(" and the option to sort is either 0 or 1.\n");
        return EXIT_FAILURE;
    }

    if (initSet(maxSetSize) == -1) return EXIT_FAILURE;

    srand(time(NULL));

    for (i = 0; i < initSetSize; i++)
        addToSet(rand() % maxSetSize);

    printf("Attempted to add %i random values. Current state of set:\n", initSetSize);
    printSet();

    for (i = 0; i < numToRemove; i++)
        removeFromSet(rand() % maxSetSize);

    printf("\nRemoved up to %i random values if present. Current state of set:\n", numToRemove);
    printSet();

    if (sortYesNo == 1)
    {
        sortSet(set, setSize);
        printf("\nCalled sortSet(). Current state of set:\n");
        printSet();
    }

    destroySet();

    return EXIT_SUCCESS;
}

//References
//
//https://github.com/Brajinder/Ds-Programs-final/blob/278a29826977e14fa15a4d3cbf2da89f11ac4e69/Data-Structures-master-final/Data-Structures-master/Array/9th/9th.cpp
//https://github.com/nicholasjn/NeuralNetwork/blob/5dfed93b8b821bb36c32dbfa7b9c294b1154c2d0/quickSort_n.c
//https://github.com/preksha1511/ds-lab/blob/86e3717c57e41c8ed7e08ba824fc1eb58773cafe/Experiment%209.cpp
//https://github.com/vle1054/parallelcomputing/blob/f0bb257ca8c2c31471dba7770e7d2f766e27e6b9/home/vle/parallelcomputing/project/pproject.cpp
//https://github.com/Rahil-17/Sorting-Algorithms-on-Parallel-computing/blob/6c74cd25589ccf6300cdbf50e2bb88655a716fcb/quick_seq.c
//
//This work was assisted by AI