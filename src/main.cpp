#include "./file/file.hpp"
#include "./heuristica/SA.hpp"
#include <chrono>

using namespace std::chrono;

int main( int argc, char** argv ) {
    std::cout << std::setprecision(10);
    try {
        std::vector <float> times, temp;
        std::vector <double> solutions;
        // Graph info = File::readFile(argv[1]);
        Graph info = File::readFile("../cmb/cmb10");
        SA sa = SA(info.edges, info.weights, info.k);
        Solution solution;

        for(float t = 0.99; t >= 0.99; t-=0.1){
            auto beg = high_resolution_clock::now();
            solution = sa.start(1000, 10, t, 1000);
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(end - beg);

            solutions.emplace_back(solution.value);
            times.emplace_back((float) duration.count());
            temp.emplace_back(t);
        }

        // File::writeFile(argv[2], solutions, times, temp);
        File::writeFile("../results/test.txt", solutions, times, temp);
        std::cout << "Fim" << std::endl;

        sa.testSolution(solution);
    }
    catch (int e) {
        switch (e){
            case 1: 
                std::cout << "Erro na leitura do arquivo" << std::endl;
                break;
            case 2:
                std::cout << "Solução gerada não é válida" << std::endl;
                break;
            case 3:
                std::cout << "Vértice não possui cor" << std::endl;
                break;
            default:
                std::cout << "An exception occurred. Exception Nr. " << e << '\n';
        }
    }
    return 0;
}