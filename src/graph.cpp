#include <bits/stdc++.h>
#pragma once
#include "vertex.cpp"
#include "level.cpp"

using namespace std;

struct Graph{
    vector<Level*> levels;

    Level* get_level(int k);
    void create_edge(int top_level,int top_k,int bot_k);
    void create_edge(Vertex* v1,Vertex* v2);
    Level* create_level();
    Graph split_verticles();
    ~Graph(){
        for(auto x:levels){
            delete x;
        }
    }

    void draw(string filename);
    friend Graph input_graph();


};

Level* Graph::get_level(int k){
    return levels[k];
}

Level* Graph::create_level(){
    Level* new_lvl = new Level();
    levels.push_back(new_lvl);
    return new_lvl;
}

void Graph::create_edge(int top_level_int,int top_k,int bot_k){
    Level* top_level=get_level(top_level_int);
    Level* bot_level=get_level(top_level_int+1);
    Vertex* top_v = top_level->get_vertex(top_k);
    Vertex* bot_v = bot_level->get_vertex(bot_k);
    top_v->add_down_neighbor(bot_v);
}
void Graph::create_edge(Vertex* v1,Vertex* v2){
    v1->add_down_neighbor(v2);
}

Graph input_graph(){
    Graph g;
    int number_of_lvl;
    cin>>number_of_lvl;
    for(int i=0;i<number_of_lvl;i++){
        g.levels.push_back(new Level());
    }
    for(int l=0;l<g.levels.size();l++){
        Level* lvl = g.levels[l];
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

Graph Graph::split_verticles(){
    Graph new_graph;
    int lvl_cnt=0;
    list<pair<Vertex*,Vertex*>>active_edge;//first element is active vertex, second is endpoint in original graph
    
    auto process_active_edges = [&](Vertex* original_vertex,Vertex* new_vertex,Level* new_lvl){
        for(auto it = active_edge.begin();it!=active_edge.end();it){
            auto& [prev,curr] = *it;
            if(curr == original_vertex){
                new_graph.create_edge(prev,new_vertex);
                it=active_edge.erase(it);
            }else{
                Vertex* edge_vertex = new Vertex(prev->label,lvl_cnt);
                new_graph.create_edge(prev,edge_vertex);
                new_lvl->add_vertex(edge_vertex);
                prev = edge_vertex;
                ++it;
            }
        }

    };
    
    for(auto lvl:levels){
        for(int i=0;i<lvl->verticles.size();i++){
            Level* new_lvl = new_graph.create_level();

            auto vertex=lvl->verticles[i];
            Vertex* new_vertex =new Vertex(vertex->label,lvl_cnt); 
            process_active_edges(vertex,new_vertex,new_lvl);

            new_lvl->add_vertex(new_vertex);

            active_edge.insert(active_edge.end(),{new_vertex,vertex});

            lvl_cnt++;
        }
        for(int i=0;i<lvl->verticles.size();i++){
            Level* new_lvl = new_graph.create_level();

            auto vertex=lvl->verticles[i];
            Vertex* new_vertex =new Vertex(vertex->label,lvl_cnt);
            process_active_edges(vertex,new_vertex,new_lvl);

            for(Vertex* neighbour : vertex->get_down_neighbors()){
                active_edge.insert(active_edge.end(),{new_vertex,neighbour});
            }

            new_lvl->add_vertex(new_vertex);
            lvl_cnt++;
        }
    }
    return new_graph;
}


