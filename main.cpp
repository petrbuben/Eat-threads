//Eaton simple demo
//Petr Buben
//monitor smyčka, Github, makefile demo

//verze 2 - threads

#include <iostream>
#include <vector>
#include <thread>
#include "monitor.hpp"

typedef void (*t_fnp)(int);
//void fu(){};

struct threadItem{
    std::thread worker;
    char * result;
};

int g_soucet = 0;

int main() {
    
    //použijeme tolik vláken jaká je kapacita systému
    int n = std::thread::hardware_concurrency();
	printf("hardware %d cores\n", n);
    
    //** practice - array of n function pointers
    t_fnp funcs[n];
    void (*funcs2[n])(int);
    funcs[0]=funcs[0];funcs2[0]=funcs2[0]; //get rid of warnings
    //** practice end
    
    //vector of functions pointers, practice, neni zde nutny
    std::vector<int(*)(char**, int)> vecfuncs(n);
    for (unsigned i=0; i < vecfuncs.size(); i++){
        vecfuncs[i] = monitor;
    }
    
    //vector of thread items
    std::vector<threadItem> thread_vec(n);
    
    //spawn a thread for each core
	for( int i=0; i < n; i++ ){
		thread_vec[i].worker = std::thread(vecfuncs[i], &thread_vec[i].result, i);
        std::this_thread::sleep_for(std::chrono::seconds(1));
	} 
    
	//wait for threads to finish
    for (int i = 0; i < n; i++){
        thread_vec[i].worker.join();
    }
	//std::cout << "\nCelkove provedeno " << c-1 << " mereni \n";
    
    //print results
    printf("\nresults\n");
    for (int i = 0; i < n; i++){
        printf("%s\n", thread_vec[i].result);
    }

	return 0;
}


