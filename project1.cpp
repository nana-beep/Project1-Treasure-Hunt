//  Project Identifier: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD

#include <vector>
#include <string>
#include "hunt.h"
#include <algorithm>
#include <deque>
#include <getopt.h>
#include <iostream>

using namespace std;

class Game {
    
public:

    vector<vector<position>> map;


    bool verbose=false;
    bool stats=false;
    bool path=false;

    string pathdir;
    string path_type;

    int num;

    int landlength=0;
    int waterlength=0;
    int landed=0;

    char maplist;

    string captainstore="STACK";
    string firstmatestore="QUEUE";
    string order="NESW";

    Location initial;
    Location finish;


    
    void read_data();

    void get_options(int argc, char** argv);
    
    void print_stats();

    void print_path(string path_value);

};


struct Members {
    
public:
    
    string huntingorder;
    string stackorqueue;
    deque<Location> pile;
    Location here;
    
    Members(string huntorder, string filing) {
        huntingorder=huntorder;
        stackorqueue=filing;
    }
    
    void search(bool &found, Location temp, vector<vector<position>> &mapforfir,  int size, int &travel);
    
};


void Members::search(bool &found, Location temp, vector<vector<position>> &mapforfir,  int size, int &travel) {
    switch (stackorqueue[0]) {
        case 'Q': {
            pile.push_back(temp);
            
            //increment the counter for distance moved
            travel=travel + 1;
            
            
            while(!pile.empty() || (found == false)) {
                
                here=pile.front();
                travel=travel+ 1;
                pile.pop_front();
                
                for(int i=0; i < 4; i++) {
                    
                    // North
                    if (huntingorder[i]=='N') {
                        
                        // invalid due to out of bounds
                        if (here.r==0)
                            continue;
                        
                        // found water
                        if (mapforfir[here.r-1][here.c].symbol=='.')
                            continue;
                        
                        // found land
                        if (mapforfir[here.r-1][here.c].symbol=='o' && !mapforfir[here.r-1][here.c].investigated) {                             
                            // mark it as discovered
                            mapforfir[here.r-1][here.c].investigated=true;
                            mapforfir[here.r-1][here.c].prev='N';
                            
                            Location temp;
                            temp.r=here.r-1;
                            temp.c=here.c;
                            pile.push_back(temp);
                        }
                        
                        // Found treasure!
                        if (mapforfir[here.r-1][here.c].symbol=='$') {
                            // mark it as discovered
                            mapforfir[here.r-1][here.c].investigated=true;
                            mapforfir[here.r-1][here.c].prev='N';
                            found=true;
                            Location temp;
                            temp.r=here.r-1;
                            temp.c=here.c;
                            pile.push_back(temp);
                            
                            break;
                        }
                        
                        // blockage
                        if (mapforfir[here.r-1][here.c].symbol=='#' && !mapforfir[here.r-1][here.c].investigated){
                            // mark it as discovered
                            mapforfir[here.r-1][here.c].investigated=true;
                            continue;
                        }
                    }

                    // South
                    if (huntingorder[i]=='S') {
                        
                        // invalid due to out of bounds
                        if (here.r==size-1)
                            continue;
                        
                        // found water
                        if (mapforfir[here.r+1][here.c].symbol=='.')
                            continue;
                        
                        // found land
                        if (mapforfir[here.r+1][here.c].symbol=='o' && !mapforfir[here.r+1][here.c].investigated) {                        
                            // mark it as discovered
                            mapforfir[here.r+1][here.c].investigated=true;
                            mapforfir[here.r+1][here.c].prev='S';
                            
                            Location temp;
                            temp.r=here.r+ 1;
                            temp.c=here.c;
                            pile.push_back(temp);
                        }
                        
                        // Found treasure!
                        if (mapforfir[here.r+1][here.c].symbol=='$') {
                            // mark it as discovered
                            mapforfir[here.r+1][here.c].investigated=true;
                            mapforfir[here.r+1][here.c].prev='S';
                            found=true;
                            Location temp;
                            temp.r=here.r+ 1;
                            temp.c=here.c;
                            pile.push_back(temp);
                            
                            break;
                        }
                        
                        // blockage
                        if (mapforfir[here.r+1][here.c].symbol=='#' && !mapforfir[here.r+1][here.c].investigated) {
                            // mark it as discovered
                            mapforfir[here.r+1][here.c].investigated=true;
                            continue;
                        }
                    }
                    
                    // East
                    if (huntingorder[i]=='E') {
                        
                        // invalid due to out of bounds
                        if (here.r==size-1)
                            continue;
                        
                        // found water
                        if (mapforfir[here.r][here.c+1].symbol=='.')
                            continue;
                        
                        // found land
                        if (mapforfir[here.r][here.c+1].symbol=='o' && !mapforfir[here.r][here.c+1].investigated) {
                            // mark it as discovered
                            mapforfir[here.r][here.c+1].investigated=true;
                            mapforfir[here.r][here.c+1].prev='E';
                            
                            Location temp;
                            temp.r=here.r;
                            temp.c=here.c+ 1;
                            pile.push_back(temp);  
                        }
                        
                        // Found treasure!
                        if (mapforfir[here.r][here.c+1].symbol=='$') {
                            // mark it as discovered
                            mapforfir[here.r][here.c+1].investigated=true;
                            
                            
                            mapforfir[here.r][here.c+1].prev='E';
                            found=true;
                            Location temp;
                            temp.r=here.r;
                            temp.c=here.c+ 1;
                            pile.push_back(temp);
                            
                            break;
                        }
                        
                        // blockage
                        if (mapforfir[here.r][here.c+1].symbol=='#' && !mapforfir[here.r][here.c+1].investigated) {
                            // mark it as discovered
                            mapforfir[here.r][here.c+1].investigated=true;
                            continue;                       
                        }
                    }
                    
                    
                    
                    // West
                    if (huntingorder[i]=='W') {
                        
                        // invalid due to out of bounds
                        if (here.r==0)
                            continue;
                        
                        // found water
                        if (mapforfir[here.r][here.c-1].symbol=='.')
                            continue;
                        
                        // found land
                        if (mapforfir[here.r][here.c-1].symbol=='o' && !mapforfir[here.r][here.c-1].investigated) {      
                            // mark it as discovered
                            mapforfir[here.r][here.c-1].investigated=true;
                            mapforfir[here.r][here.c-1].prev='W';
                            
                            Location temp;
                            temp.r=here.r;
                            temp.c=here.c-1;
                            pile.push_back(temp);
                        }
                        
                        // Found treasure!
                        if (mapforfir[here.r][here.c-1].symbol=='$') {
                            // mark it as discovered
                            mapforfir[here.r][here.c-1].investigated=true;
                            mapforfir[here.r][here.c-1].prev='W';
                            found=true;
                            Location temp;
                            temp.r=here.r;
                            temp.c=here.c-1;
                            pile.push_back(temp);
                            
                            break;
                        }
                        
                        // blockage
                        if (mapforfir[here.r][here.c-1].symbol=='#' && !mapforfir[here.r][here.c-1].investigated) {
                            // mark it as discovered
                            mapforfir[here.r][here.c-1].investigated=true;
                            continue;
                        }
                    }
                }
            }
        }
        
        case 'S': {
            pile.push_front(temp);
            travel=travel+ 1;
            
            while(!pile.empty() || !found) {
                
                here=pile.front();
                
                // increment the travel for water
                travel=travel+ 1;
                
                // delete the front of stack
                pile.pop_front();
                
                for(int i=0; i < 4; i++) {
                    
                    // North
                    if (huntingorder[i]=='N') {
                        // invalid due to out of bounds
                        if (here.r==0)
                            continue;
                        // found water
                        if (mapforfir[here.r-1][here.c].symbol=='.')
                            continue;
                        // found land
                        if (mapforfir[here.r-1][here.c].symbol=='o' && !mapforfir[here.r-1][here.c].investigated) {
                            // mark it as discovered
                            mapforfir[here.r-1][here.c].investigated=true;
                            mapforfir[here.r-1][here.c].prev='N';
                            Location temp;
                            temp.r=here.r-1;
                            temp.c=here.c;
                            pile.push_front(temp);
                        }
                        
                        // Found treasure!
                        if (mapforfir[here.r-1][here.c].symbol=='$') {
                            // mark it as discovered
                            mapforfir[here.r-1][here.c].investigated=true;
                            mapforfir[here.r-1][here.c].prev='N';
                            found=true;
                            Location temp;
                            temp.r=here.r-1;
                            temp.c=here.c;
                            pile.push_front(temp);
                            break;
                        }
                        
                        // blockage
                        if (mapforfir[here.r-1][here.c].symbol=='#' && !mapforfir[here.r-1][here.c].investigated) {
                            // mark it as discovered
                            mapforfir[here.r-1][here.c].investigated=true;
                            continue;
                        }
                    }

                    // South
                    if (huntingorder[i]=='S') {
                        
                        // invalid due to out of bounds
                        if (here.r==size-1)
                            continue;
                        
                        // found water
                        if (mapforfir[here.r+1][here.c].symbol=='.')
                            continue;
                        
                        // found land
                        if (mapforfir[here.r+1][here.c].symbol=='o' && !mapforfir[here.r+1][here.c].investigated) {
                            // mark it as discovered
                            mapforfir[here.r+1][here.c].investigated=true;   
                            mapforfir[here.r+1][here.c].prev='S';   
                            Location temp;
                            temp.r=here.r+ 1;
                            temp.c=here.c;
                            pile.push_front(temp);
                        }
                        
                        // Found treasure!
                        if (mapforfir[here.r+1][here.c].symbol=='$') {
                            // mark it as discovered
                            mapforfir[here.r+1][here.c].investigated=true;
                            mapforfir[here.r+1][here.c].prev='S';
                            found=true;
                            Location temp;
                            temp.r=here.r+ 1;
                            temp.c=here.c;
                            pile.push_front(temp);
                            
                            break;
                        }
                        
                        // blockage
                        if (mapforfir[here.r+1][here.c].symbol=='#' && !mapforfir[here.r+1][here.c].investigated) {
                            // mark it as discovered
                            mapforfir[here.r+1][here.c].investigated=true;
                            continue;
                        }
                    }
                    
                    // East
                    if (huntingorder[i]=='E') {
                        
                        // invalid due to out of bounds
                        if (here.r==size-1)
                            continue;
                        
                        // found water
                        if (mapforfir[here.r][here.c+1].symbol=='.')
                            continue;
                        
                        // found land
                        if (mapforfir[here.r][here.c+1].symbol=='o' && !mapforfir[here.r][here.c+1].investigated) {      
                            // mark it as discovered
                            mapforfir[here.r][here.c+1].investigated=true;
                            mapforfir[here.r][here.c+1].prev='E';
                            Location temp;
                            temp.r=here.r;
                            temp.c=here.c+ 1;
                            pile.push_front(temp);
                        }
                        
                        // Found treasure!
                        if (mapforfir[here.r][here.c+1].symbol=='$') {
                            // mark it as discovered
                            mapforfir[here.r][here.c+1].investigated=true;
                            mapforfir[here.r][here.c+1].prev='E';  
                            found=true;
                            Location temp;
                            temp.r=here.r;
                            temp.c=here.c+ 1;
                            pile.push_front(temp);
                            
                            break;
                        }
                        
                        // blockage
                        if (mapforfir[here.r][here.c+1].symbol=='#' && !mapforfir[here.r][here.c+1].investigated) {
                            // mark it as discovered
                            mapforfir[here.r][here.c+1].investigated=true;
                            continue;
                        }
                    }
                    
                    
                    
                    // West
                    if (huntingorder[i]=='W') {
                        
                        // invalid due to out of bounds
                        if (here.r==0)
                            continue;
                        
                        // found water
                        if (mapforfir[here.r][here.c-1].symbol=='.')
                            continue;
                        
                        // found land
                        if (mapforfir[here.r][here.c-1].symbol=='o' && !mapforfir[here.r][here.c-1].investigated) {
                            // mark it as discovered
                            mapforfir[here.r][here.c-1].investigated=true;
                            mapforfir[here.r][here.c-1].prev='W';
                            Location temp;
                            temp.r=here.r;
                            temp.c=here.c-1;
                            pile.push_front(temp);
                        }
                        
                        // Found treasure!
                        if (mapforfir[here.r][here.c-1].symbol=='$') {
                            // mark it as discovered
                            mapforfir[here.r][here.c-1].investigated=true;
                            mapforfir[here.r][here.c-1].prev='W'; 
                            found=true;
                            Location temp;
                            temp.r=here.r;
                            temp.c=here.c-1;
                            pile.push_front(temp);
                            
                            break;
                        }
                        
                        // blockage
                        if (mapforfir[here.r][here.c-1].symbol=='#' && !mapforfir[here.r][here.c-1].investigated) {
                            // mark it as discovered
                            mapforfir[here.r][here.c-1].investigated=true;
                            continue;
                        }
                    }
                }
            }
        }
    }
}


int main(int argc, char * argv[]) {
    
    Game game;
    
    game.get_options(argc, argv);
    game.read_data();
    
    Members firstmate(game.order, game.firstmatestore);
    Members captain(game.order, game.captainstore);
    
    
    bool treasure=false;
    
    // Captain implementation
    switch (captain.stackorqueue[0]) {
        case 'S': {
            // add initial posiotion
            captain.pile.push_front(game.initial);
            game.waterlength=game.waterlength+ 1;
            
            // initial is now discovered
            game.map[game.initial.r][game.initial.c].investigated=true;
            
            // verbose that game has initialed
            if (game.verbose)
                cout<<"Treasure hunt initialed at: "<<game.initial.r<<","<<game.initial.c<<endl;
            
            while(!captain.pile.empty() || !treasure) {
                captain.here=captain.pile.front();

                game.waterlength=game.waterlength + 1;
                
                // delete first of the stack
                captain.pile.pop_front();
                
                for(int i=0; i < 4; i = i +1) {
                    if (captain.huntingorder[i]=='N') {
                        
                        // invalid due to out of bounds
                        if (captain.here.r==0)
                            continue;
                        
                        // found water
                        if (game.map[captain.here.r-1][captain.here.c].symbol=='.' && !game.map[captain.here.r-1][captain.here.c].investigated) {    
                            // mark it as discovered
                            game.map[captain.here.r-1][captain.here.c].investigated=true;
                            
                            if (captain.here.r-1 != game.initial.r || captain.here.c != game.initial.c)
                                    game.map[captain.here.r-1][captain.here.c].prev='N';
                            
                            Location temp;
                            temp.r=captain.here.r-1;
                            temp.c=captain.here.c;
                            captain.pile.push_front(temp);
                            
                        }
                        
                        // found land or treasure
                        if (game.map[captain.here.r-1][captain.here.c].symbol=='$' || game.map[captain.here.r-1][captain.here.c].symbol=='o') {
                            if (!game.map[captain.here.r-1][captain.here.c].investigated) {
                                
                                // mark it as discovered
                                game.map[captain.here.r-1][captain.here.c].investigated=true;
                                
                                // came to land
                                game.landed=game.landed+ 1;
                                
                                if (game.verbose) {
                                    cout<<"Went ashore at: "<<captain.here.r-1<<","<<captain.here.c<<endl;
                                    cout<<"Searching island... ";
                                }
                                
                                Location temp;
                                temp.r=captain.here.r-1;
                                temp.c=captain.here.c;
                                
                                firstmate.search(treasure, temp, game.map, game.num, game.landlength);
                                
                                if(treasure==true) {
                                    if(game.verbose) {
                                        cout<<"party found treasure at "<<game.finish.r<<","<<game.finish.c<<endl;
                                        break;
                                    }
                                    break;
                                }
                                else {
                                    if(game.verbose) {
                                        cout<<"party returned with no treasure.\n";
                                    }
                                }
                            }
                        }
                        
                        // case of blockage
                        if (game.map[captain.here.r-1][captain.here.c].symbol=='#') {
                            if (!game.map[captain.here.r-1][captain.here.c].investigated) {
                                // mark it as discovered
                                game.map[captain.here.r-1][captain.here.c].investigated=true;
                                continue;
                            }
                        }
                    }
                    

                    
                    // South
                    if (captain.huntingorder[i]=='S') {
                        
                        // invalid due to out of bounds
                        if (captain.here.r==(game.num-1))
                            continue;
                        
                        // found water
                        if (game.map[captain.here.r+1][captain.here.c].symbol=='.' && !game.map[captain.here.r+1][captain.here.c].investigated) {    
                            // mark it as discovered
                            game.map[captain.here.r+1][captain.here.c].investigated=true;
                            
                            if (captain.here.r+1 != game.initial.r || captain.here.c != game.initial.c)
                                    game.map[captain.here.r+1][captain.here.c].prev='S';
                            
                            Location temp;
                            temp.r=captain.here.r+ 1;
                            temp.c=captain.here.c;
                            captain.pile.push_front(temp);
                        }
                        
                        // found land or treasure
                        if (game.map[captain.here.r+1][captain.here.c].symbol=='$' || game.map[captain.here.r+1][captain.here.c].symbol=='o') {
                            if (!game.map[captain.here.r+1][captain.here.c].investigated) {
                                
                                // mark it as discovered
                                game.map[captain.here.r+1][captain.here.c].investigated=true;
                                
                                // came to land
                                game.landed=game.landed+ 1;
                                
                                if (game.verbose) {
                                    cout<<"Went ashore at: "<<captain.here.r+1<<","<<captain.here.c<<endl;
                                    cout<<"Searching island...";
                                }
                                
                                Location temp;
                                temp.r=captain.here.r+ 1;
                                temp.c=captain.here.c;
                                
                                firstmate.search(treasure, temp, game.map, game.num, game.landlength);

                                if(treasure) {
                                    if(game.verbose) {
                                        cout<<"party found treasure at "<<game.finish.r<<","<<game.finish.c<<endl;
                                        break;
                                    }
                                    break;
                                }
                                else {
                                    if(game.verbose) {
                                        cout<<"party returned with no treasure.\n";
                                    }
                                }
                            }
                        }
                        
                        // blocakge
                        if (game.map[captain.here.r+1][captain.here.c].symbol=='#' && !game.map[captain.here.r+1][captain.here.c].investigated) {
                            // mark it as discovered
                            game.map[captain.here.r+1][captain.here.c].investigated=true;
                            continue; 
                        }
                    }

                    // East
                    if (captain.huntingorder[i]=='E') {
                        
                        // invalid due to out of bounds
                        if (captain.here.c==(game.num-1))
                            continue;
                        
                        // found water
                        if (game.map[captain.here.r][captain.here.c+1].symbol=='.') {
                            if (!game.map[captain.here.r][captain.here.c+1].investigated) {
                                
                                // mark it as discovered
                                game.map[captain.here.r][captain.here.c+1].investigated=true;

                                if (captain.here.r != game.initial.r || captain.here.c+1 != game.initial.c)
                                        game.map[captain.here.r][captain.here.c+1].prev='E';
                                
                                Location temp;
                                temp.r=captain.here.r;
                                temp.c=captain.here.c+ 1;
                                captain.pile.push_front(temp);
                            }
                        }
                        
                        // found land or treasure
                        if (game.map[captain.here.r][captain.here.c+1].symbol=='$' || game.map[captain.here.r][captain.here.c+1].symbol=='o') {
                            if (!game.map[captain.here.r][captain.here.c+1].investigated) {
                                game.map[captain.here.r][captain.here.c+1].investigated=true;

                                game.landed=game.landed+ 1;
                                

                                if (game.verbose) {
                                    cout<<"Went ashore at: "<<captain.here.r<<","<<captain.here.c+1<<endl;
                                    cout<<"Searching island... ";
                                }
                                
                                Location temp;
                                temp.r=captain.here.r;
                                temp.c=captain.here.c+ 1;
                                

                                firstmate.search(treasure, temp, game.map, game.num, game.landlength);
                                

                                if(treasure) {
                                    if(game.verbose) {
                                        cout<<"party found treasure at "<<game.finish.r<<","<<game.finish.c<<endl;
                                        break;
                                    }
                                    break;
                                }
                                else {
                                    if(game.verbose) {
                                        cout<<"party returned with no treasure.\n";
                                    }
                                }
                            }
                        }
                        
                        // blockage
                        if (game.map[captain.here.r][captain.here.c+1].symbol=='#') {
                            if (!game.map[captain.here.r][captain.here.c+1].investigated) {
                                // mark it as discovered
                                game.map[captain.here.r][captain.here.c+1].investigated=true;
                                continue;
                            }
                        }
                    } 

                    // West
                    if (captain.huntingorder[i]=='W') {
                        
                        // invalid due to out of bounds
                        if (captain.here.c==0)
                            continue;
                        
                        // found water
                        if (game.map[captain.here.r][captain.here.c-1].symbol=='.') {
                            if (!game.map[captain.here.r][captain.here.c-1].investigated) {
                                
                                // mark it as discovered
                                game.map[captain.here.r][captain.here.c-1].investigated=true;

                                if (captain.here.r != game.initial.r || captain.here.c-1 != game.initial.c)
                                        game.map[captain.here.r][captain.here.c-1].prev='W';
                                
                                Location temp;
                                temp.r=captain.here.r;
                                temp.c=captain.here.c-1;
                                captain.pile.push_front(temp);
                            }
                        }
                        
                        // found land or treasure
                        if (game.map[captain.here.r][captain.here.c-1].symbol=='o' ||
                            game.map[captain.here.r][captain.here.c-1].symbol=='$') {
                            if (!game.map[captain.here.r][captain.here.c-1].investigated) {
                                
                                // mark it as discovered
                                game.map[captain.here.r][captain.here.c-1].investigated=true;
                                
                                // came to land
                                game.landed=game.landed+ 1;

                                if (game.verbose) {
                                    cout<<"Went landed at: "<<captain.here.r<<","<<captain.here.c-1<<endl;
                                    cout<<"Searching island... ";
                                }
                                
                                Location temp;
                                temp.r=captain.here.r;
                                temp.c=captain.here.c-1;

                                firstmate.search(treasure, temp, game.map, game.num, game.landlength);

                                if(treasure) {
                                    if(game.verbose) {
                                        cout<<"party found treasure at "<<game.finish.r<<","<<game.finish.c<<endl;
                                        break;
                                    }
                                    break;
                                }
                                else {
                                    if(game.verbose) {
                                        cout<<"party returned with no treasure.\n";
                                    }
                                }
                            }
                        }
                        
                        // blockage
                        if (game.map[captain.here.r][captain.here.c-1].symbol=='#') {
                            if(!game.map[captain.here.r][captain.here.c-1].investigated) {
                                // mark it as discovered
                                game.map[captain.here.r][captain.here.c-1].investigated=true;
                                continue;
                            }
                        }
                    }
                }
                
                
                if (game.path)
                    game.print_path(game.path_type);

                if (game.stats)
                    game.print_stats();

                
                if (treasure==true)
                    cout<<"Treasure found at "<<game.finish.r <<","<< game.finish.c<<" with path length "<<game.pathdir.size()<<endl;
                else
                    cout<<"No treasure found after investigating "<<game.landed<<"locations.\n";
            }
            
        }
        
        case 'Q': {
            captain.pile.push_back(game.initial);
            game.waterlength=game.waterlength+ 1;
            
            // initialed the intial queue location
            game.map[game.initial.r][game.initial.c].investigated=true;
            
            if (game.verbose)
                cout<<"Treasure hunt initialed at: "<<game.initial.r<<game.initial.c<<endl;
            
            while(!captain.pile.empty() || !treasure) {
                
                captain.here=captain.pile.front();
                
                game.waterlength=game.waterlength+ 1;
                
                // delete the first queue
                captain.pile.pop_front();
                
                for(int i = 0; i < 4; i = i +1) {
                    
                    // North
                    if (captain.huntingorder[i]=='N') {
                        
                        // invalid due to out of bounds
                        if (captain.here.r==0)
                            continue;
                        
                        // found water
                        if (game.map[captain.here.r-1][captain.here.c].symbol=='.' && !game.map[captain.here.r-1][captain.here.c].investigated) {
                                
                            // mark it as discovered
                            game.map[captain.here.r-1][captain.here.c].investigated=true;
                            
                            if (captain.here.r-1 != game.initial.r || captain.here.c != game.initial.c)
                                game.map[captain.here.r-1][captain.here.c].prev='N';
                            
                            Location temp;
                            temp.r=captain.here.r-1;
                            temp.c=captain.here.c;
                            captain.pile.push_back(temp);   
                        }
                        
                        // found land or treasure
                        if (game.map[captain.here.r-1][captain.here.c].symbol=='$' || game.map[captain.here.r-1][captain.here.c].symbol=='o') {
                            if (game.map[captain.here.r-1][captain.here.c].investigated == false) {
                                
                                // mark it as discovered
                                game.map[captain.here.r-1][captain.here.c].investigated=true;
                                
                                //came to land
                                game.landed=game.landed+ 1;

                                if(game.verbose) {
                                    cout<<"Went ashore at: "<<captain.here.r-1<<","<<captain.here.c<<endl;
                                    
                                    cout<<"Searching island... ";
                                }
                                
                                Location temp;
                                temp.r=captain.here.r-1;
                                temp.c=captain.here.c;

                                firstmate.search(treasure, temp, game.map, game.num, game.landlength);
                                if(treasure) {
                                    if(game.verbose) {
                                        cout<<"party found treasure at "<<game.finish.r<<","<<game.finish.c<<endl;
                                        break;
                                    }
                                    break;
                                }
                                else {
                                    if(game.verbose) {
                                        cout<<"party returned with no treasure.\n";
                                    }
                                }
                            }
                        }
                        
                        // blocakge
                        if (game.map[captain.here.r-1][captain.here.c].symbol=='#' && !game.map[captain.here.r-1][captain.here.c].investigated) {
                            // mark it as discovered
                            game.map[captain.here.r-1][captain.here.c].investigated=true;
                            continue;
                        }
                    }

                    // South
                    if (captain.huntingorder[i]=='S') {
                        
                        // invalid due to out of bounds
                        if (captain.here.r==(game.num-1))
                            continue;
                        
                        // found water
                        if (game.map[captain.here.r+1][captain.here.c].symbol=='.' && !game.map[captain.here.r+1][captain.here.c].investigated) {
                            
                            // mark it as discovered
                            game.map[captain.here.r+1][captain.here.c].investigated=true;
                            if (captain.here.r+1 != game.initial.r || captain.here.c != game.initial.c)
                                game.map[captain.here.r+1][captain.here.c].prev='S';
                            
                            Location temp;
                            temp.r=captain.here.r+ 1;
                            temp.c=captain.here.c;
                            captain.pile.push_back(temp);
                        }
                        
                        // found land or treasure
                        if (game.map[captain.here.r+1][captain.here.c].symbol=='o' || game.map[captain.here.r+1][captain.here.c].symbol=='$') {
                            if (game.map[captain.here.r+1][captain.here.c].investigated == false) {
                                
                                // mark it as discovered
                                game.map[captain.here.r+1][captain.here.c].investigated=true;
                                
                                // came to land
                                game.landed=game.landed+ 1;

                                if(game.verbose) {
                                    cout<<"Went ashore at: "<<captain.here.r+1<<","<<captain.here.c<<endl;
                                    
                                    cout<<"Searching island... ";
                                }
                                
                                Location temp;
                                temp.r=captain.here.r+ 1;
                                temp.c=captain.here.c;

                                firstmate.search(treasure, temp, game.map, game.num, game.landlength);

                                if(treasure) {
                                    if(game.verbose) {
                                        cout<<"party found treasure at "<<game.finish.r<<","<<game.finish.c<<endl;
                                        break;
                                    }
                                    break;
                                }
                                else {
                                    if(game.verbose) {
                                        cout<<"party returned with no treasure.\n";
                                    }
                                }
                            }
                        }

                        if (game.map[captain.here.r+1][captain.here.c].symbol=='#') {
                            if(!game.map[captain.here.r+1][captain.here.c].investigated) {
                                // mark it as discovered
                                game.map[captain.here.r+1][captain.here.c].investigated=true;
                                continue;
                            }
                        }
                    }
                    
                    // East
                    if (captain.huntingorder[i]=='E') {
                        
                        // invalid due to out of bounds
                        if (captain.here.r==(game.num-1))
                            continue;
                        
                        // found water
                        if (game.map[captain.here.r][captain.here.c+1].symbol=='.' && !game.map[captain.here.r][captain.here.c+1].investigated) {
                               
                            // mark it as discovered
                            game.map[captain.here.r][captain.here.c+1].investigated=true;

                            if (captain.here.r != game.initial.r || captain.here.c+1 != game.initial.c)
                                game.map[captain.here.r][captain.here.c+1].prev='E';
                            
                            Location temp;
                            temp.r=captain.here.r;
                            temp.c=captain.here.c+ 1;
                            captain.pile.push_back(temp);
                                
        
                        }
                        
                        // found land or treasure
                        if (game.map[captain.here.r][captain.here.c+1].symbol=='$' || game.map[captain.here.r][captain.here.c+1].symbol=='o') {
                            if (game.map[captain.here.r][captain.here.c+1].investigated == false) {
                                
                                // mark it as discovered
                                game.map[captain.here.r][captain.here.c+1].investigated=true;
                                
                                // came to land
                                game.landed=game.landed+ 1;

                                if(game.verbose) {
                                    cout<<"Went ashore at: "<<captain.here.r<<","<<captain.here.c+1<<endl;
                                    
                                    cout<<"Searching island... ";
                                }
                                
                                Location temp;
                                temp.r=captain.here.r;
                                temp.c=captain.here.c+ 1;
                                firstmate.search(treasure, temp, game.map, game.num, game.landlength);
                                if(treasure) {
                                    if(game.verbose) {
                                        cout<<"party found treasure at "<<game.finish.r<<","<<game.finish.c<<endl;
                                        break;
                                    }
                                    break;
                                }
                                else {
                                    if(game.verbose) {
                                        cout<<"party returned with no treasure.\n";
                                    }
                                }
                            }
                        }
                        
                        // blockage
                        if (game.map[captain.here.r][captain.here.c+1].symbol=='#' && !game.map[captain.here.r][captain.here.c+1].investigated) {
                            // mark it as discovered
                            game.map[captain.here.r][captain.here.c+1].investigated=true;
                            continue;
                        }
                    }
                    
                    
                    
                    // West
                    if (captain.huntingorder[i]=='W') {
                        
                        // invalid due to out of bounds
                        if (captain.here.r==0)
                            continue;
                        
                        // found water
                        if (game.map[captain.here.r][captain.here.c-1].symbol=='.' && !game.map[captain.here.r][captain.here.c-1].investigated) {
                            // mark it as discoverd
                            game.map[captain.here.r][captain.here.c-1].investigated=true;

                            if (captain.here.r != game.initial.r || captain.here.c-1 != game.initial.c)
                                game.map[captain.here.r][captain.here.c-1].prev='W';
                            
                            Location temp;
                            temp.r=captain.here.r;
                            temp.c=captain.here.c-1;
                            captain.pile.push_back(temp);
                        }
                        
                        // found land or treasure
                        if (game.map[captain.here.r][captain.here.c-1].symbol=='$' || game.map[captain.here.r][captain.here.c-1].symbol=='o') {
                            if (!game.map[captain.here.r][captain.here.c-1].investigated) {
                                
                                // mark it as discovered
                                game.map[captain.here.r][captain.here.c-1].investigated=true;
                                
                                // came to land
                                game.landed=game.landed+ 1;

                                if(game.verbose) {
                                    cout<<"Went ashore at: "<<captain.here.r<<","<<captain.here.c-1<<endl;
                                    
                                    cout<<"Searching island... ";
                                }
                                
                                Location temp;
                                temp.r=captain.here.r;
                                temp.c=captain.here.c-1;

                                firstmate.search(treasure, temp, game.map, game.num, game.landlength);

                                if(treasure) {
                                    if(game.verbose) {
                                        cout<<"party found treasure at "<<game.finish.r<<","<<game.finish.c<<endl;
                                        break;
                                    }
                                    break;
                                }
                                else {
                                    if(game.verbose) {
                                        cout<<"party returned with no treasure.\n";
                                    }
                                }
                            }
                        }
                        
                        // blockage
                        if (game.map[captain.here.r][captain.here.c-1].symbol=='#' && !game.map[captain.here.r][captain.here.c-1].investigated) {
                            // mark it as discovered
                            game.map[captain.here.r][captain.here.c-1].investigated=true;
                            continue;
                        }
                    }
                }

                //print out the pathing when treasure is found
                if (treasure)
                    cout<<"Treasure found at "<<game.finish.r <<","<< game.finish.c<<" with path length "<<game.pathdir.size()<<endl;
                else
                    cout<<"No treasure found after investigating "<<game.landed<<"locations.\n";
                
                if (game.stats)
                    game.print_stats();
                if (game.path)
                    game.print_path(game.path_type);    
            }
            
        }
    }
    
    return 0;
}

void Game::read_data() {
    char first_input;
    cin>>first_input;
    // keep on reading till its not #
    while(first_input=='#') {
        cin>>first_input;
    }
    maplist=first_input;
    
    cin>>num;
    position dummy;
    map.resize(num,vector<position>(num, dummy));
    
    if(maplist=='M') {
        for(int i=0; i < num; i++) {
            for(int j=0; j < num; j++) {
                cin>>first_input;
                //skip the water
                if(first_input=='.')
                    continue;
                map[i][j].symbol=first_input;
                if (first_input=='@') {
                    initial.c=j;
                    initial.r=i;
                }
                if (first_input=='$') {
                    finish.c=j;
                    finish.r=i;
                }
            }
        }
    }

    if(maplist=='L') {
        char symbol;
        int r;
        int c;
        while(cin>>r>>c>>symbol) {
            map[r][c].symbol=symbol;
            if (symbol=='@') {
                initial.c=c;
                initial.r=r;
            }
            if (symbol=='$') {
                finish.c=c;
                finish.r=r;
            }
        }
    }   
}

void Game::get_options(int argc, char** argv) {

    bool path_used=false;
    
    int option_index=0, option=0;
    
    opterr=false;
    
    

    struct option longOpts[]={
                                { "captain", required_argument, nullptr, 'c'},
                                { "firstmate", required_argument, nullptr, 'f'},
                                { "hunt-order", required_argument, nullptr, 'o'},
                                { "show-path", required_argument, nullptr, 'p'},
                                { "help", no_argument, nullptr, 'h'},
                                { "verbose", no_argument, nullptr, 'v'},
                                { "stats", no_argument, nullptr, 's'},
                                { nullptr, 0, nullptr, '\0' }
                            };
    
    while ((option=getopt_long(argc, argv, "c:f:o:p:hvs", longOpts, &option_index)) != -1) {

        if (option=='p')
            path_used=true;                                                                             

        if (path_used) {
            cerr<<"Specify --show-path only once"<<endl;
            exit(1);
        }
            
        
        switch (option) {
            case 'c': {
                string captain_input=optarg;
                if (captain_input=="QUEUE" || captain_input=="STACK")
                    captainstore=captain_input;
                else {
                    cerr<<"Invalid argument to --captain"<<endl;
                    exit(1);
                }
                break;
            }

            case 'f': {
                string firstmate_input=optarg;
                if (firstmate_input=="STACK" || firstmate_input=="QUEUE")
                    firstmatestore=firstmate_input;
                else {
                    cerr<<"Invalid argument to --first-mate"<<endl;
                    exit(1);
                }
                break;
            }
            

            case 'o': {
                string order_input=optarg;
                if (order_input.size()==4) {
                    for (int i = 0; i < 4; i++) {
                        if (order_input[i]=='N' || order_input[i]=='S' || order_input[i]=='W'|| order_input[i]=='E') {
                            if (i==3) {
                                order=order_input;
                            }
                        }
                        else {
                            cerr<<"Invalid argument to --hunt-order"<<endl;
                            exit(1);
                        }
                    }
                }
                else {
                    cerr<<"Invalid argument to --hunt-order"<<endl;
                    exit(1);
                }
                break;
            }

            case 'p': {
                string path_input=optarg;
                if (path_input=="L" || path_input=="M"){
                    path_type=path_input;                    
                    path=true;
                }
                else{
                    cerr<<"Invalid argument to --show-path"<<endl;
                    exit(1);
                }
                break;
            }

            case 'h': {
                cout     << "The Program reads a list or map file in order to find the most efficient path to the treasure \n"
                         << "The captain will raom around water looking for land and once land is found the firstmate will go on an expedition to find treasure on the island \n"
                         << "Enter in captain or firstmate with the stack or queue method \n"
                         << "Enter hunt order to specify in which direction the members of the game need to search the treasure\n'"
                         << "Enter verbose to see the latest update of the expedition \n"
                         << "Enter stats to see the full analysis of the expedition \n"
                         << "Finally enter show-path to see the pathing to the treasure \n"
                         << "\n";
                exit(0);
            }

            case 's': {
                stats=true;
                break;
            }
            
            case 'v': {
                verbose=true;
                break;
            }
           
            default:
                cerr<<"Unknown option"<<endl;
                exit(1);
    
        }
    }
    
}

void Game::print_path(string path) {
    //unsure how to print the path
    //cout << path << endl;
}


void Game::print_stats() {
    
    cout<<"--- STATS ---\n";
    cout<<"initialing location: "<<initial.r<<","<<initial.c<<endl;
    cout<<"Water locations investigated: "<<waterlength<<endl;
    cout<<"Land locations investigated: "<<landlength<<endl;
    cout<<"Went landed: "<<landed<<endl;
    cout<<"Path length: "<<pathdir.size()<<endl;
    cout<<"--- STATS ---"<<endl;
}