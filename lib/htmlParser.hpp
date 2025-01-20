#pragma once

#include "DocumentNode.hpp"


namespace hp
{
	std::unique_ptr<hp::DocumentNode> parseHtml(const std::string& html);
}
