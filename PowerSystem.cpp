#include <iostream>

class System {
protected:
  enum class States {On, Off, DefMod};
  States state;
public:
  void set_state(States curr_state) {
    state = curr_state;
  }
  States get_state() {
    return state;
  }
};

class PowerSupplySystem : public System {
protected:
    float mains_voltage;
    float mains_amperage;
    float standby_mains_voltage;
    float standby_mains_amperage;
    friend class Battaries;
    friend class Load;
public:
    PowerSupplySystem() {
      set_state(States::On);
    }
    float get_voltage() {
      if (state == States::On)
        return mains_voltage;
      else if (state == States::DefMod)
        return standby_mains_voltage;
      else
        return 1;
    }
    float get_amperage() {
      if (state == States::On)
        return mains_amperage;
      else if (state == States::DefMod)
        return standby_mains_amperage;
      else
        return 1;
    }
};

class Battaries : public PowerSupplySystem {
private:
    static constexpr int amount_battaries = 3;
    static constexpr float nominal_voltage = 28;
    static constexpr float critical_voltage = 14;
    float current_voltages[amount_battaries];
    float standby_current_voltages[amount_battaries];
public:
    Battaries() {
      set_state(States::On);
      for (int i = 0; i < amount_battaries; i++) {
        current_voltages[i] = nominal_voltage;
        standby_current_voltages[i] = nominal_voltage;
      }
    }

    void discharge_batteries(PowerSupplySystem& obj, float* discharge_amount) {
      States state = obj.get_state();
      float* voltages;
      if (state == States::On)
        voltages = current_voltages;
      if (state == States::DefMod)
        voltages = standby_current_voltages;
      float summary_voltage = 0;
      for (int i = 0; i < amount_battaries; i++) {
        voltages[i] -= discharge_amount[i];
        summary_voltage += voltages[i];
      }
      if (state == States::On) {
        obj.mains_voltage = summary_voltage / amount_battaries;
        if (state == States::On && obj.mains_voltage < critical_voltage) {
          obj.set_state(States::DefMod);
          obj.standby_mains_voltage = nominal_voltage;
        }
      }
      else if (state == States::DefMod)
        obj.standby_mains_voltage = summary_voltage / amount_battaries;
    }
};

class Load : public PowerSupplySystem {
private:
  static constexpr float critical_amperage = 1;
  float load_resistance = 100;
  float standby_load_resistance = 50;
public:
  Load() {
    set_state(States::On);
  }
  void change_load_resistance(PowerSupplySystem& obj, float delta_load_resistance) {
    States state = obj.get_state();
    if (state == States::On) {
      load_resistance += delta_load_resistance;
      obj.mains_amperage = obj.get_voltage() / load_resistance;
      if (obj.mains_amperage > critical_amperage) {
        obj.set_state(States::DefMod);
      }
    }
    else if (state == States::DefMod) {
      standby_load_resistance += delta_load_resistance;
      obj.standby_mains_amperage = obj.get_voltage() / standby_load_resistance;
    }
  }
};

int main() {
  float discharge_amount_1[3] = {3, 3, 3};
  float discharge_amount_2[3] = {15, 15, 15};
  float discharge_amount_3[3] = {2, 2, 2};
  PowerSupplySystem PWS;
  Battaries battaries;
  Load load;

  battaries.discharge_batteries(PWS, discharge_amount_1);
  std::cout << "On-board amperage " << PWS.get_voltage() << '\n';
  load.change_load_resistance(PWS, 0);
  std::cout << "On-board amperage " << PWS.get_amperage() << '\n';
  battaries.discharge_batteries(PWS, discharge_amount_2);
  std::cout << "On-board voltage " << PWS.get_voltage() << '\n';
  load.change_load_resistance(PWS, 0);
  std::cout << "On-board amperage " << PWS.get_amperage() << '\n';
  battaries.discharge_batteries(PWS, discharge_amount_3);
  std::cout << "On-board voltage " << PWS.get_voltage() << '\n';
  load.change_load_resistance(PWS, 0);
  std::cout << "On-board amperage " << PWS.get_amperage() << '\n';
}
