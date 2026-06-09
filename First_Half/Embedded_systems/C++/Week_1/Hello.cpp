#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tuple>
using namespace std;

// int main() {
    // std::cout << "Hello World!" << std::endl;
    // return 0;
// }



// int main() {
//     string name = "Uunii";
//     vector<int> scores = {88, 93, 94};
//     map<string, int> ageMap;
//     ageMap["Bataa"] = 24;
//     ageMap["Tsetse"] = 25;

//     tuple<string, int, bool> person = {"Sulugas", 30, true};

//     cout << "Name: "<< name << endl;
//     cout << "First score: " << scores[0] << endl;
//     cout << "Bataa's age: " << ageMap["Bataa"] << endl;
//     cout << "Tuple name: " << get<0>(person) << endl;

//     return 0;
// }

// int main() {
//     int score = 93;

//     if (score >= 90) {
//         std::cout <<"Excellent!" << std::endl;
//     } else if (score >=75) {
//         std::cout <<"Good job!" << std::endl;
//     } else {
//         std::cout <<"Keep trying!" << std::endl;
//     }
//     return 0;
// }

// #include <iostream>

// int main() {
//     for (int i = 0; i < 5; ++i) {
//         std::cout << "Iteration " << i << std::endl;
//     }
//     return 0;
// }

int main() {
    int numbers[3] = {1, 2, 3};
    std::cout << numbers[0];

    std ::vector<int> nums = {10, 20, 30};
    nums.push_back(40);
    std::cout << nums[2];

    return 0;
}