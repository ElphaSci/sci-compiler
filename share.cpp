//	share.cpp	sc
// 	routines for locking/unlocking the class database so as to make sc
// 	network-compatible

#include <fcntl.h>
#include <sys/stat.h>
#include <csignal>
#include <ctime>
#include <cstdlib>
#include <sys/file.h>

#include "sol.hpp"

#include	"jeff.hpp"

#include	"sc.hpp"

#include "error.hpp"

Bool	abortIfLocked;
Bool	dontLock;

const int LOCKMODE	= O_CREAT | O_EXCL | O_RDONLY;

static int	haveLock;
static char	lockFile[] = "$$$sc.lck";

static void	Abort(int sig);

void
Lock()
{
	int		fd;
	time_t	now;
	time_t	then;

	if (dontLock)
		return;

	// First trap interrupts so that if we're aborted while holding the lock
	// we can release it.
	signal(SIGINT, Abort);
	signal(SIGABRT, Abort);

    // Create the semaphore file.  If we can't do so, loop until we can.
    while ((fd = open(lockFile, LOCKMODE, S_IRUSR | S_IWUSR)) == -1) {
        if (abortIfLocked) {
            Panic("Access to database denied");
        }
    }

    // Attempt to lock the file
    while (flock(fd, LOCK_EX) == -1) {
        perror("Error locking file");
        do {
            time(&now);
        } while (now <= then);
        then = now;
        putc('.', stderr);
    }

	flock(fd, LOCK_UN);
    close(fd);
	output("Class database locked.\n");
	haveLock = True;
}

void
Unlock()
{
	if (haveLock) {
		chmod(lockFile, (int) (S_IREAD | S_IWRITE));
		unlink(lockFile);
		output("Class database unlocked.\n");
	}
}

static void
Abort(int)
{
	Unlock();
	exit(1);
}
