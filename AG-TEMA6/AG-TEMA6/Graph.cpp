#include "Graph.h"

void Graph::AddNode(const int id, const int x, const int y)
{
	m_nodes.insert({ id, { id, x, y } });
}
void Graph::AddNode(const std::pair<int,int> coordinates)
{
	m_nodes.insert({ 
		static_cast<int>(m_nodes.size()+1),
		{static_cast<int>(m_nodes.size()+1),
		coordinates.first,
		coordinates.second}
	});
}


bool Graph::AddArch(const int idFirsNode, const int idSecondNode, int cost)
{
	if (m_arch.contains({m_nodes.at(idFirsNode), m_nodes.at(idSecondNode)}))
		return false;
	if (idFirsNode == idSecondNode)
		return false;
	m_arch.insert(
		{
			{
				m_nodes.at(idFirsNode),
				m_nodes.at(idSecondNode)
			},
			cost
		});

	return true;
}

bool Graph::AddArch(const Node firstNode, const Node secondNode, int cost)
{
	if (m_arch.contains({ firstNode, secondNode }))
	{
		if (m_arch.at({ firstNode, secondNode }) == cost)
			return false;
		m_arch[{ firstNode, secondNode }] = cost;
		if(m_arch[{ firstNode, secondNode }] == 0)
		{
			m_arch.erase({ firstNode, secondNode });
		}
		return true;
	}
	if(firstNode.id == secondNode.id)
		return false;

	m_arch.insert(
		{
			{
				firstNode,
				secondNode
			},
			cost
		});

	return true;
}

int Graph::GetWeight(const int idFirstNode, const int idSecondNode)
{
	if(!m_arch.contains({ m_nodes.at(idFirstNode), m_nodes.at(idSecondNode) }))
	{
		return -1;
	}
	return m_arch.at({ m_nodes.at(idFirstNode), m_nodes.at(idSecondNode) });
}

Graph::Node Graph::GetNode(const int id) const
{
	if(!m_nodes.contains(id))
	{
		return { -1, -1, -1 };
	}
	return m_nodes.at(id);
}

std::map<std::pair<Graph::Node, Graph::Node>, int, Graph::CustomCompare> Graph::GetArches() const
{
		return m_arch;
}

std::map<int, Graph::Node> Graph::GetNodes() const
{
	return m_nodes;
}

void Graph::RemoveArch(Node firstNode, Node secondNode)
{
		if (m_arch.contains({ firstNode, secondNode }))
			m_arch.erase({ firstNode, secondNode });
}
