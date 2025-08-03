#pragma once
#include <unordered_map>
#include <queue>
#include <string>
#include <vector>

class ACAutomaton {
public:
    struct Node {
        std::unordered_map<char, Node*> children;
        Node* fail = nullptr;
        bool isEnd = false;
        int wordLen = 0; // 敏感词长度
    };

    ACAutomaton();
    ~ACAutomaton();

    void insert(const std::string& word);
    void build();
    // 支持多字符替换
    std::string filter(const std::string& text, const std::string& replaceStr);

private:
    Node* root;
    void clear(Node* node);
};
