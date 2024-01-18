#pragma once
#include <map>
#include <vector>

class Graph
{
public:
	struct Node
	{
		int id{};
		int x{};
		int y{};

		bool operator==(const Node& rhs) const
		{
			return id == rhs.id && x == rhs.x && y == rhs.y;
		}
	};
    struct CustomCompare
	{
        bool operator()(const std::pair<Node, Node>& lhs, const std::pair<Node, Node>& rhs) const {
            if (lhs.first.id < rhs.first.id) 
            {
                return true;
            }
            if (lhs.first.id > rhs.first.id) 
            {
                return false;
            }
        	return lhs.second.id < rhs.second.id;
            
        }
    };
	
public:
	Graph() =default;
	~Graph() = default;
	void AddNode(int id, int x, int y);
	void AddNode(std::pair<int, int> coordinates);
	bool AddArch(int idFirsNode, int idSecondNode, int cost);
	bool AddArch(Node firstNode, Node secondNode, int cost);
	int GetWeight(int idFirstNode, int idSecondNode);
	Node GetNode(int id) const;
	std::map< std::pair<Node, Node>,int, CustomCompare> GetArches() const;
	std::map < int, Node> GetNodes() const;
	void RemoveArch(Node firstNode, Node secondNode);

private:
	std::map< std::pair<Node, Node>,int, CustomCompare> m_arch{};
	std::map < int, Node> m_nodes{};
};

