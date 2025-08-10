#include <fstream>
#include <sstream>
#include <vector>

#include "TCanvas.h"
#include "TColor.h"
#include "TF1.h"
#include "TFile.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TProfile.h"
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

void analysis() {
  // Creating TFile
  TFile *file = new TFile("analysis.root", "RECREATE");

  // Creating Canvas
  TCanvas *c1 = new TCanvas("c1", "c1", 200, 10, 700, 500);

  // Defining TProfile
  auto const nBins = 100;  // not less than 32!!
  auto const angCoeff{1. / 4.};
  TString xLabels[8] = {"500",  "1000", "2000",  "3000",
                        "6000", "9000", "12000", "15000"};
  auto *hProf = new TProfile("hProf", "; Oscilloscope [ns]; FPGA counter",
                             nBins, 0., 16000., 0., 4000., "S");
  hProf->SetLineColor(kBlue);
  hProf->SetLineWidth(4);
  hProf->SetMarkerColor(kBlue);
  hProf->SetMarkerStyle(20);
  hProf->SetMarkerSize(2);

  // Fill histogram
  int a{};
  int b{};
  int c{};
  std::string line;
  // 500 ns
  std::ifstream infile500("cal_500_dec.txt");
  while (std::getline(infile500, line)) {
    std::stringstream ss(line);
    while (ss >> a >> b >> c) {
      // std::cout << a << b << c << '\n';
      hProf->Fill(500., a, 1);
      hProf->Fill(500., b, 1);
      hProf->Fill(500., c, 1);
    }
  }
  // 1000 ns
  std::ifstream infile1000("cal_1000_dec.txt");
  while (std::getline(infile1000, line)) {
    std::stringstream ss(line);
    while (ss >> a >> b >> c) {
      // std::cout << a << b << c << '\n';
      hProf->Fill(1000., a, 1);
      hProf->Fill(1000., b, 1);
      hProf->Fill(1000., c, 1);
    }
  }
  // 2000 ns
  std::ifstream infile2000("cal_2000_dec.txt");
  while (std::getline(infile2000, line)) {
    std::stringstream ss(line);
    while (ss >> a >> b >> c) {
      // std::cout << a << b << c << '\n';
      hProf->Fill(2000., a, 1);
      hProf->Fill(2000., b, 1);
      hProf->Fill(2000., c, 1);
    }
  }
  // 3000 ns
  std::ifstream infile3000("cal_3000_dec.txt");
  while (std::getline(infile3000, line)) {
    std::stringstream ss(line);
    while (ss >> a >> b >> c) {
      // std::cout << a << b << c << '\n';
      hProf->Fill(3000., a, 1);
      hProf->Fill(3000., b, 1);
      hProf->Fill(3000., c, 1);
    }
  }
  // 6000 ns
  std::ifstream infile6000("cal_6000_dec.txt");
  while (std::getline(infile6000, line)) {
    std::stringstream ss(line);
    while (ss >> a >> b >> c) {
      // std::cout << a << b << c << '\n';
      hProf->Fill(6000., a, 1);
      hProf->Fill(6000., b, 1);
      hProf->Fill(6000., c, 1);
    }
  }
  // 9000 ns
  std::ifstream infile9000("cal_9000_dec.txt");
  while (std::getline(infile9000, line)) {
    std::stringstream ss(line);
    while (ss >> a >> b >> c) {
      // std::cout << a << b << c << '\n';
      hProf->Fill(9000., a, 1);
      hProf->Fill(9000., b, 1);
      hProf->Fill(9000., c, 1);
    }
  }
  // 12000 ns
  std::ifstream infile12000("cal_12000_dec.txt");
  while (std::getline(infile12000, line)) {
    std::stringstream ss(line);
    while (ss >> a >> b >> c) {
      // std::cout << a << b << c << '\n';
      hProf->Fill(12000., a, 1);
      hProf->Fill(12000., b, 1);
      hProf->Fill(12000., c, 1);
    }
  }
  // 15000 ns
  std::ifstream infile15000("cal_15000_dec.txt");
  while (std::getline(infile15000, line)) {
    std::stringstream ss(line);
    while (ss >> a >> b >> c) {
      // std::cout << a << b << c << '\n';
      hProf->Fill(15000., a, 1);
      hProf->Fill(15000., b, 1);
      hProf->Fill(15000., c, 1);
    }
  }

  // Defining fit functions
  TF1 *fProf1 = new TF1("fProf1", "pol1", 0., 15000);
  fProf1->SetLineColor(kRed);
  fProf1->SetLineWidth(4);
  fProf1->SetLineStyle(2);
  fProf1->SetParameter(0, 0.);
  fProf1->SetParameter(1, angCoeff);

  // Fit
  hProf->Fit(fProf1, "B");

  // Print data and errors
  std::cout << "\n\n*PARAMETERS*\n\nAng. coefficient: "
            << fProf1->GetParameter(1) << " #pm " << fProf1->GetParError(1)
            << "\n\n";
  std::cout << "Convers. factor: " << 1. / (fProf1->GetParameter(1)) << " #pm "
            << (fProf1->GetParError(1)) /
                   ((fProf1->GetParameter(1)) * (fProf1->GetParameter(1)))
            << "\n\n";
  std::cout << "Intercept: " << fProf1->GetParameter(0) << " #pm "
            << fProf1->GetParError(0) << "\n\n";
  std::cout << "Delay (t_0): "
            << -(fProf1->GetParameter(0) / fProf1->GetParameter(1)) << " #pm "
            << (fProf1->GetParError(0) / fProf1->GetParameter(0) +
                fProf1->GetParError(1) / fProf1->GetParameter(1)) *
                   (fProf1->GetParameter(0) / fProf1->GetParameter(1))
            << "\n\n";
  std::cout << "Reduced chi squared "
            << fProf1->GetChisquare() / fProf1->GetNDF() << "\n\n";
  std::cout << "P-value " << fProf1->GetProb() << "\n\n";

  // Legend
  TLegend *leg1 = new TLegend(.70, .7, .9, .9, "Legend");
  leg1->SetFillColor(0);
  leg1->AddEntry(hProf, "Calibration points", "PE");
  leg1->AddEntry(fProf1, "Fit", "L");

  // Draw hProfile
  gStyle->SetTitleXOffset(1.2f);
  gStyle->SetTitleYOffset(0.8f);
  hProf->DrawCopy("E");
  leg1->Draw("SAME");

  // Operation on TFile
  file->cd();
  c1->Write();
  file->Close();
}

int main() {
  analysis();

  return EXIT_SUCCESS;
}