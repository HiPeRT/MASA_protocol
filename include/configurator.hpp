#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <message.hpp>



// Ports for different messages to be added here
enum Ports : uint16_t
{
    Std_port      = 8888,
    Prystine_port = 8889,
	  toDynacar_port  = 8890,
	  fromDynacar_port  = 8891
};


template <typename T>
class Configurator {

public:
  // Public method to setup port, according to the data structure selected
  Ports getPort()
  {
    if(std::is_same<T, PrystineMessage>::value)
      return Prystine_port;
    else if(std::is_same<T, DynacarVehicleMessage>::value)
      return toDynacar_port;
    else if(std::is_same<T, DynaMessage>::value)
      return fromDynacar_port;
    else
      return Std_port;
  }
};

#endif
