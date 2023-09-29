#include <iostream>
#include "csvReader.hpp"
using namespace std;
int main() {
    auto csv = csv::csvReader::read_csv("../data.csv");
    cout << csv << endl;
    return 0;
}