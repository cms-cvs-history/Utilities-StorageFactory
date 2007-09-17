#ifndef STORAGE_FACTORY_DCACHE_STORAGE_MAKER_H
# define STORAGE_FACTORY_DCACHE_STORAGE_MAKER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Utilities/StorageFactory/interface/StorageMaker.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class DCacheStorageMaker : public StorageMaker
{
public:
    // implicit constructor
    // implicit destructor
    // implicit copy constructor
    // implicit assignment operator

protected:
    virtual seal::Storage *doOpen (const std::string &proto,
		    		 const std::string &path,
				 int mode,
				 const std::string &tmpdir);
    virtual bool	  doCheck (const std::string &proto,
		    		 const std::string &path,
				 seal::IOOffset *size = 0);
private:
    static std::string	pathForUrl (const std::string &proto,
		    		    const std::string &path);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // STORAGE_FACTORY_DCACHE_STORAGE_MAKER_H