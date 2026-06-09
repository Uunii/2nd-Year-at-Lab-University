#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <algorithm>
using namespace std;

bool greater_than_25(float value) {
    return value > 25;
}

float average (const vector<float>& temps) {
  if (temps.size() == 0) {
    throw std::runtime_error("Division by zero!");
  }
  float sum = 0;
  for (float t : temps) {
    sum += t;
  }
  return sum / temps.size();
}

int main() {
  float temps[] = {22.5, 24.1, 26.3, 21.8, 25.7};
  int size = 5;
  string location = "Lahti";
  string filename = "lahti_readings.txt";
  std::ofstream outFile(filename);

  if (outFile.is_open()) {
    for (int i = 0 ; i < size; i++) {
      outFile << temps[i] << " ";
    }
    outFile.close();
  } else {
    std::cout << "Unable to open file." << std::endl;
  }
  
  std::ifstream inFile(filename);
  std::string line;
  vector<float> temperatures;

  if (inFile.is_open()) {
    float value;
    std::cout << "Location: " << location << std::endl;
    std::cout << "Readings: ";
    while (inFile >> value) {
        std::cout << value << " ";
        temperatures.push_back(value);
    }
    cout << endl;
    inFile.close();
  } else {
    std::cout << "Unable to open file." << std::endl;
  }

  try {
    std::cout << "Average: " << average(temperatures) << std::endl;
  } catch (const std::runtime_error& e) {
    std::cout << "Error: " << e.what() << std::endl;
  }
  
  int amount = count_if(temperatures.begin(), temperatures.end(), greater_than_25);
  std::cout << "Readings above 25°C: " << amount;
  std::cout << std::endl;

  std::cout << "Saved to file: " << filename << std::endl;

  return 0;
  
}