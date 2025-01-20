#include "DocumentNode.hpp"

#include <format>


hp::DocumentNode* hp::DocumentNode::getParent() const {
	return parent;
}

void hp::DocumentNode::setParent(DocumentNode* parent) {
	this->parent = parent;
}

std::string_view hp::DocumentNode::getTag() const {
	return tag;
}

void hp::DocumentNode::setTag(std::string_view name) {
	tag = name;
}

const std::vector<std::unique_ptr<hp::DocumentNode>>& hp::DocumentNode::getChildren() const {
	return children;
}

std::vector<std::unique_ptr<hp::DocumentNode>>& hp::DocumentNode::getChildren() {
	return children;
}

void hp::DocumentNode::appendChild(std::unique_ptr<DocumentNode> child) {
	child->setParent(this);
	children.push_back(std::move(child));
}

const std::unordered_map<std::string, std::optional<std::string>>& hp::DocumentNode::getAttributes() const {
	return attributes;
}

void hp::DocumentNode::setAttribute(std::string_view name, std::optional<std::string_view> value) {
	if (value.has_value()) {
		attributes[std::string(name)] = std::string(value.value());
	}
	else {
		attributes[std::string(name)] = std::nullopt;
	}
}

std::string hp::DocumentNode::getHierarchyRepr(size_t tabCount) const {
	std::string attributesStr;
	for (const auto& attribute : attributes) {
		if (attribute.second.has_value()) {
			attributesStr += std::format(" {}=\"{}\"", attribute.first, attribute.second.value());
		}
		else {
			attributesStr += std::format(" {}", attribute.first);
		}
	}

	std::string repr;
	if (attributesStr != "") {
		repr = std::format("{}<{}{}>\n", std::string(tabCount, '\t'), tag, attributesStr);
	}
	else {
		repr = std::format("{}<{}>\n", std::string(tabCount, '\t'), tag);
	}

	if (text != "") {
		repr += std::format("{}{}\n", std::string(tabCount, '\t'), text);
	}

	for (const auto& child : children) {
		repr += child->getHierarchyRepr(tabCount + 1);
	}

	repr += std::format("{}</{}>\n", std::string(tabCount, '\t'), tag);

	return repr;
}
