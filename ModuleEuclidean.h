#ifndef ModuleEuclidean_h
#define ModuleEuclidean_h
 
// Always include these three files
#include "Arduino.h"
#include "Module.h"
#include "ModuleOutput.h"
 
class ModuleEuclidean : public Module
{
  public:
	ModuleEuclidean();
	uint16_t compute();
	
    // Inputs
	Module *clock_input;
	Module *on_input;
	Module *total_input;
	
  private:
    int step;
    int sequence[64];
	uint16_t old_clock;
	uint16_t old_on;
	uint16_t old_total;
};
 
#endif