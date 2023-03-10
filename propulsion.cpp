class PropulsionSystem : public System {
private:
    static unsigned number_of_engines;
public:
    static unsigned get_number_of_engines() {return number_of_engines;}
};
unsigned PropulsionSystem::number_of_engines = 0;

class RocketEngine : public PropulsionSystem {
private:
    struct RequeredFuel {
        FuelSystem::FuelType fuel_type;
        float consumption;
        FuelTank* connected_tank;

        RequeredFuel(FuelSystem::FuelType fuel_type, float consumption) :
            fuel_type(fuel_type), consumption(consumption);
    }

    float thrust;
    int number_of_fuels;
    RequeredFuel* requered_fuels;
public:
    RocketEngine(float thrust, int number_of_fuels, RequeredFuel* fuels) :
        thrust(thrust), number_of_fuels(number_of_fuels) {
        number_of_engines++;
        requered_fuels = new RequeredFuel[number_of_fuels];
        for (auto i = 0; i < number_of_fuels; i++) {
            requered_fuels[i] = fuels[i];
        }
    }

    ~RocketEngine() {
        delete[] requered_fuels;
    }

    //for simplicity it's possible to connect only one tank per fuel type
    bool connect_tank(FuelTank* fuel_tank) {
        for (auto i = 0; i < number_of_fuels; i++) {
            if (requered_fuels[i].fuel_type == fuel_tank.get_type()) {
                requered_fuels[i].connected_tank = fuel_tank;
                return 1;
            }
        }
        return 0;
    }

    bool run(float time) {
        for (auto i = 0; i < number_of_fuels; i++) {
            float used_fuel = time * requered_fuels[i].consumption;
            if (!requered_fuels[i].fuel_tank->drain_amount(used_fuel)) {
                return 0;
            }
        }
        return 1;
    }

    float get_thrust() {return thrust;}
};
