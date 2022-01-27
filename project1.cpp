// PROJECT IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD

#include <vector>
#include <iostream>
#include <string>
#include <getopt.h>
#include <algorithm> // std::sort
#include "hunt.h"
#include <stack>
#include <queue>
using namespace std;

class Treasure_Hunt {
    
public:
    
    // Read in the CSV music file through stdin.
    void read_data();

    // Read and process command line arguments.
    void get_options(int argc, char** argv);
    
    //define search process
    bool search(string search_goal, string container, Location loc);

    void investigateforland(Location loc, string container);

    // run the actual search
    void run(string search_goal1, string search_goal2);
    
    
private:
    std::string container_captain = "STACK";
    std::string container_firstmate = "QUEUE";

    stack<Location> stack;
    queue<Location> queue;

    std::string hunt_order = "NESW";
    std::string path;
    int mapsize = 0;
    Location start_loc;
    Location end_loc;
    char symbol;
    char start;
    char policy = '\0';
    bool verbose = false;
    bool stats = false;
    
    std::vector<vector<Location>> Map;
};



int main(int argc, char** argv){
    try{
        Treasure_Hunt hunt;
        Location loc;

        hunt.get_options(argc, argv);

        hunt.read_data();

        hunt.search("land", "STACK",loc);

        hunt.investigateforland(loc, "STACK");

        hunt.run("land", "treasure");
    }

    catch(std::runtime_error& e){
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}

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

void Treasure_Hunt::get_options(int argc, char** argv){
    int option_index = 0, option = 0;
    
    // Don't display getopt error messages about options
    opterr = false;

    bool path_exist = false;

    struct option longOpts[] = {{"captain", required_argument, nullptr, 'c'},
                                {"firstmate", required_argument, nullptr, 'f'},
                                {"hunt-order", required_argument, nullptr, 'o'},
                                {"verbose", no_argument, nullptr, 'v'},
                                {"stats", no_argument, nullptr, 's'},
                                {"show-path", required_argument, nullptr, 'p'},
                                {"help", no_argument, nullptr, 'h'},
                                {nullptr, 0, nullptr, '\0'}};
    
    while ((option = getopt_long(argc, argv, "c:f:o:vsp:h", longOpts, &option_index)) != -1){
        switch(option){

            case 'c':{
                std::string captain_input = optarg;
                if(captain_input == "QUEUE" || captain_input == "STACK"){
                    container_captain = captain_input;
                }
                else{
                    std::cerr << "Invalid argument to --captain" << std::endl;
                    exit(1);
                }
                break;
            }

            case 'f':{
                std::string firstmate_input = optarg;
                if(firstmate_input == "QUEUE" || firstmate_input == "STACK"){
                    container_firstmate = firstmate_input;
                }
                else{
                    std::cerr << "Invalid argument to --firstmate" << std::endl;
                    exit(1);
                }
                break;
            }

            //hunt order DOUBLE CHECK WHERE USED
            case 'o':{
                std::string hunt_order_input = optarg;

                //ensures that there are only four directions
                if(hunt_order_input.size() != 4){
                    std::cerr << "Invalid argument to --hunt-order" << std::endl;
                    exit(1);
                }

                //checks for any duplicates in the string
                for(int i = 0; i < hunt_order_input.length(); i++){
                    for(int j = i+1; j < hunt_order_input.length(); j++){
                        if(hunt_order_input.at(i) == hunt_order_input.at(j)){
                            std::cerr << "Invalid argument to --hunt-order" << std::endl;
                            exit(1);
                        }
                    }
                }

                //sets up hunt order based on input
                for(int i = 0; i < 4; i++){
                    if(hunt_order_input.at(i) == 'N' || hunt_order_input.at(i) == 'E' ||
                       hunt_order_input.at(i) == 'S' || hunt_order_input.at(i) == 'W'){
                           if(i == 3){
                               hunt_order = hunt_order_input;
                           }
                       }
                    else{
                        std::cerr << "Invalid argument to --hunt-order" << std::endl;
                        exit(1);
                    }
                }

                break;
            }

            //set up verbose, stats, and show path DOUBLE CHECK
            case 'v':{
                verbose = true;
                break;
            }

            case 's':{
                stats = true;
                break;
            }

            //show path, L or M.DOUBLE CHECK
            case 'p':{
                std::string path_input = optarg;
                //if it is shown none, Specify --show-path only once
                if (path_exist) {
                    cerr << "Specify --show-path only once" << endl;
                    exit(1);
                }

                //what to do if shown multiple times?


                if(path_input == "L" || path_input == "M"){
                    path = path_input;
                }
                else{
                    std::cerr << "Invalid argument to --show-path" << std::endl;
                        exit(1);
                }

                break;
            }

            case 'h':{
                std::cout <<  "This program reads a txt file in the format of a 2-D Ascii Map or a Coordinate/Terrain Triples \n"
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
        }
    }

}

//Read data into the program through stdin
void Treasure_Hunt::read_data(){
    /*
    //ignore the lines that start with #
    cin >> start;
    while(start == '#') {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> start;
    }
    
    // read in format
    char format = start;
    
    // Getting Size
    cin >> mapsize;

    // make with necessary memory
    Location loc;
    
    Map.resize(mapsize,vector<Location>(mapsize, loc));
    
    // type L
    if(format == 'L') {
        int row;
        int col;
        // Read in for L type
        while(cin >> row >> col >> symbol) {
            if(symbol == '@') {
                start_loc.col = col;
                start_loc.row = row;
            }
            
            if(symbol == '$') {
                end_loc.col = col;
                end_loc.row = row;
            }
            Map[row][col].symbol = symbol;
        }
    }
    
    // type M
    if(format == 'M') {
        // Read in in Line Format
        for(int i = 0; i < mapsize; ++i) {
            for(int j = 0; j < mapsize; ++j) {
                cin >> start;
                if(start == '.')
                    continue;
                Map[i][j].symbol = start;
                if(start == '@') {
                    start_loc.col = j;
                    start_loc.row = i;
                }
                
                if(start == '$') {
                    end_loc.col = j;
                    end_loc.row = i;
                }

                if(start == NULL){
                    cerr << "Map does not have a start location" << endl;
                    exit(1);   
                }
            }
        }
    }

    */


   /*
    Location loc;
    loc.mapsize = 0;

    std::cin >> mapsize >> std::ws;

    Map.reserve(size_t(mapsize));

    while(std::getline(std::cin, loc.first_char, '\n')){
        //skip comments
        if(loc.first_char[0] == '#'){
            size_t pos = 0;

             if ((pos = loc.first_char.find_last_of('\n')) != std::string::npos)
				// Need a +1 here to move past the \n
                loc.first_char = loc.first_char.substr(pos + 2);

            // Otherwise just grab the rest of the line.
            else {
                std::getline(std::cin, loc.first_char);
                continue;
            }
        }

    // Get the rest of the line.
        std::getline(std::cin, loc.format, '\n');
        std::cin >> loc.mapsize >> std::ws;
        
        // Put the location into the map.
        //put it where?
        Map[loc.row][loc.col].push_back(loc);
    }

    // If we didn't read in any data, throw an error.
    if (!Map.size())
        throw std::runtime_error("No data was read in! Refer to the help option to see program usage.");
    */
}

//start the hunt
bool Treasure_Hunt::search(string search_goal, string container, Location here){
    //while treasure is not found
    int count = 0;
    bool treasure_found = false;
    //access start location
    //access hunt order

    if (search_goal == "land"){
        if (container == "STACK"){
            if (here.discovered == false && here.symbol == 'o'){
                stack.push(here);
                search_goal = "treasure";
            }
        }
        else{
            if (here.discovered == false && here.symbol == 'o'){
                queue.push(here);
                search_goal = "treasure";
            }
        }
    }


    if (search_goal == "treasure"){
        if (container == "QUEUE"){
            if (here.discovered == false && here.symbol == '$'){
                stack.push(here);
                treasure_found = true;
            }

            else{
                search_goal = "land";
            }
        }
        else{
            if (here.discovered == false && here.symbol == '$'){
                queue.push(here);
                treasure_found = true;
            }

            else{
                search_goal = "land";
            }
        }
    }

    return treasure_found;
}


void Treasure_Hunt::investigateforland(Location loc, string container){
    int i = 0;
    Location north;
    Location east;
    Location south;
    Location west;
    bool game_complete = false;


    while(search("land", container, loc) != true){
        if(hunt_order[i] =='N' && loc.row-1 != NULL){ 
            north.row = loc.row-1;
            north.col = loc.col;
            game_complete = search("land", container, north); //if this returns true, game over
        }
        else if(hunt_order[i] == 'E' && loc.col + 1 != NULL){
            east.row = loc.row;
            east.col = loc.col + 1;
            game_complete = search("land", container, east);
        }
        else if(hunt_order[i] == 'S' && loc.row + 1 != NULL){
            south.row = loc.row + 1;
            south.col = loc.col;
            game_complete = search("land", container, south);
        }
        else if(hunt_order[i] == 'W' && loc.col - 1 != NULL){
            west.row = loc.row;
            west.col = loc.col - 1;
            game_complete = search("land", container, west);
        }

        if(game_complete == true){
            break;
        }

        if(i > 4){
            break;
        }

        i++;
    }
}





