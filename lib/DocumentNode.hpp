#pragma once

#include <memory>
#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>
#include <optional>


namespace hp
{

	class DocumentNode
	{
	public:
		DocumentNode* getParent() const;
		void setParent(DocumentNode* parent);

		std::string_view getTag() const;
		void setTag(std::string_view name);

		const std::vector<std::unique_ptr<DocumentNode>>& getChildren() const;
		std::vector<std::unique_ptr<DocumentNode>>& getChildren();
		void appendChild(std::unique_ptr<DocumentNode> child);

		const std::unordered_map<std::string, std::optional<std::string>>& getAttributes() const;
		void setAttribute(std::string_view name, std::optional<std::string_view> value);

		std::string getHierarchyRepr(size_t tabCount = 0) const;

	private:
		DocumentNode* parent = nullptr;
		std::string tag;
		std::string text;
		std::vector<std::unique_ptr<DocumentNode>> children;
		std::unordered_map<std::string, std::optional<std::string>> attributes;
	};

}
