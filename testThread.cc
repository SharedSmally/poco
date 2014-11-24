//https://www.justsoftwaresolutions.co.uk/threading/multithreading-in-c++0x-part-2-function-objects-and-arguments.html

#include <thread>
#include <iostream>
#include <string>

void print()
{
    std::cout<<" print thread" << std::endl;
}

void write_sum(int x,int y)
{
    std::cout<<x<<" + "<<y<<" = "<<(x+y)<<std::endl;
}


class Greeting
{
    std::string message;
public:
    explicit Greeting(std::string const& message_):
        message(message_)
    {}
    void operator()() const
    {
        std::cout<<message<<std::endl;
    }
};

int main()
{
    // function
    std::thread t0(print);
    t0.join();

    // function with parameters
    std::thread t1(write_sum,123,456);
    t1.join();

    // for object 
    std::thread t2(Greeting("goodbye"));
    t2.join();
}


