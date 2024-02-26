//
// Assessment include for Coursework 1.
//
// Do NOT alter or re-write any of these routines.
//


//
// Globals: Set size and the set itself.
//
int setSize;
int maxSetSize;
int *set;


//
// Set creation, destruction, and display.
//

// Initialises the set. Returns 0 if okay, -1 if there was an error.
int initSet( int maxSize )
{
    maxSetSize = maxSize;
    set = (int*) malloc( sizeof(int)*maxSetSize );

    if( set != 0 ) return 0;

    printf( "WARNING: Failed to allocate memory for the set." );
    return -1;
}

// Destroys the set, i.e. deletes all resources dedicated to it.
void destroySet()
{
    free( set );
}

// Displays the current state of the set.
void printSet()
{
    int i;
  
    if( setSize==0 )
    {
        printf( "Set is empty.\n" );
        return;
    }

    if( setSize==1 )
    {
        printf( "Set has one entry: %i\n", set[0] );
        return;
    }

    printf( "Set has %i entries:\n", setSize );
    for( i=0; i<setSize; i++ ) printf( "%i\t", set[i] );
    printf( "\n" );
}

