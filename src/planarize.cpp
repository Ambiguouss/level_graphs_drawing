#include <bits/stdc++.h>
using namespace std;
#include "graph.hpp"
#include "graph_draw.cpp"
#include "evenify.cpp"
#include "sat_system.cpp"


Vertex* get_true_vertex(Level* lvl){
    for(auto x:lvl->verticles){
        if(!x->dummy)return x;
    }
    return nullptr;
}

pair<int,map<Vertex*,int>> components(GraphBase<Level,Vertex>* graph){

    int lvl_cnt=graph->levels.size();
    map<Vertex*,int>visited;
    int num_components=0;
    map<Vertex*,int> vertex_to_component;
    function<int(Vertex*,int,Vertex*)> dfs = [&](Vertex* v,int lvl,Vertex* par){
        if(vertex_to_component[v]==0) vertex_to_component[v]=num_components+1;
        if(v->dummy){
            if(v->down_neighbors[0]==par)return dfs(v->up_neighbors[0],lvl-1,v);
            else return dfs(v->down_neighbors[0],lvl+1,v);

        } 
        if(visited[v]==1) return 0;
        visited[v]=1;
        for(auto x:v->down_neighbors){
            dfs(x,lvl+1,v);
        }
        for(auto x:v->up_neighbors){
            dfs(x,lvl-1,v);
        }
        return 1;

    };

    
    for(int i=0;i<lvl_cnt;i++){
        for(auto v:graph->levels[i]->verticles){
            num_components+=dfs(v,i,nullptr);
        }
    }
    return {num_components,vertex_to_component};
}

GraphBase<Level,Vertex>* planarize(GraphBase<Level,Vertex>* graph){
    vector<GraphBase<Level,Vertex>*> splited_components = graph->split_components();
    if(splited_components.size()>1){
        GraphBase<Level,Vertex>* result = new GraphBase<Level,Vertex>();
        vector<GraphBase<Level,Vertex>*> planar_components;

        for(auto g : splited_components){
            GraphBase<Level,Vertex>* d = planarize(g);
            planar_components.push_back(d);
        }
        int lvl_cnt=graph->levels.size();
        std::map<Vertex*,Vertex*>new_to_old;
        std::map<Vertex*,Vertex*> old_to_new;
        
        for(int i=0;i<lvl_cnt;i++){
            Level* new_lvl = new Level();
            result->levels.push_back(new_lvl);
            for(auto g:planar_components){
                for(auto v:g->get_level(i)->verticles){
                    Vertex* new_vert = new Vertex(v->label,v->level);
                    new_vert->dummy=v->dummy;
                    new_to_old[new_vert] = v;
                    old_to_new[v]=new_vert;
                    new_lvl->add_vertex(new_vert);
                }
            }
        }
        for(auto [new_v,old_v]:new_to_old){
            for(auto old_neig:old_v->down_neighbors){
                result->create_edge(new_v,old_to_new[old_neig]);
            }
        }
        for(auto s:splited_components){
            delete s;
        }
        return result;

    }
    for(auto s:splited_components){
        delete s;
    }
    return graph;
}

int main(){
    GraphBase<Level,Vertex> g=input_graph<Level,Vertex>();
    
    Sat_system s;
    s.create(&g);
    bool res = s.solve();
    cout<<res<<'\n';
    GraphBase<Level,Vertex> d=g.evenify();
    //vector<GraphBase<Level,Vertex>*> v= d.split_components();
    GraphBase<Level,Vertex>* p=planarize(&d);
    /*cout<<v.size()<<'\n';
    v[0]->draw("xdd.tex");
    v[1]->draw("xdd2.tex");*/
    
    p->draw("xddfull.tex");
    delete p;
    /*for(auto s:v){
        delete s;
    }*/
    

    /*GraphBase<Level,Vertex> h = g.copy();
    Sat_system t;
    t.create(&h);
    res = t.solve();
    cout<<res<<'\n';
    GraphBase<Level,Vertex> k=h.evenify();
    cout<<components(&k).first<<'\n';*/
    
}