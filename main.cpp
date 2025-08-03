#include "ACAutomaton.hpp"
#include <fstream>
#include <iostream>
#include <chrono>
#include <random>

int main() {
    ACAutomaton ac;


    // 1. 加载敏感词（10w条，words.txt每行一个）
    std::ifstream fin("words.txt");
    if (!fin) {
        std::cerr << "请准备 words.txt（10w条敏感词，每行一个）" << std::endl;
        return 1;
    }
    std::string word;
    size_t cnt = 0;
    std::vector<std::string> allWords; // 新增，保存所有敏感词
    while (std::getline(fin, word)) {
      if (!word.empty()) ac.insert(word);
      if (!word.empty()) allWords.push_back(word); // 保存到容器
      cnt++;
      if (cnt % 10000 == 0) std::cout << "已加载 " << cnt << " 条敏感词" << std::endl;
    }
    fin.close();
    std::cout << "构建fail指针..." << std::endl;

    ac.build();

    // 2. 构造测试文本
    // 随机插入部分敏感词
    std::vector<std::string> testWords;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dis(0, allWords.size() - 1);

    for (int i = 0; i < 10; ++i) {
        size_t idx = dis(gen);
        testWords.push_back(allWords[idx]);
    }

    std::string testText = "正常文本内容，测试敏感词：";
   for (const auto& w : testWords) {
       testText += w + "，插入一些无关内容。";
   }
    testText +="黄冰糖。QQ找杀手报仇。杀手。国家电力。高严 国家电力。DIY核弹。原子弹配方";
    testText += "结束。。。测试";

    // 3. 过滤并统计时间
    auto start = std::chrono::high_resolution_clock::now();
    std::string filtered = ac.filter(testText, "*");
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> diff = end - start;

    std::cout << "过滤前: " << testText << std::endl;
    std::cout << "过滤后: " << filtered << std::endl;
    std::cout << "过滤耗时: " << diff.count() << " ms" << std::endl;
    return 0;
}
