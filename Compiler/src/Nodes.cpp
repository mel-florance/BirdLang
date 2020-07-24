#include "pch.h"
#include "Nodes.h"

std::ostream& operator << (std::ostream& stream, Node* node)
{
	return stream << node->typeToStr() << ':' << node->token;
}
