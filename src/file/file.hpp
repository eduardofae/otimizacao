#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <iomanip>

struct Graph {
    std::vector <double> weights;
    std::vector <std::vector <bool>> edges;
    int k;

    Graph(int n, int k){
        this->k = k;
        this->weights.resize(n);
        this->edges.resize(n, std::vector<bool>(n, false));
    }
};

class File {
    public:
        File() = default;
        static Graph readFile(const char *filename);
        static void writeFile(const char *filename, const std::vector <double> solution, const std::vector <float> time, const std::vector <float> temp);
};