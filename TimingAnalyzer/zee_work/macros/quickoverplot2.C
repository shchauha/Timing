static const Float_t fitrange = 3.0;

inline Float_t rad2 (Float_t x, Float_t y){return x*x + y*y;}

void doFit(TH1F *& hist, TF1 *& fit, Float_t & mean, Float_t & emean, Float_t & sigma, Float_t & esigma) 
{
  TF1 * tempfit = new TF1("temp","gaus(0)",-fitrange,fitrange);
  tempfit->SetParLimits(2,0,10);
  hist->Fit("temp","RQ0B");
  const Float_t tempp0 = tempfit->GetParameter(0); // constant
  const Float_t tempp1 = tempfit->GetParameter(1); // mean
  const Float_t tempp2 = tempfit->GetParameter(2); // sigma
  delete tempfit;

  TString formname = "gaus2fm";
  TFormula form(formname.Data(),"[0]*exp(-0.5*((x-[1])/[2])**2)+[3]*exp(-0.5*((x-[1])/[4])**2)");
  fit = new TF1(Form("%s_fit",hist->GetName()),formname.Data(),-fitrange,fitrange);
  fit->SetParameters(tempp0,tempp1,tempp2,tempp0/10,tempp2*4);
  fit->SetParLimits(2,0,10);
  fit->SetParLimits(4,0,10);
  
  Int_t status = hist->Fit(fit->GetName(),"RBQ0");
  
  const Float_t const1 = fit->GetParameter(0); 
  const Float_t const2 = fit->GetParameter(3);
  const Float_t denom =  const1 + const2;
  
  //  mean   = (const1*fit->GetParameter(1) + const2*fit->GetParameter(4))/denom;
  //  sigma  = (const1*fit->GetParameter(2) + const2*fit->GetParameter(5))/denom;
  mean   = fit->GetParameter(1);
  sigma  = (const1*fit->GetParameter(2) + const2*fit->GetParameter(4))/denom;
  
  //  emean  = rad2(const1*fit->GetParError(1),const2*fit->GetParError(4));
  //  esigma = rad2(const1*fit->GetParError(2),const2*fit->GetParError(5));
  emean  = fit->GetParError(1);
  esigma = rad2(const1*fit->GetParError(2),const2*fit->GetParError(4));
  
  //  emean  = std::sqrt(emean) /denom;
  esigma = std::sqrt(esigma)/denom;
}

void DrawSubComp(TF1 *& fit, TCanvas *& canv, TF1 *& sub1, TF1 *& sub2) 
{
  sub1 = new TF1("sub1","gaus(0)",-fitrange,fitrange);
  sub1->SetParameters(fit->GetParameter(0),fit->GetParameter(1),fit->GetParameter(2));
  
  sub2 = new TF1("sub2","gaus(0)",-fitrange,fitrange);
  sub2->SetParameters(fit->GetParameter(3),fit->GetParameter(1),fit->GetParameter(4));
  
  canv->cd();

  sub1->SetLineColor(kRed) ;  // kgreen-3
  sub1->SetLineWidth(2);
  sub1->SetLineStyle(7);
  sub1->Draw("same");

  sub2->SetLineColor(kBlue); // kViolet-3
  sub2->SetLineWidth(2);
  sub2->SetLineStyle(7);
  sub2->Draw("same");
}

void quickoverplot2() {
  gStyle->SetOptStat(0);
  //  gStyle->SetOptFit(0);
  TVirtualFitter::SetDefaultFitter("Minuit2");

  TString label1 = "gaus1core";
  TString label2 = "gaus2fm";

  Bool_t  isLogY = false;
  
  TFile * file = TFile::Open("nosigman/DATA/doubleeg/plots.root");

  TCanvas * canv = new TCanvas();
  canv->cd();
  //  canv->SetLogy(isLogY);

  TH1F * h1 = (TH1F*)file->Get("td_el2seedE_EEEE_sigma_gaus1core");
  //  h1->Scale(1.0/h1->Integral());
  h1->SetLineColor(kRed);
  h1->SetMarkerColor(kRed);
  h1->SetMarkerStyle(1);
//   h1->SetTitle("Resolution vs ");
//   h1->GetXaxis()->SetTitle("Time [ns]");
//   h1->GetYaxis()->SetTitle("Events");

//   Float_t m1, em1, s1, es1;
//   TF1 * f1;
//   doFit(h1,f1,m1,em1,s1,es1);
//   f1->SetLineColor(kRed);
//   f1->SetLineWidth(2);

  TH1F * h2 = (TH1F*)file->Get("td_el2seedE_EEEE_sigma_gaus2fm");
  //  h2->Scale(1.0/h2->Integral());
  h2->SetMaximum(0.8);
  h2->SetMinimum(0.5);
  h2->SetLineColor(kBlue);
  h2->SetMarkerColor(kBlue);
  h2->SetMarkerStyle(1);

//   Float_t m2, em2, s2, es2;
//   TF1 * f2;
//   doFit(h2,f2,m2,em2,s2,es2);
//   f2->SetLineColor(kBlue);
//  f2->SetLineWidth(2);

  //  f1->Draw("same");
  h2->Draw("ep");
  h1->Draw("ep SAME");
  //  f2->Draw("same");

  TLegend * leg = new TLegend(0.15,0.7,0.35,0.85);
  leg->AddEntry(h1,label1.Data(),"epl");
  leg->AddEntry(h2,label2.Data(),"epl");
  leg->Draw("same");

//   TPaveText * text = new TPaveText(0.65,0.65,0.85,0.85,"NDC");
//   text->SetFillColorAlpha(kWhite,0.f);
//   text->AddText(Form("#mu_{s} = %f #pm %f",m1,em1));
//   text->AddText(Form("#sigma_{s} = %f #pm %f",s1,es1));
//   text->AddLine(0.0,0.5,1.0,0.5);
//   text->AddText(Form("#mu_{w} = %f #pm %f",m2,em2));
//   text->AddText(Form("#sigma_{w} = %f #pm %f",s2,es2));
//   text->SetTextSize(0.03);
//   text->Draw("same");
  
  canv->SaveAs(Form("%s_vs_%s_%s_EEEE_el2seedE.pdf", label1.Data(), label2.Data(), (isLogY?"log":"lin") ));
}
