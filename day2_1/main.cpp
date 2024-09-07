#include <fstream>
#include <iostream>

const struct {
    int reds = 12;
    int blues = 14;
    int greens = 13;
} maxes;

int ParseLine(std::string_view line) {
    std::string lineNumberString = "0";
    int i = 0;
    while (i < line.length()) {
        if (isdigit(line[i])) {
            lineNumberString.push_back(line[i]);
        } else if (line[i] == ':') {
            i++;
            break;
        }
        i++;
    }
    std::string_view str = line.substr(i);
    int l = 0;
    while (l < str.length()) {
        int r = l;
        std::string ballCountString = "0";
        while (r < str.length() && str[r] != ',' && str[r] != ';') {
            if (isdigit(str[r])) {
                ballCountString.push_back(str[r]);
            }
            r++;
        }
        int ballCount = stoi(ballCountString);
        std::string_view s = str.substr(l, r - l + 1);
        if (s.find("red") != std::string::npos) {
            if (ballCount > maxes.reds) {
                return 0;
            }
        }
        if (s.find("blue") != std::string::npos) {
            if (ballCount > maxes.blues) {
                return 0;
            }
        }
        if (s.find("green") != std::string::npos) {
            if (ballCount > maxes.greens) {
                return 0;
            }
        }
        l = r + 1;
    }

    return stoi(lineNumberString);
}

int main(int argc, char **argv) {

    if (argc < 2) {
        std::cout << "no input provided\n";
        return -1;
    }

    std::ifstream inputFile(argv[1]);

    if (!inputFile.good()) {
        std::cout << "Filestream error, could not find " << argv[1] << "\n";
        return -1;
    }

    int result = 0;
    std::string line;
    while (std::getline(inputFile, line)) {
        result += ParseLine(line);
    }

    std::cout << "result: " << result << "\n";
    inputFile.close();
    return 0;
}
