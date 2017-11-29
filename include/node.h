#pragma once

#include "position.h"

///Custom node variable type
class Node
{
public:
	Node() {}
	Node(const Position &p, const bool v = false)
	{
		position = p;
		visited = v;
	}

	Position position;
	bool visited;
};