#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma once
#include <QMainWindow>
#include <QColor>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


#include <QMouseEvent>
#include <QPaintEvent>
#include <QMessageBox>
#include "graph.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void mouseReleaseEvent(QMouseEvent* m) override;
    void paintEvent(QPaintEvent* ev) override;
    void resizeEvent(QResizeEvent *ev) override;



private slots:


private:
    Ui::MainWindow *ui;

    Graph m_graph;
    float m_radius = 6;

    Node* m_firstSelectedNode;
    Node* m_secondSelectedNode;
    bool isFirstNodeSelected;
    std::vector<Node*> m_shortestPath;


};
#endif // MAINWINDOW_H
