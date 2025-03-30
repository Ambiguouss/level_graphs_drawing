#include <bits/stdc++.h>
#pragma once
using namespace std;
struct Vertex{
    string label;
    int level;
    vector<Vertex*> down_neighbors;

    int _index=-1;//usful only for sat
    Vertex(string _label,int _level):label(_label),level(_level){}

    void add_down_neighbor(Vertex * v){
        down_neighbors.push_back(v);
    }

    vector<Vertex*> get_down_neighbors(){
        return down_neighbors;
    }
    virtual ~Vertex(){};
};

struct ExtraVertex : Vertex{
    Vertex* original;
    ExtraVertex(string _label,int _level,Vertex* origin):
        Vertex(_label,_level),original(origin){}

};