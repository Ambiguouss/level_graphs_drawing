#include <bits/stdc++.h>
#pragma once
#include "vertex.cpp"
#include "level.cpp"
#include "sat_system.hpp"
#include "graph.hpp"

using namespace std;


template <typename LevelType, typename VertexType>
LevelType* GraphBase<LevelType,VertexType>::get_level(int k){
    return levels[k];
}

template <typename LevelType, typename VertexType>
LevelType* GraphBase<LevelType,VertexType>::create_level(){
    LevelType* new_lvl = new LevelType();
    levels.push_back(new_lvl);
    return new_lvl;
}

template <typename LevelType, typename VertexType>
void GraphBase<LevelType,VertexType>::create_edge(int top_level_int,int top_k,int bot_k){
    LevelType* top_level=get_level(top_level_int);
    LevelType* bot_level=get_level(top_level_int+1);
    VertexType* top_v = top_level->get_vertex(top_k);
    VertexType* bot_v = bot_level->get_vertex(bot_k);
    top_v->add_down_neighbor(bot_v);
    bot_v->add_up_neighbor(top_v);
}
template <typename LevelType, typename VertexType>
void GraphBase<LevelType,VertexType>::create_edge(VertexType* v1,VertexType* v2){
    v1->add_down_neighbor(v2);
    v2->add_up_neighbor(v1);
}
template<typename LevelType,typename VertexType>
GraphBase<LevelType,VertexType> input_graph(){
    GraphBase<LevelType,VertexType> g;
    int number_of_lvl;
    cin>>number_of_lvl;
    for(int i=0;i<number_of_lvl;i++){
        g.create_level();
    }
    for(int l=0;l<g.levels.size();l++){
        LevelType* lvl = g.get_level(l);
        int no_vertex;
        cin>>no_vertex;
        for(int i=0;i<no_vertex;i++){
            lvl->add_vertex(new Vertex(to_string(i+1),l));
        }
    }
    for(int i=0;i<number_of_lvl-1;i++){
        int no_edges;
        cin>>no_edges;
        for(int j=0;j<no_edges;j++){
            int top_k,bot_k;
            cin>>top_k>>bot_k;
            g.create_edge(i,top_k,bot_k);
        }
    }
    return g;
}

template <typename LevelType, typename VertexType>
GraphBase<ExtraLevel,ExtraVertex> GraphBase<LevelType,VertexType>::split_verticles(){
    GraphBase<ExtraLevel,ExtraVertex> new_graph;
    int lvl_cnt=0;
    list<pair<ExtraVertex*,VertexType*>>active_edge;//first element is active vertex, second is endpoint in original graph
    
    auto process_active_edges = [&](VertexType* original_vertex,ExtraVertex* new_vertex,ExtraLevel* new_lvl){
        for(auto it = active_edge.begin();it!=active_edge.end();it){
            auto& [prev,curr] = *it;
            if(curr == original_vertex){
                new_graph.create_edge(prev,new_vertex);
                it=active_edge.erase(it);
            }else{
                Vertex* to_be_origin;
                if(original_vertex->level==curr->level){
                    to_be_origin=curr;
                }else{
                    to_be_origin=prev->original;
                }
                ExtraVertex* edge_vertex = new ExtraVertex(prev->label,lvl_cnt,to_be_origin);
                new_graph.create_edge(prev,edge_vertex);
                new_lvl->add_vertex(edge_vertex);
                prev = edge_vertex;
                ++it;
            }
        }

    };
    
    for(auto lvl:levels){
        for(int i=0;i<lvl->verticles.size();i++){
            
            auto vertex=lvl->verticles[i];
            

            ExtraLevel* new_lvl = new ExtraLevel(lvl,vertex);
            new_graph.levels.push_back(new_lvl);

            ExtraVertex* new_vertex =new ExtraVertex(vertex->label,lvl_cnt,vertex); 
            process_active_edges(vertex,new_vertex,new_lvl);

            new_lvl->add_vertex(new_vertex);

            active_edge.insert(active_edge.end(),{new_vertex,vertex});

            lvl_cnt++;
        }
        for(int i=0;i<lvl->verticles.size();i++){
            auto vertex=lvl->verticles[i];
            ExtraLevel* new_lvl = new ExtraLevel(lvl,vertex);
            new_graph.levels.push_back(new_lvl);
            ExtraVertex* new_vertex =new ExtraVertex(vertex->label,lvl_cnt,vertex);
            process_active_edges(vertex,new_vertex,new_lvl);

            for(VertexType* neighbour : vertex->get_down_neighbors()){
                active_edge.insert(active_edge.end(),{new_vertex,neighbour});
            }

            new_lvl->add_vertex(new_vertex);
            lvl_cnt++;
        }
    }


    return new_graph;
}


template <typename LevelType, typename VertexType>
GraphBase<LevelType,VertexType>* GraphBase<LevelType,VertexType>::copy(){
    GraphBase<LevelType,VertexType>* result = new GraphBase<LevelType,VertexType>();
    map<VertexType*,VertexType*>new_to_old;
    map<VertexType*,VertexType*> old_to_new;
    for(int i=0;i<levels.size();i++){
        LevelType* lvl = levels[i];
        LevelType* new_lvl = new LevelType();
        result->levels.push_back(new_lvl);
        for(auto v : lvl->verticles){
            VertexType* new_ver = new VertexType(v->label,v->level);
            new_lvl->verticles.push_back(new_ver);
            old_to_new[v]=new_ver;
            new_to_old[new_ver]=v;
        }
        
    }

    for(int i=0;i<result->levels.size();i++){
        for(int j=0;j<result->levels[i]->verticles.size();j++){
            VertexType* v = result->levels[i]->verticles[j];
            VertexType* old = new_to_old[v];
            for(auto nei : old->down_neighbors){
                Vertex* new_nei = old_to_new[nei];
                v->down_neighbors.push_back(new_nei);
                new_nei->up_neighbors.push_back(v);
            }
        }
    }
    return result;
}

template <typename LevelType, typename VertexType>
vector<GraphBase<LevelType,VertexType>*> GraphBase<LevelType,VertexType>::split_components(){
    vector<GraphBase<LevelType,VertexType>*> result;

    map<Vertex*,int> visited;
    function<void(Vertex*,int,Vertex*)> dfs = [&](Vertex* v,int lvl,Vertex* par){
        if(visited[v]!=0)return;
        visited[v]=result.size();
        //Vertex* new_ver= new Vertex(v->label,v->level);
        //new_ver->dummy=v->dummy;
        //result[result.size()-1]->get_level(lvl)->verticles.push_back(new_ver);
        /*if(par!=nullptr){
            if(v->level<par->level){
                result[result.size()-1]->create_edge(new_ver,new_par);
            }else{
                result[result.size()-1]->create_edge(new_par,new_ver);
            }
        }*/
        for(auto x:v->down_neighbors){
            dfs(x,lvl+1,v);
        }
        for(auto x:v->up_neighbors){
            dfs(x,lvl-1,v);
        }

    };

    for(int i=0;i<levels.size();i++){
        for(auto v:levels[i]->verticles){
            if(!visited[v]){
                GraphBase<LevelType,VertexType>* g= new GraphBase<LevelType,VertexType>();
                result.push_back(g);
                dfs(v,i,nullptr);
                for(int j=0;j<levels.size();j++){
                    LevelType* new_lvl = new LevelType();
                    g->levels.push_back(new_lvl);
                }
            }
        }
    }
    map<VertexType*,VertexType*>new_to_old;
    map<VertexType*,VertexType*> old_to_new;

    for(int i=0;i<levels.size();i++){
        for(auto v:levels[i]->verticles){
            Vertex* new_ver= new Vertex(v->label,v->level);
            new_ver->dummy=v->dummy;
            result[visited[v]-1]->levels[i]->verticles.push_back(new_ver);
            new_to_old[new_ver]=v;
            old_to_new[v]=new_ver;
        }
    }

    for(int i=0;i<levels.size();i++){
        for(int j=0;j<levels[i]->verticles.size();j++){
            VertexType* v = levels[i]->verticles[j];
            VertexType* new_v = old_to_new[v];
            for(auto nei : v->down_neighbors){
                Vertex* new_nei = old_to_new[nei];
                new_v->down_neighbors.push_back(new_nei);
                new_nei->up_neighbors.push_back(new_v);
            }
        }
    }


    return result;

    /*map<VertexType*,VertexType*>new_to_old;
    map<VertexType*,VertexType*> old_to_new;
    for(int i=0;i<levels.size();i++){
        LevelType* lvl = levels[i];
        LevelType* new_lvl = new LevelType();
        result.levels.push_back(new_lvl);
        for(auto v : lvl->verticles){
            VertexType* new_ver = new VertexType(v->label,v->level);
            new_lvl->verticles.push_back(new_ver);
            old_to_new[v]=new_ver;
            new_to_old[new_ver]=v;
        }
        
    }

    for(int i=0;i<result.levels.size();i++){
        for(int j=0;j<result.levels[i]->verticles.size();j++){
            VertexType* v = result.levels[i]->verticles[j];
            VertexType* old = new_to_old[v];
            for(auto nei : old->down_neighbors){
                Vertex* new_nei = old_to_new[nei];
                v->down_neighbors.push_back(new_nei);
                new_nei->up_neighbors.push_back(v);
            }
        }
    }*/
    return result;
}