#ifndef INSTANCE_H
#define INSTANCE_H
#include <iostream>

class Instance {
  private:
    int instance_unique_id;
  public:
    int static serial_id;
    Instance() {
      Instance::serial_id++;
      instance_unique_id = Instance::serial_id;
    }
    int get_unique_id() { return instance_unique_id; }
};
#endif
