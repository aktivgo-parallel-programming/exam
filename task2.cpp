#include <iostream>
#include <omp.h>
#include <random>
#include <ctime>
#include <chrono>

using namespace std;

const int LEFT_RANDOM_BORDER = -10000000, RIGHT_RANDOM_BORDER = 10000000;

void fill_array_random(int* arr, int size, int left_border, int right_border);
long long sum_negative_parallel(int* arr, int size, int p);
long long sum_negative(int* arr, int size);

int main()
{
    int n;
    cout << "Enter size of array: ";
    cin >> n;
    int* arr = new int[n];

    char user_choice;
    cout << "Generate array by random? (y/n) ";
    cin >> user_choice;

    if (user_choice == 'y')
    {
        srand(time(nullptr));
        fill_array_random(arr, n, LEFT_RANDOM_BORDER, RIGHT_RANDOM_BORDER);
    }
    else
    {
        for (int i = 0; i < n; ++i)
        {
            cout << "Enter " << i + 1 << " element: ";
            cin >> arr[i];
        }
    }

    int p;
    cout << "Enter number of threads: ";
    cin >> p;

    long sum;
    double start, end;

    start = omp_get_wtime();
    sum = sum_negative_parallel(arr, n, p);
    end = omp_get_wtime();
    cout << "\nParallel algorihm:\n";
    cout << "\tSum: " << sum;
    cout << "\n\tExecution time: " << end - start << '\n';

    start = omp_get_wtime();
    sum = sum_negative(arr, n);
    end = omp_get_wtime();
    cout << "\nSequential algorihm:\n";
    cout << "\tSum: " << sum;
    cout << "\n\tExecution time: " << end - start << '\n';

    return 0;
}

long long sum_negative(int* arr, int size)
{
    long long sum = 0;

    for (int i = 0; i < size; ++i)
    {
        if (arr[i] < 0)
        {
            sum += arr[i];
        }
    }

    return sum;
}

long long sum_negative_parallel(int* arr, int size, int p)
{
    int i;
    long long sum = 0;

    #pragma omp parallel num_threads(p) shared(i) reduction(+: sum)
    {
        #pragma omp for schedule(static)
        for (i = 0; i < size; ++i) //итерации независимы
        {
            if (arr[i] < 0)
                sum += arr[i];
        }
    }

    return sum;
}

void fill_array_random(int* arr, int size, int left_border, int right_border)
{
    for (int i = 0; i < size; ++i)
    {
        arr[i] = rand() % (right_border - left_border) + left_border;
    }
}
