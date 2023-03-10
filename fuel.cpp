#include <iostream>

class System {
private:
  enum States {On, Off, DefMod};
  States state;
public:
  void set_state(States curr_state) {
    state = curr_state;
  }
  States get_state() {
    return state;
  }
};

//==============================//
class FuelSystem : public System {
private:
    struct FuelInfo {
        float capacity;
        float amount;
        FuelInfo() : capacity(0), amount(0) {}
    };
protected:
    static FuelInfo fuel_info[4];
public:
    enum FuelType {LiquidFuel, Oxidizer, Monopropellant, XenonGas};

    static float get_total_capacity(FuelType fuel_type) {
        return fuel_info[fuel_type].capacity;
    }
    static float get_total_amount(FuelType fuel_type) {
        return fuel_info[fuel_type].amount;
    }
};
FuelSystem::FuelInfo FuelSystem::fuel_info[] = {};

class FuelTank : public FuelSystem {
private:
    const FuelType fuel_type;
    const float capacity;
    float amount;
public:
    FuelTank(FuelType fuel_type, float capacity) : 
        fuel_type(fuel_type), capacity(capacity) {
        fuel_info[fuel_type].capacity += capacity;
    }

    bool drain_fuel(float drain_amount) {
        if (drain_amount > amount) return 0;
        amount -= drain_amount;
        fuel_info[fuel_type].amount -= drain_amount;
        return 1;
    }

    bool pump_fuel(float pump_amount) {
        if (pump_amount + amount > capacity) return 0;
        amount += pump_amount;
        fuel_info[fuel_type].amount += pump_amount;
        return 1;
    }

    FuelType get_type() {return fuel_type;}
    float get_capacity() {return capacity;}
    float get_amount() {return amount;}
};
//==============================//

int main() {
    std::cout << std::boolalpha;

    FuelTank tank1(FuelSystem::Monopropellant, 100);
    std::cout << "Pump 100 Monoprop to tank1: " << tank1.pump_fuel(100) << '\n';

    std::cout << "Drain 25 Monoprop from tank1: " << tank1.drain_fuel(25) <<'\n';
    std::cout << "Pump 1000 Monoprop to tank1: " << tank1.pump_fuel(1000) << '\n';

    FuelTank tank2(FuelSystem::Monopropellant, 50);
    std::cout << "Pump 50 Monoprop to tank2: " << tank2.pump_fuel(50) << '\n';
    std::cout << "Total amount of Monoprop: " << FuelSystem::get_total_amount(FuelSystem::Monopropellant) << std::endl;

    return 0;
}
