//
// fileio
//
// This file contains various file I/O and path manipulation routines.
//
// author: Stephen Nichols
//

#include "string.hpp"
#include "fileio.hpp"
//
// This function builds a path name from the provided variables and stores it in dest.
// 
void MakeName ( char *dest, char *dir, char *name, char *ext )
{
	*dest = 0;

	// let's put the directory
	strcat ( dest, dir );

	// let's put the name if any different
	if ( dir != name ) {
		//
		// If the dir has no trailing backslash and the name has no leading backslash and
		// the dir is not a drive selector, add a backslash.
		//
		int dirLen = strlen ( dir );

        char lastChar = dir[dirLen - 1];
        if (dirLen && (lastChar != '/') && (lastChar != ':') && (*name != '/') )
			strcat ( dest, "/" );
			
		strcat ( dest, name );
	}

	// let's put the extension if any different
	if ( name != ext )
		strcat ( dest, ext );
}

//
// This function looks at the passed string and returns a pointer to the string if an
// extension is found.  Otherwise, it returns a pointer to the end of the string.
//
char *_ExtPtr ( char *str )
{
    if ( strchr ( str, '.' ) ) 
        return str;

    return &str[strlen(str)];
}

