#include "graph.cpp"

void Graph::draw(string filename=""){
    if(filename==""){
        filename="images/test.tex";
        return;
    }
    ofstream outFile(filename);

    if (!outFile) {
        cerr << "Error opening file!" << endl;
        return;
    }

    // Start the TikZ picture
    outFile << "\\documentclass{article}\n";
    outFile << "\\usepackage{tikz}\n";
    outFile << "\\usepackage{adjustbox}\n";
    outFile << "\\begin{document}\n";
    outFile << "\\adjustbox{max size={\\textwidth}{\\textheight}}{%\n";
    outFile << "\\begin{tikzpicture}[]\n";



    for(int lvl=0;lvl<levels.size();lvl++){
        Level* level = levels[lvl];
        for(int i=0;i<level->verticles.size();i++){
            Vertex* v = level->get_permuted_vertex(i);
            outFile << "\\node[draw, circle] (" <<v  << ") at (" 
                << i << "," << -lvl
                << ") {" << v->label << "};\n";
        }
    }
    
    for(int lvl=0;lvl<levels.size();lvl++){
        Level* level = levels[lvl];
        for(int i=0;i<level->verticles.size();i++){
            Vertex* v = level->get_permuted_vertex(i);
            for(auto x:v->get_down_neighbors()){
                outFile << "\\draw (" << v<< ") -- (" 
                << x<< ");\n";
            }
            //outFile << "\\node[draw, circle] (" <<v  << ") at (" 
                //<< i << "," << -lvl
                //<< ") {" << v->label << "};\n";
        }
    }
    // Draw edges
    /*for (auto level_edges : edges) {
        for(auto edge : level_edges->edges){
            outFile << "\\draw (" << edge->get_top()<< ") -- (" 
            << edge->get_bot()<< ");\n";

        }
    }*/

    // End TikZ picture
    outFile << "\\end{tikzpicture}%\n}\n";
    outFile << "\\end{document}\n";

    outFile.close();
    cout << "TikZ file generated: " << filename << endl;
}