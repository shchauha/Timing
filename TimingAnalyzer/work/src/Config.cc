#include "../interface/Config.hh"

namespace Config {

  // default parameters for Config
  TString outdir     = "output";
  Bool_t  doPURW     = false;
  Bool_t  doAnalysis = false;
  Bool_t  doStacks   = false;
  Bool_t  doDemo     = false;
  Bool_t  useDEG     = false; // should be true
  Bool_t  useDYll    = false; // should also be true
  Bool_t  useQCD     = false;
  Bool_t  useGJets   = false;

  TStrBoolMap SampleMap;      // set in main.cc
  ColorMap    colorMap;       // set in main.cc
  TStrMap     SampleTitleMap; // set in main.cc
}
