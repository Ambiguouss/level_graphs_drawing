#pragma once
#include "graph.cpp"

template <typename LevelType, typename VertexType>
void GraphBase<LevelType,VertexType>::draw(string filename){

    for(auto lvl:levels){
        cout<<lvl<<'\n';
        cout<<lvl->verticles.size()<<'\n';
    }

    for(auto lvl:levels){
        for(int i=0;i<lvl->verticles.size();i++){
            cout<<lvl->get_vertex(i+1)->label<<' ';
        }
        cout<<'\n';
    }

    if(filename==""){
        filename="images/test.tex";
        return;
    }
    ofstream outFile(filename);

    if (!outFile) {
        cerr << "Error opening file!" << endl;
        return;
    }

    outFile << "\\documentclass{article}\n";
    outFile << "\\usepackage{tikz}\n";
    outFile << "\\usepackage{adjustbox}\n";
    outFile << "\\begin{document}\n";
    outFile << "\\adjustbox{max size={\\textwidth}{\\textheight}}{%\n";
    outFile << "\\begin{tikzpicture}[]\n";



    for(int lvl=0;lvl<levels.size();lvl++){
        LevelType* level = levels[lvl];
        for(int i=0;i<level->verticles.size();i++){
            VertexType* v = level->get_vertex(i+1);
            if(!v->dummy){
                outFile << "\\node[draw, circle] (" <<v  << ") at (" 
                    << i << "," << -lvl
                    << ") {" << v->label << "};\n";
            }else{
                outFile << "\\node[draw, circle,minimum size=0pt, inner sep=0pt] (" <<v  << ") at (" 
                << i << "," << -lvl
                << ") {" << "};\n";
            }
        }
    }
    
    for(int lvl=0;lvl<levels.size();lvl++){
        LevelType* level = levels[lvl];
        for(int i=0;i<level->verticles.size();i++){
            VertexType* v = level->get_vertex(i+1);
            for(auto x:v->get_down_neighbors()){
                outFile << "\\draw (" << v<< ") -- (" 
                << x<< ");\n";
            }
        }
    }
    outFile << "\\end{tikzpicture}%\n}\n";
    outFile << "\\end{document}\n";

    outFile.close();
    cout << "TikZ file generated: " << filename << endl;
}