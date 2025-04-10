#include <bits/stdc++.h>
#pragma once
#include "vertex.cpp"
using namespace std;

struct Level{
    vector<Vertex*> verticles;
    vector<int> permutation;
    Vertex* get_vertex(int k){
        return verticles[k-1];
    }
    void add_vertex(Vertex* v){
        verticles.push_back(v);
        permutation.push_back(permutation.size());
    }
    Vertex* get_permuted_vertex(int k){
        return verticles[permutation[k]];
    }

    virtual ~Level(){
        for(auto x:verticles){
            delete x;
        }
    }

};

struct ExtraLevel : Level{
    Level* original_level;
    Vertex* original_vertex;
    vector<ExtraVertex*> verticles;
    ExtraLevel(Level* origin,Vertex* origin_v):Level(),original_level(origin),original_vertex(origin_v){};
    void add_vertex(ExtraVertex* v){
        verticles.push_back(v);
        permutation.push_back(permutation.size());
    }
    ExtraVertex* get_vertex(int k){
        return verticles[k-1];
    }
    ~ExtraLevel(){
        for(auto x:verticles){
            delete x;
        }
    }
};