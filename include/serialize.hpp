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
    uint8_t orientation;
    Categories category;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( latitude, longitude, speed, orientation, category );
    }

};


//Hypothetic structure for vehicle
struct RoadVehicle {
    float latitude;
    float longitude;
    uint8_t speed;
    uint8_t orientation;
    //Categories category;
    uint8_t acc_x;
    uint8_t acc_y;
    uint8_t rpm;
    uint8_t gear;
    bool is_distracted;
    //bool we_steer;
    float  km_to_automation;
    int8_t steer_value;
    //autopilot fields
    //bool    autopilot;
    float   drive_length;
    float   start_auto;
    float   end_auto;
    float   fallback_pos;
    uint8_t car_pos;

    //uint16_t yaw;
    //uint8_t steer_ang;
    //uint8_t gyr_x;
    //uint8_t gyr_y;


    // More fields to be added here

    template<class Archive>
    void serialize(Archive &archive)
    {
        archive(
                latitude,
                longitude,
                speed,
                orientation,
                //category,
                acc_x,
                acc_y,
                rpm,
                gear,
                is_distracted,
		            //we_steer,
                km_to_automation,
                steer_value,
		            //autopilot,
                drive_length,
                start_auto,
                end_auto,
                fallback_pos,
                car_pos
                //yaw,
                //steer_ang,
                //gyr_x,
                //gyr_y
              );
    }
};




struct Message{
    uint32_t cam_idx;
    uint64_t t_stamp_ms;
    uint16_t num_objects;
    std::vector<RoadUser> objects;
    std::vector<TrafficLight> lights;
    RoadVehicle vehicle;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( cam_idx, t_stamp_ms, num_objects, objects, lights, vehicle);
    }
};




#endif
