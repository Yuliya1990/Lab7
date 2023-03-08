#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

int f(int x)
{
    // Імітуємо обчислення f(x)
    this_thread::sleep_for(chrono::milliseconds(2000));
    return x % 2 == 0 ? 0 : 1;
}

int g(int x)
{
    // Імітуємо обчислення g(x)
    while (true)
    {
        x++;
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
}

int main()
{
    int x;
    cout << "Enter x: ";
    cin >> x;

    int result_f = 0;
    int result_g = 0;
    bool is_f_finished = false;
    bool is_g_finished = false;
    bool continue_without_asking = false;

    thread f_thread([&] {
        auto start = chrono::high_resolution_clock::now();
        result_f = f(x);
        auto stop = chrono::high_resolution_clock::now();
        is_f_finished = true;
        cout << "f(x) done for" << chrono::duration_cast<chrono::milliseconds>(stop - start).count() << " mc" << endl;
        });

    thread g_thread([&] {
        auto start = chrono::high_resolution_clock::now();
        result_g = g(x);
        auto stop = chrono::high_resolution_clock::now();
        is_g_finished = true;
        cout << "g(x) done for" << chrono::duration_cast<chrono::milliseconds>(stop - start).count() << " mc" << endl;
        });

    // Чекаємо, доки обидва потоки завершаться
    while (!is_f_finished || !is_g_finished && !continue_without_asking)
    {
        this_thread::sleep_for(chrono::milliseconds(10000)); // Чекаємо 10 секунд
        if (!is_f_finished && f_thread.joinable())
        {
            cout << "What do you want to do? 1) Continue 2) Stop 3) Continue without asking again" << endl;
            int choice;
            cin >> choice;
            if (choice == 2) {
                f_thread.detach(); break;
            }
            else if (choice == 3) continue_without_asking = true;
        }
        if (!is_g_finished && g_thread.joinable())
        {
            cout << "What do you want to do? 1) Continue 2) Stop 3) Continue without asking again" << endl;
            int choice;
            cin >> choice;
            if (choice == 2) { g_thread.detach(); break; }
            else if (choice == 1) continue;
            else if (choice == 3) continue_without_asking = true;
        }
    }
    // Обчислюємо f(x) || g(x)
    bool result = result_f || result_g;

    cout << "Result: " << result << endl;

    // Очікуємо завершення обох потоків
    if (f_thread.joinable()) f_thread.join();
    if (g_thread.joinable()) g_thread.join();

    return 0;

}
