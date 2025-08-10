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

Double_t fitFunc(Double_t *x, Double_t *par) {
  Double_t fitVal = (par[0] * (TMath::Exp((-x[0]) / par[1])) *
                         (1. + ((1. / par[2]) * (TMath::Exp(-x[0] / par[3])))) +
                     par[4]);
  return fitVal;
}

void difference(int bins, double range1, double range2) {
  // Creating TFile
  TFile *file = new TFile("difference.root", "RECREATE");

  // Creating Canvas
  TCanvas *c1 = new TCanvas("c1", "c1", 200, 10, 700, 500);

  // Defining TProfile
  TString xLabels[8] = {"500",  "1000", "2000",  "3000",
                        "6000", "9000", "12000", "15000"};
  auto *hExpo = new TH1F("hExpo", "; Time [ns]; Events", bins, 0., 20000.);
  hExpo->Sumw2();
  hExpo->SetLineColor(kBlue);
  hExpo->SetLineWidth(1);
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
  std::ifstream infile500("2B_data_2.txt");
  while (std::getline(infile500, line)) {
    std::stringstream ss(line);
    while (ss >> a >> b >> c >> d >> e >> f) {
      if (d == "fff" && e == "fff" && f == "fff") {
        continue;
      } else {
        if (d != "fff" && e != "fff" && f == "fff") {
          hExpo->Fill(
              Double_t{(std::stoi(hexadecimal + e, nullptr, 16) + 27.2806) /
                       0.249969},
              1);
        }
      }
    }
  }

  auto *hExpo1 = new TH1F("hExpo1", "; Time [ns]; Events", bins, 0., 20000.);
  hExpo1->Sumw2();
  hExpo1->SetLineColor(kRed);
  hExpo1->SetLineWidth(1);
  hExpo1->SetMarkerColor(kRed);
  hExpo1->SetMarkerStyle(20);
  hExpo1->SetMarkerSize(1);

  std::ifstream infile5001("2B_data_3.txt");
  while (std::getline(infile5001, line)) {
    std::stringstream ss(line);
    while (ss >> a >> b >> c >> d >> e >> f) {
      if (c < 427919.661) {
        if (d == "fff" && e == "fff" && f == "fff") {
          continue;
        } else {
          if (d != "fff" && e != "fff" && f == "fff") {
            hExpo1->Fill(
                Double_t{(std::stoi(hexadecimal + e, nullptr, 16) + 27.2806) /
                         0.249969},
                1);
          }
          // if (f != "fff") {
          //   hExpo1->Fill(
          //       (std::stoi(hexadecimal + f, nullptr, 16) + 27.2806) /
          //       0.249969, 1);
          // }
        }
      }
      break;
    }
  }

  TH1F *hist_diff =
      (TH1F *)hExpo->Clone("hist_diff");  // Crea una copia di hist1
  hist_diff->Add(hExpo1, -1);
  hist_diff->SetLineColor(kBlue);
  hExpo1->SetLineWidth(1);
  hExpo1->SetMarkerColor(kBlue);
  hExpo1->SetMarkerStyle(20);
  hExpo1->SetMarkerSize(1);

  // Defining fit data
  TF1 *fFunc = new TF1("fitFunc", fitFunc, range1, range2, 5);
  fFunc->SetLineColor(kRed);
  fFunc->SetLineWidth(4);
  fFunc->SetLineStyle(2);
  fFunc->SetParNames("Amplitude", "tau_0", "R", "tau_c", "b");
  fFunc->SetParameter(0, 500.);                     // Amplitude
  fFunc->SetParameter(1, 2197);                     // \tau_0 [ns]
  fFunc->FixParameter(2, 1.1);                      // R
  fFunc->SetParameter(3, Double_t{(1e3) / (4.4)});  // \tau_c [ns]
  fFunc->SetParameter(4, 10.);                      // Background

  // fFunc->SetParLimits(0, 100, 400.);  // Amplitude
  // fFunc->SetParLimits(1, 2150., 2249.);  // \tau_0 [ns]
  //  fFunc->SetParLimits(2, 1.1, 1.2);      // R
  // // fFunc->SetParLimits(3, 190., 210.);  // \tau_{\mu^-} [ns]
  //  fFunc->SetParLimits(3, 220., 230.);  // \tau_c [ns]
  fFunc->SetParLimits(4, 0., 900.);  // Background

  //  Fit
  hist_diff->Fit(fFunc, "RM");

  std::cout << "Integral: " << hist_diff->Integral() << '\n';
  std::cout << "entries 0: " << hExpo->GetEntries() << '\n';
  std::cout << "entries 1: " << hExpo1->GetEntries() << '\n';
  std::cout << "P-value: " << fFunc->GetProb() << '\n';
  std::cout << "Reduced chi squared: "
            << fFunc->GetChisquare() / fFunc->GetNDF() << '\n';

  // Legend
  TLegend *leg1 = new TLegend(.70, .7, .9, .9, "Legend");
  leg1->SetFillColor(0);
  leg1->AddEntry(hExpo, "Decay events in iron", "LPE");
  leg1->AddEntry(fFunc, "Fit", "L");

  setFitStyle();

  // Draw hProfile
  // hExpo->DrawCopy();
  // hExpo1->DrawCopy("same");
  gStyle->SetTitleXOffset(1.2f);
  gStyle->SetTitleYOffset(0.8f);
  hist_diff->DrawCopy("HIST");
  hist_diff->DrawCopy("E,SAME");
  leg1->Draw("SAME");

  // Operation on TFile
  file->cd();
  c1->Write();
  file->Close();
}

int main() {
  for (int i = 90; i < 110; i++) {
    std::cout << "\n\nbin= " << i << "\n\n";
    for (int j = 150.; j < 300; j += 50) {
      std::cout << "\n\nrange1= " << j << "\n\n";
      difference(i, j, 10000.);
    }
  }

  return EXIT_SUCCESS;
}

// il meno peggio al momento è nBins = 200
// range di fit 250., 10000. -> stessi parametri di fit sopra -> nessuno fissato
// la roba strana sono le entries che sono veramente pochine