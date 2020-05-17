//threads demo, one function serving threads
//with future promise, conditional variable/lock and atomic variable
//by Petr Buben

#include <iostream>
#include <thread>
#include <vector>
#include <future>
#include <chrono>
#include <mutex>
#include <condition_variable>

using namespace std;

typedef struct Device{
		int c=0;
		int i=0;
	} S;
    
atomic<int> aisum;
int copyPromise = 0;
condition_variable cv;
mutex m;

void ft(Device& dev, promise<int>& pr, future<int>& fu) {
    this_thread::sleep_for(chrono::seconds(dev.i));

	if(dev.i==0){
		cout<<"thread id "<<dev.c<<" as supplier of future promise\n";
		cout<<"promise set as "<<dev.i+8<<endl;
        cout<<"copyPromise "<<copyPromise<<endl;
        pr.set_value(dev.i+8);//fut get is waiting for this, above first
        
        //cv
        std::lock_guard<mutex> lg(m);
        copyPromise = dev.i + 8;
        cv.notify_one(); //notifies cv.wait thread
	}
	else{
		if(dev.i==1){
            int f=fu.get(); //first wait, then cout
        cout<<"\nthread id "<<dev.c<<" as receiver of future promise\n";
		cout<<"receiver thread id "<<dev.c<<" received future promise as "<< f<<endl<<endl;   
              
        //cv
        unique_lock<mutex> ul(m); //must be unique
        cv.wait(ul,[] {return (copyPromise !=0)? true : false;});
        cout<<"after cv.wait, copyPromise now is " << copyPromise<<endl<<endl;
        cout<<"data sum "<<aisum<<endl;
        }
   
    aisum+=dev.i;
    cout<<"receiver thread id "<<dev.c <<" data read "<<dev.i<<", sum is "<< aisum<<endl; 
	}
} 

int main() {
	promise<int> prom;
	future<int> fut;
	fut=prom.get_future();
    
    int hw = thread::hardware_concurrency();
    cout<<"number of cores available "<<hw<<endl<<endl;
	
	vector<Device> vdev;
	for(int d=0;d<hw;d++){
		vdev.push_back(Device {d,d});
	}
	
	vector <thread > vt;
	for(int i=0;i<hw;i++)
		vt.push_back (thread (ft,ref(vdev[i]), ref(prom), ref(fut)));
	
	for(thread &t:vt) 
	t. join() ;
    
    cout<<"\nmain thread total sum "<<aisum<<endl<<endl;

	return 0;
}