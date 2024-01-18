#pragma once
#include <format>
#include <vector>

#include "Graph.h"

typedef Graph::Node Node;
typedef std::pair<Node, Node> Arch;
class MaxFluxAccepted
{
public:
	static std::pair<int, std::vector<Arch>> MaxFlux(const Graph& graph, Node source, Node sink);

private:
	static std::vector<Node> BFS(Node source, Node sink);
	static void FindMinimumCutNodes(const Node& source);
	static std::vector<Arch> FindMinimumCutArches(const Graph& graph);

private:
	static inline Graph m_graph{};
	static inline std::vector<Node> m_minimumCutNodes{};
};

