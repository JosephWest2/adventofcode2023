#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

int main(int argc, char **argv) {

    if (argc < 2) {
        std::cout << "No input provided\n";
        return EXIT_FAILURE;
    }

    std::ifstream inputFile(argv[1]);

    if (!inputFile.good()) {
        std::cout << "Invalid input / file not found: " << argv[1] << "\n";
        return EXIT_FAILURE;
    }
    std::vector<std::string> lines;
    std::string ifLine;
    while (getline(inputFile, ifLine)) {
        if (ifLine.length() > 0) {
            lines.push_back(ifLine);
        }
    }
    int result = 0;
    std::unordered_map<int, std::vector<int>> gears;
    int rows = lines.size();
    int cols = lines[0].length();
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (!isdigit(lines[r][c])) {
                continue;
            }
            int c2 = c;
            while (c2 < cols) {
                c2++;
                if (!isdigit(lines[r][c2])) {
                    break;
                }
            }
            int num = stoi(lines[r].substr(c, c2 - c));
            bool valid = false;
            int i = c > 0 ? c - 1 : c;
            while (i <= c2 && i < cols) {
                if (r > 0 && !isalnum(lines[r - 1][i]) &&
                    lines[r - 1][i] != '.') {
                    if (lines[r - 1][i] == '*') {
                        gears[(r - 1) * cols + i].push_back(num);
                    }
                    valid = true;
                }
                if (r < rows - 1 && !isalnum(lines[r + 1][i]) &&
                    lines[r + 1][i] != '.') {
                    if (lines[r + 1][i] == '*') {
                        gears[(r + 1) * cols + i].push_back(num);
                    }
                    valid = true;
                }
                i++;
            }
            if (c > 0 && !isalnum(lines[r][c - 1]) && lines[r][c - 1] != '.') {
                if (lines[r][c - 1] == '*') {
                    gears[r * cols + c - 1].push_back(num);
                }
                valid = true;
            }
            if (c2 < cols && !isalnum(lines[r][c2]) && lines[r][c2] != '.') {
                if (lines[r][c2] == '*') {
                    gears[r * cols + c2].push_back(num);
                }
                valid = true;
            }
            if (valid) {
                result += num;
            }
            c = c2;
        }
    }
    int gearSum = 0;
    for (const auto &[key, val] : gears) {
        if (val.size() == 2) {
            gearSum += val[0] * val[1];
        }
    }

    std::cout << "Result nums: " << result << "\nresult gears: " << gearSum
              << "\n";
    return EXIT_SUCCESS;
}
