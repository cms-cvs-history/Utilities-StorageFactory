//<<<<<< INCLUDES                                                       >>>>>>

#include "Utilities/StorageFactory/interface/StorageFactory.h"
#include "Utilities/StorageFactory/interface/StorageAccount.h"
#include "PluginManager/PluginManager.h"
#include "SealZip/ZipArchive.h"
#include "SealZip/ZipMember.h"
#include "SealBase/Storage.h"
#include "SealBase/TempFile.h"
#include "SealBase/Filename.h"
#include "SealBase/Error.h"
#include "SealBase/DebugAids.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

using namespace seal;
static int sizeRatio (ZipMember *m)
{
    if (m->size (ZipMember::UNCOMPRESSED) == 0)
	return 0;

    return int (100. - (100. * m->size (ZipMember::COMPRESSED)
			/ m->size (ZipMember::UNCOMPRESSED))
		+ .5);
}

int main (int argc, char **argv)
{
    Signal::handleFatal (argv [0]);
    PluginManager::get ()->initialise ();

    // Prepare temporary directory
    Filename tmpdir;
    TempFile::dir (tmpdir);

    // Check arguments
    if (argc != 2)
    {
	std::cerr << "usage: " << argv[0] << " zip-storage\n";
	return EXIT_FAILURE;
    }

    // Loate the storage object
    Storage	*store = 0;
    int		ret;

    try
    {
	store = StorageFactory::get ()->open (argv[1], IOFlags::OpenRead, tmpdir.name ());
	if (! store)
	{
	    std::cerr << argv[1] << ": error: cannot open\n";
	    return EXIT_FAILURE;
	}

	// Read it as a zip file
	ZipArchive zip (store);
	for (ZipArchive::Iterator i = zip.begin (); i != zip.end (); ++i)
	    std::cout << std::setw (7) << (*i)->size (ZipMember::UNCOMPRESSED)
		      << " " << (*i)->method () << ":" << (*i)->level () << " "
		      << std::setw (7) << (*i)->size (ZipMember::COMPRESSED) << " "
		      << std::setw (3) << sizeRatio (*i) << "% "
		      << (*i)->time ().format (true, "  %Y-%m-%d %H:%M:%S ")
		      << std::hex << std::setw (8) << (*i)->crc32 () << std::dec
		      << " " << (*i)->name () << "\n";

	// Delete temporaries
	zip.close ();
	delete store;

	ret = EXIT_SUCCESS;
    }
    catch (Error &e)
    {
	delete store;
	std::cerr << argv [1] << ": error: " << e.explain () << "\n";
	ret = EXIT_FAILURE;
    }

    Filename::remove (tmpdir, true);

    std::cerr << "stats:\n" << StorageAccount::summaryText ();
    return ret;
}
