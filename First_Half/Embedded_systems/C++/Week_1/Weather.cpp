#include <iostream>
#include <string>
#include <vector>
#include <tuple>
using namespace std;

// int main() {          // Version done by using tuple
//     vector<string> days = {"Monday:    ", "Tuesday:   ", "Wednesday: ", "Thursday:  ", "Friday:    ", "Saturday:  ", "Sunday:    "};
//     vector<double> temperatures = {  15.2, 16.8, 28.3, 23.8, 18.4, 9.2, 8.1};
        
//     for (int i = 0; i < days.size(); i++) {
//         std::cout << days[i] << temperatures[i] << "°C  ";

//         if (temperatures[i] > 20) {
//             std::cout << " Warm" << std::endl;
//         } else if (temperatures[i] < 10) {
//             std::cout << "  Cold" << std::endl;
//         } else {
//             std::cout << " OK" << std::endl;
//         }
//     }

//     tuple<string, double> hotday = {"Wednesday ", 28.3};
//     std::cout << "The hottest day is " << get<0> (hotday) << "with "<< get<1> (hotday) << "°C"<< std::endl;
//     return 0;
//     }

int main() {          // Version that finds the hottest day itself without using tuple

    vector<string> days = {"Monday:    ", "Tuesday:   ", "Wednesday: ", "Thursday:  ", "Friday:    ", "Saturday:  ", "Sunday:    "};
    vector<double> temperatures = {  15.2, 16.8, 28.3, 23.8, 18.4, 9.2, 8.1};
        
    for (int i = 0; i < days.size(); i++) {
        std::cout << days[i] << temperatures[i] << "°C  ";

        if (temperatures[i] > 20) {
            std::cout << " Warm" << std::endl;
        } else if (temperatures[i] < 10) {
            std::cout << "  Cold" << std::endl;
        } else {
            std::cout << " OK" << std::endl;
        }
    }
    double maxtemp = temperatures[0];
    string maxdays = days[0];
    for (int j = 1; j < temperatures.size(); j++) {
        if (temperatures[j] > maxtemp) {
            maxtemp = temperatures[j];
            maxdays = days[j];
        }
    }
    std::cout << "The hottest day is " <<maxdays << " with " << maxtemp << "°C" << std::endl;
    return 0;
    }