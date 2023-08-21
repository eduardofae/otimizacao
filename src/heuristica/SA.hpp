#include <vector>
#include <limits>
#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>

struct Solution {
    std::vector <int> vertex;
    std::vector <std::vector <int>> colors;
    double value;
    int color;

    Solution() = default;

    Solution(int n, int k){
        this->vertex.resize(n, -1);
        this->colors.resize(k);
        this->value = 0;
        this->color = -1;
    }
};


class SA {
    public:
        SA(std::vector <std::vector <bool>> &edges, std::vector <double> &weights, int k);
        Solution start(float t_max, float t_min, float r, int I);
        void testSolution(Solution solution);
    private:
        Solution bestSolution;
        Solution curSolution;
        std::vector <std::vector <bool>> edges;
        std::vector <double> weights;
        int colors;

        Solution initialSolution;

        void getInitialSolution();
        Solution getNewSolution(int I);
        std::vector <int> getConnections();
        void getValue(Solution *solution);
        void parseNoneColor();

        void printSolution();
};
