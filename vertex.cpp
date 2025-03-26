#include <bits/stdc++.h>
#pragma once
using namespace std;
struct Vertex{
    //int level;
    string label;
    int level;
    vector<Vertex*> down_neighbors;
    Vertex(string _label,int _level):label(_label),level(_level){}

    void add_down_neighbor(Vertex * v){
        down_neighbors.push_back(v);
    }

    vector<Vertex*> get_down_neighbors(){
        return down_neighbors;
    }
};