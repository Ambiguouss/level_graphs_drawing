#include <bits/stdc++.h>
#pragma once
#include "edge.cpp"

using namespace std;


struct LevelEdges{
    int top_level;
    //bottom level is top level+1
    vector<Edge*> edges;
    
    Edge* get_edge(int k){
        return edges[k];
    }

    ~LevelEdges(){
        for(auto x:edges){
            delete x;
        }
    }
};
