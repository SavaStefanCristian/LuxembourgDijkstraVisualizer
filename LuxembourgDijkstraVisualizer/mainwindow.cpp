#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>

#include <QWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    isFirstNodeSelected = false;
    QString xmlPath{"Harta_Luxemburg.xml"};
    m_graph.BuildFromXML(xmlPath);
    m_graph.FitToScreen(size());
    m_radius = fmin(size().height(),size().width())/200;
    update();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mouseReleaseEvent(QMouseEvent *m)
{
    if(m->button() == Qt::LeftButton)
    {
        std::vector<Node*> nodes = m_graph.GetNodes();
        std::unordered_map<Node*, std::unordered_map<Node*,int>> edges = m_graph.GetEdges();
        for(Node* n : nodes) {
            if((n->GetPos().x() - m->pos().x())*(n->GetPos().x() - m->pos().x())+
                    (n->GetPos().y() - m->pos().y())*(n->GetPos().y() - m->pos().y())
                        < m_radius*m_radius) {
                if(!isFirstNodeSelected) {
                    isFirstNodeSelected = true;
                    m_firstSelectedNode = n;
                    m_secondSelectedNode = nullptr;
                    m_shortestPath.clear();
                }
                else {
                    if(m_firstSelectedNode != n) {
                        m_secondSelectedNode = n;
                        m_shortestPath = m_graph.Dijkstra(m_firstSelectedNode,m_secondSelectedNode);
                    }
                    isFirstNodeSelected = false;
                }
                update();
                break;
            }
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *ev)
{
    QPainter p{this};
    std::vector<Node*> nodes = m_graph.GetNodes();
    std::unordered_map<Node*,std::unordered_map<Node*,int>> edges = m_graph.GetEdges();
    if(m_shortestPath.empty())
        p.setPen(Qt::white);
    else
        p.setPen(Qt::gray);
    for(auto& [node,connections] : edges)
    {
        int x1 = node->GetPos().x(), y1 = node->GetPos().y();
        for(auto [currCon,cost] : connections) {
            int x2 = currCon->GetPos().x(), y2 = currCon->GetPos().y();
            p.drawLine(x1,y1,x2,y2);
        }
    }
    for(auto& n : nodes)
    {
        if(m_firstSelectedNode == n) {
            QRectF r{n->GetPos().x()-m_radius,n->GetPos().y()-m_radius, m_radius*2, m_radius*2};
            p.setPen(QPen(Qt::green, m_radius/3));
            p.drawEllipse(r);
            p.drawPoint(n->GetPos());
            continue;
        }
        if(m_secondSelectedNode == n) {
            QRectF r{n->GetPos().x()-m_radius,n->GetPos().y()-m_radius, m_radius*2, m_radius*2};
            p.setPen(QPen(Qt::red, m_radius/3));
            p.drawEllipse(r);
            p.drawPoint(n->GetPos());
            continue;
        }
    }
    if (m_shortestPath.size() < 2) return;
    p.setPen(QPen(Qt::yellow, m_radius/3));
    Node* prev = m_shortestPath[0];
    for(int i = 1; i < m_shortestPath.size(); ++i)
    {
        Node*& node = m_shortestPath[i];
        int x1 = node->GetPos().x(), y1 = node->GetPos().y();
        int x2 = prev->GetPos().x(), y2 = prev->GetPos().y();
        p.drawLine(x1,y1,x2,y2);
        prev = node;
    }
}

void MainWindow::resizeEvent(QResizeEvent *ev)
{
    m_graph.FitToScreen(size());
    m_radius = fmin(size().height(),size().width())/150;
    update();
}




