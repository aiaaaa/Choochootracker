#pragma once
#include <cstdio>
#include <cstdint>

// Run on the actual target before reading user projects. The SDK's default
// scanner rejects hh conversions despite the application's C++17 build mode.
inline bool vitaStdioCompatible() {
  uint8_t byte[3]={0x5a,0,0xa5}, hex[3]={0x5a,0,0xa5};
  int8_t sign[3]={0x5a,0,0x5a};
  uint16_t wide=0;
  float rate=0;
  const int count=sscanf("255,-12,A7,16000,59.25", "%hhu,%hhd,%hhX,%hu,%f",
                         &byte[1],&sign[1],&hex[1],&wide,&rate);
  return count==5 && byte[1]==255 && sign[1]==-12 && hex[1]==0xa7 &&
    wide==16000 && rate==59.25f && byte[0]==0x5a && byte[2]==0xa5 &&
    sign[0]==0x5a && sign[2]==0x5a && hex[0]==0x5a && hex[2]==0xa5;
}
