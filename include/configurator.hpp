#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <messages.hpp>
#include <typeinfo>
#include <string.h>


// Ports for different messages to be added here
enum Ports : uint16_t
{
    Std_port      = 8888,
    Prystine_port = 8889,
	  toDynacar_port  = 8890,
	  fromDynacar_port  = 8891
};


class Configurator {

public:
  // Public method to setup port, according to the data structure selected
  Ports getPort(const char * type)
  {
    if(strcmp(type, typeid(PrystineMessage).name()) == 0)
      return Prystine_port;
    else if(strcmp(type, typeid(DynacarVehicleMessage).name()) == 0)
      return toDynacar_port;
    else if(strcmp(type, typeid(DynaMessage).name()) == 0)
      return fromDynacar_port;
    else
      return Std_port;
  }

};

#endif
