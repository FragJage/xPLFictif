#include <iostream>
#include <exception>
#include "xPLFictif.h"

int main(int argc, char* argv[])
{
  int res = 0;

  try
  {
    xPLFictif xPLDev;

  	Service* pService = Service::Create("xPLFictif", "Fictives sensors for xPL protocol", &xPLDev);
    res = pService->Start(argc, argv);
  	Service::Destroy();
  }
  catch(const exception &e)
  {
      std::cout << e.what();
  }
	return res;
}
