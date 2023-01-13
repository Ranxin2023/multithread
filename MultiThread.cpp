// MultiThread.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include<condition_variable>
#include <iostream>
#include<mutex>
//#include<semaphore>
#include<thread>

#define NUM_PER_LINE 12
using namespace std;
int num;
bool once_appeared;
bool twice_appeared;
condition_variable cv1, cv2, cv3;
mutex m;
int sem;
int line_count = 0;
void print_num() {
    //lock(lock2, lock1);
    //cout << "enter thread2" << endl;
    unique_lock<mutex> lock(m);
    while (!twice_appeared) {
        //cout << "enter thread2 loop" << endl;
        if (sem != 1) {
            cv2.wait(lock);
        }
        if (!twice_appeared) {
            
            if (line_count == NUM_PER_LINE) {
                cout << endl;
                line_count -= NUM_PER_LINE;
            }
            cout << num << "\t";
            line_count++;
        }
        
        sem = 2;
        //if (!twice_appeared)
        cv3.notify_one();
        
    }
    
}
class Judge {
public:
    void operator()() {
        //cout << "enter thread3" << endl;
        unique_lock<mutex> lock(m);
        while (!twice_appeared) {
            //cout << "enter thread3 loop" << endl;
            if (sem != 2) {
                cv3.wait(lock);
            }
            if (num == 1) {
                if (!once_appeared) {
                    once_appeared = true;
                }
                else {
                    twice_appeared = true;
                }
            }
            sem = 0;      
            cv1.notify_one();
            
        }
        //cout << "end3" << endl;
    }
};
    
int main()
{
    auto calculate = []() {
        //cout << "enter thread1" << endl;
        unique_lock<mutex> lock(m);
        while (!twice_appeared) {
            //cout << "enter thread1 loop" << endl;
            if (sem != 0) {
                cv1.wait(lock);
            }
            if (num % 2 == 0) num /= 2;
            else  num = num * 3 + 1;
            sem = 1;
            
            cv2.notify_one();
            
        }
    };
    
    string option;
    cout << "++++++++++++++++++++++Welcome to 3x plus 1 game!*********************************" << endl;
    while (1) {
        cout << "**********Select the option you want*************" << endl;
        cout << "----------Option [0]: Start--------------" << endl;
        cout << "----------Option [1]: Exit---------------" << endl;
        cin >> option;
        if (option == "exit" ||option=="1") {
            break;
        }
        else if(option == "start" || option == "0") {
            once_appeared =false;
            twice_appeared = false;
            sem = 0;
            line_count = 0;
            while (1) {
                cout << "Enter the number of x that is positive: " << endl;
                cin >> num;
                if (num > 0) {
                    break;
                }
                else {
                    cerr << "The number is not positive. Please try again!" << endl;
                }
            }
            cout << "The number after calculated is:" << endl;
            
            thread * th1=new thread(calculate);
            thread * th2=new thread(print_num);
            thread * th3=new thread(Judge());
            
            
            
            th1->join();
            th2->join();
            th3->join();
            cout << "\n************End One Pass! Success!*************" << endl;
            cout << string(50, '_') << endl;
            sem = 0;
            delete th1;
            delete th2;
            delete th3;
            
        }
        else {
            cerr << "-----Illegal input. Please enter again!------" << endl;
        }
    }
    
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
