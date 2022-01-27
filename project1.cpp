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

class Game {
    
public:
    
    // Members
    // Optimization Tip: Make sure to order from biggest size to smallest
    // 2-D Vector
    vector<vector<position>> Map;
    
    // DEFAULT
    string Captian_method = "STACK";
    string Firstmate_method = "QUEUE";
    string Order = "NESW";
    char type;
    string path_type;
    unsigned int n;
    int land_traveled = 0;
    int water_traveled = 0;
    int ashore = 0;
    bool verbose = false;
    bool stats = false;
    bool path = false;
    
    Location start;
    Location end;
    
    // Read in the .txt treasure file through stdin.
    void read_data();

    // Read and process command line arguments.
    void get_options(int argc, char** argv);
    
    // Print Stats
    //void print_stats();
    
    // Print Path
    //void print_path(string path_value);
    
};

// ----------------------------------------------------------------------------
//                          Members Declarations
// ----------------------------------------------------------------------------

struct Members {
    
public:
    
    string hunt_strat;
    string SorQ;
    deque<Location> investigate;
    Location CapCurrent;
    Location FirCurrent;
    
    Members(string hunt_idea, string storing) {
        hunt_strat = hunt_idea;
        SorQ = storing;
    }
    
    void search(bool &found, int &travel, Members firstmate, Game ship);
    
};


// ----------------------------------------------------------------------------
//                               Driver
// ----------------------------------------------------------------------------

int main(int argc, char * argv[]) {
    
    // Xcode Redirect Speed up - I/O Stream
    ios_base::sync_with_stdio(false);
    xcode_redirect(argc, argv);
    
    // Ship is the person who will know everything and be able to provide you information
    Game ship;
    
    ship.get_options(argc, argv);
    ship.read_data();
    
    // Create the Members
    Members captain(ship.Order, ship.Captian_method);
    Members firstmate(ship.Order, ship.Firstmate_method);
    
    bool treasure_found = false;
    
    // Captain "STACK" or "QUEUE"
    switch (captain.SorQ[0]) {
        // STACK
        case 'S': {
            // Adding "START LOCATION" to 'investigate' deque
            captain.investigate.push_front(ship.start);
            ship.water_traveled = ship.water_traveled + 1;
            
            // Verbose "Treasure hunt started at: ..."
            if (ship.verbose)
                cout << "Treasure hunt started at: " << ship.start.row << ship.start.col << "/n";
            
            while(!captain.investigate.empty() || !treasure_found) {
                
                // Set CapCurrent location to top of stack
                captain.CapCurrent = captain.investigate.front();
                
                // Check to see if it has not been discovered
                if (!ship.Map[captain.CapCurrent.row][captain.CapCurrent.col].discovered) {
                    ship.Map[captain.CapCurrent.row][captain.CapCurrent.col].discovered = true;
                    ship.water_traveled = ship.water_traveled + 1;
                }
                
                // Removing from sail container
                captain.investigate.pop_front();
                
                // Loop through Hunt Order
                for(string::size_type i = 0; i < captain.hunt_strat.size(); ++i) {
                    
                    // North
                    if (captain.hunt_strat[i] == 'N') {
                        
                        // 'out of bounds'
                        if (captain.CapCurrent.row == 0)
                            continue;
                        
                        // '.'
                        if (ship.Map[captain.CapCurrent.row - 1][captain.CapCurrent.col].symbol == '.') {
                            if (!ship.Map[captain.CapCurrent.row - 1][captain.CapCurrent.col].discovered) {
                                
                                // Set previous
                                if (captain.CapCurrent.row - 1 != ship.start.row || captain.CapCurrent.col != ship.start.col)
                                        ship.Map[captain.CapCurrent.row - 1][captain.CapCurrent.col].prev = 'N';
                                
                                Location temp;
                                temp.row = captain.CapCurrent.row - 1;
                                temp.col = captain.CapCurrent.col;
                                captain.investigate.push_front(temp);
                            }
                        }
                        
                        // 'o' || '$'
                        if (ship.Map[captain.CapCurrent.row - 1][captain.CapCurrent.col].symbol == 'o' ||
                            ship.Map[captain.CapCurrent.row - 1][captain.CapCurrent.col].symbol == '$') {
                            if (ship.Map[captain.CapCurrent.row - 1][captain.CapCurrent.col].discovered == false) {
                                
                                // Went ashore
                                ship.ashore = ship.ashore++;
                                
                                // Verbose "Went ashore at: ..."
                                if (ship.verbose) {
                                    cout << "Went ashore at: " << captain.CapCurrent.row - 1 << "," << captain.CapCurrent.col << "/n";
                                    cout << "Searching island... ";
                                }
                                Location temp;
                                temp.row = captain.CapCurrent.row - 1;
                                temp.col = captain.CapCurrent.col;
                                captain.investigate.push_front(temp);
                                
                                // Run first mate --- set previous in the search function
                                firstmate.search(treasure_found,ship.land_traveled, firstmate, ship);

                                
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
                        if (ship.Map[captain.CapCurrent.row - 1][captain.CapCurrent.col].symbol == '#') {
                            continue;
                        }
                    }
                    
                    // East
                    if (captain.hunt_strat[i] == 'E') {
                        
                        // 'out of bounds'
                        if (captain.CapCurrent.col == (ship.n - 1))
                            continue;
                        
                        // '.'
                        if (ship.Map[captain.CapCurrent.row][captain.CapCurrent.col + 1].symbol == '.') {
                            if (!ship.Map[captain.CapCurrent.row][captain.CapCurrent.col + 1].discovered) {
                                
                                // Set previous
                                if (captain.CapCurrent.row != ship.start.row || captain.CapCurrent.col + 1 != ship.start.col)
                                        ship.Map[captain.CapCurrent.row][captain.CapCurrent.col + 1].prev = 'E';
                                
                                Location temp;
                                temp.row = captain.CapCurrent.row;
                                temp.col = captain.CapCurrent.col + 1;
                                captain.investigate.push_front(temp);
                            }
                        }
                        
                        // 'o' || '$'
                        if (ship.Map[captain.CapCurrent.row][captain.CapCurrent.col + 1].symbol == 'o' ||
                            ship.Map[captain.CapCurrent.row][captain.CapCurrent.col + 1].symbol == '$') {
                            if (!ship.Map[captain.CapCurrent.row][captain.CapCurrent.col + 1].discovered) {
                                
                                // Went ashore
                                ship.ashore = ship.ashore + 1;
                                
                                // Verbose "Went ashore at: ..."
                                if (ship.verbose) {
                                    cout << "Went ashore at: " << captain.CapCurrent.row << "," << captain.CapCurrent.col + 1 << "/n";
                                    cout << "Searching island... ";
                                }
                                
                                // Run first mate --- set previous in the search function
                                firstmate.search(treasure_found,ship.land_traveled, firstmate, ship);
                                
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
                        if (ship.Map[captain.CapCurrent.row][captain.CapCurrent.col + 1].symbol == '#') {
                            continue;
                        }
                    }
                    
                    // South
                    if (captain.hunt_strat[i] == 'S') {
                        
                        // 'out of bounds'
                        if (captain.CapCurrent.row == (ship.n - 1))
                            continue;
                        
                        // '.'
                        if (ship.Map[captain.CapCurrent.row + 1][captain.CapCurrent.col].symbol == '.') {
                            if (!ship.Map[captain.CapCurrent.row + 1][captain.CapCurrent.col].discovered) {
                                
                                // Set previous
                                if (captain.CapCurrent.row + 1 != ship.start.row || captain.CapCurrent.col != ship.start.col)
                                        ship.Map[captain.CapCurrent.row + 1][captain.CapCurrent.col].prev = 'S';
                                
                                Location temp;
                                temp.row = captain.CapCurrent.row + 1;
                                temp.col = captain.CapCurrent.col;
                                captain.investigate.push_front(temp);
                            }
                        }
                        
                        // 'o' || '$'
                        if (ship.Map[captain.CapCurrent.row + 1][captain.CapCurrent.col].symbol == 'o' ||
                            ship.Map[captain.CapCurrent.row + 1][captain.CapCurrent.col].symbol == '$') {
                            if (!ship.Map[captain.CapCurrent.row + 1][captain.CapCurrent.col].discovered) {
                                
                                // Went ashore
                                ship.ashore = ship.ashore + 1;
                                
                                // Verbose "Went ashore at: ..."
                                if (ship.verbose) {
                                    cout << "Went ashore at: " << captain.CapCurrent.row + 1 << "," << captain.CapCurrent.col << "/n";
                                    cout << "Searching island... ";
                                }
                                
                                // Run first mate --- set previous in the search function
                                firstmate.search(treasure_found,ship.land_traveled, firstmate, ship);
                                
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
                        if (ship.Map[captain.CapCurrent.row + 1][captain.CapCurrent.col].symbol == '#') {
                            continue;
                        }
                    }
                    
                    // West
                    if (captain.hunt_strat[i] == 'W') {
                        
                        // 'out of bounds'
                        if (captain.CapCurrent.col == 0)
                            continue;
                        
                        // '.'
                        if (ship.Map[captain.CapCurrent.row][captain.CapCurrent.col - 1].symbol == '.') {
                            if (!ship.Map[captain.CapCurrent.row][captain.CapCurrent.col - 1].discovered) {
                                
                                // Set previous
                                if (captain.CapCurrent.row != ship.start.row || captain.CapCurrent.col - 1 != ship.start.col)
                                        ship.Map[captain.CapCurrent.row][captain.CapCurrent.col - 1].prev = 'W';
                                
                                Location temp;
                                temp.row = captain.CapCurrent.row;
                                temp.col = captain.CapCurrent.col - 1;
                                captain.investigate.push_front(temp);
                            }
                        }
                        
                        // 'o' || '$'
                        if (ship.Map[captain.CapCurrent.row][captain.CapCurrent.col - 1].symbol == 'o' ||
                            ship.Map[captain.CapCurrent.row][captain.CapCurrent.col - 1].symbol == '$') {
                            if (!ship.Map[captain.CapCurrent.row][captain.CapCurrent.col - 1].discovered) {
                                
                                // Went ashore
                                ship.ashore = ship.ashore + 1;
                                
                                // Verbose "Went ashore at: ..."
                                if (ship.verbose) {
                                    cout << "Went ashore at: " << captain.CapCurrent.row << "," << captain.CapCurrent.col - 1 << "/n";
                                    cout << "Searching island... ";
                                }
                                
                                // Run first mate --- set previous in the search function
                                firstmate.search(treasure_found,ship.land_traveled, firstmate, ship);
                                
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
                        if (ship.Map[captain.CapCurrent.row][captain.CapCurrent.col - 1].symbol == '#') {
                            continue;
                        }
                    }
                }
                
                // Output
                //if (ship.stats)
                    //ship.print_stats();
                //if (ship.path)
                    //ship.print_path(ship.path_type);
                
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
        The order of discovery of adjacent tiles around the CapCurrent location,
        a four character string using exactly one of each of the four characters 'N', 'E', 'S', and 'W'
        (if unspecified, the default order is: North->East->South->West)
 
    [--verbose | -v]
        Print verbose output while searching
 
    [--stats | -s]
        Display statistics after the search is complete
 
    [--show-path | -p] <M|L>
        Display a treasure map or the list of locations that describe the path
 
 */

void Game::get_options(int argc, char** argv) {
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
                    Captian_method = captain_input;
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
                    Firstmate_method = firstmate_input;
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
                string Order_input = optarg;
                if (Order_input.size() == 4) {
                    for (string::size_type i = 0; i < Order_input.size(); ++i) {
                        if (Order_input[i] == 'N'
                            || Order_input[i] == 'W'
                            || Order_input[i] == 'E'
                            || Order_input[i] == 'S') {
                            if (i == 3) {
                                Order = Order_input;
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
void Game::read_data() {
    // Skipping Comments
    char first_char;
    
    cin >> first_char;
    while(first_char == '#') {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> first_char;
    }
    
    // Read in Format Type
    type = first_char;
    
    // Getting Size
    cin >> n;
    position d;
    // Populating with neccessary memory
    Map.resize(n,vector<position>(n, d));
    
    if(type == 'L') {
        unsigned int row;
        unsigned int col;
        char symbol;
        // Read in for L type
        while(cin >> row >> col >> symbol) {
            if (symbol == '@') {
                start.col = col;
                start.row = row;
            }
            
            if (symbol == '$') {
                end.col = col;
                end.row = row;
            }
            Map[row][col].symbol = symbol;
        }
    }
    
    // Reading in for M type
    if(type == 'M') {
        // Read in in Line Format
        for(unsigned int i = 0; i < n; ++i) {
            for(unsigned int j = 0; j < n; ++j) {
                cin >> first_char;
                if(first_char == '.')
                    continue;
                Map[i][j].symbol = first_char;
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

void Members::search(bool &found, int &travel, Members firstmate, Game ship){
    while(!firstmate.investigate.empty() || !found) {
                    
        // Set CapCurrent location to top of stack
        firstmate.FirCurrent = firstmate.investigate.front();
        
        // Check to see if it has not been discovered
        if (!ship.Map[firstmate.FirCurrent.row][firstmate.FirCurrent.col].discovered) {
            ship.Map[firstmate.FirCurrent.row][firstmate.FirCurrent.col].discovered = true;
            ship.water_traveled = ship.water_traveled + 1;
        }
        // Removing from sail container
        firstmate.investigate.pop_front();

        for(string::size_type i = 0; i < firstmate.hunt_strat.size(); ++i) {
                    
                    // North
                    if (firstmate.hunt_strat[i] == 'N') {
                        
                        // 'out of bounds'
                        if (firstmate.FirCurrent.row == 0)
                            continue;
                        
                        // '.'
                        if (ship.Map[firstmate.FirCurrent.row - 1][firstmate.FirCurrent.col].symbol == 'o') {
                            if (!ship.Map[firstmate.FirCurrent.row - 1][firstmate.FirCurrent.col].discovered) {
                                
                                // Set previous
                                if (firstmate.CapCurrent.row - 1 != ship.start.row || firstmate.FirCurrent.col != ship.start.col)
                                        ship.Map[firstmate.CapCurrent.row - 1][firstmate.FirCurrent.col].prev = 'N';
                                
                                Location temp;
                                temp.row = firstmate.FirCurrent.row - 1;
                                temp.col = firstmate.FirCurrent.col;
                                firstmate.investigate.push_front(temp);
                            }
                        }
                    }

                    if (ship.Map[firstmate.FirCurrent.row - 1][firstmate.FirCurrent.col].symbol == '$'){
                        found = true;
                        break;
                    }

                    //East
                    if (firstmate.hunt_strat[i] == 'E') {
                        
                        // 'out of bounds'
                        if (firstmate.FirCurrent.row == 0)
                            continue;
                        
                        // '.'
                        if (ship.Map[firstmate.FirCurrent.row][firstmate.FirCurrent.col + 1].symbol == 'o') {
                            if (!ship.Map[firstmate.FirCurrent.row][firstmate.FirCurrent.col + 1].discovered) {
                                
                                // Set previous
                                if (firstmate.CapCurrent.row != ship.start.row || firstmate.FirCurrent.col + 1 != ship.start.col)
                                        ship.Map[firstmate.CapCurrent.row][firstmate.FirCurrent.col + 1].prev = 'E';
                                
                                Location temp;
                                temp.row = firstmate.FirCurrent.row;
                                temp.col = firstmate.FirCurrent.col + 1;
                                firstmate.investigate.push_front(temp);
                            }
                        }
                    }

                    if (ship.Map[firstmate.FirCurrent.row][firstmate.FirCurrent.col + 1].symbol == '$'){
                        found = true;
                        break;
                    }

                    //South
                    if (firstmate.hunt_strat[i] == 'S') {
                        
                        // 'out of bounds'
                        if (firstmate.FirCurrent.row == 0)
                            continue;
                        
                        // '.'
                        if (ship.Map[firstmate.FirCurrent.row + 1][firstmate.FirCurrent.col].symbol == 'o') {
                            if (!ship.Map[firstmate.FirCurrent.row - 1][firstmate.FirCurrent.col].discovered) {
                                
                                // Set previous
                                if (firstmate.CapCurrent.row + 1 != ship.start.row || firstmate.FirCurrent.col != ship.start.col)
                                        ship.Map[firstmate.CapCurrent.row + 1][firstmate.FirCurrent.col].prev = 'S';
                                
                                Location temp;
                                temp.row = firstmate.FirCurrent.row + 1;
                                temp.col = firstmate.FirCurrent.col;
                                firstmate.investigate.push_front(temp);
                            }
                        }
                    }

                    if (ship.Map[firstmate.FirCurrent.row + 1][firstmate.FirCurrent.col].symbol == '$'){
                        found = true;
                        break;
                    }

                    //West
                    if (firstmate.hunt_strat[i] == 'W') {
                        
                        // 'out of bounds'
                        if (firstmate.FirCurrent.row == 0)
                            continue;
                        
                        // '.'
                        if (ship.Map[firstmate.FirCurrent.row][firstmate.FirCurrent.col - 1].symbol == 'o') {
                            if (!ship.Map[firstmate.FirCurrent.row][firstmate.FirCurrent.col - 1].discovered) {
                                
                                // Set previous
                                if (firstmate.CapCurrent.row != ship.start.row || firstmate.FirCurrent.col - 1 != ship.start.col)
                                        ship.Map[firstmate.CapCurrent.row][firstmate.FirCurrent.col - 1].prev = 'W';
                                
                                Location temp;
                                temp.row = firstmate.FirCurrent.row;
                                temp.col = firstmate.FirCurrent.col - 1;
                                firstmate.investigate.push_front(temp);
                            }
                        }
                    }

                    if (ship.Map[firstmate.FirCurrent.row][firstmate.FirCurrent.col - 1].symbol == '$'){
                        found = true;
                        break;
                    }
                    
        }

    }
}
