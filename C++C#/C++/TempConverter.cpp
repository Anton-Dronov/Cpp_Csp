#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cstdio>

struct Data {
    float time;
    float temperature;
};

void sorting(std::vector<Data> dat) {
    std::vector<Data> tempor;
    float taym = 0;
    for (auto& e : dat) {
        if (e.time > taym) {
            bool k = 0;
            for (auto& o : tempor) {
                if (e.time == o.time) {
                    k = 1;
                }
            }
            if (k == 0) {
                tempor.push_back(e);
                taym = e.time;
            }
        }
    }
    for (unsigned i = 0; i < tempor.size(); i++) {
        dat[i] = tempor[i];
    }
}

float average(std::vector<Data>& vec) {
    float ave = 0;
    int count = 0;
    float sum = 0;
    for (float i = vec[0].time; i < vec[0].time + 10;) {
        sum += vec[count].temperature;
        count++;
        i = vec[0 + count].time;
    }
    ave = sum / count;
    return ave;
}

int main() {
    std::string fname = "";
    std::cin >> fname;
    std::vector<Data> data;
    if (std::ifstream ifs{ fname }) {
        int filesize = ifs.tellg();
        int size = filesize / sizeof(Data);
        ifs.seekg(0);
        for (int i = 0; i < size; i++) {
            Data nov;
            ifs.read(reinterpret_cast<char*>(&nov), sizeof(nov));
            data.push_back(nov);
        }
    }
    else {
        std::cout << "Sorry, file " << fname << " couldn't be found!";
    }
    sorting(data);
    std::cout << average(data);
}