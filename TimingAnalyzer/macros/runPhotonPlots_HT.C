#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"

void setupcpp11() // customize ACLiC's behavior ...
{
  TString o;
  // customize MakeSharedLib
  o = TString(gSystem->GetMakeSharedLib());
  o = o.ReplaceAll(" -c ", " -std=c++0x -c ");
  gSystem->SetMakeSharedLib(o.Data());
  // customize MakeExe
  o = TString(gSystem->GetMakeExe());
  o = o.ReplaceAll(" -c ", " -std=c++0x -c ");
  gSystem->SetMakeExe(o.Data());
} 

void runPhotonPlots_HT(TString cuts, TString sample, TString bin) 
{
  setupcpp11(); 

  gROOT->LoadMacro("PlotPhotons.cc++g");

  // config is:
  // filename, isMC, applyevnocut, applyevcut, outdir, 
  // applyjetptcut, jetptcut, applyphptcut, phptcut,
  // applyphvidcut, phvid, applyrhecut, 
  // applyecalacceptcut
  
  PlotPhotons photonPlots(Form("input/%s/%s-HT%s.root",sample.Data(),sample.Data(),bin.Data()),false,false,(cuts.EqualTo("cuts",TString::kExact)?true:false),Form("output/%s/%s/%s",sample.Data(),cuts.Data(),bin.Data()),true,35.f,true,100.f,true,"medium",true,1.f,true);

  // which plots to do
  // first bool = generic plots
  // second bool = efficiency
  // third bool = analysis plots
  photonPlots.DoPlots(true,true,true);
}