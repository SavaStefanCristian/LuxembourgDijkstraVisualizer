#include "graph.h"
#include <QDebug>
#include <QFile>
#include <QXmlStreamReader>
#include <queue>

Graph::Graph() {
    m_minLatitude = 180;
    m_maxLatitude = -180;
    m_minLongitude = 180;
    m_maxLongitude = -180;
}

Graph::~Graph()
{
    for(size_t i = 0; i < m_nodes.size(); i++) {
        if(m_nodes[i]) delete m_nodes[i];
    }
}

void Graph::BuildFromXML(const QString& path)
{
    QFile file(QString{path});
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() <<"Could not open file!\n";
        return;
    }

    QXmlStreamReader xml(&file);

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();

        if (xml.isStartElement()) {
            if (xml.name() == QString{"node"}) {
                int id = xml.attributes().value("id").toInt();
                if(id < 0 || id != m_nodes.size()) {
                    qDebug() << "Invalid node id!\n";
                    return;
                }
                float latitude = -xml.attributes().value("latitude").toFloat() / 100000;
                if(latitude > m_maxLatitude) m_maxLatitude = latitude;
                if(latitude < m_minLatitude) m_minLatitude = latitude;
                float longitude = xml.attributes().value("longitude").toFloat() / 10000;
                if(longitude > m_maxLongitude) m_maxLongitude = longitude;
                if(longitude < m_minLongitude) m_minLongitude = longitude;
                m_nodes.push_back(new Node{id, latitude, longitude});
            }
            else if (xml.name() == QString{"arc"}) {
                int node1Id = xml.attributes().value("from").toInt();
                int node2Id = xml.attributes().value("to").toInt();
                int lenght = xml.attributes().value("length").toInt();
                if(node1Id < 0 || node1Id >= m_nodes.size() || node2Id < 0 || node2Id >= m_nodes.size()) {
                    qDebug() <<"Invalid arc ids!\n";
                    return;
                }
                m_edges[m_nodes[node1Id]][m_nodes[node2Id]] = lenght;
            }
        }
    }
    file.close();
}

void Graph::FitToScreen(QSize screenSize)
{
    int width = screenSize.width() * 0.95f;
    int height = screenSize.height() * 0.95f;
    int verticalPadding = (screenSize.height()-height)/2;
    int horizontalPadding = (screenSize.width()-width)/2;
    int scalingSize = 0;
    if(width > height) {
        horizontalPadding += (width-height)/2;
        scalingSize = height;
    }
    else {
        verticalPadding += (height-width)/2;
        scalingSize = width;
    }
    for(Node* node : m_nodes) {
        float latitude = node->GetLatitude();
        float longitude = node->GetLongitude();
        float y = (latitude - m_minLatitude) / (m_maxLatitude - m_minLatitude);
        float x = (longitude - m_minLongitude) / (m_maxLongitude - m_minLongitude);
        node->SetPos({horizontalPadding + scalingSize*x,verticalPadding + scalingSize*y});
    }
}

std::vector<Node*> Graph::Dijkstra(Node *start, Node *end)
{
    if(m_edges.find(start)==m_edges.end() || m_edges.find(end) == m_edges.end()) return {};
    if (start == end) return {start};
    std::unordered_map<Node*,std::pair<Node*,int>> parentAndLength;
    parentAndLength[start] = {start,0};
    std::priority_queue<std::pair<Node*,int>,std::vector<std::pair<Node*,int>>, PairFunctor> openList;
    openList.push({start,0});
    while(!openList.empty()) {
        auto [currNode, cost] = openList.top();
        openList.pop();
        auto it = m_edges.find(currNode);
        if (it == m_edges.end()) continue;
        for (const auto& [connection, length] : it->second) {
            int newCost = cost + length;
            if (parentAndLength.find(connection) == parentAndLength.end() || newCost < parentAndLength[connection].second) {
                parentAndLength[connection] = {currNode, newCost};
                openList.push({connection, newCost});
            }
        }
    }
    std::vector<Node*> path;
    if(parentAndLength.find(end)==parentAndLength.end()) return {};
    Node* curr = end;
    path.push_back(end);
    while(curr!=start) {
        Node*& parent =parentAndLength[curr].first;
        path.push_back(parent);
        curr = parent;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

