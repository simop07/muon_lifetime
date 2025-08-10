#include <fstream>
#include <sstream>
#include <vector>

#include "TCanvas.h"
#include "TColor.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1F.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TMath.h"
#include "TROOT.h"
#include "TRandom.h"
#include "TString.h"
#include "TStyle.h"
// #include "TMultiGraph.h"
// #include "TPad.h"
// #include "TPaveText.h"

void setFitStyle() {
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(10);
  gStyle->SetOptFit(0);
  gStyle->SetPalette(57);
  gStyle->SetOptTitle(1);
  gStyle->SetTitleFontSize(0.091f);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetTitleStyle(0);
  gStyle->SetStatY(0.9);
  gStyle->SetStatX(0.9);
  gStyle->SetStatW(0.2);
  gStyle->SetStatH(0.2);
  gStyle->SetTitleX(0.5);
  gStyle->SetTitleY(1.);
  gStyle->SetTitleAlign(23);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetTitleXOffset(1.2f);
  gStyle->SetTitleYOffset(0.8f);
  gStyle->SetPalette(kRainBow);
  // gStyle->SetPadTopMargin(-9.);
  // gStyle->SetPadRightMargin(-9.);
  // gStyle->SetPadBottomMargin(-9.);
  // gStyle->SetPadLeftMargin(-9.);
  // gStyle->SetTitleW(0.5f);
}

// User defined function
Double_t fitFuncCapture(Double_t *x, Double_t *par) {
  Double_t fitVal = (par[0] * (TMath::Exp(-x[0] / par[1])) *
                         (1. + ((1. / par[2]) * (TMath::Exp(-x[0] / par[3])))) +
                     par[4]);
  return fitVal;
}

Double_t fitFuncNoCapture(Double_t *x, Double_t *par) {
  Double_t fitVal =
      (par[0] * (TMath::Exp(-x[0] / par[1])) * (1. + (1. / par[2])) + par[4]);
  return fitVal;
}

Double_t fitFuncCaptureQ(Double_t *x, Double_t *par) {
  Double_t fitVal =
      (par[0] * (TMath::Exp(-x[0] / par[1])) *
           (1. +
            ((1. / par[2]) * (TMath::Exp(-x[0] * ((par[1] - (0.975 * par[3])) /
                                                  (par[3] * par[1])))))) +
       par[4]);
  return fitVal;
}

void expo(int bin, double range1, double range2) {
  // Creating TFile
  TFile *file = new TFile("expo.root", "RECREATE");

  // Creating Canvas
  TCanvas *c1 = new TCanvas("c1", "c1", 200, 10, 700, 500);
  TCanvas *c2 = new TCanvas("c2", "c2", 200, 10, 700, 500);
  TCanvas *c3 = new TCanvas("c3", "c3", 200, 10, 700, 500);
  TCanvas *c4 = new TCanvas("c4", "c4", 200, 10, 700, 500);
  TCanvas *c5 = new TCanvas("c5", "c5", 200, 10, 700, 500);
  TCanvas *c6 = new TCanvas("c6", "c6", 200, 10, 700, 500);

  // Defining Histo
  auto *hExpo = new TH1F("hExpo", "; Time [ns]; Events", bin, 0., 20000.);
  hExpo->SetLineColor(kBlue);
  hExpo->SetLineWidth(1);
  hExpo->SetMarkerColor(kBlue);
  hExpo->SetMarkerStyle(20);
  hExpo->SetMarkerSize(1);

  // Defining Histo
  auto *hExpoP3 = new TH1F("hExpoP3", "; Time [ns]; Events", bin, 0., 20000.);
  hExpoP3->SetLineColor(kBlue);
  hExpoP3->SetLineWidth(1);
  hExpoP3->SetMarkerColor(kBlue);
  hExpoP3->SetMarkerStyle(20);
  hExpoP3->SetMarkerSize(1);

  // Defining Histo
  auto *hExpoP2 = new TH1F("hExpoP2", "; Time [ns]; Events", bin, 0., 20000.);
  hExpoP2->SetLineColor(kBlue);
  hExpoP2->SetLineWidth(1);
  hExpoP2->SetMarkerColor(kBlue);
  hExpoP2->SetMarkerStyle(20);
  hExpoP2->SetMarkerSize(1);

  // Defining Histo
  auto *hExpoP1_P2 =
      new TH1F("hExpoP1_P2", "; Time [ns]; Events", bin, 0., 20000.);
  hExpoP1_P2->SetLineColor(kBlue);
  hExpoP1_P2->SetLineWidth(1);
  hExpoP1_P2->SetMarkerColor(kBlue);
  hExpoP1_P2->SetMarkerStyle(20);
  hExpoP1_P2->SetMarkerSize(1);

  // Fill histogram
  std::string a{};
  long long int b{};
  long double c{};
  std::string d{};
  std::string e{};
  std::string f{};
  std::string line;
  std::string hexadecimal{"0x"};
  // 500 ns
  std::ifstream infile500("2B_data_2.txt");
  while (std::getline(infile500, line)) {
    std::stringstream ss(line);
    while (ss >> a >> b >> c >> d >> e >> f) {
      if (d == "fff" && e == "fff" && f == "fff") {
        continue;
      } else {
        if (d != "fff" && e != "fff" && f == "fff") {  // P1 e P2, no P3
          hExpoP1_P2->Fill(
              Double_t{(std::stoi(hexadecimal + e, nullptr, 16) + 27.2806) /
                       0.249969},
              1);
          hExpo->Fill(
              Double_t{(std::stoi(hexadecimal + e, nullptr, 16) + 27.2806) /
                       0.249969},
              1);
        }
        if (d == "fff" && e != "fff" && f == "fff") {  // no P1, si P2, no P3
          hExpoP2->Fill(
              Double_t{(std::stoi(hexadecimal + e, nullptr, 16) + 27.2806) /
                       0.249969},
              1);
          hExpo->Fill(
              Double_t{(std::stoi(hexadecimal + e, nullptr, 16) + 27.2806) /
                       0.249969},
              1);
        }
        if (d == "fff" && e == "fff" && f != "fff") {
          hExpoP3->Fill(
              Double_t{(std::stoi(hexadecimal + f, nullptr, 16) + 27.2806) /
                       0.249969},
              1);
          hExpo->Fill(
              Double_t{(std::stoi(hexadecimal + f, nullptr, 16) + 27.2806) /
                       0.249969},
              1);
        }
      }
    }
  }

  // Defining fit data
  TF1 *fFunc = new TF1("fitFunc", fitFuncCapture, range1, range2, 5);
  fFunc->SetLineColor(kRed);
  fFunc->SetLineWidth(4);
  fFunc->SetLineStyle(2);
  fFunc->SetParNames("Amplitude", "#tau_0", "R", "#tau_{c}", "b");
  fFunc->SetParameter(0, 2000.);  // Amplitude o 500
  fFunc->SetParameter(1, 2197.);  // \tau_0 [ns]
  fFunc->FixParameter(2, 1.1);    // R
  // // fFunc->FixParameter(3, 200.);  // \tau_{\mu^-} [ns]
  fFunc->SetParameter(3, Double_t{(1e3) / (4.4)});  // \tau_c [ns]
  fFunc->SetParameter(4, 10.);                      // Background

  // // fFunc->SetParLimits(0, 100, 400.);     // Amplitude
  // // fFunc->SetParLimits(1, 2150., 2249.);  // \tau_0 [ns]
  // // fFunc->SetParLimits(2, 1.1, 2.);    // R
  // // fFunc->SetParLimits(3, 190., 210.);    // \tau_{\mu^-} [ns]
  // // fFunc->SetParLimits(3, 200., 1000.);  // \tau_c [ns]
  // fFunc->SetParLimits(4, 0., 100.);  // Background

  // Fit
  hExpo->Fit(fFunc, "RM");
  // Print data and errors
  std::cout << "\n\n****FIT TOTAL****\n\nP value: " << fFunc->GetProb()
            << "\n\n";
  std::cout << "Reduced chi squared: "
            << fFunc->GetChisquare() / fFunc->GetNDF() << "\n\n";

  // // Fit
  // fFunc->SetParameter(0, 2000.);  // Amplitude o 500
  // fFunc->SetParameter(1, 2197.);  // \tau_0 [ns]
  // fFunc->SetParameter(2, 1.27);   // R
  // // fFunc->FixParameter(3, 200.);  // \tau_{\mu^-} [ns]
  // fFunc->SetParameter(3, Double_t{(1e3) / (4.4)});  // \tau_c [ns]
  // fFunc->SetParameter(
  //     4, 10.);  // Background
  //               // fFunc->SetParLimits(0, 100, 400.);     // Amplitude
  // // fFunc->SetParLimits(1, 2150., 2249.);  // \tau_0 [ns]
  // // fFunc->SetParLimits(2, 1.0001, 2.);    // R
  // // fFunc->SetParLimits(3, 190., 210.);    // \tau_{\mu^-} [ns]
  // // fFunc->SetParLimits(3, 200., 1000.);  // \tau_c [ns]
  // fFunc->SetParLimits(4, 0., 100.);  // Background
  // hExpoP3->Fit(fFunc, "RM");
  // // Print data and errors
  // std::cout << "\n\n****P3****\n\nP value: " << fFunc->GetProb() << "\n\n";
  // std::cout << "Reduced chi squared: "
  //           << fFunc->GetChisquare() / fFunc->GetNDF() << "\n\n";

  // // Fit
  // fFunc->SetParameter(0, 2000.);  // Amplitude o 500
  // fFunc->SetParameter(1, 2197.);  // \tau_0 [ns]
  // fFunc->SetParameter(2, 1.27);   // R
  // // fFunc->FixParameter(3, 200.);  // \tau_{\mu^-} [ns]
  // fFunc->SetParameter(3, Double_t{(1e3) / (4.4)});  // \tau_c [ns]
  // fFunc->SetParameter(
  //     4, 10.);  // Background
  //               // fFunc->SetParLimits(0, 100, 400.);     // Amplitude
  // // fFunc->SetParLimits(1, 2150., 2249.);  // \tau_0 [ns]
  // // fFunc->SetParLimits(2, 1.0001, 2.);    // R
  // // fFunc->SetParLimits(3, 190., 210.);    // \tau_{\mu^-} [ns]
  // // fFunc->SetParLimits(3, 200., 1000.);  // \tau_c [ns]
  // fFunc->SetParLimits(4, 0., 100.);  // Background
  // hExpoP2->Fit(fFunc, "RM");
  // // Print data and errors
  // std::cout << "\n\n****P2****\n\nP value: " << fFunc->GetProb() << "\n\n";
  // std::cout << "Reduced chi squared: "
  //           << fFunc->GetChisquare() / fFunc->GetNDF() << "\n\n";

  // Fit
  // fFunc->SetParameter(0, 2000.);  // Amplitude o 500
  // fFunc->SetParameter(1, 2197.);  // \tau_0 [ns]
  // fFunc->FixParameter(2, 1.1);    // R
  // // fFunc->FixParameter(3, 200.);  // \tau_{\mu^-} [ns]
  // fFunc->SetParameter(3, Double_t{(1e3) / (4.4)});  // \tau_c [ns]
  // fFunc->SetParameter(4, 10.);                      // Background
  // // fFunc->SetParLimits(0, 100, 400.);     // Amplitude
  // // fFunc->SetParLimits(1, 2150., 2249.);  // \tau_0 [ns]
  // // fFunc->SetParLimits(2, 1.0001, 2.);  // R
  // // fFunc->SetParLimits(3, 190., 210.);    // \tau_{\mu^-} [ns]
  // // fFunc->SetParLimits(3, 200., 1000.);  // \tau_c [ns]
  // fFunc->SetParLimits(4, 0., 100.);  // Background
  // hExpoP1_P2->Fit(fFunc, "RM");
  // Print data and errors
  // std::cout << "\n\n****P1_P2****\n\nP value: " << fFunc->GetProb() <<
  // "\n\n"; std::cout << "Reduced chi squared: "
  //           << fFunc->GetChisquare() / fFunc->GetNDF() << "\n\n";

  // Entries
  std::cout << "Entries TOTAL: " << hExpo->GetEntries() << '\n';
  std::cout << "Entries P3 no P2 no P1: " << hExpoP3->GetEntries() << '\n';
  std::cout << "Entries P2 without P1, no P3: " << hExpoP2->GetEntries()
            << '\n';
  std::cout << "Entries P2 and P1, no P3: " << hExpoP1_P2->GetEntries() << '\n';

  // Legend
  TLegend *leg1 = new TLegend(.70, .7, .9, .9, "Legend");
  leg1->SetFillColor(0);
  leg1->AddEntry(hExpo, "Decay events", "LPE");
  leg1->AddEntry(fFunc, "Fit", "L");

  // Legend
  TLegend *leg2 = new TLegend(.70, .7, .9, .9, "Legend");
  leg2->SetFillColor(0);
  leg2->AddEntry(hExpo, "Decay events", "LPE");

  setFitStyle();

  // Draw hProfile
  c1->cd();
  gStyle->SetTitleXOffset(1.2f);
  gStyle->SetTitleYOffset(0.8f);
  hExpo->DrawCopy("HIST");
  hExpo->DrawCopy("E,SAME ");
  leg1->Draw("SAME");

  // Draw hProfile logy
  c2->cd();
  c2->SetLogy();
  gStyle->SetTitleXOffset(1.2f);
  gStyle->SetTitleYOffset(0.8f);
  hExpo->DrawCopy("HIST");
  hExpo->DrawCopy("E,SAME");
  leg1->Draw("SAME");

  // Draw hProfile
  c3->cd();
  gStyle->SetTitleXOffset(1.2f);
  gStyle->SetTitleYOffset(0.8f);
  hExpoP3->DrawCopy("HIST");
  hExpoP3->DrawCopy("E,SAME");
  leg2->Draw("SAME");

  // Draw hProfile
  c4->cd();
  gStyle->SetTitleXOffset(1.2f);
  gStyle->SetTitleYOffset(0.8f);
  hExpoP2->DrawCopy("HIST");
  hExpoP2->DrawCopy("E,SAME");
  leg2->Draw("SAME");

  // Draw hProfile
  c5->cd();
  gStyle->SetTitleXOffset(1.2f);
  gStyle->SetTitleYOffset(0.8f);
  hExpoP1_P2->DrawCopy("HIST");
  hExpoP1_P2->DrawCopy("E,SAME");
  leg2->Draw("SAME");

  // // Draw
  // c6->cd();
  // c6->SetLogy();
  // gStyle->SetTitleXOffset(1.2f);
  // gStyle->SetTitleYOffset(0.8f);
  // hExpoP1_P2->DrawCopy("HIST");
  // hExpoP1_P2->DrawCopy("E,SAME ");
  // leg2->Draw("SAME");

  // Operation on TFile
  file->cd();
  c1->Write();
  c2->Write();
  c3->Write();
  c4->Write();
  c5->Write();
  c6->Write();
  file->Close();
}

int main() {
  for (int i = 90; i < 110; i++) {
    std::cout << "\n\nbin= " << i << "\n\n";
    for (int j = 150.; j < 300; j += 50) {
      std::cout << "\n\nrange1= " << j << "\n\n";
      expo(i, j, 10000.);
    }
  }

  return EXIT_SUCCESS;
}

//  FCN=49.8082 FROM HESSE     STATUS=OK             23 CALLS         435 TOTAL
//                      EDM=1.23008e-07    STRATEGY= 1      ERROR MATRIX
//                      ACCURATE
//   EXT PARAMETER                                   STEP         FIRST
//   NO.   NAME      VALUE            ERROR          SIZE      DERIVATIVE
//    1  Amplitude    3.26228e+02   1.17161e+01   4.08682e-03  -3.88954e-06
//    2  #tau_0       2.16450e+03   5.06589e+01   1.99653e-02   5.09588e-08
//    3  R            1.43063e+00   6.35537e-01   3.38639e-04  -6.16402e-04
//    4  #tau_{c}     2.27273e+02     fixed
//    5  b            7.05773e-08   4.20059e+00   2.80398e-04** at limit **

// ****P1_P2****

// P value: 0.220739

// Reduced chi squared: 1.15833

// using 95 bins