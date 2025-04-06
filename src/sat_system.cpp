#include <bits/stdc++.h>
#pragma once
using namespace std;
#include "sat_var.cpp"
#include "sat_system.hpp"


bool Sat_system::solve(){
    for(auto var : variables){
        if(var->truth!=0)continue;
        if(!var->process(1))return false;
    }
    return true;
}

bool Sat_system::less(Vertex* a, Vertex* b){
    if(a==b)return false;
    return lvl_ind_ind_to_var[a->level][a->_index][b->_index]->truth==1;
}

void Sat_system::create(GraphBase<Level,Vertex>* g){
    vector<Level*> levels= g->levels;
    int l=levels.size();
    
    for(int i=0;i<l;i++){
        index_to_vertex.push_back({});
        int index=0;
        for(auto v:levels[i]->verticles){
            v->_index=index++;
            index_to_vertex[i].push_back(v);
        }
    }

    for(int i=0;i<l;i++){
        int k=index_to_vertex[i].size();
        lvl_ind_ind_to_var.push_back(vector<vector<Sat_var*>>(k,vector<Sat_var*>(k)));
        
        for(int j=0;j<k;j++){
            for(int jj=0;jj<k;jj++){
                if(j==jj)continue;
                Vertex* v1 = index_to_vertex[i][j];
                Vertex* v2 = index_to_vertex[i][jj];
                Sat_var* sat_variable = new Sat_var(v1,v2);
                variables.push_back(sat_variable);
                lvl_ind_ind_to_var[i][j][jj]=sat_variable;
                if(j>jj){
                    Sat_var * anti_var=lvl_ind_ind_to_var[i][jj][j];
                    sat_variable->inv = anti_var;
                    anti_var->inv = sat_variable;
                }
            }
        }

    }


    for(int i=0;i<l-1;i++){
        int k=index_to_vertex[i].size();
        for(int j=0;j<k;j++){
            for(int jj=0;jj<k;jj++){
                if(j==jj)continue;
                Vertex* v1 = index_to_vertex[i][j];
                Vertex* v2 = index_to_vertex[i][jj];
                Sat_var* v_var = lvl_ind_ind_to_var[i][j][jj];
                for(auto x1:v1->down_neighbors){
                    for(auto x2:v2->down_neighbors){
                        if(x1==x2)continue;
                        Sat_var* x_var = lvl_ind_ind_to_var[i+1][x1->_index][x2->_index];
                        v_var->equiv.push_back(x_var);
                        x_var->equiv.push_back(v_var);
                    }    
                }
                
            }
        }

    }
    
    

}