#include <iostream>
#include <thread>
#include <mutex>

//nebudeme v tomto pripade pouzivat condition variable pro synchronizaci threads

extern int g_soucet;
std::mutex m;

int monitor(char ** result, int pi){
    *result = new char[256];
    int in;
    
     std::cout<<"device enter 1 digit number, thread " << pi <<std::endl;
     
     if (!(std::cin))  std::cin.clear(); // cin fix
     std::cin>>in;
     
     if (m.try_lock()){
     g_soucet += in;  //critical code
     m.unlock();
     }
     
     snprintf(*result, 256, "Thread id:%d  data in:%d  soucet %d  function number %d", std::this_thread::get_id(), in, g_soucet, pi);
     
     return pi;
}      