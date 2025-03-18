#include <bits/stdc++.h>
#pragma once
#include "vertex.cpp"
using namespace std;

struct Edge{
    Vertex *v_top,*v_bot;

    Edge(Vertex* _v1, Vertex* _v2) : v_top(_v1), v_bot(_v2) {}

    Vertex* get_top(){
        return v_top;
    }
    Vertex* get_bot(){
        return v_bot;
    }
};