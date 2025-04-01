#pragma once
#include "bits/stdc++.h"
using namespace std;
#include "graph.hpp"
#include "sat_system.hpp"

template <typename LevelType, typename VertexType>
GraphBase<LevelType,VertexType> GraphBase<LevelType,VertexType>::evenify(){
    Sat_system sat;
    sat.create(this);
    if(!sat.solve())throw std::invalid_argument("Graph can't be drawn planarly");
    for(auto v:sat.variables){
        cout<<v->truth<<'\n';
    }



    GraphBase<ExtraLevel,ExtraVertex> extra_graph = split_verticles();
    GraphBase<LevelType,VertexType> hanani_tutte_graph;
    
    vector<vector<Vertex*>> index_to_vertex;
    vector<vector<Vertex*>> new_index_to_vertex;
    vector<ExtraLevel*> extra_levels= extra_graph.levels;
    int l=extra_levels.size();
    
    for(int i=0;i<l;i++){
        index_to_vertex.push_back({});
        int index=0;
        for(auto v:extra_levels[i]->verticles){
            v->_index=index++;
            index_to_vertex[i].push_back(v);
        }
    }
    
    for(int i=0;i<extra_graph.levels.size();i++){
        
        cout<<"i: "<<i<<'\n';
        ExtraLevel* lvl = extra_graph.levels[i];
        new_index_to_vertex.push_back(vector<Vertex*>(lvl->verticles.size()));
        VertexType* special = lvl->original_vertex;
        ExtraVertex* main_vertex;
        LevelType* new_lvl = new LevelType();
        hanani_tutte_graph.levels.push_back(new_lvl);
        vector<pair<VertexType*,int>> before,after;
        for(auto v : lvl->verticles){
            cout<<"inside\n";
            VertexType* org_v = v->original;
            if(sat.less(org_v,special))before.push_back({org_v,v->_index});
            else if(sat.less(special,org_v))after.push_back({org_v,v->_index});
            else main_vertex = v;
        }
        for(auto [v,ind]:before){
            ExtraVertex* ext_ver = new ExtraVertex(v->label,i,v);
            ext_ver->dummy=true;
            ext_ver->_index=ind;
            new_index_to_vertex[i][ind]=ext_ver;
            new_lvl->verticles.push_back(ext_ver);
        }
        ExtraVertex* ext_org = new ExtraVertex(special->label,i,special);
        ext_org->_index=main_vertex->_index;
        new_index_to_vertex[i][ext_org->_index]=ext_org;
        new_lvl->verticles.push_back(ext_org);
        for(auto [v,ind]:after){
            ExtraVertex* ext_ver = new ExtraVertex(v->label,i,v);
            ext_ver->dummy=true;
            ext_ver->_index=ind;
            new_index_to_vertex[i][ind]=ext_ver;
            new_lvl->verticles.push_back(ext_ver);
        }
    }

    for(int i=0;i<hanani_tutte_graph.levels.size();i++){
        for(int j=0;j<hanani_tutte_graph.levels[i]->verticles.size();j++){
            Vertex* v = hanani_tutte_graph.levels[i]->verticles[j];
            cout<<i<<' '<<v->_index<<'\n';
            Vertex* old = index_to_vertex[i][v->_index];
            for(auto nei : old->down_neighbors){
                Vertex* new_nei = new_index_to_vertex[i+1][nei->_index];
                v->down_neighbors.push_back(new_nei);
            }
        }
    }

    for(auto lvl:hanani_tutte_graph.levels){
        for(auto v:lvl->verticles){
            cout<<v->label<<' ';
        }
        cout<<'\n';
    }


    return hanani_tutte_graph;
}