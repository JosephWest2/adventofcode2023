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
struct SeedRange {
    long lower = 0;
    long upper = 0;
};

void PrintVector(const std::vector<SeedRange> &v) {
    std::cout << '\n';
    for (const auto n : v) {
        std::cout << "{" << n.lower << "," << n.upper << "}";
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

std::vector<SeedRange> ApplyMapToRanges(const std::vector<MapInfo> &seedMap,
                                        std::vector<SeedRange> &seedRanges) {
    std::vector<SeedRange> output;
    for (auto r : seedRanges) {
        for (auto map : seedMap) {
            bool fullInclusion = false;
            if (r.lower >= map.lowerBoundInclusive &&
                r.lower <= map.upperBoundInclusive) {
                if (r.upper <= map.upperBoundInclusive) {
                    fullInclusion = true;
                    output.push_back(SeedRange{.lower = r.lower + map.offset,
                                               .upper = r.upper + map.offset});
                } else {
                    output.push_back(SeedRange{
                        .lower = r.lower + map.offset,
                        .upper = map.upperBoundInclusive + map.offset});
                    output.push_back(
                        SeedRange{.lower = map.upperBoundInclusive + 1,
                                  .upper = r.upper});
                }
            }
            if (!fullInclusion && r.upper <= map.upperBoundInclusive &&
                r.upper >= map.lowerBoundInclusive) {
                if (r.lower >= map.lowerBoundInclusive) {
                    output.push_back(SeedRange{.lower = r.lower + map.offset,
                                               .upper = r.upper + map.offset});
                } else {
                    output.push_back(
                        SeedRange{.lower = map.lowerBoundInclusive + map.offset,
                                  .upper = r.upper + map.offset});
                    output.push_back(
                        SeedRange{.lower = r.lower,
                                  .upper = map.lowerBoundInclusive - 1});
                }
            }
        }
    }
    std::cout << "hi\n";
    PrintVector(output);
    return output;
}

int main(int argc, char **argv) {
    std::cout << "long size: " << sizeof(long) << '\n';

    if (argc < 3) {
        std::cout << "please provide the seeds, then the map file names\n";
        return EXIT_FAILURE;
    }

    std::vector<long> seeds;
    std::vector<SeedRange> seedRanges;

    std::ifstream seedFile(argv[1]);
    if (!seedFile) {
        std::cout << "Failed to load seed file " << argv[1] << '\n';
        return EXIT_FAILURE;
    }

    char c;
    std::string numString = "";
    int count = 0;
    long lower = 0;
    while (seedFile.get(c)) {
        if (!isdigit(c)) {
            if (numString.length() != 0) {
                if (count % 2 == 0) {
                    lower = std::stol(numString);
                } else {
                    seedRanges.push_back(SeedRange{
                        .lower = lower, .upper = std::stol(numString) + lower});
                }
                seeds.push_back(std::stol(numString));
                count++;
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
    std::vector<MapInfo> seedMap;
    std::string line;
    while (getline(mapFile, line)) {

        if (line.find("map") != std::string::npos) {
            ApplyMap(seedMap, seeds);
            seedRanges = ApplyMapToRanges(seedMap, seedRanges);
            PrintVector(seedRanges);
            seedMap.clear();
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
            seedMap.push_back(mapInfo);
        }
    }
    ApplyMap(seedMap, seeds);
    seedRanges = ApplyMapToRanges(seedMap, seedRanges);

    long min;
    bool initialized = false;
    for (auto n : seeds) {
        if (!initialized) {
            min = n;
            initialized = true;
        }
        min = std::min(min, n);
    }
    long rangeMin;
    initialized = false;
    for (auto range : seedRanges) {

        if (!initialized) {
            rangeMin = range.lower;
            initialized = true;
        }
        rangeMin = std::min(rangeMin, range.lower);
    }
    std::cout << "Min: " << min << '\n';
    std::cout << "RangeMin: " << rangeMin << '\n';
    return EXIT_SUCCESS;
}
