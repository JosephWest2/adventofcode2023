
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>

int main(int argc, char **argv) {

    if (argc < 2) {
        std::cout << "No input provided\n";
        return EXIT_FAILURE;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile.good()) {
        std::cout << "Error loading input file: " << argv[1] << "\n";
        return EXIT_FAILURE;
    }
    int lineCount = 0;
    std::string line;
    while (getline(inputFile, line)) {
        if (line.length() > 0) {
            lineCount++;
        }
    }
    inputFile.clear();
    inputFile.seekg(0);
    std::vector<size_t> cardCounts(lineCount, 1);
    size_t pointsTotal = 0;
    int currentCard = 0;
    while (getline(inputFile, line)) {
        std::string_view lineView(line);
        if (lineView.length() == 0) {
            continue;
        }
        for (int i = 0; i < lineView.length(); i++) {
            if (lineView[i] == ':') {
                lineView = lineView.substr(i);
                break;
            }
        }
        std::unordered_set<std::string> winningNumbers;
        std::string currentNumString = "";
        for (int i = 0; i < lineView.length(); i++) {
            char c = lineView[i];
            if (isdigit(c)) {
                currentNumString.push_back(c);
                continue;
            }
            if (currentNumString.length() > 0) {
                winningNumbers.insert(currentNumString);
            }
            currentNumString = "";
            if (c == '|') {
                lineView = lineView.substr(i);
                break;
            }
        }
        int numMatches = 0;
        currentNumString = "";
        for (int i = 0; i < lineView.length(); i++) {
            char c = lineView[i];
            if (isdigit(c)) {
                currentNumString.push_back(c);
                continue;
            }
            if (currentNumString.length() > 0 &&
                winningNumbers.count(currentNumString) == 1) {
                numMatches++;
            }
            currentNumString = "";
        }
        if (currentNumString.length() > 0 &&
            winningNumbers.count(currentNumString) == 1) {
            numMatches++;
        }


        int cardPoints = std::pow(2,(-1 + numMatches));
        std::cout << currentCard << " " << cardCounts.size() << " "
                  << cardPoints << "\n";
        for (int i = 1;
             i < numMatches + 1 && i + currentCard < cardCounts.size(); i++) {
            cardCounts[currentCard + i] += cardCounts[currentCard];
        }
        currentCard++;
        pointsTotal += cardPoints;
    }

    size_t totalCards = 0;
    for (size_t n : cardCounts) {
        std::cout << n << ",";
        totalCards += n;
    }
    std::cout << "\n";

    std::cout << "Point total: " << pointsTotal << "\n";
    std::cout << "Card total: " << totalCards << "\n";
    return EXIT_SUCCESS;
}
