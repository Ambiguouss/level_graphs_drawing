#include <bits/stdc++.h>
#pragma once
using namespace std;
#include "vertex.cpp"

struct Sat_var{
    Vertex* first;
    Vertex* second;
    int truth=0;//-1 for false, 0 for unknown, 1 for truth
    Sat_var* inv;
    vector<Sat_var*> equiv;
    Sat_var(Vertex* v1,Vertex* v2):first(v1),second(v2){};

    bool process(int val){
        if(val==0) throw std::invalid_argument("Don't assign truth as 0");
        if(truth==-val)return false;
        truth=val;
        bool ok=1;
        if(inv->truth==val)return false;
        if(inv->truth==0){
            if(!inv->process(-val))return false;
        }
        for(auto nei:equiv){
            if(nei->truth==-val)return false;
            if(nei->truth==0){
                if(!nei->process(val)) return false;
            }
        }
        return true;
    }
};