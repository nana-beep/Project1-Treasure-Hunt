// PROJECT IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD
#include <vector>
#include <iostream>
#include <deque>
#include <fstream>
#include <sstream>
#include <getopt.h>
#include <algorithm> // std::sort

using namespace std;

struct Location{
    bool discovered = false;
    int row = 0;
    int col = 0;
    int mapsize = 0;
    string format;
    string symbol;
    Location *prev;

    // class Captain_Hunt{
    //     public:
            
    // };

    // class FirstMate_Hunt{
    //     public:
    // };
};


/*
struct Map{
    Location *start;
    Location *end;
    int mapsize;
};

typedef vector<vector<vector<Location>>>Island;

void Map_init(Island &island, Map &map);
void Map_make(Island &island, Map &map, char symbol);
void Valid_symbol(char symbol);
void Path_find(Island &island, Map &map, deque<Location*> &route);
//void Valid_location(Location *next, Location *prev, deque<Location*> &route, Map &map);
void retrace(Map &map, deque<Location*> &show_route);
*/