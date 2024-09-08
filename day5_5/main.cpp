#include <bits/stdc++.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

struct MapRange {
    long from;
    long to;
    long range;
};

template <typename T> void PrintVector(vector<pair<T, T>> v) {
    for (auto e : v) {
        cout << '{' << e.first << ',' << e.second << '}';
    }
    cout << '\n';
}

void PrintVector(vector<MapRange> &m) {
    for (auto e : m) {
        cout << '{' << e.from << ',' << e.to << ',' << e.range << '}';
    }
    cout << '\n';
}
void PrintVector(vector<string> &strs) {
    for (auto s : strs) {
        cout << s << ',';
    }
    cout << '\n';
}

void SplitString(const string &str, vector<string> &output) {
    stringstream line_stream(str);
    string s;
    while (getline(line_stream, s, ' ')) {
        output.push_back(s);
    }
}
void ParseRanges(const vector<string> &strings,
                 vector<pair<long, long>> &output) {
    int i = 0;
    long prev = 0;
    for (auto segment : strings) {
        if (segment.length() > 0 && isdigit(segment[0])) {
            long current = stol(string(segment));
            if (i % 2 == 1) {
                output.push_back({prev, current + prev});
            }
            i++;
            prev = current;
        }
    }
}
vector<MapRange> GetNextMap(ifstream &input_file) {
    std::vector<MapRange> output;
    string line;
    while (getline(input_file, line)) {
        if (output.size() > 0 && !isdigit(line[0])) {
            break;
        }
        vector<string> split_line;
        SplitString(line, split_line);
        if (split_line.size() == 3) {
            MapRange r = {
                .from = stol(split_line[1]),
                .to = stol(split_line[0]),
                .range = stol(split_line[2]),
            };
            output.push_back(r);
        }
    }
    return output;
}
void ApplyMaps(const vector<MapRange> &maps,
               vector<pair<long, long>> &seed_ranges) {
    vector<pair<long, long>> output;
    for (auto range : seed_ranges) {
        for (auto map : maps) {
            long map_top = map.from + map.range;
            long offset = map.to - map.from;
            if (range.first >= map.from && range.first <= map_top) {
                long min = std::min(range.second, map_top);
                output.push_back({range.first + offset, min + offset});
                range = {min + 1, range.second};
            } else if (range.second <= map_top && range.second >= map.from) {
                long max = std::max(range.first, map.from);
                output.push_back({max + offset, range.second + offset});
                range = {range.first, max - 1};
            }
            if (range.first >= range.second) {
                break;
            }
        }
        if (range.first < range.second) {
            output.push_back(range);
        }
    }
    seed_ranges = std::move(output);
}

int main(int argc, char **argv) {

    if (argc < 2) {
        cout << "No input file provided\n";
        return EXIT_FAILURE;
    }

    ifstream input_file(argv[1]);
    if (!input_file) {
        cout << "Error parsing input file: " << argv[1] << '\n';
        return EXIT_FAILURE;
    }

    string line;
    getline(input_file, line);

    vector<string> line_segments;
    SplitString(line, line_segments);

    vector<pair<long, long>> seed_ranges;
    ParseRanges(line_segments, seed_ranges);

    PrintVector(seed_ranges);
    while (!input_file.eof()) {
        auto map = GetNextMap(input_file);
        ApplyMaps(map, seed_ranges);
        PrintVector(seed_ranges);
    }

    long min = 0;
    bool init = false;
    for (auto range : seed_ranges) {
        if (!init) {
            min = range.first;
            init = true;
        }
        min = std::min(min, range.first);
    }

    std::cout << "MIN: " << min << '\n';

    return EXIT_SUCCESS;
}
