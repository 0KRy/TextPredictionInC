#include "WordTree.hpp"

TreeNode::TreeNode(bool eow)
{
    endOfWord = eow;
}

WordTree::WordTree()
{
    root = std::make_shared<TreeNode>(false);
    m_size = 0;
}

bool alphaChk(std::string word)
{
    if (word.size() > 1)
    {
        return std::isalpha(word[0]) && alphaChk(word.substr(1, word.size() - 1));
    }
    else
    {
        return std::isalpha(word[0]);
    }
}

std::string allLower(std::string word)
{
    if (word.size() >= 1)
    {
        return static_cast<char>(std::tolower(word[0])) + allLower(word.substr(1, word.size() - 1));
    }
    else
    {
        return "" + static_cast<char>(std::tolower(word[0]));
    }
}

// adds rest of word when empty path is found
void WordTree::addEmpty(std::string word, std::shared_ptr<TreeNode> current)
{
    int index = std::tolower(word[0]) - 97;
    std::shared_ptr<TreeNode> next = std::make_shared<TreeNode>((word.size() == 1));
    (*current).children[index] = next;
    if ((word.size() == 1))
    {
        m_size += 1;
    }
    else
    {
        addEmpty(word.substr(1, word.size() - 1), next);
    }
}

// moves on nonempty path and switched to addempty on empty path
void WordTree::addContained(std::string word, std::shared_ptr<TreeNode> current)
{
    int index = static_cast<int>(std::tolower(word[0])) - 97;
    if ((*current).children[index] == 0)
    {
        std::shared_ptr<TreeNode> next = std::make_shared<TreeNode>((word.size() == 1));
        (*current).children[index] = next;
        if (!(word.size() == 1))
        {
            addEmpty(word.substr(1, word.size() - 1), next);
        }
    }
    else
    {
        std::shared_ptr<TreeNode> next = (*current).children[index];
        if (word.size() == 1)
        {
            if (!((*next).endOfWord == true))
            {
                m_size += 1;
                (*next).endOfWord = true;
            }
        }
        else
        {
            addContained(word.substr(1, word.size() - 1), next);
        }
    }
}

// setsup calls to the recursive addEmpty and addContained functions
void WordTree::add(std::string word)
{
    if (alphaChk(allLower(word)) && !word.empty())
    {
        int index = std::tolower(word[0]) - 97;
        if ((*root).children[index] == 0)
        {
            std::shared_ptr<TreeNode> next = std::make_shared<TreeNode>((word.size() == 1));
            (*root).children[index] = next;
            if (!(word.size() == 1))
            {
                addEmpty(word.substr(1, word.size() - 1), next);
            }
        }
        else
        {
            std::shared_ptr<TreeNode> next = (*root).children[index];
            if (!(word.size() == 1))
            {
                addContained(word.substr(1, word.size() - 1), next);
            }
            else
            {
                if (!((*next).endOfWord == true))
                {
                    m_size += 1;
                    (*next).endOfWord = true;
                }
            }
        }
    }
}

// public find switches to recursive.
bool WordTree::find(std::string word)
{

    if (alphaChk(word) && !word.empty())
    {
        int index = std::tolower(word[0]) - 97;

        if ((*root).children[index] == 0)
        {
            return (false);
        }
        else
        {
            if (word.size() == 1)
            {
                return ((*(*root).children[index]).endOfWord);
            }
            else
            {
                std::shared_ptr<TreeNode> next = (*root).children[index];
                return (find(allLower(word.substr(1, word.size() - 1)), next));
            }
        }
    }
    else
    {
        return false;
    }
}

// private recursive find
bool WordTree::find(std::string word, std::shared_ptr<TreeNode> current)
{
    int index = std::tolower(word[0]) - 97;
    if ((*current).children[index] == 0)
    {
        return (false);
    }
    else
    {
        if (word.size() == 1)
        {
            return ((*(*current).children[index]).endOfWord);
        }
        else
        {
            std::shared_ptr<TreeNode> next = (*current).children[index];
            return (find(allLower(word.substr(1, word.size() - 1)), next));
        }
    }
}

std::vector<std::string> WordTree::predict(std::string partial, std::uint8_t howMany)
{

    std::vector<std::string> prediction;
    std::shared_ptr<TreeNode> start_node = root;
    int predicted = 0;
    if (alphaChk(partial) && !partial.empty())
    {

        if (!(partial == ""))
        {

            for (char node : allLower(partial))
            {

                int index = static_cast<int>(node) - 97;
                if ((*start_node).children[index] == 0)
                {
                    // prediction.push_back(partial);

                    return prediction;
                }
                else
                {
                    std::shared_ptr<TreeNode> next_start = (*start_node).children[index]; // seg fault

                    start_node = next_start;
                }
            }
        }

        std::queue<std::shared_ptr<TreeNode>> search_queue;
        std::queue<std::string> word_queue;
        search_queue.push(start_node);
        word_queue.push(allLower(partial));
        bool prefix_mod = false;

        while (!(search_queue.empty()) && (predicted < howMany))
        {
            std::shared_ptr<TreeNode> current_search;
            std::string current_word;
            current_search = search_queue.front();
            current_word = word_queue.front();
            for (int i = 0; i < 26; i++)
            {
                std::shared_ptr<TreeNode> next = (*current_search).children[i];
                if (!(next == 0))
                {
                    search_queue.push(next);
                    word_queue.push(current_word + static_cast<char>(i + 97));
                }
            }
            if ((*current_search).endOfWord && prefix_mod)
            {
                prediction.push_back(current_word);
                predicted++;
            }
            search_queue.pop();
            word_queue.pop();
            prefix_mod = true;
        }
    }
    return (prediction);
}
