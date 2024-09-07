#include <fstream>
#include <iostream>
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
            std::cout << ifLine << "\n";
            lines.push_back(ifLine);
        }
    }
    int result = 0;
    int rows = lines.size();
    int cols = lines[0].length();
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {

            if (isdigit(lines[r][c])) {
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
                    if (r > 0) {
                        if (!isalnum(lines[r-1][i]) && lines[r-1][i] != '.') {
                            valid = true;
                        }
                    }
                    if (r < rows - 1) {
                        if (!isalnum(lines[r+1][i]) && lines[r+1][i] != '.') {
                            valid = true;
                        }
                    }
                    i++;
                }
                if (c > 0 && !isalnum(lines[r][c-1]) && lines[r][c-1] != '.') {
                    valid = true;
                }
                if (c2 < cols && !isalnum(lines[r][c2]) && lines[r][c2] != '.') {
                    valid = true;
                }
                if (valid) {
                    result += num;
                }
                std::cout << "{" << num << "}";
                c = c2;
            }
        }
    }

    std::cout << "Result: " << result << "\n";
    return EXIT_SUCCESS;
}
