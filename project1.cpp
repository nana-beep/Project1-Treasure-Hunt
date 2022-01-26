// PROJECT IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD

#include <vector>
#include <iostream>
#include <string>
#include <getopt.h>
#include <algorithm> // std::sort
#include "hunt.h"


class Treasure_Hunt {
    
public:
    
    // Read in the CSV music file through stdin.
    void read_data();

    // Read and process command line arguments.
    void get_options(int argc, char** argv);
    
    // Sort and print the data.
    void run();
    
    
private:
    std::string container_captain = "STACK";
    std::string container_firstmate = "QUEUE";
    std::string hunt_order = "NESW";
    std::string path;
    bool verbose = false;
    bool stats = false;
    char policy = '\0';
    std::vector<vector<vector<Location>>> Map;
};



int main(int argc, char** argv){
    try{
        Treasure_Hunt hunt;

        hunt.get_options(argc, argv);

        hunt.read_data();

        hunt.run();
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
                bool hunt_order_input_exist = false;

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
                               hunt_order_input_exist = true;
                           }
                       }
                    else{
                        std::cerr << "Invalid argument to --hunt-order" << std::endl;
                        exit(1);
                    }
                }

                //set up default DOUBLE CHECK
                if(hunt_order_input_exist == false){
                    hunt_order = 'NESW';
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
    Location loc;
    int mapsize = 0;

    std::cin >> mapsize >> std::ws;

    Map.reserve(size_t(mapsize));

    while(std::getline(std::cin, loc.symbol, '\n')){
        //skip comments
        if(loc.symbol[0] == '#'){
            size_t pos = 0;

             if ((pos = loc.symbol.find_last_of('\n')) != std::string::npos)
				// Need a +1 here to move past the \n
                loc.symbol = loc.symbol.substr(pos + 2);

            // Otherwise just grab the rest of the line.
            else {
                std::getline(std::cin, loc.symbol);
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
}

//start the hunt
void Treasure_Hunt::run(){
    //while treasure is not found
    while(/*THE TREASURE IS NOT FOUND*/){

        //find start location. if not found, state 'Map does not have a start location'

        //when captain goes
        if(policy == 'c'){
            if(container_captain == "QUEUE"){

            }

            else if(container_captain == "STACK"){

            }
        //based on hunt order, move through '.' only

        //if 'o' is discovered, stops until firstmate is done with search
        //if true, do not search again  
        }

        //when first mate goes
        if(policy == 'f'){
            if(container_firstmate == "QUEUE"){

            }

            else if(container_captain == "STACK"){
                
            }
        //start on the 'o' captain stopped at
        //based on hunt order, move through 'o' only

        //if 'o' is discovered that spot ==true and firstmate does not search again
        //keep going until all 'o' is discovered or treasure is found

        //otherwise, stop and let captain travel again
        }

        //if '$' is not found, return 'Map does not have treasure'
    }
}



