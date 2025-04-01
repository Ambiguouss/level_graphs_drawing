#include "graph.cpp"
#include "graph_draw.cpp"
#include "evenify.cpp"
#include "sat_system.cpp"
#include <bits/stdc++.h>

using namespace std;

int main(int argc, char *argv[]){
    GraphBase<Level,Vertex> g=input_graph<Level,Vertex>();
    Sat_system s;
    s.create(&g);
    bool res = s.solve();
    cout<<res<<'\n';
    
    if(argc<3){
        g.draw();
    }else{
        g.draw(argv[1]);
    }
    GraphBase<Level,Vertex> d=g.evenify();
    if(argc<3){
        d.draw();
    }else{
        d.draw(argv[2]);
    }
 
}