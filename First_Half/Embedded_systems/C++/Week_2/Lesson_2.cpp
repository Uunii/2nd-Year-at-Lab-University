#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <algorithm>
using namespace std;


// int add(int x, int y) {
//     return x + y;
// }

// int main() {
//     std::cout << add(3, 4) << std::endl;
//     return 0;
// }

// class Car {
// public:
//     string brand;
//     int year;

//     void honk() {
//         cout << "Beep Beep" << endl;
//     }
// };

// int main() {
//     Car myCar;
//     myCar.brand = "Toyota";
//     myCar.year = 2020;
//     myCar.honk();

//     return 0;
// }

// int main() {
//     std::fstream outFile("example.txt");
//     if (outFile.is_open()) {
//         outFile << "Hello World!" << std::endl;
//         outFile << "This is a second line" << std::endl;
//         outFile.close();
//     } else {
//         std::cout <<"Unable to open file" << std::endl;
//     }

//     return 0;
// }

// int main() {
//     std::fstream inFile("example.txt");
//     std::string line;

//     if (inFile.is_open()) {
//         while (std::getline(inFile, line)) {
//             std::cout << line << std::endl;
//         }
//         inFile.close();
//     }else{
//         std::cout << "Unable to open file" << std::endl;
//     }
//     return 0;
// }

int divide(int a, int b){
    if (b == 0){
        throw std::runtime_error("Division by zero!");
    }
    return a / b;
}

int main() {
    try {
        std::cout << divide (10,0) << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    return 0;
}

// int main() {
//     std::vector<int> nums = {5, 2, 9, 1};
//     std::sort(nums.begin(), nums.end());

//     for (int num : nums) {
//         std::cout << num << " ";
//     }
//     return 0;
// }

int main() {
    std::vector<int> nums = {10, 20, 30, 40};
    auto it = std::find(nums.begin(), nums.end(),30);

    if (it != nums.end()) {
        std::cout << "Found: " << *it << std::endl;
    } else {
        std::cout << "Not found." <<std::endl;
    }
    return 0;
}

// bool greater_than_5(int value) {
//     return value > 5;
// }

// int main() {
//     vector<int> numbers = {1, 7, 3, 5, 9, 2};
//     int amount = count_if(numbers.begin(), numbers.end(), greater_than_5);
//     cout << "There are " << amount << " values greater than 5."<< endl;
//     return 0;
// }