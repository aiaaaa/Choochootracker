#pragma once
// Self-authored waveforms and minimal tracker arrangements; no downloaded music.
#include <vector>
#include <string>
#include <cmath>
#include <cstring>
namespace luckyFixture {
using Bytes=std::vector<uint8_t>;
inline void text(Bytes& b,size_t p,const char* s) { memcpy(b.data()+p,s,strlen(s)); }
inline void le16(Bytes& b,size_t p,unsigned n) { b[p]=n; b[p+1]=n>>8; }
inline void le32(Bytes& b,size_t p,unsigned n) { le16(b,p,n);le16(b,p+2,n>>16); }
inline void be16(Bytes& b,size_t p,unsigned n) { b[p]=n>>8;b[p+1]=n; }
inline int16_t wave(unsigned i,unsigned channel=0) { return int16_t(std::sin(i*6.283185307179586/32)*(channel?6000:12000)); }
inline Bytes mod(unsigned variation=0) {
  Bytes b(1084+1024+4*256); text(b,0,variation?"Fixture B":"Fixture A");
  const char* names[]={"One shot","Whole loop","Attack sustain","Tuned +half"};
  for(unsigned i=0;i<4;++i) {
    unsigned p=20+i*30;text(b,p,names[i]);be16(b,p+22,128);b[p+25]=64;
    if(i==1) be16(b,p+28,128);
    else if(i==2) { be16(b,p+26,32);be16(b,p+28,64); }
    else be16(b,p+28,1);
    if(i==3) b[p+24]=4;
    for(unsigned j=0;j<256;++j) b[2108+i*256+j]=uint8_t(int8_t(wave(j+variation)/256));
  }
  b[950]=1;text(b,1080,"M.K.");
  b[1084]=1;b[1085]=0xac;b[1086]=0x10; // instrument 1, period 428
  b[1084+4*16+2]=0x0b;b[1084+4*16+3]=0; // short one-pass arrangement
  return b;
}
inline Bytes xm(bool sixteen,bool stereo) {
  unsigned stride=(sixteen?2:1)*(stereo?2:1), frames=256;
  Bytes b(336+9+5+263+40+frames*stride);
  text(b,0,"Extended Module: ");text(b,17,"Own XM fixture");b[37]=0x1a;text(b,38,"ChooChoo tests");le16(b,58,0x104);le32(b,60,276);
  le16(b,64,1);le16(b,68,1);le16(b,70,1);le16(b,72,1);le16(b,74,1);le16(b,76,6);le16(b,78,125);
  unsigned p=336;le32(b,p,9);le16(b,p+5,1);le16(b,p+7,5);b[p+9]=49;b[p+10]=1;
  p+=14;le32(b,p,263);text(b,p+4,"Own sample");le16(b,p+27,1);le32(b,p+29,40);
  p+=263;le32(b,p,frames*stride);le32(b,p+8,frames*stride);b[p+12]=64;b[p+13]=32;b[p+14]=1+(sixteen?16:0)+(stereo?32:0);b[p+15]=128;b[p+16]=12;text(b,p+18,"Tuned XM");p+=40;
  for(unsigned c=0;c<(stereo?2U:1U);++c) {
    int previous=0;
    for(unsigned i=0;i<frames;++i) { int now=sixteen?wave(i,c):int8_t(wave(i,c)/256);int delta=now-previous;previous=now;
      if(sixteen) {le16(b,p,uint16_t(delta));p+=2;} else b[p++]=uint8_t(delta);
    }
  }
  return b;
}
inline Bytes s3m(bool sixteen,bool stereo) {
  unsigned frames=256,stride=(sixteen?2:1)*(stereo?2:1);
  Bytes b(512+frames*stride);text(b,0,"Own S3M fixture");b[28]=0x1a;b[29]=16;le16(b,32,1);le16(b,34,1);le16(b,36,1);le16(b,40,0x1320);le16(b,42,1);text(b,44,"SCRM");b[48]=64;b[49]=6;b[50]=125;b[51]=0xb0;
  for(unsigned i=0;i<32;++i)b[64+i]=255;b[64]=0;b[96]=0;le16(b,97,8);le16(b,99,16);
  unsigned p=128;b[p]=1;le16(b,p+14,32);le32(b,p+16,frames);le32(b,p+24,frames);b[p+28]=64;b[p+31]=1+(sixteen?4:0)+(stereo?2:0);le32(b,p+32,8363);text(b,p+48,"Own S3M sample");text(b,p+76,"SCRS");
  le16(b,256,67);b[258]=0x20;b[259]=0x40;b[260]=1; // C4, instrument 1, then 64 row terminators
  p=512;for(unsigned c=0;c<(stereo?2U:1U);++c)for(unsigned i=0;i<frames;++i){if(sixteen){le16(b,p,uint16_t(wave(i,c)));p+=2;}else b[p++]=uint8_t(int8_t(wave(i,c)/256));}
  return b;
}
inline Bytes it(bool sixteen,bool stereo) {
  unsigned frames=256,stride=(sixteen?2:1)*(stereo?2:1);
  Bytes b(512+frames*stride);text(b,0,"IMPM");text(b,4,"Own IT fixture");le16(b,32,1);le16(b,36,1);le16(b,38,1);le16(b,40,0x214);le16(b,42,0x214);b[48]=128;b[49]=48;b[50]=6;b[51]=125;b[52]=128;
  for(unsigned i=0;i<64;++i){b[64+i]=32;b[128+i]=64;}b[192]=0;le32(b,193,256);le32(b,197,336);
  unsigned p=256;text(b,p,"IMPS");b[p+17]=64;b[p+18]=1+16+(sixteen?2:0)+(stereo?4:0);b[p+19]=64;text(b,p+20,"Own IT sample");b[p+46]=1;le32(b,p+48,frames);le32(b,p+56,frames);le32(b,p+60,8363);le32(b,p+72,512);
  le16(b,336,5);le16(b,338,1);b[344]=0x81;b[345]=3;b[346]=48;b[347]=1;b[348]=0;
  p=512;for(unsigned c=0;c<(stereo?2U:1U);++c)for(unsigned i=0;i<frames;++i){if(sixteen){le16(b,p,uint16_t(wave(i,c)));p+=2;}else b[p++]=uint8_t(int8_t(wave(i,c)/256));}
  return b;
}
}
