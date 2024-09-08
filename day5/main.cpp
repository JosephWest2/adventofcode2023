#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <range/v3/algorithm/transform.hpp>
#include <range/v3/all.hpp>
#include <string>
#include <vector>

struct MapInfo {
    long offset = 0;
    long lowerBoundInclusive = 0;
    long upperBoundInclusive = 0;
};

void PrintVector(const std::vector<long> &v) {
    std::cout << '\n';
    for (const auto n : v) {
        std::cout << n << ",";
    }
    std::cout << '\n';
}

void ApplyMap(const std::vector<MapInfo> &m, std::vector<long> &v) {
    auto transformation = [m](auto n) {
        for (auto mapInfo : m) {
            if (n >= mapInfo.lowerBoundInclusive &&
                n <= mapInfo.upperBoundInclusive) {
                return n + mapInfo.offset;
            }
        }
        return n;
    };
    v = v | ranges::views::transform(transformation) |
        ranges::to<std::vector>();
}

int main(int argc, char **argv) {

    std::cout << "long size: " << sizeof(long) << '\n';

    if (argc < 3) {
        std::cout << "please provide the seeds, then the map file names\n";
        return EXIT_FAILURE;
    }

    std::vector<long> v;

    std::ifstream seedFile(argv[1]);
    if (!seedFile) {
        std::cout << "Failed to load seed file " << argv[1] << '\n';
        return EXIT_FAILURE;
    }

    char c;
    std::string numString = "";
    while (seedFile.get(c)) {

        if (!isdigit(c)) {
            if (numString.length() != 0) {
                v.push_back(std::stol(numString));
            }
            numString = "";
            continue;
        }
        numString.push_back(c);
    }
    seedFile.close();

    std::ifstream mapFile(argv[2]);
    if (!mapFile) {
        std::cout << "Failed to load map file " << argv[2] << '\n';
        return EXIT_FAILURE;
    }
    std::vector<MapInfo> m;
    std::string line;
    while (getline(mapFile, line)) {

        if (line.find("map") != std::string::npos) {
            ApplyMap(m, v);
            PrintVector(v);
            m.clear();
            continue;
        }
        std::vector<long> nums;
        while (line.length() != 0) {
            long i = line.find(" ");
            std::string nString = line.substr(0, i);
            long ii = std::stol(nString);
            nums.push_back(ii);
            if (i >= std::string::npos || nums.size() == 3) {
                break;
            }
            line = line.substr(i + 1);
        }
        if (nums.size() == 3) {
            std::cout << nums[0] << "," << nums[1] << "," << nums[2] << "\n";
            MapInfo mapInfo{
                .offset = nums[0] - nums[1],
                .lowerBoundInclusive = nums[1],
                .upperBoundInclusive = nums[1] + nums[2],
            };
            m.push_back(mapInfo);
        }
    }
    ApplyMap(m, v);

    long min;
    bool initialized = false;
    for (auto n : v) {
        if (!initialized) {
            min = n;
            initialized = true;
        }
        min = std::min(min, n);
    }
    std::cout << "Min: " << min << '\n';
    return EXIT_SUCCESS;
}
