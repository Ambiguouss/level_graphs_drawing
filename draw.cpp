#include "graph.cpp"
#include <bits/stdc++.h>

using namespace std;

int main(int argc, char *argv[]){
    Graph g=input_graph();
    char* filename;
    if(argc<2){
        filename="images/test.tex";
        return 0;
    }else{
        filename = argv[1];
    }
    ofstream outFile(filename);

    if (!outFile) {
        cerr << "Error opening file!" << endl;
        return 0;
    }

    // Start the TikZ picture
    outFile << "\\documentclass{article}\n";
    outFile << "\\usepackage{tikz}\n";
    outFile << "\\begin{document}\n";
    outFile << "\\begin{tikzpicture}[scale=3, transform shape]\n";



    for(int lvl=0;lvl<g.levels.size();lvl++){
        Level* level = g.levels[lvl];
        for(int i=0;i<level->verticles.size();i++){
            Vertex* v = level->get_permuted_vertex(i);
            outFile << "\\node[draw, circle] (" <<v->level<<'&'<<v->label  << ") at (" 
                << i << "," << -lvl
                << ") {" << v->label << "};\n";
        }
    }
    

    // Draw edges
    for (auto level_edges : g.edges) {
        for(auto edge : level_edges->edges){
            outFile << "\\draw (" << edge->get_top()->level<<'&'<< edge->get_top()->label<< ") -- (" 
            << edge->get_bot()->level<<'&'<< edge->get_bot()->label<< ");\n";

        }
    }

    // End TikZ picture
    outFile << "\\end{tikzpicture}\n";
    outFile << "\\end{document}\n";

    outFile.close();
    cout << "TikZ file generated: " << filename << endl;
 
}