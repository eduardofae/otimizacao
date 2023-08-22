#include "./file.hpp"

Graph File::readFile(const char *filename){
    std::ifstream file (filename);
    if(file.fail()) throw 1;

    int n, m, k;
    file >> n;
    file >> m;
    file >> k;
    Graph info = Graph(n, k);
    for(int i = 0; i < n; i++) {
        file >> info.weights[i];
    }
    for(int i = 0; i < m; i++){
        int j, k;
        file >> j;
        file >> k;
        info.edges[j][k] = true;
        info.edges[k][j] = true;
    }

    file.close();
    return info;
}

void File::writeFile(const char *filename, const std::vector <double> solution, const std::vector <float> time, 
                     const std::vector <float> variable, const std::vector <unsigned int> seeds, int iteracoes){
    std::ofstream file(filename);

    file << std::setprecision(10) << "Solução Inicial: " << solution.at(0) << "\nSeed Inicial: " << seeds.at(0) << "\n\n";

    if(solution.size() == 2){
        for(int i = 1; i < solution.size(); i++){
            file << "\nO resultado médio do Simulated Annealing foi: " << solution.at(i) << "\nCom tempo médio de: " << time.at(i-1) << "\nSeeds Utilizadas: ";
            for(int j = 0; j < iteracoes; j++){
                file << seeds.at((i-1)*iteracoes+j+1) << " ";
            }
            file << "\n\n";
        }
    }
    else{
        for(int i = 1; i < solution.size(); i++){
            file << std::setprecision(10) << "Para o valor da variável: " << variable.at(i-1) << "\nO resultado médio do Simulated Annealing foi: " << solution.at(i) << "\nCom tempo médio de: " << time.at(i-1) << "\nSeeds Utilizadas: ";
            for(int j = 0; j < iteracoes; j++){
                file << seeds.at((i-1)*iteracoes+j+1) << " ";
            }
            file << "\n\n";
        }
    }

    file.close();
}