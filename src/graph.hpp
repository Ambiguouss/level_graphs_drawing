#pragma once


template <typename LevelType, typename VertexType>
struct GraphBase{
    vector<LevelType*> levels;

    vector<LevelType*> get_levels(){
        return levels;
    }
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
    GraphBase<LevelType,VertexType> evenify();
    void draw(string filename="");
    template <typename L, typename V>
    friend GraphBase<L,V> input_graph();
};
