#include "ACAutomaton.hpp"
#include <fstream>
#include <iostream>
#include <chrono>
#include <random>

int main() {
    ACAutomaton ac;


    // 1. �������дʣ�10w����words.txtÿ��һ����
    std::ifstream fin("words.txt");
    if (!fin) {
        std::cerr << "��׼�� words.txt��10w�����дʣ�ÿ��һ����" << std::endl;
        return 1;
    }
    std::string word;
    size_t cnt = 0;
    std::vector<std::string> allWords; // �����������������д�
    while (std::getline(fin, word)) {
      if (!word.empty()) ac.insert(word);
      if (!word.empty()) allWords.push_back(word); // ���浽����
      cnt++;
      if (cnt % 10000 == 0) std::cout << "�Ѽ��� " << cnt << " �����д�" << std::endl;
    }
    fin.close();
    std::cout << "����failָ��..." << std::endl;

    ac.build();

    // 2. ��������ı�
    // ������벿�����д�
    std::vector<std::string> testWords;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dis(0, allWords.size() - 1);

    for (int i = 0; i < 10; ++i) {
        size_t idx = dis(gen);
        testWords.push_back(allWords[idx]);
    }

    std::string testText = "�����ı����ݣ��������дʣ�";
   for (const auto& w : testWords) {
       testText += w + "������һЩ�޹����ݡ�";
   }
    testText +="�Ʊ��ǡ�QQ��ɱ�ֱ���ɱ�֡����ҵ��������� ���ҵ�����DIY�˵���ԭ�ӵ��䷽";
    testText += "��������������";

    // 3. ���˲�ͳ��ʱ��
    auto start = std::chrono::high_resolution_clock::now();
    std::string filtered = ac.filter(testText, "*");
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> diff = end - start;

    std::cout << "����ǰ: " << testText << std::endl;
    std::cout << "���˺�: " << filtered << std::endl;
    std::cout << "���˺�ʱ: " << diff.count() << " ms" << std::endl;
    return 0;
}
