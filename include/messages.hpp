#ifndef MESSAGE_H
#define MESSAGE_H

#include "serialize.hpp"


struct Message{
    uint32_t cam_idx;
    uint64_t t_stamp_ms;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive(cam_idx, t_stamp_ms);
    }
};


struct MasaMessage : Message{
    uint16_t num_objects;
    std::vector<RoadUser> objects;
    std::vector<TrafficLight> lights;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive(cam_idx, t_stamp_ms, num_objects, objects, lights);
    }
};



//Message struct for Prystine HMI use-case scenario
struct PrystineMessage: Message{
    PrystineVehicle pv;
    PrystineGpsData gps;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive(cam_idx, t_stamp_ms, pv, gps);
    }
};



//Dynacar
struct DynacarVehicleMessage : Message{
	DynacarVehicle vehicle;

	template<class Archive>
	void serialize(Archive & archive){
		archive( cam_idx, t_stamp_ms, vehicle);
	}
};

struct DynaMessage : Message{
	double speed;
	double speed_km;
	double x;
	double y;
	double z;

	template<class Archive>
	void serialize(Archive & archive){
		archive( cam_idx, t_stamp_ms, speed, speed_km, x, y, z);
	}
};


#endif
