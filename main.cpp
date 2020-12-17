
#include <Windows.h>
#include <iostream>
#include <time.h>
#include <cstring>

using namespace std;
int const arr_size = 10;
volatile LONG arr[arr_size];
int Min = arr[0];
HANDLE hSemaphore;


void Thread_1(){

    srand(time(NULL));
    rand();
    for (int i = 0; i < arr_size; i++) {
        arr[i] = rand() * 500 / (RAND_MAX) +0;
        ReleaseSemaphore(hSemaphore, 1, NULL);
    }
    cout << "Origin array " << endl;
    for (int i = 0; i < arr_size; i++) {
        cout << arr[i] << " ";
    }
    cout<<endl;
    Sleep(500);
}


void Thread_2(){
        int min = arr[0];
        for (int i = 0; i < arr_size; i++)
        {
            WaitForSingleObject(hSemaphore, INFINITE);
            if (arr[i] < min)
                min = arr[i];
            Sleep(500);
        }

        cout << "min value = " ;
        cout << min << endl;

    }
int main()
{
    HANDLE hThread1, hThread2;
    DWORD IDThread1, IDThread2;

    hSemaphore = CreateSemaphore(NULL, 0, arr_size, NULL);
    if (hSemaphore == NULL)
    {
        int iErrorCode = GetLastError();
        cerr << "Error while semaphore creating. Error code: " << iErrorCode << endl;
        return iErrorCode;
    }

    if ((hThread1 = CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)Thread_1, NULL, 0, &IDThread1))==NULL){
        int error = GetLastError();
        cout<<"error create Thread 1 "<<error<<endl;
        return error;
    }
    if ((hThread2 = CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)Thread_2, NULL, 0, &IDThread2))==NULL) {
        int error = GetLastError();
        cout << "error create Thread 2 " << error << endl;
        CloseHandle(hThread1);
        return error;
    }

    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);
    CloseHandle(hSemaphore);
    CloseHandle(hThread1);
    CloseHandle(hThread2);

    return 0;
}