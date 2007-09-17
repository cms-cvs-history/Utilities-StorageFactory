//<<<<<< INCLUDES                                                       >>>>>>

#include "Utilities/StorageFactory/plugins/RFIOStorageMaker.h"
#include "Utilities/RFIOAdaptor/interface/RFIOFile.h"
#include "Utilities/RFIOAdaptor/interface/RFIO.h"

#include <cstdlib>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

namespace {

  /* cope with new RFIO TURL stile
   * try to correct most obvious mispelling (//////) 
   * try to cope with /dpm and /castor syntax too
   */
  std::string normalizeURL(const std::string &path) {
    std::string ret;
    // look for options
    size_t p = path.find("?");
    if (p==std::string::npos) {
      // old syntax
      p=0;       
      // special treatment for /dpm: use old syntax
      size_t e = path.find_first_not_of ("/");
      size_t c = path.find("/castor/");
      if (c==e-1) {
	p = c;
	ret = "rfio:///?path=";
      }
      else {
        // special treatment for /castor
        c = path.find("/dpm/");
        if (c==e-1) {
	  p = c;
	  ret = "rfio:///?path=";
        }
      }
    }
    else {
      // new syntax, normalize host...
      ret = path.substr(0,p);
      //get the host (if any)
      size_t h = ret.find_first_not_of('/');
      size_t s = ret.find_last_not_of('/');
      ret.resize(s+1);
      ret.replace(0,h,"rfio://");
      ret+='/';
    }
    return ret+path.substr(p);
  }
}

RFIOStorageMaker::RFIOStorageMaker() {
// init rfio MT
  Cthread_init();

  // this was suggested by Olof at castor-operations
  // It should help debugging CASTOR/rfio problems
  // by forcing "control messages" to be moved out of the
  // TCP buffer on clients faster.
  putenv("RFIO_TCP_NODELAY=1");
}

seal::Storage *
RFIOStorageMaker::doOpen (const std::string & /* proto */,
		        const std::string &path,
			int mode,
		        const std::string & /* tmpdir */)
{ return new RFIOFile (normalizeURL(path), mode); }

bool
RFIOStorageMaker::doCheck (const std::string &proto,
		         const std::string &path,
		         seal::IOOffset *size /* = 0 */)
{
    std::string npath = normalizeURL(path);
    if (rfio_access (npath.c_str (), R_OK) != 0)
	return false;

    if (size)
    {
	struct stat64 buf;
	if (rfio_stat64 (npath.c_str (), &buf) != 0)
	    return false;

	*size = buf.st_size;
    }

    return true;
}