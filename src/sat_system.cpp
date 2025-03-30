#include <bits/stdc++.h>
#pragma once
using namespace std;
#include "graph.cpp"
#include "sat_var.cpp"

struct Sat_system{
    vector<Sat_var*> variables;
    vector<vector<Vertex*>> index_to_vertex;
    vector<vector<vector<Sat_var*>>> lvl_ind_ind_to_var;
    
    ~Sat_system(){
        for(auto x:variables){
            delete x;
        }
    }

    

    bool solve();
    
    void create(GraphBase<Level,Vertex>*g);
    
};

bool Sat_system::solve(){
    for(auto var : variables){
        if(var->truth!=0)continue;
        if(!var->process(1))return false;
    }
    return true;
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
    /*for(auto var: variables){
        cout<<var->first->level<<' ';
        cout<<var->first->_index<<' ';
        cout<<var->second->_index<<": \n";
        for(auto e: var->equiv){
            cout<<"equiv ";
            cout<<e->first->level<<' ';
            cout<<e->first->_index<<' ';
            cout<<e->second->_index<<"\n";
        }
    }*/
    

}