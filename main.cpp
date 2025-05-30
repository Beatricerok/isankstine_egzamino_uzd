#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <regex>
#include <algorithm>

int main() {
    std::ifstream input("input.txt");
    std::ofstream word_out("word_count.txt");
    std::ofstream cross_out("cross_reference.txt");
    std::ofstream url_out("URLs.txt");

    if (!input) {
        std::cerr << "Nepavyko atidaryti input.txt\n";
        return 1;
    }

    std::map<std::string, int> word_count;
    std::map<std::string, std::set<int>> word_lines;
    std::set<std::string> urls;

    std::regex url_pattern(R"((https?://)?(www\.)?[a-zA-Z0-9\.-]+\.[a-z]{2,}(/[^\s]*)?)");

    std::string line;
    int line_number = 0;

    while (std::getline(input, line)) {
        ++line_number;

        // Rasti URL'us
        for (std::sregex_iterator it(line.begin(), line.end(), url_pattern), end; it != end; ++it)
            urls.insert(it->str());

        std::istringstream iss(line);
        std::string word;

        while (iss >> word) {
            // Pašalinti skyrybos ženklus, bet palikti lietuviškas raides
            word = std::regex_replace(word, std::regex(R"([^a-zA-ZąčęėįšųūžĄČĘĖĮŠŲŪŽ])"), "");
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            if (word.empty()) continue;

            word_count[word]++;
            word_lines[word].insert(line_number);
        }
    }

    // Išvesti žodžių dažnius (>1 kartą)
    for (const auto& [word, count] : word_count)
        if (count > 1)
            word_out << word << ": " << count << "\n";

    // Išvesti cross-reference lentelę
    for (const auto& [word, lines] : word_lines)
        if (word_count[word] > 1) {
            cross_out << word << ": ";
            for (int ln : lines)
                cross_out << ln << " ";
            cross_out << "\n";
        }

    // Išvesti URL'us
    for (const auto& url : urls)
        url_out << url << "\n";

    std::cout << "Baigta\n";
    return 0;
}
