#include "project.h"
#include "sid_patch.h"

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
  if(g>=genericModSIDPulse)return t==InstrumentType::SID?&sid[g-genericModSIDPulse]:nullptr;
  bool fm=t==InstrumentType::OPLL||t==InstrumentType::VRC7||t==InstrumentType::OPL2||t==InstrumentType::OPL3||t==InstrumentType::GenesisFM||t==InstrumentType::ArcadeFM||t==InstrumentType::DX7;
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
