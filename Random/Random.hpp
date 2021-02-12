#include <random>

// Simple number generator classes

class RandomInt {
private:
    std::default_random_engine re;
    std::uniform_int_distribution<> dist;
    std::random_device rd;
public:
    RandomInt(int low, int high) : re{rd()}, dist{low, high} {}

    int operator()() { return dist(re); }  // Get a random integer in [low:high]

    void change_bounds(int lower_bound, int upper_bound) {
        dist.param( decltype(dist)::param_type(lower_bound, upper_bound) );
    }
};

class RandomDouble {
private: 
    std::default_random_engine re;
    std::uniform_real_distribution<double> dist;
    std::random_device rd;
public:
    RandomDouble(double low, double high) : re{rd()}, dist{low, high} {}

    double operator()() { return dist(re); }  // Get a random double in [low:high)
    
    void change_bounds(int lower_bound, int upper_bound) {
        dist.param( decltype(dist)::param_type(lower_bound, upper_bound) );
    }
};
