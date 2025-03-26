#include "graph.cpp"
#include <bits/stdc++.h>

using namespace std;

int main(int argc, char *argv[]){
    Graph g=input_graph();
    Graph d=g.split_verticles();
    if(argc<2){
        d.draw();
    }else{
        d.draw(argv[1]);
    }
 
}