#pragma once
#include "Graph.h"
#include <QtWidgets/QMainWindow>
#include "ui_GraphDisplay.h"
#include <QMouseEvent>
#include <QPainter>

typedef Graph::Node Node;

class GraphDisplay : public QMainWindow
{
    Q_OBJECT

public:
    GraphDisplay(QWidget *parent = nullptr);
    ~GraphDisplay() override = default;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private slots:
    void On_FordFulkersonButton_Clicked();
private:
    void ArchWeightBox(int& weight);
    bool ValidatePosition(QMouseEvent* event);

private:
	static constexpr int UI_BUTTONS_X {125};
    static constexpr int UI_BUTTONS_Y {60};
private:
    Ui::GraphDisplayClass ui;
    Graph m_graph{};
    Node m_firstNode{};
    int m_maxFlux{};
    std::vector<std::pair<Node, Node>> m_minimCutArches{};
};
