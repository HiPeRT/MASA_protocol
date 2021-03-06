#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/vector.hpp>


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

struct TrafficLight{
    float latitude;
    float longitude;
    uint8_t orientation;
    LightStatus status;
    uint8_t time_to_change;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( latitude, longitude, orientation, status, time_to_change );
    }
};

struct RoadUser{
    float latitude;
    float longitude;
    uint8_t speed;
    uint16_t orientation;
    Categories category;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( latitude, longitude, speed, orientation, category );
    }

};


//Hypothetic structure for vehicle
struct Vehicle : RoadUser{
    uint8_t rpm;
    uint8_t gear;

    // More fields to be added here

    template<class Archive>
    void serialize(Archive &archive)
    {
        archive(
                latitude,
                longitude,
                speed,
                orientation,
                category,
                rpm,
                gear
              );
    }
};



struct PrystineGpsData {
  float     destinationLatitude;
  float     destinationLongitude;

  template<class Archive>
  void serialize(Archive &archive)
  {
      archive(
              destinationLatitude,
              destinationLongitude
            );
  }

};


//Vehicle struct for Christine HMI use-case scenario
struct PrystineVehicle : Vehicle {
    bool    is_distracted;
    float   km_to_automation;
    int8_t  steer_value;
    float   drive_length;
    float   start_auto;
    float   end_auto;
    uint8_t car_pos;

    template<class Archive>
    void serialize(Archive &archive)
    {
        archive(
                latitude,
                longitude,
                speed,
                orientation,
                category,
                rpm,
                gear,
                is_distracted,
                km_to_automation,
                steer_value,
                drive_length,
                start_auto,
                end_auto,
                car_pos
              );
    }
};


struct DynacarVehicle : Vehicle{
	double speed_d;
	double speed_km;
	double throttle;
	double brake;
	double n_steering;

    template<class Archive>
    void serialize(Archive &archive)
    {
        archive(
                latitude,
                longitude,
                speed_d,
				speed_km,
				throttle,
				brake,
				n_steering
              );
    }
};







#endif
