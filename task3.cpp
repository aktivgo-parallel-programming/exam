#include <iostream>
#include <mpi.h>
#include <random>
#include <ctime>
#include <string>

using namespace std;

const int LEFT_RANDOM_BORDER = -10000, RIGHT_RANDOM_BORDER = 20000;

void fill_array_random(int* arr, int size, int left_border, int right_border);
void print_array(int* arr, int size);

int main(int argc, char** argv)
{
    srand(time(nullptr));
    int min;
    int proc_rank, proc_num = 4;
    int* sendbuff;
    int* send_counts;
    int* displs;
    int* recv_buff;
    int recv_count;

    int n = 1e8;
    sendbuff = new int[n];
    fill_array_random(sendbuff, n, LEFT_RANDOM_BORDER, RIGHT_RANDOM_BORDER);

    send_counts = new int[proc_num];
    displs = new int[proc_num];
    int elementsPerProcess = n / proc_num;
    for (int i = 0; i < proc_num - 1; ++i)
    {
        displs[i] = i * elementsPerProcess;
        send_counts[i] = elementsPerProcess;
    }
    displs[proc_num - 1] = (proc_num - 1) * elementsPerProcess;
    send_counts[proc_num - 1] = n - (proc_num - 1) * elementsPerProcess;

    double start, end, diff;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    if (proc_rank == 0) {
        start = MPI_Wtime();
    }

    recv_count = send_counts[proc_rank];
    recv_buff = new int[recv_count];
    MPI_Scatterv(sendbuff, send_counts, displs,
        MPI_INT, recv_buff, recv_count, MPI_INT, 0, MPI_COMM_WORLD);

    int local_min = INT_MAX;
    for (int i = 0; i < recv_count; ++i)
    {
        if (recv_buff[i] % 2 == 0 && to_string(recv_buff[i]).find('7') == -1 && recv_buff[i] < local_min)
        {
            local_min = recv_buff[i];
        }
    }

    MPI_Reduce(&local_min, &min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

    if (proc_rank == 0)
    {
        end = MPI_Wtime();
        diff = end - start;
        cout << "Execution time: " << diff << endl;
        /*cout << "array: ";
        print_array(sendbuff, n);
        cout << "displaces: ";
        print_array(displs, proc_num);
        cout << "send counts: ";
        print_array(send_counts, proc_num);*/
        if (min == INT_MAX)
        {
            cout << "Element not found\n";
        }
        else
        {
            cout << "Element : " << min << '\n';
        }
    }

    MPI_Finalize();
    return 0;
}

void fill_array_random(int* arr, int size, int left_border, int right_border)
{
    for (int i = 0; i < size; ++i)
    {
        arr[i] = rand() % (right_border - left_border) + left_border;
    }
}

void print_array(int* arr, int size) {
    for (int i = 0; i < size; ++i) {
        cout << arr[i] << " ";
    }
    cout << '\n';
}
