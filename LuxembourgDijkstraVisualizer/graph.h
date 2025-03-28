#ifndef GRAPH_H
#define GRAPH_H
#pragma once

#include "node.h"
#include <vector>
#include <unordered_map>
#include <Qstring>
#include <QSizeF>


class Graph
{
public:
    Graph();
    ~Graph();
    const std::vector<Node*>& GetNodes() {return m_nodes;}
    const std::unordered_map<Node*,std::unordered_map<Node*,int>>& GetEdges() {return m_edges;}
    void BuildFromXML(const QString& path);
    void FitToScreen(QSize screenSize);
    std::vector<Node*> Dijkstra(Node* start, Node* end);

private:
    std::vector<Node*> m_nodes;
    std::unordered_map<Node*,std::unordered_map<Node*,int>> m_edges;
    float m_minLatitude, m_maxLatitude, m_minLongitude, m_maxLongitude;

    class PairFunctor {
    public:
        const bool operator()(const std::pair<Node*,int>& lhs, const std::pair<Node*,int>& rhs) const {
            return lhs.second > rhs.second;
        }
    };
};

#endif // GRAPH_H
