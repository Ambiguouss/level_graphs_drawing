#include <bits/stdc++.h>
#pragma once
#include "vertex.cpp"
#include "level.cpp"
#include "edge.cpp"
#include "levelEdge.cpp"

using namespace std;

struct Graph{
    vector<Level*> levels;
    vector<LevelEdges*> edges;

    Level* get_level(int k);
    Edge* create_edge(int top_level,int top_k,int bot_k);

    ~Graph(){
        for(auto x:levels){
            delete x;
        }
        for(auto x:edges){
            delete x;
        }
    }


    friend Graph input_graph();


};

Level* Graph::get_level(int k){
    return levels[k];
}

Edge* Graph::create_edge(int top_level_int,int top_k,int bot_k){
    Level* top_level=get_level(top_level_int);
    Level* bot_level=get_level(top_level_int+1);
    Vertex* top_v = top_level->get_vertex(top_k);
    Vertex* bot_v = bot_level->get_vertex(bot_k);
    Edge* e = new Edge(top_v,bot_v);
    return e;
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
            lvl->verticles.push_back(new Vertex(i+1,l));
            lvl->permutation.push_back(i);
        }
    }
    for(int i=0;i<number_of_lvl-1;i++){
        g.edges.push_back(new LevelEdges());
    }
    for(int i=0;i<number_of_lvl-1;i++){
        int no_edges;
        cin>>no_edges;
        for(int j=0;j<no_edges;j++){
            int top_k,bot_k;
            cin>>top_k>>bot_k;
            g.edges[i]->edges.push_back(g.create_edge(i,top_k,bot_k));
        }
    }
    return g;
}
