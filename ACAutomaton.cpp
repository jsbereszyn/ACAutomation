#include "ACAutomaton.hpp"
#include <algorithm>

ACAutomaton::ACAutomaton() {
    root = new Node();
}

ACAutomaton::~ACAutomaton() {
    clear(root);
}

void ACAutomaton::clear(Node* node) {
    for (auto& kv : node->children) {
        clear(kv.second);
    }
    delete node;
}

void ACAutomaton::insert(const std::string& word) {
    Node* cur = root;
    for (char c : word) {
        if (!cur->children.count(c)) {
            cur->children[c] = new Node();
        }
        cur = cur->children[c];
    }
    cur->isEnd = true;
    cur->wordLen = word.length();
}

void ACAutomaton::build() {
    std::queue<Node*> q;
    root->fail = root;
    for (auto& kv : root->children) {
        kv.second->fail = root;
        q.push(kv.second);
    }
    while (!q.empty()) {
        Node* cur = q.front(); q.pop();
        for (auto& kv : cur->children) {
            char c = kv.first;
            Node* child = kv.second;
            Node* failNode = cur->fail;
            while (failNode != root && !failNode->children.count(c)) {
                failNode = failNode->fail;
            }
            if (failNode->children.count(c) && failNode->children[c] != child) {
                child->fail = failNode->children[c];
            } else {
                child->fail = root;
            }
            q.push(child);
        }
    }
}

// 支持多字符替换（如 "***"、"[敏感]" 等）
std::string ACAutomaton::filter(const std::string& text, const std::string& replaceStr) {
    Node* node = root;
    std::vector<std::pair<int, int>> intervals;
    int n = text.size();
    for (int i = 0; i < n; ++i) {
        char c = text[i];
        while (node != root && !node->children.count(c)) {
            node = node->fail;
        }
        if (node->children.count(c)) {
            node = node->children[c];
        }
        Node* tmp = node;
        while (tmp != root) {
            if (tmp->isEnd) {
                int start = i - tmp->wordLen + 1;
                int end = i;
                intervals.emplace_back(start, end);
            }
            tmp = tmp->fail;
        }
    }
    // 合并重叠区间
    if (intervals.empty()) return text;
    std::sort(intervals.begin(), intervals.end());
    std::vector<std::pair<int, int>> merged;
    merged.push_back(intervals[0]);
    for (size_t i = 1; i < intervals.size(); ++i) {
        if (intervals[i].first <= merged.back().second) {
            merged.back().second = std::max(merged.back().second, intervals[i].second);
        } else {
            merged.push_back(intervals[i]);
        }
    }
    // 构造最终结果
    std::string result;
    int idx = 0;
    for (const auto& itv : merged) {
        if (idx < itv.first)
            result.append(text.begin() + idx, text.begin() + itv.first);
        result.append(replaceStr);
        idx = itv.second + 1;
    }
    if (idx < n) result.append(text.begin() + idx, text.end());
    return result;
}
