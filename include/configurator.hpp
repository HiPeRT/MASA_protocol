#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <message.hpp>
#include <typeinfo>
#include <string.h>


enum Categories : uint8_t
{
    C_person = 14,
    C_bycicle = 1,
    C_car = 6,
    C_motorbike = 13,
    C_bus = 5,
    C_marelli1 = 20,
    C_marelli2 = 21,
    C_quattroporte = 30,
    C_levante = 31,
    C_rover = 40
};


enum LightStatus : uint8_t
{
    L_green = 1,
    L_yellow = 2,
    L_red = 3
};


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
    if(strcmp(type, typeid(PrystineMessage).name()))
      return Prystine_port;
    else if(strcmp(type, typeid(DynacarVehicleMessage).name()))
      return toDynacar_port;
    else if(strcmp(type, typeid(DynaMessage).name()))
      return fromDynacar_port;
    else
      return Std_port;
  }

};

#endif
