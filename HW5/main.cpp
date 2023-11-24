#include "WordTree.hpp"
#include "rlutil.h"

#include <algorithm>
#include <fstream>

std::shared_ptr<WordTree> readDictionary(std::string filename)
{
    auto wordTree = std::make_shared<WordTree>();
    std::ifstream inFile = std::ifstream(filename, std::ios::in);

    while (!inFile.eof())
    {
        std::string word;
        std::getline(inFile, word);
        // Need to consume the carriage return character for some systems, if it exists
        if (!word.empty() && word[word.size() - 1] == '\r')
        {
            word.erase(word.end() - 1);
        }
        // Keep only if everything is an alphabetic character -- Have to send isalpha an unsigned char or
        // it will throw exception on negative values; e.g., characters with accent marks.
        if (std::all_of(word.begin(), word.end(), [](unsigned char c)
                        {
                            return std::isalpha(c);
                        }))
        {
            std::transform(word.begin(), word.end(), word.begin(), [](char c)
                           {
                               return static_cast<char>(std::tolower(c));
                           });
            wordTree->add(word);
        }
    }

    return wordTree;
}

int main()
{
    WordTree predict_tree = *readDictionary("./dictionary.txt");
    rlutil::cls();
    int x = 1;
    int y = 1;
    std::string word = "";
    std::vector<std::string> lines;
    lines.push_back("");
    while (true)
    {
        if (x > 80)
        {
            x = 0;
            lines.push_back("");
            y++;
        }
        rlutil::locate(x, y);
        auto key = rlutil::getkey();

        if (key == rlutil::KEY_BACKSPACE)
        {
            if (x > 0)
            {
                x -= 1;
                if (x < 1 && y > 1)
                {
                    x = 40;
                    y--;
                }
                rlutil::locate(x, y);
                char blank = ' ';
                rlutil::setChar(blank);
                x -= 1;
                std::string back_line = lines[y - 1].substr(0, lines[y - 1].size() - 1);
                lines[y - 1] = back_line;
                std::string new_word = word.substr(0, word.size() - 1);
                word = new_word;
            }
            else
            {
                x = 1;
            }
        }
        else
        {
            if (key == rlutil::KEY_SPACE)
            {
                word = "";
                lines[y - 1] += " ";
            }
            else
            {
                rlutil::setChar(static_cast<char>(key));
                word += static_cast<char>(key);
                lines[y - 1] += static_cast<char>(key);
            }
        }
        rlutil::cls();
        int z = 0;
        for (std::string line : lines)
        {
            rlutil::locate(0, z);
            std::cout << line << std::endl;
            z++;
        }

        rlutil::locate(0, y + 1);
        std::cout << " --- prediction --- " << std::endl;
        for (std::string new_word : predict_tree.predict(word, 10))
        {
            std::cout << new_word << std::endl;
        }
        x++;
    }
}
