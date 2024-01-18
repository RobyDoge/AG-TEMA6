#include "GraphDisplay.h"

#include <QStringList>
#include <QInputDialog>
#include <ranges>

#include "MaxFluxAccepted.h"


GraphDisplay::GraphDisplay(QWidget *parent)
    : QMainWindow(parent)
{
	m_firstNode.id = -1;
    ui.setupUi(this);
	connect(ui.fordFulkersonButton, &QPushButton::clicked, this, &GraphDisplay::On_FordFulkersonButton_Clicked);
}

void GraphDisplay::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::RightButton)
	{
		if (ValidatePosition(event))
		{
			m_graph.AddNode({ event->pos().x(),event->pos().y()});
			update();
		}
	}
	else if (event->button() == Qt::LeftButton)
	{
		for (const auto& node : m_graph.GetNodes() | std::views::values)
		{
			if (fabs(event->pos().x() - node.x) < 10 &&
				fabs(event->pos().y() - node.y) < 10)
			{
				if (m_firstNode.id == -1)
					m_firstNode = node;
				else
				{
					if(m_firstNode.id == node.id)
					{
						m_firstNode.id = -1;
						break;
					}
					int weight{ 0 };
					ArchWeightBox(weight);
					if (m_graph.AddArch(m_firstNode,node,weight))
					{
						m_firstNode.id = -1;
					}
				}
				update();
				break;
			}
		}
	}
}

void GraphDisplay::paintEvent(QPaintEvent* event)
{

	QPainter painter(this);
	for (const auto& [id, x, y] : m_graph.GetNodes() | std::views::values)
	{
		QRect r(x - 10, y - 10, 20, 20);
		painter.drawEllipse(r);
		QString s;
		s.setNum(id);
		painter.drawText(r, Qt::AlignCenter, s);
	}
	QPen aux;
	aux.setColor(Qt::black);
	painter.setPen(aux);
	for (const auto& [arch,weight] : m_graph.GetArches())
	{
		
		const auto& [firstNode, secondNode] = arch;
		QColor lineColor;
		std::ranges::find(m_minimCutArches, arch) != m_minimCutArches.end() ?
			lineColor= Qt::red : lineColor= Qt::black;
		QPen linePen(lineColor);
		painter.setPen(linePen);
		painter.drawLine(firstNode.x, firstNode.y,
			secondNode.x, secondNode.y);

		const int midX = (firstNode.x + secondNode.x) / 2;
		const int midY = (firstNode.y + secondNode.y) / 2;
		QString weightString = QString::number(weight);
		QRect textRect = painter.boundingRect(midX, midY, 0, 0, Qt::AlignCenter, weightString);
		QColor textColor = Qt::red; // Change to your desired color
		QPen textPen(textColor);
		painter.setPen(textPen);
		painter.drawText(midX - textRect.width() / 2, midY - textRect.height() / 2, textRect.width(),
		                 textRect.height(), Qt::AlignCenter, weightString);

		lineColor = Qt::black; // Change to your desired color
		linePen.setColor(lineColor);
		painter.setPen(linePen);

		const double angle = std::atan2(secondNode.y - firstNode.y,
			secondNode.x - firstNode.x);
		constexpr double arrowLength = 10.0;
		const double arrowX1 = secondNode.x - arrowLength * std::cos(angle - M_PI / 6);
		const double arrowY1 = secondNode.y - arrowLength * std::sin(angle - M_PI / 6);
		const double arrowX2 = secondNode.x - arrowLength * std::cos(angle + M_PI / 6);
		const double arrowY2 = secondNode.y - arrowLength * std::sin(angle + M_PI / 6);

		QPolygonF arrowhead;
		arrowhead.append(QPointF(secondNode.x, secondNode.y));
		arrowhead.append(QPointF(arrowX1, arrowY1));
		arrowhead.append(QPointF(arrowX2, arrowY2));

		painter.setBrush(Qt::black);
		painter.setPen(Qt::black);
		painter.drawPolygon(arrowhead);
	}
}

void GraphDisplay::On_FordFulkersonButton_Clicked()
{
	bool ok;
	const QString inputText = QInputDialog::getMultiLineText(this, tr("Enter Nodes id"),
	                                                         tr("Enter Nodes id:"), "0\n0", &ok);
	const QStringList inputList = inputText.split('\n');

	
	if (inputList.size() != 2) 
	{
		return;
	}
	const Node firstNode = m_graph.GetNode(inputList.at(0).toInt());
	const Node secondNode = m_graph.GetNode(inputList.at(1).toInt());
	if(firstNode.id == -1 || secondNode.id == -1)
	{
		return;
	}
	auto [maxFlux,minimCutArches] = MaxFluxAccepted::MaxFlux(m_graph, firstNode, secondNode);
	m_maxFlux= maxFlux;
	m_minimCutArches = minimCutArches;
	const QString maxFluxString = "Fluxul Maxim Este: " + QString::number(maxFlux);
	ui.maxFluxDisplayLW->addItem(maxFluxString);
	update();
}

void GraphDisplay::ArchWeightBox(int& weight) {
	bool ok;
	const int input = QInputDialog::getInt(this, tr("Enter Edge Cost"), tr("Cost:"), 0, -10000, 10000, 1, &ok);

	if (ok)
	{
		weight = input;
	}
	else {
		weight = 0;
	}
}

bool GraphDisplay::ValidatePosition(QMouseEvent* event)
{
	if (event->pos().y() <= UI_BUTTONS_Y && event->pos().x() <= UI_BUTTONS_X)
		return false;

	return std::ranges::all_of(m_graph.GetNodes(), [event](const std::pair<int,Node>& aux)
	{
		const auto& [id, x, y] = aux.second;
		const int dx = event->pos().x() - x;
		const int dy = event->pos().y() - y;
		const auto distance = std::sqrt(dx * dx + dy * dy);

		if (constexpr auto minDistance = 20; distance < minDistance)
		{
			return false;
		}
		return true;
		});
}

