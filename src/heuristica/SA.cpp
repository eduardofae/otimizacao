#include "./SA.hpp"

SA::SA(std::vector <std::vector <bool>> &edges, std::vector <double> &weights, int k, unsigned int seed) {
    this->bestSolution = Solution(weights.size(), k);
    this->curSolution  = Solution(weights.size(), k);
    this->edges = edges;
    this->weights = weights;
    this->colors = k;
    this->initialSolution = Solution(weights.size(), k);
    this->seed = seed;
    calculateInitialSolution();
    testSolution(this->initialSolution);
}

void SA::calculateInitialSolution() {
    std::vector <int> connections = getConnections();
    for(int color = 0; color < colors; color++){
        int vertex = -1, max = -1;
        std::vector <int> neighbours;
        for(int v = 0; v < connections.size(); v++){
            if(this->initialSolution.vertex.at(v) == -1 && connections.at(v) > max){
                vertex = v;
                max = connections.at(v);
            }
        }
        if(vertex == -1) break;
        initialSolution.vertex[vertex] = color;
        initialSolution.colors[color].emplace_back(vertex);
        for(int v = 0; v < connections.size(); v++){
            if(edges.at(vertex).at(v)){
                neighbours.emplace_back(v);
            }
        }

        std::vector <int> nConections = connections;
        while(vertex != -1){
            for(int v = 0; v < nConections.size(); v++){
                int count = 0;
                for(auto &n : neighbours){
                    if(edges.at(v).at(n)){
                        count++;
                    }
                }
                nConections[v] = count;
            }
            max = -1;
            vertex = -1;
            for(int v = 0; v < nConections.size(); v++){
                bool valid = true;
                if(this->initialSolution.vertex.at(v) == -1 && nConections.at(v) > max){
                    for(auto &vert : initialSolution.colors[color]){
                        if(edges.at(vert).at(v)) {
                            valid = false;
                            break;
                        }
                    }
                    if(valid){
                        vertex = v;
                        max = nConections.at(v);
                    }
                }
            }
            if(vertex != -1) {
                for(int v = 0; v < weights.size(); v++){
                    if(edges.at(vertex).at(v) && std::find(neighbours.begin(), neighbours.end(), v) == neighbours.end()){
                        neighbours.emplace_back(v);
                    }
                }
                initialSolution.vertex[vertex] = color;
                initialSolution.colors[color].emplace_back(vertex);
            } 
        }
    }
    treatNoneColor();
    getValue(&this->initialSolution);
}

Solution SA::start(float t_max, float t_min, float r, int I) {
    this->bestSolution = initialSolution;
    this->curSolution = initialSolution;

    std::random_device rd;
    std::uniform_real_distribution<double> dist(0,1);

    for(float t = t_max; t > t_min; t *= r){
        for(int i = 0; i < I; i++){
            Solution newSolution = getNewSolution(I);
            double delta = newSolution.value - curSolution.value;
            if(delta <= 0){
                this->curSolution = newSolution;
                if(newSolution.value < bestSolution.value){
                    this->bestSolution = newSolution;
                    //std::cout << "Temperature: " << t << "\nValue: " << bestSolution.value << "\n\n";
                }
            }
            else{
                if(dist(rd) <= std::exp(-delta/t)){
                    this->curSolution = newSolution;
                }
            }
        }
    }

    // printSolution();

    return this->bestSolution;
}

Solution SA::getNewSolution(int I){
    Solution solution = curSolution;
    std::vector <int> colors;

    for(int i = 0; i < this->colors; i++){
        colors.emplace_back(i);
    }

    std::random_shuffle(solution.colors[solution.color].begin(), solution.colors[solution.color].end());
    std::random_shuffle(colors.begin(), colors.end());

    for(auto &vert : solution.colors[solution.color]){
        for(auto &color : colors){
            if(color == solution.color) continue;
            bool valid = true;
            for(auto &v : solution.colors[color]){
                if(edges.at(vert).at(v)) {
                    valid = false;
                    break;
                }
            }
            if(valid){
                solution.vertex[vert] = color;
                solution.colors[color].emplace_back(vert);
                solution.colors[solution.color].erase(std::remove(solution.colors[solution.color].begin(), solution.colors[solution.color].end(), vert), solution.colors[solution.color].end());
                double max = -1;
                for(int c = 0; c < this->colors; c++){
                    double value = 0;
                    for(auto &v : solution.colors[c]){
                        value += weights.at(v);
                    }
                    if(value > max){
                        solution.value = value;
                        solution.color = c;
                        max = value;
                    }
                } 
                return solution;
            }
        }
    }

    std::random_device rd;
    std::uniform_int_distribution<int> colorTypes(0,this->colors-1);
    std::uniform_int_distribution<int> vertex(0,weights.size()-1);

    for(int i = 0; i < I; i++) {
        bool valid = true;
        int vert = vertex(rd);
        int color = colorTypes(rd);
        for(auto &v : solution.colors[color]){
            if(edges.at(v).at(vert)) {
                valid = false;
                break;
            }
        }
        if(valid){
            solution.vertex[vert] = color;
            solution.colors[color].emplace_back(vert);
            solution.colors[solution.color].erase(std::remove(solution.colors[solution.color].begin(), solution.colors[solution.color].end(), vert), solution.colors[solution.color].end());
            double max = -1;
            for(int c = 0; c < this->colors; c++){
                double value = 0;
                for(auto &v : solution.colors[c]){
                    value += weights.at(v);
                }
                if(value > max){
                    solution.value = value;
                    solution.color = c;
                    max = value;
                }
            }
            return solution;
        }
    }
    return solution;
}

void SA::printSolution(){
    std::cout << "Vertices: ";
    for(int i = 0; i < weights.size(); i++)
        std::cout << i << " ";
    std::cout << std::endl << "Pesos:    ";
    for(int i = 0; i < weights.size(); i++)
        std::cout << weights.at(i) << " ";
    std::cout << std::endl << "Cores:    ";
    for(int color = 0; color < colors; color++){
        std::cout << color << ": { ";
        for(auto &c : bestSolution.colors.at(bestSolution.color))
            std::cout << c << " ";
        std::cout << "}\n\n";
    }
    std::cout << std::endl;
}

void SA::testSolution(Solution solution){
    for(int v = 0; v < solution.vertex.size(); v++){
        if(solution.vertex.at(v) == -1) throw 3;
    }
    for(int color = 0; color < colors; color++){
        int size = solution.colors.at(color).size();
        for(int vertex = 0; vertex < size - 1; vertex++){
            int cVertex = solution.colors.at(color).at(vertex);
            for(int vert = vertex+1; vert < solution.colors.at(color).size(); vert++){
                int cVert = solution.colors.at(color).at(vert);
                if(edges.at(cVertex).at(cVert)){
                    throw 2;
                }
            }
        }
    }
    std::cout << "Solução válida" << std::endl;
}

std::vector <int> SA::getConnections(){
    std::vector <int> connections;
    for(int i = 0; i < weights.size(); i++){
        int count = 0;
        for(int j = 0; j < weights.size(); j++){
            if(edges.at(i).at(j)){
                count++;
            }
        }
        connections.emplace_back(count);
    }
    return connections;
}

void SA::getValue(Solution *solution){
    double max = -1;
    for(int c = 0; c < this->colors; c++){
        double value = 0;
        for(auto &v : solution->colors[c]){
            value += weights.at(v);
        }
        if(value > max){
            solution->value = value;
            solution->color = c;
            max = value;
        }
    }
}

void SA::treatNoneColor(){
    std::srand(this->seed);

    std::vector <int> colors, vertices;
    for(int i = 0; i < this->colors; i++){
        colors.emplace_back(i);
    }
    for(int i = 0; i < this->weights.size(); i++){
        vertices.emplace_back(i);
    }
    std::random_shuffle(colors.begin(), colors.end());
    std::random_shuffle(vertices.begin(), vertices.end());

    for(int vertex = 0; vertex < weights.size(); vertex++){
        if(initialSolution.vertex.at(vertex) == -1){
            bool valid;
            int lastVert;
            for(auto &vert : vertices){
                if(edges.at(vertex).at(vert)){
                    lastVert = vert;
                    for(auto &color : colors){
                        valid = true;
                        for(auto &v : initialSolution.colors.at(color)){
                            if(edges.at(vert).at(v)){
                                valid = false;
                                break;
                            }
                        }
                        if(valid){
                            for(auto &v : initialSolution.colors.at(color)){
                                if(v != vert && edges.at(vertex).at(v)){
                                    valid = false;
                                    break;
                                }
                            }
                            if(valid){
                                int c = initialSolution.vertex[vert]; 
                                initialSolution.vertex[vert] = color;
                                initialSolution.colors[color].emplace_back(vertex);
                                initialSolution.colors[c].erase(std::remove(initialSolution.colors[c].begin(), initialSolution.colors[c].end(), vert), initialSolution.colors[c].end());
                                
                                initialSolution.vertex[vertex] = c;
                                initialSolution.colors[c].emplace_back(vertex);
                                break;
                            }
                        }
                    }
                    if(valid) break;
                }   
            }
            if(!valid){
                int c = initialSolution.vertex[lastVert]; 
                initialSolution.vertex[lastVert] = -1;
                initialSolution.colors[c].erase(std::remove(initialSolution.colors[c].begin(), initialSolution.colors[c].end(), lastVert), initialSolution.colors[c].end());

                std::random_shuffle(colors.begin(), colors.end());
                std::random_shuffle(vertices.begin(), vertices.end());

                vertex = -1;
            }
        }
    }
}

void SA::refreshSeed(unsigned int seed){
    this->seed = seed;
}

Solution SA::getInitialSolution(){
    return this->initialSolution;
}