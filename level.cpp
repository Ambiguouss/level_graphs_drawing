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

    Vertex* get_permuted_vertex(int k){
        return verticles[permutation[k]];
    }

    ~Level(){
        for(auto x:verticles){
            delete x;
        }
    }

};