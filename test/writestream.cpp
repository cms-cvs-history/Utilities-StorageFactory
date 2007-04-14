//<<<<<< INCLUDES                                                       >>>>>>

#include "Utilities/StorageFactory/interface/StorageFactory.h"
#include "Utilities/StorageFactory/interface/StorageAccount.h"
#include "FWCore/PluginManager/interface/PluginManager.h"
#include "FWCore/PluginManager/interface/standard.h"
#include "SealIOTools/StorageStreamBuf.h"
#include "SealBase/DebugAids.h"
#include "SealBase/Signal.h"
#include "SealBase/Error.h"
#include "SealBase/File.h"
#include <iostream>

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
int main (int, char **argv)
{
    Signal::handleFatal (argv [0]);
    edmplugin::PluginManager::configure(edmplugin::standard::config());

    try
    {
	std::string		user (UserInfo::self ()->id ());
	std::string		path (std::string ("rfio:/castor/cern.ch/user/")
			       	      + user[0] + "/" + user + "/rfiotest");

    StorageFactory::get ()->enableAccounting(true);
    Storage			*s = StorageFactory::get ()->open
	    (path.c_str (), IOFlags::OpenWrite | IOFlags::OpenCreate | IOFlags::OpenTruncate);
	StorageStreamBuf	buf (s);
	std::ostream		output (&buf);

	for (int i = 0; i < 100; ++i)
	    output << "sample output line " << i << std::endl;

	output.flush ();
	s->close ();
    }
    catch (Error &e)
    {
	std::cerr << e.explain () << std::endl;
	return EXIT_FAILURE;
    }

    std::cerr << "stats:\n" << StorageAccount::summaryText () << std::endl;
    return EXIT_SUCCESS;
}
