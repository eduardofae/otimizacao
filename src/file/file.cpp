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

void File::writeFile(const char *filename, const std::vector <double> solution, const std::vector <float> time, const std::vector <float> temp){
    std::ofstream file(filename);

    for(int i = 0; i < solution.size(); i++)
        file << std::setprecision(10) << "Para um N° de Iterações de: " << temp.at(i) << "\nO resultado do Simulated Annealing foi: " << solution.at(i) << "\nCom tempo de: " << time.at(i) << "\n\n";

    file.close();
}