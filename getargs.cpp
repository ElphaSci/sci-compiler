//
// getargs
//
// This is a reconstructed file for processing an argument list.  Functionality is based
// on what I see as necessary from usage in sc.cpp.
//
// author: Stephen Nichols
//

#include <cstdlib>
#include <cstdio>
#include <cctype>

#include "getargs.hpp"
#include "string.hpp"

// define the GA_PROC type
typedef void(*ga_proc_t) ( char *str );

// this tracks if the arguments have been parsed
int gArgsInitted = 0;

// this is set to the total number of valid Arg structures that are contained in the switches array
int gSwitchCount = 0;

// this is the name of the program
char *gProgName = NULL;

// this function prints the usage information
void ShowUsage ( void )
{
    // this only works if getargs has been called
    if ( !gArgsInitted )
        return;

    // print the usage string
    printf ( "usage: %s %s\n", gProgName, usageStr );

    // print each switch option
    for ( int i=0; i<gSwitchCount; i++ ) {
        Arg *arg = &switches[i];

        switch ( arg->type ) {
            case GA_BOOL:
            case GA_PROC: 
                printf ( "\t-%c\t%s\n", arg->switchVal, arg->desc );
                break;

            case GA_STR: 
                printf ( "\t-%c\t%s <default is \"%s\">\n", arg->switchVal, arg->desc, (char *)*arg->value );
                break;

            case GA_INT: 
                printf ( "\t-%c\t%s <default is %d>\n", arg->switchVal, arg->desc, *arg->value );
                break;
        }
    }

    exit ( 1 );
}

// this function parses the available arguments and returns the number of valid arguments (1 if none)
int getargs ( int argc, char **argv )
{
    // set the program name
    char *slashPtr = strrchr ( argv[0], '/' );

    if ( slashPtr )
        gProgName = strdup ( slashPtr + 1 );
    else
        gProgName = strdup ( argv[0] );

    for (int i = 0; gProgName[i] != '\0'; i++) {
        gProgName[i] = std::tolower(static_cast<unsigned char>(gProgName[i]));
    }

    // toss the .EXE extension
    slashPtr = strrchr ( gProgName, '.' );

    if ( slashPtr )
        *slashPtr = 0;

    // figure out how many valid switches there are
    Arg *arg = &switches[gSwitchCount];

    while ( arg->desc ) {
        gSwitchCount++;
        arg = &switches[gSwitchCount];
    }

    // set the initted variable
    gArgsInitted = 1;

    // step through each argument and make sure they are valid
    for ( int i=1; i<argc; i++ ) {
        char *argStr = argv[i];

        // if the argStr has a '-' at the front, validate it
        if ( *argStr == '-' ) {
            int valid = 0;

            // check each switch option
            for ( int j=0; j<gSwitchCount; j++ ) {
                arg = &switches[j];

                // the switch matches, it might be
                if ( argStr[1] == arg->switchVal ) {
                    valid = 1;

                    switch ( arg->type ) {
                        case GA_BOOL: {
                            // if there is any more string left, this is not valid
                            if ( argStr[2] != 0 ) 
                                return 1;

                            // set this argument
                            *arg->value = 1;
                        }

                        break;

                        case GA_INT: {
                            // if the rest of the string is not numeric, this is not valid
                            char *ptr = &argStr[2];
                            int count = 0;

                            // count the digits
                            while ( *ptr && isdigit ( *ptr ) ) {
                                ptr++;
                                count++;
                            }

                            // this must be a non-digit
                            if ( *ptr )
                                return 1;

                            // there were no digits!
                            if ( count == 0 )
                                return 1;

                            // set the argument
                            *arg->value = atoi ( &argStr[2] );
                        }

                        break;

                        case GA_STR: { 
                            // there must be at least one more character
                            if ( argStr[2] == 0 )
                                return 1;

                            *((char **)arg->value) = newStr ( &argStr[2] );
                        }

                        break;

                        case GA_PROC: { 
                            // there must be at least one more character
                            if ( argStr[2] == 0 )
                                return 1;

                            char *str = newStr ( &argStr[2] );

                            ga_proc_t proc = (ga_proc_t)arg->value;
                            proc ( str );

                            free ( str );
                        }

                        break;
                    }

                    break;
                }
            }

            // if this argument is not valid, return error
            if ( !valid )
                return 1;

            // zero out the argument for the exargs pass
            *argStr = 0;
        }
    }

    return (argc==1);
}

// this function removes all of the switch arguments and leaves the rest alone
void exargs ( int *argcPtr, char ***argvPtr )
{
    // get the current argv/argc
    int argc = *argcPtr;
    char **argv = *argvPtr;

    // allocate a new argv
    char **newArgs = (char **)malloc ( sizeof ( char * ) * 10240 );  // a bit of a kludge, but nobody will add more than 10240 command line parameters (or build list entries)
    int putIdx = 0;

    // step through all arguments and get rid of the ones with 0 length
    for ( int i=0; i<argc; i++ ) {
        // copy this argument
        switch ( *argv[i] ) {
            case 0:
                break;

            // let's read this parameter as though it were a build list
            case '@': {
                char fileName[1024];
                char *name = argv[i];
                name++;

                FILE *file = fopen ( name, "rt" );
                
                if ( file ) {
                    while ( !feof ( file ) ) {
                        fileName[0] = 0;
                        fgets ( fileName, 1024, file );
    
                        // trim this file name
                        trimstr ( fileName );
    
                        // add the file to the argument list
                        if ( fileName[0] ) {
                            newArgs[putIdx] = newStr ( fileName );
                            putIdx++;
                        }
                    }

                    fclose ( file );
                } else {
                    newArgs[putIdx] = argv[i];
                    putIdx++;
                }
            }

            break;

            default: {
                newArgs[putIdx] = argv[i];
                putIdx++;
            }
        }
    }

    *argvPtr = newArgs;
    *argcPtr = putIdx;
}


