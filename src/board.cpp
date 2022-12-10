#include "board.h"

#include <iomanip>
#include <array>
#include <chrono>
#include <unordered_map>
#include <fstream>

#if !(defined(_WIN32) || defined(_WIN64))
#include <unistd.h>
#endif

static const std::array<const uint8_t, 33> _index_lookup = {
             2,  3,  4,
             9, 10, 11,
    14, 15, 16, 17, 18, 19, 20,
    21, 22, 23, 24, 25, 26, 27,
    28, 29, 30, 31, 32, 33, 34,
            37, 38, 39,
            44, 45, 46
};

static const std::array<const uint8_t, 49> _coordinate_lookup = {
    0xff, 0xff,    0,    1,    2, 0xff, 0xff,
    0xff, 0xff,    3,    4,    5, 0xff, 0xff,
       6,    7,    8,    9,   10,   11,   12,
      13,   14,   15,   16,   17,   18,   19,
      20,   21,   22,   23,   24,   25,   26,
    0xff, 0xff,   27,   28,   29, 0xff, 0xff,
    0xff, 0xff,   30,   31,   32, 0xff, 0xff
};

static const std::unordered_map<std::string, std::string> algorithm_full_names = {
    {"bfs", "Breadth-First Search"},
    {"dfs", "Depth-First Search"},
    {"idp", "Iterative Deepening Search"},
    {"dfs-random", "Depth-First Search with Random Selection"},
    {"dfs-heuristic", "Depth-First Search with a Node Selection Heuristic"}
};

static std::string algorithm;
static std::chrono::_V2::steady_clock::time_point begin;

namespace peg_solitaire {

    uint8_t translate_index(uint8_t index) {
        return index < 33 ? _index_lookup[index] : -1;
    }

    uint8_t translate_coordinate(uint8_t coordinate) {
        return coordinate < 49 ? _coordinate_lookup[coordinate] : -1;
    }

    void print_board(std::bitset<33> board, uint64_t nodes_expanded) {
        std::cout << algorithm << "\n";

        int bi = 0;
        int c = peg_solitaire::translate_index(bi);

        for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 7; j++) {
                if (c / 7 == i && c % 7 == j) {
                    std::cout << (board[bi] ? " o " : "   ");
                    c = peg_solitaire::translate_index(++bi);
                } else {
                    std::cout << " - ";
                }
            }
            std::cout << "\n";
        }

        std::cout << "Remaining pieces: " << std::setfill(' ') << std::setw(2) << board.count() << "\n";

        std::cout << "Count: " << std::setfill(' ') << std::setw(13) << nodes_expanded << "\n";

        auto ram_usage = peg_solitaire::process_mem_usage();

        std::cout << "Ram usage: " << std::setfill(' ') << std::setw(7) << std::setprecision(1) << std::fixed << ram_usage.first << ram_usage.second << "\n";

        auto elapsed_time = peg_solitaire::parse_elapsed_time();

        uint16_t milliseconds = elapsed_time % 1000;
        uint8_t seconds = (elapsed_time / 1000) % 60;
        uint8_t minutes = (elapsed_time / 60000) % 60;
        uint8_t hours = (elapsed_time / 3600000) % 60;

        std::cout << "Time:   " 
            << std::setfill('0') << std::setw(2) << (int)hours << ":" 
            << std::setfill('0') << std::setw(2) << (int)minutes << ":" 
            << std::setfill('0') << std::setw(2) << (int)seconds << ":"
            << std::setfill('0') << std::setw(3) << (int)milliseconds << "\n";
    }

    void set_algorithm(std::string alg) {
        algorithm = algorithm_full_names.at(alg);
        begin = std::chrono::steady_clock::now();
    }

    std::pair<double, std::string> process_mem_usage() {
        /*
        Source
        https://gist.github.com/thirdwing/da4621eb163a886a03c5
        */
#if !(defined(_WIN32) || defined(_WIN64))
        double resident_set = 0.0;

        // the two fields we want
        unsigned long vsize;
        long rss;
        {
            std::string ignore;
            std::ifstream ifs("/proc/self/stat", std::ios_base::in);
            ifs >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                    >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                    >> ignore >> ignore >> vsize >> rss;
        }

        long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
        resident_set = rss * page_size_kb;

        int i; for (i = 0; i < 2; i++) {
            if (resident_set < 1024) break;
            resident_set /= 1024;
        }

        return {resident_set, std::string(1, "KMG"[i]) + "B"};
#elif
        return {0, "KB"};
#endif
    }

    uint64_t parse_elapsed_time() {
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    }
}