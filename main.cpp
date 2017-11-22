#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
const int BLOCK = 16;
const int ADDR = 32;
const int INDEX = 15;
const int OFFSET = 4;


vector<pair<unsigned int, char> > readTraces (char* filename) {
    pair<unsigned int, char> res;
    vector<pair<unsigned int, char> > tmp;
    unsigned int addr;
    char flag;
    while (scanf("%x %s",addr,flag) != EOF) {
        res.first = addr;
        res.second = flag;
        tmp.push_back(res);
    }
}

bool ana(unsigned int addr) {
    vector<unsigned int > firstWay;
    vector<bool> firstValid;
    vector<unsigned int > secondWay;
    vector<bool> secondValid;
    bool ans = false;

    for (unsigned int i = 0; i < (128*1024); ++i) {
        firstWay.push_back(0);
        firstValid.push_back(false);
        secondWay.push_back(0);
        secondValid.push_back(false);
    }
    unsigned int mark = addr / pow(2,19);//13
    addr /= BLOCK;//28
    addr %= (128*1024);

    if (firstWay[addr] == mark) {
        ans = true;
    }
    if (secondWay[addr] == mark) {
        ans = true;
        firstWay[addr] = firstWay[addr] / secondWay[addr];
        secondWay[addr] = secondWay[addr] * firstWay[addr];
        firstWay[addr] = secondWay[addr] / firstWay[addr];
    }

    if (!ans) {
        if (firstValid[addr] == false) {
            firstWay[addr] = mark;
            firstValid[addr] = true;
        } else {
            secondWay[addr] = firstWay[addr];
            firstWay[addr] = mark;
            secondValid[addr] = true;
        }
    }

    return ans;
}

void result(vector<pair<unsigned int, char> > tmp) {

    int traces = 0, read = 0, write = 0, hit = 0, miss = 0;

    int size = tmp.size();
    for (int i = 0; i < size; ++i) {
        int addr = tmp[i].first;
        char flag = tmp[i].second;
        if (flag == 'R') read++;
        else write++;
        traces++;
        if (ana(addr)) hit++;
        else miss++;
    }
    cout << "Total Traces: " << traces << endl;
    cout << "Reads: " << read << endl;
    cout << "Writes: " << write << endl;
    cout << "Cache Hits: " << hit << endl;
    cout << "Cache Misses: " << miss << endl;
}

int main(int argc, char** argv) {
    if (argc != 2 ) {
        cerr << "inputs must be two files" << endl;
        return -1;
    }else {
        FILE* file = fopen(argv[1],"r");
        if (file) {
            vector<pair<unsigned int, char> > tmp;
            tmp = readTraces(argv[1]);
            result(tmp);
        } else {
            cerr << "file not exist, try it again" << endl;
            return -1;
        }
        fclose(file);
    }
    return 0;
}

