//  Project Identifier: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD

#include <vector>
#include <algorithm>
#include <getopt.h>
#include <iostream>
#include <string>
#include "hunt.h"
#include "xcode_redirect.hpp"
#include <deque>
using namespace std;
 
// ----------------------------------------------------------------------------
//                       Quarter Master Declarations
// ----------------------------------------------------------------------------

class QuarterMaster {
    
public:
    
    // Members
    // Optimization Tip: Make sure to order from biggest size to smallest
    // 2-D Vector
    vector<vector<location>> world_map;
    
    // DEFAULT
    string container_cap = "STACK";
    string container_first = "QUEUE";
    string huntOrder = "NESW";
    char map_or_list_format;
    string path_type;
    unsigned int n;
    int land_traveled = 0;
    int water_traveled = 0;
    int ashore = 0;
    bool verbose = false;
    bool stats = false;
    bool path = false;
    
    coordinate start;
    coordinate end;
    
    // Read in the .txt treasure file through stdin.
    void read_data();

    // Read and process command line arguments.
    void get_options(int argc, char** argv);
    
    // Print Stats
    void print_stats();
    
    // Print Path
    void print_path(string path_value);
    
};

// ----------------------------------------------------------------------------
//                          Crew Declarations
// ----------------------------------------------------------------------------

struct Crew {
    
public:
    
    string hunt_strat;
    string container_type;
    deque<coordinate> to_explore;
    coordinate current;
    
    Crew(string hunt_idea, string storing) {
        hunt_strat = hunt_idea;
        container_type = storing;
    }
    
    void search(bool &found, int &travel);
    
};


// ----------------------------------------------------------------------------
//                               Driver
// ----------------------------------------------------------------------------

int main(int argc, char * argv[]) {
    
    // Xcode Redirect Speed up - I/O Stream
    ios_base::sync_with_stdio(false);
    xcode_redirect(argc, argv);
    
    // Ship is the person who will know everything and be able to provide you information
    QuarterMaster ship;
    
    ship.get_options(argc, argv);
    ship.read_data();
    
    // Create the Crew
    Crew captain(ship.huntOrder, ship.container_cap);
    Crew firstmate(ship.huntOrder, ship.container_first);
    
    bool treasure_found = false;
    
    // Captain "STACK" or "QUEUE"
    switch (captain.container_type[0]) {
        // STACK
        case 'S': {
            // Adding "START LOCATION" to 'to_explore' deque
            captain.to_explore.push_front(ship.start);
            ship.water_traveled = ship.water_traveled + 1;
            
            // Verbose "Treasure hunt started at: ..."
            if (ship.verbose)
                cout << "Treasure hunt started at: " << ship.start.row << ship.start.col << "/n";
            
            while(!captain.to_explore.empty() || !treasure_found) {
                
                // Set current location to top of stack
                captain.current = captain.to_explore.front();
                
                // Check to see if it has not been discovered
                if (!ship.world_map[captain.current.row][captain.current.col].discovered) {
                    ship.world_map[captain.current.row][captain.current.col].discovered = true;
                    ship.water_traveled = ship.water_traveled + 1;
                }
                
                // Removing from sail container
                captain.to_explore.pop_front();
                
                // Loop through Hunt Order
                for(string::size_type i = 0; i < captain.hunt_strat.size(); ++i) {
                    
                    // North
                    if (captain.hunt_strat[i] == 'N') {
                        
                        // 'out of bounds'
                        if (captain.current.row == 0)
                            continue;
                        
                        // '.'
                        if (ship.world_map[captain.current.row - 1][captain.current.col].terrian == '.') {
                            if (!ship.world_map[captain.current.row - 1][captain.current.col].discovered) {
                                
                                // Set previous
                                if (captain.current.row - 1 != ship.start.row || captain.current.col != ship.start.col)
                                        ship.world_map[captain.current.row - 1][captain.current.col].previos_step = 'W';
                                
                                coordinate temp;
                                temp.row = captain.current.row - 1;
                                temp.col = captain.current.col;
                                captain.to_explore.push_front(temp);
                            }
                        }
                        
                        // 'o' || '$'
                        if (ship.world_map[captain.current.row - 1][captain.current.col].terrian == 'o' ||
                            ship.world_map[captain.current.row - 1][captain.current.col].terrian == '$') {
                            if (!ship.world_map[captain.current.row - 1][captain.current.col].discovered) {
                                
                                // Went ashore
                                ship.ashore = ship.ashore + 1;
                                
                                // Verbose "Went ashore at: ..."
                                if (ship.verbose) {
                                    cout << "Went ashore at: " << captain.current.row - 1 << "," << captain.current.col << "/n";
                                    cout << "Searching island... ";
                                }
                                
                                // Run first mate --- set previous in the search function
                                firstmate.search(treasure_found,ship.land_traveled);
                                
                                // Check for treasure
                                if (treasure_found)
                                    break;
                                
                                // Verbose "party..."
                                if (ship.verbose) {
                                    if (treasure_found)
                                        cout << "party found treasure at " << ship.end.row << ship.end.col << ".\n";
                                    else
                                        cout << "party returned with no treasure.\n";
                                }
                            }
                        }
                        
                        // '#'
                        if (ship.world_map[captain.current.row - 1][captain.current.col].terrian == '#') {
                            continue;
                        }
                    }
                    
                    // East
                    if (captain.hunt_strat[i] == 'E') {
                        
                        // 'out of bounds'
                        if (captain.current.col == (ship.n - 1))
                            continue;
                        
                        // '.'
                        if (ship.world_map[captain.current.row][captain.current.col + 1].terrian == '.') {
                            if (!ship.world_map[captain.current.row][captain.current.col + 1].discovered) {
                                
                                // Set previous
                                if (captain.current.row != ship.start.row || captain.current.col + 1 != ship.start.col)
                                        ship.world_map[captain.current.row][captain.current.col + 1].previos_step = 'E';
                                
                                coordinate temp;
                                temp.row = captain.current.row;
                                temp.col = captain.current.col + 1;
                                captain.to_explore.push_front(temp);
                            }
                        }
                        
                        // 'o' || '$'
                        if (ship.world_map[captain.current.row][captain.current.col + 1].terrian == 'o' ||
                            ship.world_map[captain.current.row][captain.current.col + 1].terrian == '$') {
                            if (!ship.world_map[captain.current.row][captain.current.col + 1].discovered) {
                                
                                // Went ashore
                                ship.ashore = ship.ashore + 1;
                                
                                // Verbose "Went ashore at: ..."
                                if (ship.verbose) {
                                    cout << "Went ashore at: " << captain.current.row << "," << captain.current.col + 1 << "/n";
                                    cout << "Searching island... ";
                                }
                                
                                // Run first mate --- set previous in the search function
                                firstmate.search(treasure_found,ship.land_traveled);
                                
                                // Check for treasure
                                if (treasure_found)
                                    break;
                                
                                // Verbose "party..."
                                if (ship.verbose) {
                                    if (treasure_found)
                                        cout << "party found treasure at " << ship.end.row << ship.end.col << ".\n";
                                    else
                                        cout << "party returned with no treasure.\n";
                                }
                            }
                        }
                        
                        // '#'
                        if (ship.world_map[captain.current.row][captain.current.col + 1].terrian == '#') {
                            continue;
                        }
                    }
                    
                    // South
                    if (captain.hunt_strat[i] == 'S') {
                        
                        // 'out of bounds'
                        if (captain.current.row == (ship.n - 1))
                            continue;
                        
                        // '.'
                        if (ship.world_map[captain.current.row + 1][captain.current.col].terrian == '.') {
                            if (!ship.world_map[captain.current.row + 1][captain.current.col].discovered) {
                                
                                // Set previous
                                if (captain.current.row + 1 != ship.start.row || captain.current.col != ship.start.col)
                                        ship.world_map[captain.current.row + 1][captain.current.col].previos_step = 'S';
                                
                                coordinate temp;
                                temp.row = captain.current.row + 1;
                                temp.col = captain.current.col;
                                captain.to_explore.push_front(temp);
                            }
                        }
                        
                        // 'o' || '$'
                        if (ship.world_map[captain.current.row + 1][captain.current.col].terrian == 'o' ||
                            ship.world_map[captain.current.row + 1][captain.current.col].terrian == '$') {
                            if (!ship.world_map[captain.current.row + 1][captain.current.col].discovered) {
                                
                                // Went ashore
                                ship.ashore = ship.ashore + 1;
                                
                                // Verbose "Went ashore at: ..."
                                if (ship.verbose) {
                                    cout << "Went ashore at: " << captain.current.row + 1 << "," << captain.current.col << "/n";
                                    cout << "Searching island... ";
                                }
                                
                                // Run first mate --- set previous in the search function
                                firstmate.search(treasure_found,ship.land_traveled);
                                
                                // Check for treasure
                                if (treasure_found)
                                    break;
                                
                                // Verbose "party..."
                                if (ship.verbose) {
                                    if (treasure_found)
                                        cout << "party found treasure at " << ship.end.row << ship.end.col << ".\n";
                                    else
                                        cout << "party returned with no treasure.\n";
                                }
                            }
                        }
                        
                        // '#'
                        if (ship.world_map[captain.current.row + 1][captain.current.col].terrian == '#') {
                            continue;
                        }
                    }
                    
                    // West
                    if (captain.hunt_strat[i] == 'W') {
                        
                        // 'out of bounds'
                        if (captain.current.col == 0)
                            continue;
                        
                        // '.'
                        if (ship.world_map[captain.current.row][captain.current.col - 1].terrian == '.') {
                            if (!ship.world_map[captain.current.row][captain.current.col - 1].discovered) {
                                
                                // Set previous
                                if (captain.current.row != ship.start.row || captain.current.col - 1 != ship.start.col)
                                        ship.world_map[captain.current.row][captain.current.col - 1].previos_step = 'W';
                                
                                coordinate temp;
                                temp.row = captain.current.row;
                                temp.col = captain.current.col - 1;
                                captain.to_explore.push_front(temp);
                            }
                        }
                        
                        // 'o' || '$'
                        if (ship.world_map[captain.current.row][captain.current.col - 1].terrian == 'o' ||
                            ship.world_map[captain.current.row][captain.current.col - 1].terrian == '$') {
                            if (!ship.world_map[captain.current.row][captain.current.col - 1].discovered) {
                                
                                // Went ashore
                                ship.ashore = ship.ashore + 1;
                                
                                // Verbose "Went ashore at: ..."
                                if (ship.verbose) {
                                    cout << "Went ashore at: " << captain.current.row << "," << captain.current.col - 1 << "/n";
                                    cout << "Searching island... ";
                                }
                                
                                // Run first mate --- set previous in the search function
                                firstmate.search(treasure_found,ship.land_traveled);
                                
                                // Check for treasure
                                if (treasure_found)
                                    break;
                                
                                // Verbose "party..."
                                if (ship.verbose) {
                                    if (treasure_found)
                                        cout << "party found treasure at " << ship.end.row << ship.end.col << ".\n";
                                    else
                                        cout << "party returned with no treasure.\n";
                                }
                            }
                        }
                        
                        // '#'
                        if (ship.world_map[captain.current.row][captain.current.col - 1].terrian == '#') {
                            continue;
                        }
                    }
                }
                
                // Output
                if (ship.stats)
                    ship.print_stats();
                if (ship.path)
                    ship.print_path(ship.path_type);
                
                // Will need a path length later when doing "backtracing" have to find path back no matter what
                if (treasure_found)
                    cout << "Treasure found at " << ship.end.row <<","<< ship.end.col << " with path length " << 7 << ".\n";
                else
                    cout << "No treasure found after investigating " << ship.ashore << "locations.\n";
            }
            
        }
        
        // QUEUE
        case 'Q': {
            
        }
    }
    
    // insert code here...(Whole process of will be here)
    return 0;
}


// ----------------------------------------------------------------------------
//                      Quarter Master Definitions
// ----------------------------------------------------------------------------

/*
    This program can be run with five different command line options:
 
    [--help | -h]
        Print a useful help message and exit, ignores all other options
 
    [--captain | -c] <"QUEUE"|"STACK">
        The route-finding container used while sailing in the water
        (if unspecified, container default is stack)
 
    [--firstmate| -f] <"QUEUE"|"STACK">
        The route-finding container used while searching on land
        (if unspecified, container default is queue)
 
    [--hunt-order | -o] <ORDER>
        The order of discovery of adjacent tiles around the current location,
        a four character string using exactly one of each of the four characters 'N', 'E', 'S', and 'W'
        (if unspecified, the default order is: North->East->South->West)
 
    [--verbose | -v]
        Print verbose output while searching
 
    [--stats | -s]
        Display statistics after the search is complete
 
    [--show-path | -p] <M|L>
        Display a treasure map or the list of locations that describe the path
 
 */

void QuarterMaster::get_options(int argc, char** argv) {
    int option_index = 0, option = 0;
    
    // Don't display getopt error messages about options
    opterr = false;
    
    // Checking path flag
    bool path_used = false;

    // use getopt to find command line options
    struct option longOpts[] = {{ "help", no_argument, nullptr, 'h'},
                                { "captain", required_argument, nullptr, 'c'},
                                { "firstmate", required_argument, nullptr, 'f'},
                                { "hunt-order", required_argument, nullptr, 'o'},
                                { "verbose", no_argument, nullptr, 'v'},
                                { "stats", no_argument, nullptr, 's'},
                                { "show-path", required_argument, nullptr, 'p'},
                                { nullptr, 0, nullptr, '\0' }};
    
    while ((option = getopt_long(argc, argv, "hc:f:o:vsp:", longOpts, &option_index)) != -1) {
        // Checking for show-path more than once
        if (path_used) {
            cerr << "Specify --show-path only once" << endl;
            exit(1);
        }
            
        if (option == 'p')
            path_used = true;
        
        switch (option) {
            // Help
            case 'h': {
                cout      <<  "This program reads a txt file in the format of a 2-D Ascii Map or a Coordinate/Terrain Triples \n"
                          <<  "Both files will contain the following structures: \n"
                          <<  "\t 1. Zero or more lines of comments, each of which...\n\t ~ have an octothorpe (#) in column zero \n"
                          <<                                                              "\t ~ can contain any combination of zero or more printable                                                                    ascii characters \n"
                          <<                                                              "\t ~ end with a newline character \n"
                          <<  "\n"
                          <<  "\t 2. A filetype specifier, which is...\n\t ~ a single ASCII character \n"
                          <<                                               "\t ~ either M or L (for map or list files) \n"
                          <<                                               "\t ~ followed by a newline character \n"
                          <<  "\n"
                          <<  "\t 3. A map size value, which is...\n\t ~ a positive integer, >= 2 \n"
                          <<                                          "\t ~ both the width and the height of the map \n"
                          <<                                          "\t ~ followed by a newline character \n"
                          <<  "The starting location is always a water square, and the treasure location is always a land square. \n"
                          <<  "Usage: \'./hunt\n\t[--captain | -c] <Which container 'STACK' or 'QUEUE'>\n"
                          <<                      "\t[--firtmate | -f] <Which container 'STACK' or 'QUEUE'>\n"
                          <<                      "\t[--hunt-order | -o] <Which order do you want to discover: 'N', 'E', 'S', 'W' (must have each one                                              EXACTLY once)>\n"
                          <<                      "\t[--verbose | -v]\n"
                          <<                      "\t[--stats | -s]\n"
                          <<                      "\t[--show-path | -p] <How do you want to see the path of treasure as a map or line: 'M' or 'L'>\n"
                          <<                      "\t[--help | -h]\n"
                          <<                      "\t< <2D Ascii Map.txt or CTT.txt>\' \n";
                exit(0);
            }
            
            // Captain
            /*
            Conditionals:
                - the argument provided must be either "QUEUE" or "STACK"
            */
            case 'c': {
                string captain_input = optarg;
                if (captain_input == "QUEUE" || captain_input == "STACK")
                    container_cap = captain_input;
                else {
                    cerr << "Invalid argument to --captain" << endl;
                    exit(1);
                }
                break;
            }
            
            // First Mate
            /*
            Conditionals:
                - the argument provided must be either "QUEUE" or "STACK"
            */
            case 'f': {
                string firstmate_input = optarg;
                if (firstmate_input == "QUEUE" || firstmate_input == "STACK")
                    container_first = firstmate_input;
                else {
                    cerr << "Invalid argument to --first-mate" << endl;
                    exit(1);
                }
                break;
            }
            
            // Hunt Order
            /*
            Conditionals:
                - the argument provided must be four characters long
                - the argument provided must contain one and only one of each of "NESW" (in any order)
            */
            case 'o': {
                // Make sure that its correct in input otherwise stderr and exit(1)
                string huntOrder_input = optarg;
                if (huntOrder_input.size() == 4) {
                    for (string::size_type i = 0; i < huntOrder_input.size(); ++i) {
                        if (huntOrder_input[i] == 'N'
                            || huntOrder_input[i] == 'W'
                            || huntOrder_input[i] == 'E'
                            || huntOrder_input[i] == 'S') {
                            if (i == 3) {
                                huntOrder = huntOrder_input;
                            }
                        }
                        else {
                            cerr << "Invalid argument to --hunt-order" << endl;
                            exit(1);
                        }
                    }
                }
                else {
                    cerr << "Invalid argument to --hunt-order" << endl;
                    exit(1);
                }
                break;
            }
            
            // Verbose
            case 'v': {
                verbose = true;
                break;
            }
            
            // Stats
            case 's': {
                stats = true;
                break;
            }
                
            // Show Path
            /*
            Conditionals:
                - the argument provided must be 'M' or 'L'
                - The Show Path option can only be specified once (Check in the beginning of get_options())
            */
            case 'p': {
                string path_input = optarg;
                if (path_input == "M" || path_input == "L"){
                    path = true;
                    path_type = path_input;
                }
                else{
                    cerr << "Invalid argument to --show-path" << endl;
                    exit(1);
                }
                break;
            }
            
            /*
            CONDITION: All short or long options not in the spec should result in program
                termination with a non-zero exit code
            */
            default:
                cerr << "Unknown option" << endl;
                exit(1);
    
        }
    }
    
}

// Reading in the Map or List .txt
void QuarterMaster::read_data() {
    // Skipping Comments
    char first_char;
    
    cin >> first_char;
    while(first_char == '#') {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> first_char;
    }
    
    // Read in Format Type
    map_or_list_format = first_char;
    
    // Getting Size
    cin >> n;
    location d;
    // Populating with neccessary memory
    world_map.resize(n,vector<location>(n, d));
    
    if(map_or_list_format == 'L') {
        unsigned int row;
        unsigned int col;
        char terrain;
        // Read in for L type
        while(cin >> row >> col >> terrain) {
            if (terrain == '@') {
                start.col = col;
                start.row = row;
            }
            
            if (terrain == '$') {
                end.col = col;
                end.row = row;
            }
            world_map[row][col].terrian = terrain;
        }
    }
    
    // Reading in for M type
    if(map_or_list_format == 'M') {
        // Read in in Line Format
        for(unsigned int i = 0; i < n; ++i) {
            for(unsigned int j = 0; j < n; ++j) {
                cin >> first_char;
                if(first_char == '.')
                    continue;
                world_map[i][j].terrian = first_char;
                if (first_char == '@') {
                    start.col = j;
                    start.row = i;
                }
                
                if (first_char == '$') {
                    end.col = j;
                    end.row = i;
                }
            }
        }
    }
}
