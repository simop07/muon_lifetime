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
Double_t fitFunc(Double_t *x, Double_t *par) {
  Double_t fitVal = (par[0] * (TMath::Exp((-x[0]) / par[1])) *  //+
                         (1. + ((1. / par[2]) * (TMath::Exp(-x[0] / par[3])))) +
                     par[4]);
  return fitVal;
}

void expoNoIron() {
  // Creating TFile
  TFile *file = new TFile("expoNoIron.root", "RECREATE");

  // Creating Canvas
  TCanvas *c1 = new TCanvas("c1", "c1", 200, 10, 700, 500);

  // Defining TProfile
  auto const nBins = 250;
  TString xLabels[8] = {"500",  "1000", "2000",  "3000",
                        "6000", "9000", "12000", "15000"};
  auto *hExpo =
      new TH1F("hExpo", "Expo histo; Time [ns]; Events", nBins, 0., 16000.);
  hExpo->SetLineColor(kBlue);
  hExpo->SetLineWidth(4);
  hExpo->SetMarkerColor(kBlue);
  hExpo->SetMarkerStyle(20);
  hExpo->SetMarkerSize(1);

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
  std::ifstream infile500("2B_data_3.txt");
  while (std::getline(infile500, line)) {
    std::stringstream ss(line);
    while (ss >> a >> b >> c >> d >> e >> f) {
      if (d == "fff" && e == "fff" && f == "fff") {
        continue;
      } else {
        if (d != "fff") {
          hExpo->Fill(
              (std::stoi(hexadecimal + d, nullptr, 16) + 27.2806) / 0.249969,
              1);
        }
        if (e != "fff") {
          hExpo->Fill(
              (std::stoi(hexadecimal + e, nullptr, 16) + 27.2806) / 0.249969,
              1);
        }
        if (f != "fff") {
          hExpo->Fill(
              (std::stoi(hexadecimal + f, nullptr, 16) + 27.2806) / 0.249969,
              1);
        }
      }
    }
  }

  // Defining fit data
  TF1 *fFunc = new TF1("fitFunc", fitFunc, 250., 10000., 5);
  fFunc->SetLineColor(kRed);
  fFunc->SetLineWidth(4);
  fFunc->SetLineStyle(2);
  fFunc->SetParNames("Amplitude", "tau_0", "R");
  fFunc->SetParameter(0, 500.);  // Amplitude
  fFunc->SetParameter(1, 2197);  // \tau_0 [ns]
  fFunc->SetParameter(2, 10);    // R -> è il bkg se uso 3 parametri di fit
  fFunc->SetParameter(3, Double_t{(1e3) / (4.4)});  // \tau_c [ns]
  fFunc->SetParameter(4, 10.);                      // Background

  // fFunc->SetParLimits(0, 100, 400.);  // Amplitude
  // fFunc->SetParLimits(1, 2.0, 2.3);  // \tau_0 [ns]
  // / fFunc->SetParLimits(2, 1.2, 1.31);  // R
  // // fFunc->SetParLimits(3, 200., 230.);  // \tau_c [ns]
  // fFunc->SetParLimits(4, 0., 5.);  // Background

  // Fit
  hExpo->Fit(fFunc, "R");

  std::cout << "Reduced chi squared: "
            << fFunc->GetChisquare() / fFunc->GetNDF() << '\n';

  // // Print data and errors
  // std::cout << "\n\n**PARAMETERS\n\nAng. coefficient: "
  //           << fProf1->GetParameter(1) << " #pm " << fProf1->GetParError(1)
  //           << "\n\n";
  // std::cout << "Convers. factor: " << 1. / (fProf1->GetParameter(1)) << " #pm
  // "
  //           << (fProf1->GetParError(1)) /
  //                  ((fProf1->GetParameter(1)) * (fProf1->GetParameter(1)))
  //           << "\n\n";
  // std::cout << "Intercept: " << fProf1->GetParameter(0) << " #pm "
  //           << fProf1->GetParError(0) << "\n\n";

  // Legend
  TLegend *leg1 = new TLegend(.70, .7, .9, .9, "Legend");
  leg1->SetFillColor(0);
  leg1->AddEntry(hExpo, "Decay events", "LPE");
  leg1->AddEntry(fFunc, "Fit", "L");

  // Draw hProfile
  hExpo->DrawCopy();
  leg1->Draw("same");

  // Operation on TFile
  file->cd();
  c1->Write();
  file->Close();
}

int main() {
  expoNoIron();

  return EXIT_SUCCESS;
}

// COMMENTS

// best fit if we fit with 5 parameters and we do not fix any -> best fitting
// with nBins 200/250 range of fitting 250-10000, amplitude set at 500, tau_0
// set at 2179, R set at 1.27, tau_c set at 200, b set at 10 reduced chi squared
// is at 2 /3 tau_0 is compatible with a relativ error of 80/2200 ish

// Best fit if we fit with 3 parameters: we don't fix any nBins 200/250
// range of fitting 320-8500, amplitude set at 500, tau_0 set at 2179, b set at
// 10 reduced chi squared is at 2 /3 tau_0 is compatible with a relative error
// of ish -> if i sum the error it becomes 2.17