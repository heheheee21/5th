#include <curses.h>
#include <iostream>
#include <string>
#include <cmath>
#include <ctime>
#include <chrono>


using namespace std;
using namespace std::chrono;

using namespace std;


int check(WINDOW* win, int x, int y, const char* prompt, int limit_min, int limit_max)//проверка корректности ввода
{
    int num; //наше число
    bool is_valid = false; //флаг
    while (!is_valid) 
    {
        char* input_str = new char[256];//храним строку пользователя
        echo();
        wgetstr(win, input_str);//считываем строку
        noecho();
        try
        {
            num = stof(input_str);//из строки в число
            if (num >= limit_min && num <= limit_max) //Добавляем проверку на ограничения
            {
                is_valid = true;//все ок если в нашем диапозоне, выходим
            }
            else
            {
                throw invalid_argument(""); //если не в диапозоне или букавы, то мимо
            }
        }
        catch (const invalid_argument&) //Исправляем тип исключения на const invalid_argument&
        {
            mvwprintw(win, x, y, "                                      ");
            mvwprintw(win, x, y, prompt);
            wrefresh(win);
        }
    }
    return num;
}

int find_min_first_elem(int* A, int* B, int* C, int N, int p, int q)
// Функция находит минимальное значение элемента, среди трех массивов A, B, C,расположенных в диапазоне от p до q
{
    int min_val = INT_MAX;
    int min_arr = -1;

    //Перебираем все элементы в диапазоне от p до q включительно
    for (int i = 0; i < N; i++)
    {
        //Если индекс элемента находится в диапазоне от p до q, включительно
        if (i >= p && i <= q)
        {
            //Если текущий элемент массива A меньше минимального значения,
            //то запоминаем его как минимальное значение
            if (A[i] < min_val)
            {
                min_val = A[i];
                min_arr = 1;
            }
            //Если текущий элемент массива B меньше минимального значения,
            //то запоминаем его как минимальное значение
            if (B[i] < min_val)
            {
                min_val = B[i];
                min_arr = 2;
            }
            //Если текущий элемент массива C меньше минимального значения,
            //то запоминаем его как минимальное значение
            if (C[i] < min_val)
            {
                min_val = C[i];
                min_arr = 3;
            }
        }
    }
    return min_arr;
}

void fillArray(int* arr, int size, int minVal, int maxVal) 
{
    

    for (int i = 0; i < size; i++) 
    {
        arr[i] = rand() % (maxVal - minVal + 1) + minVal; //Заполнение случайным числом
    }
}

void printArray(WINDOW*win, int* arr, int size, int y, int x)//красивый вывод массива
{
    for (int i = 0; i < size; i++)
    {
        string element_stringA = to_string(arr[i]);
        int element_lengthA = element_stringA.length();

        int x_pos = 3 + i * 5 + x; //3 - отступ от края 5 - длина числа
        int y_pos = y;

        if (element_lengthA == 1)
            mvwprintw(win, y_pos, x_pos + 2, "%d", arr[i]);
        else if (element_lengthA == 2)
            mvwprintw(win, y_pos, x_pos + 1, "%2d", arr[i]);
        else if (element_lengthA == 3)
            mvwprintw(win, y_pos, x_pos, "%3d", arr[i]);
        else if (element_lengthA == 4)
            mvwprintw(win, y_pos, x_pos, "%4d", arr[i]);
    }
    wrefresh(win);
}


void insertionSort(int* arr, int n, WINDOW* win, int y ) //сортировка вставками
{
    auto start_rec = high_resolution_clock::now();
    int cmpCount = 0; //переменная для хранения количества сравнений
    int swapCount = 0; //переменная для хранения количества перестановок

    for (int i = 1; i < n; i++) //цикл по элементам массива, начиная с 1-го элемента
    {
        int key = arr[i]; //сохраняем значение текущего элемента
        int j = i - 1; //начинаем сравнение с элемента, который стоит перед текущим

        while (j >= 0 && arr[j] > key) //пока не дошли до начала массива и значение элемента больше текущего
        {
            cmpCount++; //увеличиваем счетчик сравнений
            arr[j + 1] = arr[j]; //сдвигаем элемент на одну позицию вправо
            swapCount++; //увеличиваем счетчик перестановок
            j--; //переходим к следующему элементу слева
        }

        cmpCount++; //увеличиваем счетчик сравнений
        arr[j + 1] = key; //вставляем текущий элемент на нужное место
        swapCount++; //увеличиваем счетчик перестановок
    }
    auto end_rec = high_resolution_clock::now();
    duration<float> duration_rec = end_rec - start_rec;
    
    mvwprintw(win, y, 1, "Insertion Sort:");
    printArray(win, arr, n, y+1, 0);
    mvwprintw(win, y + 2, 1, "Comparison count: %d", cmpCount);
    mvwprintw(win, y + 3, 1, "Swap count: %d", swapCount);
    mvwprintw(win, y + 4, 1, "Time (ms): %f", duration_rec.count());
    wrefresh(win);
}

void selectionSort(int* arr, int n, WINDOW* win, int y)//сорировка выбором
{
    auto start_rec = high_resolution_clock::now();

    int cmpCount = 0; //переменная для подсчета количества сравнений
    int swapCount = 0; //переменная для подсчета количества перестановок

    for (int i = 0; i < n - 1; i++) //внешний цикл от 0 до n-2
    {
        int minIndex = i; //индекс текущего минимального элемента равен i
        for (int j = i + 1; j < n; j++) //внутренний цикл от i+1 до n-1
        {
            cmpCount++; //увеличиваем счетчик сравнений
            if (arr[j] < arr[minIndex]) //если текущий элемент меньше текущего минимального элемента
                minIndex = j; //обновляем индекс текущего минимального элемента
        }

        if (minIndex != i) //если текущий минимальный элемент находится не на своем месте
        {
            swapCount++; //увеличиваем счетчик перестановок
            int temp = arr[i]; //сохраняем текущий элемент во временной переменной
            arr[i] = arr[minIndex]; //перемещаем минимальный элемент на место текущего элемента
            arr[minIndex] = temp; //возвращаем текущий элемент на место минимального элемента
        }
    }
    auto end_rec = high_resolution_clock::now();
    duration<float> duration_rec = end_rec - start_rec;
    mvwprintw(win, y, 30, "Selection Sort:");
    printArray(win, arr, n, y + 1, 30);
    mvwprintw(win, y + 2, 30, "Comparison count: %d", cmpCount);
    mvwprintw(win, y + 3, 30, "Swap count: %d", swapCount);
    mvwprintw(win, y + 4, 30, "Time (ms): %f", duration_rec.count());
    wrefresh(win);
}

int findMaxIndexInRange(int* A, int N, int a, int b) 
{
    int maxIndex = -1; //индекс максимального элемента
    bool foundPositive = false; //найден ли первый положительный элемент

    //ищем первый положительный элемент и запоминаем его индекс
    int positiveIndex = 0;
    while (!foundPositive && positiveIndex < N) 
    {
        if (A[positiveIndex] > 0)
            foundPositive = true;
        else
            positiveIndex++;
    }

    //если не нашли положительный элемент, возвращаем -1
    if (!foundPositive) 
        return -1;
    else
    {
        // ищем максимальный элемент в заданном диапазоне,
        // начиная с индекса первого положительного элемента
        for (int i = positiveIndex + 1; i < N; i++)
        {
            if (A[i] >= a && A[i] <= b && (maxIndex == -1 || A[i] > A[maxIndex]))
                maxIndex = i;
        }
        // возвращаем индекс максимального элемента
        return maxIndex;
    }
}

int main()
{
    initscr();
    srand(time(NULL));
    //начало curses
    noecho();
    //не отоброжаем ввод пользователя
    WINDOW* menuwin = newwin(5, 20, 2, 40);
    //создаем окно для меню
    box(menuwin, 0, 0);
    //выводим рамки окон
    mvwprintw(menuwin, 0, 1, "Choose:");
    //заголовок окон
    start_color();
    //делаем окна цветными
    /*
        COLOR_BLACK
        COLOR_RED
        COLOR_GREEN
        COLOR_YELLOW
        COLOR_BLUE
        COLOR_MAGENTA
        COLOR_CYAN
        COLOR_WHITE
        */
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    //создаем окно для ввода данных
    WINDOW* inputwin = newwin(13, 40, 7, 10);
    //создаем окно для вывода данных
    WINDOW* outputwin = newwin(23, 68, 7, 50);
    box(inputwin, 0, 0);
    box(outputwin, 0, 0);
    mvwprintw(inputwin, 0, 1, "Input:");
    mvwprintw(outputwin, 0, 1, "Output:");
    wbkgd(inputwin, COLOR_PAIR(1));
    wbkgd(outputwin, COLOR_PAIR(2));
    //делаем данные видимыми
    wrefresh(inputwin);
    wrefresh(outputwin);
    wrefresh(menuwin);
    //начало менюшки
    keypad(menuwin, true);
    //управление стрелочками
    curs_set(0);
    //убираем курсор
    string choices[3] = { "First task", "Second task", "Exit" };
    //выбор в менюшке
    int choice;
    int highlight = 0;
    bool work = 1;
    //переменные для работы с меню
    while (work)
    {
        for (int i = 0; i < 3; i++) 
        {
            if (i == highlight)
                wattron(menuwin, A_REVERSE);
            mvwprintw(menuwin, i + 1, 1, choices[i].c_str());
            wattroff(menuwin, A_REVERSE);
        }
        choice = wgetch(menuwin);
        switch (choice) {
        case KEY_UP:
            highlight--;
            if (highlight == -1)
                highlight = 0;
            break;
        case KEY_DOWN:
            highlight++;
            if (highlight == 3)
                highlight = 2;
            break;
        default:
            break;
        }
        if (choice == 10 && highlight == 0)
        {
            echo();
            curs_set(1);
            wclear(outputwin);
            box(outputwin, 0, 0);
            mvwprintw(outputwin, 0, 1, "Output:");
            wclear(inputwin);
            box(inputwin, 0, 0);
            mvwprintw(inputwin, 0, 1, "Input:");
            wrefresh(outputwin);
            mvwprintw(inputwin, 1, 1, "Enter N: ");
            wrefresh(inputwin);
            int N = check(inputwin, 1, 1, "Invalid N, try again: ", 2, 7);

            wclear(inputwin);
            box(inputwin, 0, 0);
            mvwprintw(inputwin, 0, 1, "Input:");
            mvwprintw(inputwin, 1, 1, "Enter N: %d", N);

            wrefresh(inputwin);


            int* A = new int[N];
            int* B = new int[N];
            int* C = new int[N];
            mvwprintw(inputwin, 2, 1, "Do u want to fill array");
            mvwprintw(inputwin, 3, 1, "by yourself? ");
            wrefresh(inputwin);
            string choicess[2] = { "Yes", "No"};
            int cchoice;
            int hiighlight = 0;
            bool woork = 1;
            WINDOW* menuUwin = newwin(4, 5, 10, 25);
            box(menuUwin, 0, 0);
            wrefresh(menuUwin);
            keypad(menuUwin, true);
            //управление стрелочками
            curs_set(0);
            //переменные для работы с меню
            while (woork)
            {
                for (int i = 0; i < 2; i++)
                {
                    if (i == hiighlight)
                        wattron(menuUwin, A_REVERSE);
                    mvwprintw(menuUwin, i + 1, 1, choicess[i].c_str());
                    wattroff(menuUwin, A_REVERSE);
                }
                cchoice = wgetch(menuUwin);
                switch (cchoice)
                {
                case KEY_UP:
                    hiighlight--;
                    if (hiighlight == -1)
                        hiighlight = 0;
                    break;
                case KEY_DOWN:
                    hiighlight++;
                    if (hiighlight == 2)
                        hiighlight = 1;
                    break;
                default:
                    break;
                }
                if (cchoice == 10 && hiighlight == 0)
                {
                    wclear(inputwin);
                    box(inputwin, 0, 0);
                    mvwprintw(inputwin, 0, 1, "Input:");
                    curs_set(1);
                    for (int i = 0; i < N; i++)
                    {
                        wclear(inputwin);
                        box(inputwin, 0, 0);
                        mvwprintw(inputwin, 0, 1, "Input:");
                        mvwprintw(inputwin, 1, 1, "Enter data for A array: ");
                        A[i] = check(inputwin, 1, 1, "Invalid array member, try again: ", INT_MIN, INT_MAX);
                    }
                    for (int i = 0; i < N; i++)
                    {
                        wclear(inputwin);
                        box(inputwin, 0, 0);
                        mvwprintw(inputwin, 0, 1, "Input:");
                        mvwprintw(inputwin, 1, 1, "Enter data for A array: %d", A[N - 1]);
                        mvwprintw(inputwin, 3, 1, "Enter data for B array: ");
                        B[i] = check(inputwin, 3, 1, "Invalid array member, try again: ", INT_MIN, INT_MAX);
                    }
                    for (int i = 0; i < N; i++)
                    {
                        wclear(inputwin);
                        box(inputwin, 0, 0);
                        mvwprintw(inputwin, 0, 1, "Input:");
                        mvwprintw(inputwin, 1, 1, "Enter data for A array: %d", A[N - 1]);
                        mvwprintw(inputwin, 3, 1, "Enter data for B array: %d", B[N - 1]);
                        mvwprintw(inputwin, 5, 1, "Enter data for C array: ");
                        C[i] = check(inputwin, 5, 1, "Invalid array member, try again: ", INT_MIN, INT_MAX);
                    }
                    woork = 0;
                    break;
                }
                if (cchoice == 10 && hiighlight == 1)
                {
                    fillArray(A, N, 1, 10000);
                    fillArray(B, N, 1, 10000);
                    fillArray(C, N, 1, 10000);
                    woork = 0;
                    break;
                }
                
            }
            mvwprintw(outputwin, 1, 1, "A:");
            mvwprintw(outputwin, 2, 1, "B:");
            mvwprintw(outputwin, 3, 1, "C:");
            printArray(outputwin, A, N, 1, 0);
            printArray(outputwin, B, N, 2, 0);
            printArray(outputwin, C, N, 3, 0);
           
            wrefresh(outputwin);
            wclear(inputwin);
            box(inputwin, 0, 0);
            mvwprintw(inputwin, 0, 1, "Input:");
            curs_set(1);
            mvwprintw(inputwin, 1, 1, "Enter P: ");
            wrefresh(inputwin);
            int p = check(inputwin, 1, 1, "Invalid P, try again: ", 0, N - 1);
            wclear(inputwin);
            box(inputwin, 0, 0);
            mvwprintw(inputwin, 0, 1, "Input:");
            mvwprintw(inputwin, 1, 1, "Enter P: %d", p);

            mvwprintw(inputwin, 2, 1, "Enter Q: ");
            wrefresh(inputwin);
            int q = check(inputwin, 2, 1, "Invalid Q, try again: ", 0, N - 1);
            wclear(inputwin);
            box(inputwin, 0, 0);
            mvwprintw(inputwin, 0, 1, "Input:");
            mvwprintw(inputwin, 1, 1, "Enter P: %d", p);
            mvwprintw(inputwin, 2, 1, "Enter Q: %d", q);

            int answer = find_min_first_elem(A, B, C, N, p, q);
            insertionSort(A, N, outputwin, 5);
            selectionSort(A, N, outputwin, 5);

            insertionSort(B, N, outputwin, 10);
            selectionSort(B, N, outputwin, 10);

            insertionSort(C, N, outputwin, 15);
            selectionSort(C, N, outputwin, 15);

            if(answer == 1)
                mvwprintw(outputwin, 21, 1, "Answer: Array A");
            if (answer == 2)
                mvwprintw(outputwin, 21, 1, "Answer: Array B");
            if (answer == 3)
                mvwprintw(outputwin, 21, 1, "Answer: Array C");

            wrefresh(inputwin);
            wrefresh(outputwin);
            curs_set(0);
            delete[] A;
            delete[] B;
            delete[] C;
        }
        if (choice == 10 && highlight == 1)
        {
            //очищаем окно от прошлых данных
            wclear(outputwin);
            box(outputwin, 0, 0);
            mvwprintw(outputwin, 0, 1, "Output:");
            wclear(inputwin);
            box(inputwin, 0, 0);
            mvwprintw(inputwin, 0, 1, "Input:");
            mvwprintw(inputwin, 1, 1, "Enter N: ");
            wrefresh(inputwin);
            wrefresh(outputwin);
            int N = check(inputwin, 1, 1, "Invalid N, try again: ", 2, 7);

            wclear(inputwin);
            box(inputwin, 0, 0);
            mvwprintw(inputwin, 0, 1, "Input:");
            mvwprintw(inputwin, 1, 1, "Enter N: %d", N);

            wrefresh(inputwin);
            int* A = new int[N];
            mvwprintw(inputwin, 2, 1, "Do u want to fill array");
            mvwprintw(inputwin, 3, 1, "by yourself? ");
            wrefresh(inputwin);
            string choicess[2] = { "Yes", "No" };
            int cchoice;
            int hiighlight = 0;
            bool woork = 1;
            WINDOW* menuUwin = newwin(4, 5, 10, 25);
            box(menuUwin, 0, 0);
            wrefresh(menuUwin);
            keypad(menuUwin, true);
            //управление стрелочками
            curs_set(0);
            //переменные для работы с меню
            while (woork)
            {
                for (int i = 0; i < 2; i++)
                {
                    if (i == hiighlight)
                        wattron(menuUwin, A_REVERSE);
                    mvwprintw(menuUwin, i + 1, 1, choicess[i].c_str());
                    wattroff(menuUwin, A_REVERSE);
                }
                cchoice = wgetch(menuUwin);
                switch (cchoice)
                {
                case KEY_UP:
                    hiighlight--;
                    if (hiighlight == -1)
                        hiighlight = 0;
                    break;
                case KEY_DOWN:
                    hiighlight++;
                    if (hiighlight == 2)
                        hiighlight = 1;
                    break;
                default:
                    break;
                }
                if (cchoice == 10 && hiighlight == 0)
                {
                    curs_set(1);
                    for (int i = 0; i < N; i++)
                    {
                        mvwprintw(inputwin, 7 + i, 1, "Enter data for array: ");
                        A[i] = check(inputwin, 7 + i, 1, "Invalid array member, try again: ", INT_MIN, INT_MAX);
                    }
                    woork = 0;
                    break;
                }
                if (cchoice == 10 && hiighlight == 1)
                {
                    fillArray(A, N, 1, 10000);
                    woork = 0;
                    break;
                }

            }
            mvwprintw(outputwin, 1, 1, "A:");
            printArray(outputwin, A, N, 1, 0);

            wclear(inputwin);
            box(inputwin, 0, 0);
            mvwprintw(inputwin, 0, 1, "Input:");
            mvwprintw(inputwin, 1, 1, "Enter A: ");
            wrefresh(inputwin);
            int a = check(inputwin, 1, 1, "Invalid A, try again: ", INT_MIN, INT_MAX);
            wclear(inputwin);
            box(inputwin, 0, 0);
            mvwprintw(inputwin, 0, 1, "Input:");
            mvwprintw(inputwin, 1, 1, "Enter A: %d", a);

            mvwprintw(inputwin, 2, 1, "Enter B: ");
            wrefresh(inputwin);
            int B = check(inputwin, 2, 1, "Invalid B, try again: ", INT_MIN, INT_MAX);
            wclear(inputwin);
            box(inputwin, 0, 0);
            mvwprintw(inputwin, 0, 1, "Input:");
            mvwprintw(inputwin, 1, 1, "Enter A: %d", a);
            mvwprintw(inputwin, 2, 1, "Enter B: %d", B);


            int maxIndex = findMaxIndexInRange(A, N, a, B);

            if (maxIndex != -1)
                mvwprintw(outputwin, 10, 1, "Max index: %d", maxIndex);
            else
                mvwprintw(outputwin, 10, 1, "Max index did not found");

            insertionSort(A, N, outputwin, 3);
            selectionSort(A, N, outputwin, 3);

            
                
            wrefresh(inputwin);
            wrefresh(outputwin);
        }
        if (choice == 10 && highlight == 2)
        {
            wclear(outputwin);
            box(outputwin, 0, 0);
            mvwprintw(outputwin, 0, 1, "Output:");
            mvwprintw(outputwin, 1, 1, "Have a good day!");
            wrefresh(outputwin);
            work = 0;
            break;
        }
    }
    wgetch(outputwin);
    endwin();
    return 0;
}