//https://www.justsoftwaresolutions.co.uk/threading/multithreading-in-c++0x-part-2-function-objects-and-arguments.html

#include <thread>
#include <iostream>
#include <string>
#include <functional> // for std::ref

//Type1: function()
void print()
{
    std::cout<<"Thread: from function()" << std::endl;
}

//Type2: function(a,b)
void write_sum(int x,int y)
{
	std::cout<<"Thread: function(x, y)" << std::endl;
    std::cout<<x<<" + "<<y<<" = "<<(x+y)<<std::endl;
}

//Type3: Object+operator()
class Greeting
{
    std::string message;
public:
    explicit Greeting(std::string const& message_):
        message(message_)
    {}
    void operator()() const
    {
    	std::cout<<"Thread: function object()" << std::endl;
        std::cout<<message<<std::endl;
    }

    void greeting() const
    {
    	std::cout<<"Thread: object + method()" << std::endl;
        std::cout<<"hello:"<< message << std::endl;
    }

    //thread function cannot be overridden
    void greetArg(std::string const& arg) const
    {
    	std::cout<<"Thread: object + method()" << std::endl;
    	std::cout<<"hello:"<< message <<"; arg=" << arg << std::endl;
    }


};

class RefObject
{
public:
    void operator()() const
    {
        std::cout<<"Ref Object thread: this="<<this<<std::endl;
    }
};

//using std::ref(x)
void increment(int& i)
{
    ++i;
}


int main()
{
	/////////////////////////////////////
	//all related are copied to thread internal storage
    //thread with lamda function
    std::thread t0( std::thread([](){
        std::cout << "Thread lamda function:  from thread "
        		  << std::this_thread::get_id() << std::endl;
    }));
    t0.join();

	// function
    std::thread t1(print);
    t1.join();

    // function with parameters
    std::thread t2(write_sum,123,456);
    t2.join();

    //function object
    //function objects and arguments are copied to the thread's internal storage
    std::thread t30(Greeting("goodbye"));
    t30.join();

    Greeting g0("seperate goodbye");
    std::thread t31(g0);
    t31.join();

    //object with method
    Greeting x("Object method");
    std::thread t40(&Greeting::greeting,&x);
    t40.join();

    //method cannot be overridn; fails if using greeting(arg)
    //std::thread t41(&Greeting::greeting,&x,"with arg");
    std::thread t41(&Greeting::greetArg,&x,"with arg");
    t41.join();

    //using shared_ptr
    std::shared_ptr<Greeting> p(new Greeting("shared_ptr"));
    std::thread t42(&Greeting::greetArg, p,"goodbye");
    t42.join();
    /////////////////////////////////////

    ///////////////////////////////////// using reference
    RefObject rx;
    rx();
    std::thread t51(std::ref(rx));
    t51.join();
    std::thread t52(rx);
    t52.join();

    //std::ref can be used for other arguments
    int i=42;
    std::cout<<"before thread: x="<<i<<std::endl;
    std::thread t(increment,std::ref(i));
    t.join();
    std::cout<<"after thread: x="<<i<<std::endl;

}


