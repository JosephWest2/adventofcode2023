#include <fstream>
#include <iostream>
#include <optional>

int main(int argc, char **argv) {

    if (argc < 2) {
        std::cout << "No file name given\n";
        return -1;
    }

    std::ifstream inputFile(argv[1]);
    std::string textLine;
    int sum = 0;
    int lineCount = 0;
    while (getline(inputFile, textLine)) {
        lineCount++;
        int i = 0;
        std::optional<char> first;
        std::optional<char> second;
        while (i < textLine.length()) {
            if (isdigit(textLine[i])) {
                if (!first.has_value()) {
                    first = textLine[i];
                }
                second = textLine[i];
            }
            i++;
        }
        std::string s;
        s.push_back(first.value_or('0'));
        s.push_back(second.value_or('0'));
        sum += stoi(s);
    }
    std::cout << "The number is: " << sum << "\n";
    std::cout << "Lines checked: " << lineCount << "\n";
    return 0;
}
