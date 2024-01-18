#include "MaxFluxAccepted.h"

#include <queue>
#include <ranges>
#include <vector>

std::pair<int,std::vector<Arch>> MaxFluxAccepted::MaxFlux(const Graph& graph, const Node source, const Node sink)
{
	int maxFlux{};
	const Graph copyGraph = { graph };
	m_graph = copyGraph;


	while (true)
	{
		std::vector<Node> path = BFS(source, sink);
		if (path.empty())
			break;
		auto minWeight = std::numeric_limits<int>::max();
		for (int i = 0; i < path.size() - 1; i++)
		{
			const int& weight = m_graph.GetWeight(path[i].id, path[i + 1].id);
			if (weight < minWeight)
				minWeight = weight;
		}
		for (int i = 0; i < path.size() - 1; i++)
		{
			const int weight = m_graph.GetWeight(path[i].id, path[i + 1].id);

			m_graph.AddArch(path[i], path[i + 1], weight - minWeight);
			m_graph.AddArch(path[i + 1], path[i], weight + minWeight);
			
		}
		maxFlux += minWeight;
	}

	FindMinimumCutNodes(source);

	return {maxFlux,FindMinimumCutArches(graph)};
}

std::vector<Node> MaxFluxAccepted::BFS(Node source, const Node sink)
{
	std::vector<Node> path{};
	std::queue<Node> queue{};
	std::vector<Node> visited{};
	queue.push(source);
	while (!queue.empty())
	{
		source = queue.front();
		queue.pop();
		if (source == sink)
		{
			path.push_back(source);
			while (source != visited[0])
			{
				for (const auto& node : visited)
				{
					if (m_graph.GetArches().contains({ node,source}))
					{
						path.push_back(node);
						source = node;
						break;
					}
				}
			}
			std::ranges::reverse(path);
			return path;
		}
		visited.push_back(source);
		for (const auto&  node : m_graph.GetNodes() | std::views::values)
		{
			if (std::ranges::find(visited, node) == visited.end() && m_graph.GetArches().contains({source,node}))
			{
				queue.push(node);
			}
		}
	}
	return path;
}

void MaxFluxAccepted::FindMinimumCutNodes(const Node& source)
{
	std::vector<Node> visited{};
	std::queue<Node> queue{};
	queue.push(source);
	while (!queue.empty())
	{
		Node firstNode = queue.front();
		queue.pop();
		visited.push_back(firstNode);
		for (const auto& node : m_graph.GetNodes() | std::views::values)
		{
			if (std::ranges::find(visited, node) == visited.end() && m_graph.GetArches().contains({ firstNode, node}))
			{
				queue.push(node);
			}
		}
	}
	m_minimumCutNodes = visited;
}

std::vector<Arch> MaxFluxAccepted::FindMinimumCutArches(const Graph& graph)
{
	std::vector<Arch> minimumCutArches{};
	for (const auto& node : m_minimumCutNodes)
	{
		for (const auto& arch : graph.GetArches() | std::views::keys)
		{
			if (arch.first == node && std::ranges::find(m_minimumCutNodes,arch.second)==m_minimumCutNodes.end())
			{
				minimumCutArches.push_back(arch);
			}
		}
	}
	return minimumCutArches;



}
