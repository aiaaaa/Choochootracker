#include "project.h"
#include "sid_patch.h"

int instrumentFMOperatorCount(const Instrument* i) {
  if(!i)return 0;
  switch(i->type) {
    case InstrumentType::OPLL:case InstrumentType::VRC7:case InstrumentType::OPL2:return 2;
    case InstrumentType::OPL3:return i->chip.opl.topology==OPLTopology::twoOperator?2:4;
    case InstrumentType::GenesisFM:case InstrumentType::ArcadeFM:return 4;
    case InstrumentType::DX7:return 6;
    default:return 0;
  }
}

bool instrumentNativeFXInfo(const Instrument* i,int fx,NativeFXInfo* out) {
  if(!i||!out)return false;
  if(fx==fxFBK) {
    if(!instrumentFMOperatorCount(i))return false;
    NativeFXInfo feedback{};
    if(!instrumentNativeFXInfo(i,fxFFB,&feedback))return false;
    *out={7,feedback.preset-1,false};return true;
  }
  if(fx>=fxOL1&&fx<=fxOL6) {
    int op=fx-fxOL1;
    if(op>=instrumentFMOperatorCount(i))return false;
    int maximum=63,value=0;
    switch(i->type) {
      case InstrumentType::OPLL:case InstrumentType::VRC7:
        maximum=op?15:63;value=op?15:63-(i->chip.opll.patch[2]&63);break;
      case InstrumentType::OPL2:case InstrumentType::OPL3:value=63-i->chip.opl.operators[op].level;break;
      case InstrumentType::GenesisFM:case InstrumentType::ArcadeFM:
        maximum=127;value=127-i->chip.fourOp.operators[op].level;break;
      case InstrumentType::DX7:maximum=99;value=i->chip.dx7.voice[(5-op)*21+16];break;
      default:return false;
    }
    *out={maximum,value,false};return true;
  }
  if(!instrumentFXAvailableForInstrument(i,fx))return false;
  for(int g=genericModFMBrightness;g<genericModTotalCount;++g) {
    const auto* d=instrumentNativeModDestination(i->type,g);
    if(!d||d->fx!=fx)continue;
    bool relative=g==genericModFMBrightness || (g>=genericModFMOperator1&&g<=genericModFMOperator6) || (g>=genericModFMTime&&g<=genericModFMLFODepth);
    int value=instrumentNativeControlValue(i,g);
    if(g==genericModFMFeedback) {
      // FFB reserves zero for "preset"; explicit levels are encoded as 1..8.
      if(!value) {
        if(i->type==InstrumentType::OPLL||i->type==InstrumentType::VRC7)value=(i->chip.opll.patch[3]&7)+1;
        else if(i->type==InstrumentType::OPL2||i->type==InstrumentType::OPL3)value=i->chip.opl.feedback[0]+1;
        else if(i->type==InstrumentType::DX7)value=i->chip.dx7.voice[135]+1;
        else value=i->chip.fourOp.feedback+1;
      }
    }
    *out={d->range,value,relative};return true;
  }
  return false;
}

const InstrumentModDestination* instrumentNativeModDestination(InstrumentType t, int g) {
  static const InstrumentModDestination controls[] = {
    {"Brightness",fxFBR,255,InstrumentMotionValue::raw},
    {"Feedback",fxFFB,8,InstrumentMotionValue::raw},
    {"Mode",fxCMD,3,InstrumentMotionValue::raw},
    {"Noise rate",fxCNR,3,InstrumentMotionValue::raw},
    {"Noise divisor",fxCND,7,InstrumentMotionValue::raw},
    {"Noise shift",fxCNS,13,InstrumentMotionValue::raw},
    {"Sweep period",fxCSP,7,InstrumentMotionValue::raw},
    {"Sweep shift",fxCSS,7,InstrumentMotionValue::raw},
    {"Sweep down",fxCSD,1,InstrumentMotionValue::raw},
    {"GB env level",fxCEI,15,InstrumentMotionValue::raw},
    {"GB env period",fxCEP,7,InstrumentMotionValue::raw},
    {"GB env rise",fxCED,1,InstrumentMotionValue::raw},
  };
  static const InstrumentModDestination segaMode={"Tone / Noise",fxCMD,2,InstrumentMotionValue::raw};
  static const InstrumentModDestination pulseMode={"Duty",fxCMD,3,InstrumentMotionValue::raw};
  static const InstrumentModDestination noiseMode={"Noise width",fxCMD,1,InstrumentMotionValue::raw};
  if(g<genericModFMBrightness||g>=genericModTotalCount)return nullptr;
  static const InstrumentModDestination sid[] = {
    {"Pulse width",fxSCP,255,InstrumentMotionValue::raw},
    {"SID cutoff",fxSCT,255,InstrumentMotionValue::raw},
    {"SID resonance",fxSRN,15,InstrumentMotionValue::raw},
    {"SID waveform",fxSWV,7,InstrumentMotionValue::raw},
    {"SID filter mode",fxSFTY,7,InstrumentMotionValue::raw},
    {"Macro speed",fxSMR,255,InstrumentMotionValue::raw},
    {"Ring modulation",fxSRG,1,InstrumentMotionValue::raw},
    {"Hard sync",fxSSY,1,InstrumentMotionValue::raw},
  };
  if(g>=genericModSIDPulse&&g<=genericModSIDSync)return t==InstrumentType::SID?&sid[g-genericModSIDPulse]:nullptr;
  bool fm=t==InstrumentType::OPLL||t==InstrumentType::VRC7||t==InstrumentType::OPL2||t==InstrumentType::OPL3||t==InstrumentType::GenesisFM||t==InstrumentType::ArcadeFM||t==InstrumentType::DX7;
  static const InstrumentModDestination macros[] = {
    {"FM envelope time",fxFET,255,InstrumentMotionValue::raw},
    {"FM tone decay",fxFTD,255,InstrumentMotionValue::raw},
    {"FM detune spread",fxFDT,255,InstrumentMotionValue::raw},
    {"FM harmonic ratio",fxFHR,255,InstrumentMotionValue::raw},
    {"FM LFO rate",fxFLR,255,InstrumentMotionValue::raw},
    {"FM LFO depth",fxFLD,255,InstrumentMotionValue::raw},
  };
  if(g>=genericModFMTime&&g<=genericModFMLFODepth) {
    bool extended=t==InstrumentType::GenesisFM||t==InstrumentType::ArcadeFM||t==InstrumentType::DX7;
    return fm&&((g!=genericModFMDetune&&g<genericModFMLFORate)||extended)?&macros[g-genericModFMTime]:nullptr;
  }
  static const InstrumentModDestination sidEnvelope[] = {
    {"SID attack",fxSAT,15,InstrumentMotionValue::raw},
    {"SID decay",fxSDE,15,InstrumentMotionValue::raw},
    {"SID sustain",fxSSU,15,InstrumentMotionValue::raw},
    {"SID release",fxSRL,15,InstrumentMotionValue::raw},
    {"SID partner ratio",fxSPR,15,InstrumentMotionValue::raw},
  };
  if(g>=genericModSIDAttack)return t==InstrumentType::SID?&sidEnvelope[g-genericModSIDAttack]:nullptr;
  static const InstrumentModDestination operators[] = {
    {"Operator 1 level",fxFO1,255,InstrumentMotionValue::raw},
    {"Operator 2 level",fxFO2,255,InstrumentMotionValue::raw},
    {"Operator 3 level",fxFO3,255,InstrumentMotionValue::raw},
    {"Operator 4 level",fxFO4,255,InstrumentMotionValue::raw},
    {"Operator 5 level",fxFO5,255,InstrumentMotionValue::raw},
    {"Operator 6 level",fxFO6,255,InstrumentMotionValue::raw},
  };
  if(g>=genericModFMOperator1) {
    int count=t==InstrumentType::DX7?6:(t==InstrumentType::OPLL||t==InstrumentType::VRC7||t==InstrumentType::OPL2)?2:4;
    return fm&&g<genericModFMOperator1+count?&operators[g-genericModFMOperator1]:nullptr;
  }
  if(g<=genericModFMFeedback)return fm?&controls[g-genericModFMBrightness]:nullptr;
  if(g==genericModChipMode)return t==InstrumentType::SegaPSG?&segaMode:t==InstrumentType::GBPulse?&pulseMode:t==InstrumentType::GBNoise?&noiseMode:nullptr;
  bool valid=g==genericModChipNoiseRate?t==InstrumentType::SegaPSG:
    g<=genericModChipNoiseShift?t==InstrumentType::GBNoise:
    g<=genericModChipSweepDirection?t==InstrumentType::GBPulse:
    (t==InstrumentType::GBPulse||t==InstrumentType::GBNoise);
  return valid?&controls[g-genericModFMBrightness]:nullptr;
}

InstrumentFMTone* instrumentFMToneSettings(Instrument* i) {
  switch(i->type) {
    case InstrumentType::OPLL:case InstrumentType::VRC7:return &i->chip.opll.tone;
    case InstrumentType::OPL2:case InstrumentType::OPL3:return &i->chip.opl.tone;
    case InstrumentType::GenesisFM:case InstrumentType::ArcadeFM:return &i->chip.fourOp.tone;
    case InstrumentType::DX7:return &i->chip.dx7.tone;
    default:return nullptr;
  }
}
int instrumentNativeControlValue(const Instrument* i,int g) {
  // This access is read-only; the mutable overload also serves the UI editor.
  const auto* tone=instrumentFMToneSettings(const_cast<Instrument*>(i));
  if(tone) {
    if(g>=genericModFMTime&&g<=genericModFMLFODepth)return 128+tone->macro[g-genericModFMTime];
    if(g>=genericModFMOperator1&&g<=genericModFMOperator6)return 128+tone->operatorOffset[g-genericModFMOperator1];
    return g==genericModFMBrightness?fmBrightnessToByte(tone->brightness):g==genericModFMFeedback?tone->feedback:0;
  }
  if(i->type==InstrumentType::SID) {
    const auto* v=i->chip.sid.value;
    switch(g){
      case genericModSIDPulse:return (v[sidPulse]*255+2047)/4095;
      case genericModSIDCutoff:return (v[sidCutoff]*255+1023)/2047;
      case genericModSIDResonance:return v[sidResonance];
      case genericModSIDWave:return v[sidWave]-1;
      case genericModSIDFilterMode:return v[sidFilterMode];
      case genericModSIDMacroRate:return ((v[sidMacroRate]-1)*255+99)/199;
      case genericModSIDRing:return v[sidRing];
      case genericModSIDSync:return v[sidSync];
      case genericModSIDAttack:return v[sidAttack];
      case genericModSIDDecay:return v[sidDecay];
      case genericModSIDSustain:return v[sidSustain];
      case genericModSIDRelease:return v[sidRelease];
      case genericModSIDPartner:return v[sidPartnerRatio]-1;
      default:return 0;
    }
  }
  const auto& p=i->chip.simpleChip;
  switch(g) {
    case genericModChipMode:return p.mode;
    case genericModChipNoiseRate:return p.noiseRate;
    case genericModChipNoiseDivisor:return p.noiseDivisor;
    case genericModChipNoiseShift:return p.noiseShift;
    case genericModChipSweepPeriod:return p.sweepPeriod;
    case genericModChipSweepShift:return p.sweepShift;
    case genericModChipSweepDirection:return p.sweepNegate;
    case genericModChipEnvelopeInitial:return p.envelopeInitial;
    case genericModChipEnvelopePeriod:return p.envelopePeriod;
    case genericModChipEnvelopeDirection:return p.envelopeIncrease;
    default:return 0;
  }
}
