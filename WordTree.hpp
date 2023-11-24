#pragma once
#include <array>
#include <cstdint>
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <vector>

class TreeNode
{
  public:
    TreeNode(bool eow);
    bool endOfWord;
    std::array<std::shared_ptr<TreeNode>, 26> children;
};

class WordTree
{
  public:
    WordTree();
    void add(std::string word);
    bool find(std::string word);
    std::vector<std::string> predict(std::string partial, std::uint8_t howMany);
    std::size_t size() { return m_size; }

  private:
    std::shared_ptr<TreeNode> root;
    bool find(std::string word, std::shared_ptr<TreeNode> current);
    void addEmpty(std::string word, std::shared_ptr<TreeNode> current);
    void addContained(std::string word, std::shared_ptr<TreeNode> current);
    std::size_t m_size;
};
