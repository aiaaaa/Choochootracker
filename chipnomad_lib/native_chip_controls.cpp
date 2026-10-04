#include "project.h"

const InstrumentModDestination* instrumentNativeModDestination(InstrumentType t, int g) {
  static const InstrumentModDestination controls[] = {
    {"Brightness",fxFBR,126,InstrumentMotionValue::raw},
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
  bool fm=t==InstrumentType::OPLL||t==InstrumentType::VRC7||t==InstrumentType::OPL2||t==InstrumentType::OPL3||t==InstrumentType::GenesisFM||t==InstrumentType::ArcadeFM||t==InstrumentType::DX7;
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
  if(tone)return g==genericModFMBrightness?int(tone->brightness)+63:g==genericModFMFeedback?tone->feedback:0;
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
