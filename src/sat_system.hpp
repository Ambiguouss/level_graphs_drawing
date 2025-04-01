#pragma once
#include <bits/stdc++.h>
#include "sat_var.cpp"

template <typename LevelType, typename VertexType>
struct GraphBase;

struct Sat_system{
    vector<Sat_var*> variables;
    vector<vector<Vertex*>> index_to_vertex;
    vector<vector<vector<Sat_var*>>> lvl_ind_ind_to_var;
    
    ~Sat_system(){
        for(auto x:variables){
            delete x;
        }
    }

    bool less(Vertex* a, Vertex* b);

    bool solve();
    
    void create(GraphBase<Level,Vertex>*g);
    
};