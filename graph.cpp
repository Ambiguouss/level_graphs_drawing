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
    Graph split_verticles();
    void add_edge(int top_level,int top_k,int bot_k);
    ~Graph(){
        for(auto x:levels){
            delete x;
        }
        for(auto x:edges){
            delete x;
        }
    }

    void draw(string filename);
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
    top_v->add_down_neighbor(bot_v);
    Edge* e = new Edge(top_v,bot_v);
    return e;
}

void Graph::add_edge(int top_level_int,int top_k,int bot_k){
    edges[top_level_int]->edges.push_back(create_edge(top_level_int,top_k,bot_k));
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

Graph Graph::split_verticles(){
    Graph new_graph;
    int lvl_cnt=0;
    //list<Vertex*>active_vertex;
    list<pair<Vertex*,Vertex*>>active_edge;//first element is active vertex, second is endpoint in original graph
    for(auto lvl:levels){
        for(int i=0;i<lvl->verticles.size();i++){
            Level* new_lvl = new Level();
            LevelEdges* level_edges = new LevelEdges();
            new_graph.levels.push_back(new_lvl);
            new_graph.edges.push_back(level_edges);

            auto vertex=lvl->verticles[i];
            Vertex* new_vertex =new Vertex(vertex->label,lvl_cnt); 
            for(auto it = active_edge.begin();it!=active_edge.end();it){
                auto& [prev,curr] = *it;
                if(curr == vertex){
                    level_edges->edges.push_back(new Edge(prev,new_vertex));
                    it=active_edge.erase(it);
                }else{
                    cout<<prev->label<<' ';
                    Vertex* edge_vertex = new Vertex(prev->label,lvl_cnt);
                    level_edges->edges.push_back(new Edge(prev,edge_vertex));
                    new_lvl->add_vertex(edge_vertex);
                    prev = edge_vertex;
                    ++it;
                }
            }
            cout<<'\n';


            new_lvl->add_vertex(new_vertex);

            active_edge.insert(active_edge.end(),{new_vertex,vertex});

            lvl_cnt++;
        }
        for(int i=0;i<lvl->verticles.size();i++){
            Level* new_lvl = new Level();
            LevelEdges* level_edges = new LevelEdges();
            new_graph.levels.push_back(new_lvl);
            new_graph.edges.push_back(level_edges);

            auto vertex=lvl->verticles[i];
            Vertex* new_vertex =new Vertex(vertex->label,lvl_cnt);
            for(auto it = active_edge.begin();it!=active_edge.end();it){
                auto& [prev,curr] = *it;
                if(curr == vertex){
                    level_edges->edges.push_back(new Edge(prev,new_vertex));
                    it=active_edge.erase(it);
                }else{
                    Vertex* edge_vertex = new Vertex(prev->label,lvl_cnt);
                    level_edges->edges.push_back(new Edge(prev,edge_vertex));
                    new_lvl->add_vertex(edge_vertex);
                    prev = edge_vertex;
                    ++it;
                }
            }

            for(Vertex* neighbour : vertex->get_down_neighbors()){
                active_edge.insert(active_edge.end(),{new_vertex,neighbour});
            }

            new_lvl->add_vertex(new_vertex);
            lvl_cnt++;
        }


      
    }
    return new_graph;
}


void Graph::draw(string filename=""){
    if(filename==""){
        filename="images/test.tex";
        return;
    }
    ofstream outFile(filename);

    if (!outFile) {
        cerr << "Error opening file!" << endl;
        return;
    }

    // Start the TikZ picture
    outFile << "\\documentclass{article}\n";
    outFile << "\\usepackage{tikz}\n";
    outFile << "\\begin{document}\n";
    outFile << "\\resizebox{!}{\\textheight}{%\n";
    outFile << "\\begin{tikzpicture}[]\n";



    for(int lvl=0;lvl<levels.size();lvl++){
        Level* level = levels[lvl];
        for(int i=0;i<level->verticles.size();i++){
            Vertex* v = level->get_permuted_vertex(i);
            outFile << "\\node[draw, circle] (" <<v  << ") at (" 
                << i << "," << -lvl
                << ") {" << v->label << "};\n";
        }
    }
    

    // Draw edges
    for (auto level_edges : edges) {
        for(auto edge : level_edges->edges){
            outFile << "\\draw (" << edge->get_top()<< ") -- (" 
            << edge->get_bot()<< ");\n";

        }
    }

    // End TikZ picture
    outFile << "\\end{tikzpicture}%\n}\n";
    outFile << "\\end{document}\n";

    outFile.close();
    cout << "TikZ file generated: " << filename << endl;
}