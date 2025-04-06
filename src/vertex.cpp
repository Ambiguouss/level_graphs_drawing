#include <bits/stdc++.h>
#pragma once
using namespace std;
struct Vertex{
    string label;
    int level;
    vector<Vertex*> down_neighbors;
    vector<Vertex*> up_neighbors;

    int _index=-1;//useful only for sat
    bool dummy=false;
    Vertex(string _label,int _level):label(_label),level(_level){}

    void add_down_neighbor(Vertex * v){
        down_neighbors.push_back(v);
    }

    void add_up_neighbor(Vertex * v){
        up_neighbors.push_back(v);
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