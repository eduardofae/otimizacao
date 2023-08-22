#include "./file/file.hpp"
#include "./heuristica/SA.hpp"
#include <chrono>

using namespace std::chrono;

#define ITERACOES 10

int main( int argc, char** argv ) {
    std::cout << std::setprecision(10);
    try {
        std::vector <float> times, variable;
        std::vector <double> solutions;
        std::vector <unsigned int> seeds;
        seeds.emplace_back(std::time(0));
        Graph info = File::readFile(argv[1]);
        SA sa = SA(info.edges, info.weights, info.k, seeds.at(0));
        solutions.emplace_back(sa.getInitialSolution().value);
        // std::cout << "Initial Value: " << solution.at(0) << "\n\n";

        Solution solution;
        for(int var = 1000; var >= 1000; var-=100){
            float totalTime = 0;
            double totalValue = 0;
            if(var == 0) var = 10;
            for(int i = 0; i < ITERACOES; i++){
                unsigned int seed = std::time(0);
                sa.refreshSeed(seed);
                auto beg = high_resolution_clock::now();
                solution = sa.start(1000, 10, 0.99, var);
                auto end = high_resolution_clock::now();
                auto duration = duration_cast<milliseconds>(end - beg);
                totalTime += (float) duration.count()/1000;
                totalValue += solution.value;
                seeds.emplace_back(seed);
            }
            solutions.emplace_back(totalValue/ITERACOES);
            times.emplace_back(totalTime/ITERACOES);
            variable.emplace_back(var);
        }

        File::writeFile(argv[2], solutions, times, variable, seeds, ITERACOES);
        // std::cout << "Fim" << std::endl;
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