# A simple csv file reader

## background

When I wanted to rewrite the codes from python to C++ in my machine learning homework, I needed to read data from a csv file first. But I found that the lib existed can't satisfy my requirements. So I started this project.

## introduction

The initial target is to sitimulate the `pandas` in python, but it is too hard. So the first edition can only read csv file, and print it on console.

There are three classes : `csvReader`, `csvData` and `Row`. `csvReader` has only three static member function, to complete the loading task. `Row` is the class of one row in csv file, and `csvData` means the whole data in csv file. The types of data both are `std::string`.

## example

When we want to read csv file `data.csv`, the only thing we do is `auto csv = csv::csvReader::read_csv(filepath);`. And we can directly `cout` the single data, single row, or whole file.

```cpp
#include <iostream>
#include "csvReader.hpp"
using namespace std;
int main() {
    auto csv = csv::csvReader::read_csv("../data.csv");
    cout << csv[0]["密度"] << endl;
    cout << csv[0] << endl;
    cout << csv << endl;
    return 0;
}
```

## prospect

In the future version, `csvData` can export the data from a colomn into `std::vector` in the type we want. And it can modify the file, save the file.

The output of whole file is not so correct when there has some Chinese characters. It need to be fixed.