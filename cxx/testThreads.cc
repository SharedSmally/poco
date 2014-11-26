#include <thread>
#include <iostream>
#include <vector>

void hello(){
    std::cout << "Hello from thread " << std::this_thread::get_id() << std::endl;
}

static const int num_threads = 2;

void testThreadArray() {
    std::cout << "Launch threads[]"<<std::endl;

    std::thread t[num_threads];

     //Launch a group of threads
     for (int i = 0; i < num_threads; ++i) {
         t[i] = std::thread(hello);
     }

     //Join the threads with the main thread
     for (int i = 0; i < num_threads; ++i) {
         t[i].join();
     }
}

void testThreadVector() {
    std::cout << "\nLaunch vector<thread>"<<std::endl;
    std::vector<std::thread> threads;

    for(int i = 0; i < num_threads; ++i){
        threads.push_back(std::thread(hello));
    }

    for(auto& thread : threads){
        thread.join();
    }

}
int main(){
	testThreadArray();
	testThreadVector();
    return 0;
}


