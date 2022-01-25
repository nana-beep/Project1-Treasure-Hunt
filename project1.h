#include <vector>
#include <iostream>
#include <stack>
#include <queue>
#include <deque>
#include <getopt.h>
#include <sstream>
#include <list>


using namespace std;

struct Location {
	int row;
	int col;
	bool discovered;
	bool investigated;
	char character;

};

struct Map {
	Location* start;
	Location* end;
	char input;
};

//aosidaoiwoe

typedef vector<vector<vector<Location>>> Island;

//initializes the map
void Map_init(Island &island, Map &map);

//fills the map
void Map_make(Island& island, Map& map, char character);

//checks if characters on map are valid
void Valid_character(char character);





