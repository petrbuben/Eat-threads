//threads demo, one function serving threads
//with future promise, conditional variable/lock and atomic variable functionality demonstration
//by Petr Buben
//slightly modified, 2022

#include <iostream>
#include <thread>
#include <vector>
#include <future>
#include <chrono>
#include <mutex>
#include <condition_variable>

//using namespace std;

typedef struct Device{
		int c=0;
		int i=0;
	} S;
    
std::atomic<int> aisum;
int copyPromise = 0;
std::condition_variable cv;
std::mutex m;

void ft(Device& dev, std::promise<int>& pr, std::future<int>& fu) {
	std::this_thread::sleep_for(std::chrono::seconds(dev.i));

	if(dev.i==0){
		std::cout<<"thread id "<<dev.c<<" as supplier of future promise\n";
		std::cout<<"promise set as "<<dev.i+8<<std::endl;
		std::cout<<"copyPromise "<<copyPromise<<std::endl;
        pr.set_value(dev.i+8);//fut get is waiting for this, above first
        
        //cv
        std::lock_guard<std::mutex> lg(m);
        copyPromise = dev.i + 8;
        cv.notify_one(); //notifies cv.wait thread
	}
	else{
		if(dev.i==1){
            int f=fu.get(); //first wait, then cout
	    std::cout<<"\nthread id "<<dev.c<<" as receiver of future promise\n";
	    std::cout<<"receiver thread id "<<dev.c<<" received future promise as "<< f<<std::endl<<std::endl;   
              
        //cv
	    std::unique_lock<std::mutex> ul(m); //must be unique
        cv.wait(ul,[] {return (copyPromise !=0)? true : false;});
	std::cout<<"after cv.wait, copyPromise now is " << copyPromise<<std::endl<<std::endl;
	std::cout<<"data sum "<<aisum<<std::endl;
        }
   
    aisum+=dev.i;
    std::cout<<"receiver thread id "<<dev.c <<" data read "<<dev.i<<", data sum is "<< aisum<<std::endl; 
	}
} 

int main() {
	std::promise<int> prom;
	std::future<int> fut;
	fut=prom.get_future();
    
    int hw = std::thread::hardware_concurrency();
    std::cout<<"number of cores available "<<hw<<std::endl<<std::endl;
	
    	std::vector<Device> vdev;
	for(int d=0; d<hw; d++){
		vdev.push_back(Device {d,d});
	}
	
	std::vector <std::thread > vt;
	for(int i=0; i<hw; i++)
		vt.push_back (std::thread (ft,std::ref(vdev[i]), std::ref(prom), std::ref(fut)));
	
	for(std::thread &t:vt) 
	t.join() ;
    
	std::cout<<"\nmain thread total data sum "<<aisum<<std::endl;
	std::cout<<"number of data reads: " <<hw<<std::endl;
	return 0;
}
