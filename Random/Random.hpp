#include <random>
#include <chrono>

class RandomInt {
private:
    std::default_random_engine re;
    std::uniform_int_distribution<> dist;
public:
    // We use as seed the time the class gets constructed to avoid same results on
    // multiple calls with same bounds.
    // We could use std::random_device but then we get an error when we run with Valgrind. That's because
    // when the Ubuntu system has a random number library which uses the RDRAND instruction,
    // older versions of Valgrind do not recognise that instruction.
    // So Valgrind will stumble on an unknown instruction and will not be able to
    // translate it into the VEX intermediate language. This fixed with newer versions like 3.16.1.

    RandomInt(int low, int high) : dist{low, high} { 
        re.seed(std::chrono::system_clock::now().time_since_epoch().count()); 
    }

    int operator()() { return dist(re); }  // Get a random integer in [low:high]

    void change_bounds(int lower_bound, int upper_bound) {
        dist.param( decltype(dist)::param_type(lower_bound, upper_bound) );
    }
};

class RandomDouble {
private: 
    std::default_random_engine re;
    std::uniform_real_distribution<double> dist;
public:
    RandomDouble(double low, double high) : dist{low, high} {
        re.seed(std::chrono::system_clock::now().time_since_epoch().count());         
    }

    double operator()() { return dist(re); }  // Get a random double in [low:high)
    
    void change_bounds(int lower_bound, int upper_bound) {
        dist.param( decltype(dist)::param_type(lower_bound, upper_bound) );
    }
};