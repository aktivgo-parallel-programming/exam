#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <vector>

using namespace std;

vector<int> read_file(string file_name);
void get_part_sum(vector<int>& vec, vector<int>& sums, int index, int length);
void print_vector(vector<int>& vec);
int min(int a, int b);

int main()
{
    string file_name;
    cout << "Enter file name: ";
    //указать абсолютный путь к файлу
    cin >> file_name;
    vector<int> numbers = read_file(file_name);

    int p;
    cout << "Enter number of threads: ";
    cin >> p;

    thread* threads = new thread[p];
    int elements_per_thread = ceil((double)numbers.size() / p);
    vector<int> sums(numbers.size());
    for (int i = 0; i < p; ++i)
    {
        threads[i] = thread(get_part_sum, ref(numbers), ref(sums), i, elements_per_thread);
    }
    for (int i = 0; i < p; ++i)
    {
        threads[i].join();
    }

    cout << "Sums: ";
    print_vector(sums);

    delete[] threads;
    return 0;
}

void get_part_sum(vector<int>& vec, vector<int>& sums, int index, int length)
{
    int vec_size = vec.size();
    int size = min(index * length + length, vec_size);
    for (int i = index * length; i < size; ++i)
    {
        for (int j = 0; j < vec_size; ++j)
        {
            if (vec[j] <= vec[i])
            {
                sums[i] += vec[j];
            }
        }
    }
}

vector<int> read_file(string file_name)
{
    vector<int> file_numbers;
    ifstream instream(file_name);

    int number;
    while (instream >> number)
    {
        file_numbers.push_back(number);
    }
    instream.close();

    return file_numbers;
}

int min(int a, int b)
{
    return a < b ? a : b;
}

void print_vector(vector<int>& vec)
{
    for (vector<int>::iterator it = vec.begin(); it < vec.end(); ++it)
    {
        cout << *it << " ";
    }
    cout << '\n';
}
