#ifndef UTILS_H_
#define UTILS_H_

#include <chrono>
#include <random>
#include <map>
#include <string>

class Utils
{
public:
    Utils();
    void seed_random();
    double get_random();
    void start_timer(std::string timer);
    void stop_timer(std::string timer);
    double get_last_time(std::string timer);
    double get_total_time(std::string timer);
    double get_prom_time(std::string timer);

private:
    std::map<std::string, std::chrono::time_point<std::chrono::system_clock>> start;
    std::map<std::string, double> last;
    std::map<std::string, double> total;
    std::map<std::string, int> count;
};

#endif
