#include<string>
#include<iostream>

std::string
pathForUrl (const std::string &proto, const std::string &path)
{
  size_t p=path.find("/pnfs");
  if (p<3) return (proto == "gsidcap") ? proto + ':'+ path.substr(p) : path.substr(p);
  // remove multiple "/"
  p=path.find_first_not_of('/');
  // it's url, return the full thing
  return proto + "://" + path.substr(p);
}


int main() {

  std::cout << pathForUrl("dcache","///pnfs/cms/WAX/7/PHEDEX2") << std::endl;
  std::cout << pathForUrl("dcache","/pnfs/cms/WAX/7/PHEDEX2") << std::endl;
  std::cout << pathForUrl("gsidcap","///pnfs/cms/WAX/7/PHEDEX2") << std::endl;
  std::cout << pathForUrl("gsidcap","/pnfs/cms/WAX/7/PHEDEX2") << std::endl;
  std::cout << pathForUrl("dcache","//host:20/pnfs/cms/WAX/7/PHEDEX2") << std::endl;
  std::cout << pathForUrl("dcache","//pnfs/cms/WAX/7/PHEDEX2") << std::endl;
  std::cout << pathForUrl("gsidcap","//host:20/pnfs/cms/WAX/7/PHEDEX2") << std::endl;
  std::cout << pathForUrl("gsidcap","///host:20/pnfs/cms/WAX/7/PHEDEX2") << std::endl;

  return 0;

}
