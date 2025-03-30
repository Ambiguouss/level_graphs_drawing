#include <bits/stdc++.h>
#pragma once
#include "vertex.cpp"
#include "level.cpp"

using namespace std;

template <typename LevelType, typename VertexType>
struct GraphBase{
    vector<LevelType*> levels;

    LevelType* get_level(int k);
    void create_edge(int top_level,int top_k,int bot_k);
    void create_edge(VertexType* v1,VertexType* v2);
    LevelType* create_level();
    GraphBase<ExtraLevel,ExtraVertex> split_verticles();
    ~GraphBase(){
        for(auto x:levels){
            delete x;
        }
    }

    void draw(string filename="");
    template <typename L, typename V>
    friend GraphBase<L,V> input_graph();
};


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
}
template <typename LevelType, typename VertexType>
void GraphBase<LevelType,VertexType>::create_edge(VertexType* v1,VertexType* v2){
    v1->add_down_neighbor(v2);
}
template<typename LevelType,typename VertexType>
GraphBase<LevelType,VertexType> input_graph(){
    GraphBase<LevelType,VertexType> g;
    int number_of_lvl;
    cin>>number_of_lvl;
    for(int i=0;i<number_of_lvl;i++){
        g.levels.push_back(new Level());
    }
    for(int l=0;l<g.levels.size();l++){
        LevelType* lvl = g.levels[l];
        int no_vertex;
        cin>>no_vertex;
        for(int i=0;i<no_vertex;i++){
            lvl->verticles.push_back(new Vertex(to_string(i+1),l));
            lvl->permutation.push_back(i);
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
                ExtraVertex* edge_vertex = new ExtraVertex(prev->label,lvl_cnt,original_vertex);
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


