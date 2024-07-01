// =============================================================================
//
// Name: Karplus-Strong
// Type: Audio
// Written by: Răzvan Lazea-Postelnicu, 2016
// 
// Description: A simple Karplus-Strong implementation
//
// SR  - Source pitch
// MOD - Source selection
// [1] - filter cutoff
// [2] - filter resonance
// [3] - delay rate aka pitch
// GATE - trigger
//
// =============================================================================

#ifndef KarplusStrong_h
#define KarplusStrong_h

#include "Synth.h"

class KarplusStrong : public Synth
{
  public:
    KarplusStrong(Inputs *inputs); 
};

#endif
