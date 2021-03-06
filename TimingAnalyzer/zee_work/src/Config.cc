#include "../interface/Config.hh"

namespace Config {

  // default parameters for Config
  TString outdir     = "output";
  Bool_t  dumpRanges = false;
  Bool_t  doPURW     = false;
  Bool_t  doAnalysis = false;
  Bool_t  doStacks   = false;
  Bool_t  doDemo     = false;
  Bool_t  useDEG     = false; 
  Bool_t  useSEL     = false; 
  Bool_t  useDYll    = false; 
  Bool_t  useQCD     = false;
  Bool_t  useGJets   = false;
  Bool_t  doStandard = false;
  Bool_t  doTimeRes  = false;
  Bool_t  doSingleE  = false;
  Bool_t  doEffE     = false;
  Bool_t  doNvtx     = false;
  Bool_t  doEta      = false;
  Bool_t  doVtxZ     = false;
  Bool_t  doRuns     = false;
  Bool_t  doTrigEff  = false;
  Bool_t  applyTOF   = false;
  Bool_t  wgtedTime  = false;
  Bool_t  useSigma_n = false;
  Bool_t  saveFits   = false;
  Bool_t  dumpStatus = false;
  TString year       = "2016";
  TString formname   = "gaus2fm"; // gaus1, gaus1core, gaus2, gaus2fm, gaus3fm
  TString outtype    = "pdf";

  TStrBoolMap SampleMap;       // set in main.cc
  ColorMap    colorMap;        // set in main.cc
  TStrMap     SampleTitleMap;  // set in main.cc
  TStrFltMap  SampleXsecMap;   // set in main.cc
  TStrFltMap  SampleWgtsumMap; // set in main.cc
  TStrMap     XTitleMap;       // set in main.cc
  TStrDblVMap XBinsMap;        // set in main.cc
}
