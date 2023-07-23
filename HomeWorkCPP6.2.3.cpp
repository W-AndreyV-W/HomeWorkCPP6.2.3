#include <iostream>
#include <thread>
#include <mutex>
#include <random>
#include <algorithm>
#include <vector>
#include <windows.h>

void print(std::vector<int>& num) {

    for (const auto n : num) {
        std::cout << n << "\t";
    }

    std::cout << std::endl;
}

class Data {

public:

    Data(int size) {

        vector_random(num_1, size);
        std::cout << "Вектор 1: ";
        print(num_1);

        vector_random(num_2, size);
        std::cout << "Вектор 2: ";
        print(num_2);
    }

    void vector_random(std::vector<int>& num, int size_vector) {

        std::random_device rv;
        std::mt19937 gen(rv());
        std::uniform_int_distribution<> dist(0, 100);

        for (int i = 0; i < size_vector; i++) {

            num.push_back(dist(gen));
        }
    }

    std::mutex mix_1;
    std::mutex mix_2;

    std::vector<int> num_1;
    std::vector<int> num_2;

private:

};

void swap_lock(Data& data) {

    std::lock(data.mix_1, data.mix_2);

    std::swap(data.num_1, data.num_2);

    std::cout << "\n" << "lock: \n";
    std::cout << "Вектор 1: ";
    print(data.num_1);

    data.mix_1.unlock();

    std::cout << "Вектор 2: ";
    print(data.num_2);

    data.mix_2.unlock();
}

void swap_scoped(Data& data) {

    std::scoped_lock lock(data.mix_1, data.mix_2);

    std::swap(data.num_1, data.num_2);

    std::cout << "\n" << "scoped_lock: \n";
    std::cout << "Вектор 1: ";
    print(data.num_1);
    std::cout << "Вектор 2: ";
    print(data.num_2);
}

void swap_unique(Data& data) {

    std::unique_lock lock_1(data.mix_1);
    std::unique_lock lock_2(data.mix_2);

    std::swap(data.num_1, data.num_2);

    std::cout << "\n" << "unique_lock: \n";
    std::cout << "Вектор 1: ";
    print(data.num_1);
    std::cout << "Вектор 2: ";
    print(data.num_2);
}


int main() {

    setlocale(LC_ALL, "Russian");

    Data data(10);

    std::thread flow_1(swap_lock, std::ref(data));
    std::thread flow_2(swap_scoped, std::ref(data));
    std::thread flow_3(swap_unique, std::ref(data));

    flow_1.join();
    flow_2.join();
    flow_3.join();
}