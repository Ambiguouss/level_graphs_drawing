#include "graph.cpp"
#include "graph_draw.cpp"
#include <bits/stdc++.h>

using namespace std;

int main(int argc, char *argv[]){
    Graph g=input_graph();
    if(argc<3){
        g.draw();
    }else{
        g.draw(argv[1]);
    }
    Graph d=g.split_verticles();
    if(argc<3){
        d.draw();
    }else{
        d.draw(argv[2]);
    }
 
}