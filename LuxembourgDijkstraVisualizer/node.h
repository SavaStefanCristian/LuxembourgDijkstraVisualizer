#ifndef NODE_H
#define NODE_H
#pragma once
#include <QPoint>

class Node
{
public:
    Node();
    Node(int id, float latitude, float longitude) : m_id{id}, m_latitude{latitude}, m_longitude{longitude} {}
    void SetId(int id) {m_id = id;}
    void SetPos(QPointF pos) {m_pos = pos;}
    QPointF GetPos() {return m_pos;}
    int GetId() {return m_id;}
    float GetLatitude() {return m_latitude;}
    float GetLongitude() {return m_longitude;}
private:
    int m_id;
    QPointF m_pos;
    float m_latitude;
    float m_longitude;
};

#endif // NODE_H
