#include "menuTrees.h"

int main( int argc, char *argv[] )
{
    if( argc > 1 ) {
        
        fprintf(stdout, "Parameter listing:\n"); fflush(stdout);
        
        for (int i = 1; i<argc; i++) {
            fprintf(stdout, "%4i: %s %s\n", i,"input file name:", argv[i]); fflush(stdout);
        }
        
        for (int i = 1; i<argc; i++) {
            makeTree(argv[i]);
        }
        
        return 0;
    
    }
    else {
        
        fprintf( stderr, "Error: %s\n", "No file menu provided in arguments.");
        return 1;
    
    }
}
