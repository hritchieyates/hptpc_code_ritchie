#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TSystem.h"
#include "TH2.h"
#include "TH1.h"
#include "TGraph.h"




// directory with the raw data files
#define DATA_DIR "$HPTPC_RESULTS/"

// directory for the output root files
#define RESULT_DIR "$HPTPC_RESULTS/"

// directory for the output root files
#define CUT_DIR "$HPTPC_RESULTS_CUT/"

/* the standard single page plots */
void plotAmplitudeSpectraMany(std::string inputFileName = "test.root", std::string inputFile2Name = "anotherslightlydifferenttest.root", bool cuts = true, int rebin = 16, double min = -50, double max = 3000, std::string outFileExtension = "png", bool changeRange = true){
  // open file and catch, in case it doesn't exist
  std::string inDir;
  std::string outDir;

  if(!cuts){
    inDir = DATA_DIR;
    outDir = RESULT_DIR;  
  }
  else{
    inDir = CUT_DIR;
    outDir = CUT_DIR;
  }

  TFile* inputData = new TFile((inDir+"/"+inputFileName).c_str());
  if(!inputData->IsOpen()){
    std::cout << inputFileName << " could not be opened" << std::endl;
    return;
  } 
  TFile* inputData2 = new TFile((inDir+"/"+inputFile2Name).c_str());
  if(!inputData2->IsOpen()){
    std::cout << inputFile2Name << "could not be opened" << std::endl;
    return;
  }
  
  // since we know the file's content, we get the required histograms by name
  std::cout << "got file" << std::endl;
  inputData->cd();


  TCanvas* ampSpectraCanvas = new TCanvas("ampSpectraCanvas", "ampSpectraCanvas");
  ampSpectraCanvas->cd();
  ampSpectraCanvas->SetLogy();

  TH1D* HPeakMinusBaselineAnode31 = (TH1D*)inputData->Get("HPeakMinusBaselineAnode3");
  
  ///int nbins = HPeakMinusBaselineAnode31->GetNbinsX();
  //TH1D* HPeakMinusBaselineAnode31Multiplied = new TH1D("HPeakMinusBaselineAnode31Multiplied", "HPeakMinusBaselineAnode31Multiplied", nbins, min, max);
  // for(int i = 0; i < nbins; ++i){
  // HPeakMinusBaselineAnode31Multiplied->SetBinContent(i+1, HPeakMinusBaselineAnode31->GetBinContent(i+1));
  // }
  /*
  HPeakMinusBaselineAnode31Multiplied->Rebin(rebin);
  HPeakMinusBaselineAnode31Multiplied->SetStats(false);
  if(changeRange){ 
    HPeakMinusBaselineAnode31Multiplied->GetYaxis()->SetRangeUser(0.5, 5e+3);
    HPeakMinusBaselineAnode31Multiplied->GetXaxis()->SetRangeUser(min, max); 
  }
  HPeakMinusBaselineAnode31Multiplied->GetXaxis()->SetTitle("Amplitude [mV]");
  HPeakMinusBaselineAnode31Multiplied->SetFillColor(45);
  HPeakMinusBaselineAnode31Multiplied->Draw(""); 
  */
  HPeakMinusBaselineAnode31->Rebin(16);
  HPeakMinusBaselineAnode31->SetStats(false);
  if(changeRange){ 
    HPeakMinusBaselineAnode31->GetYaxis()->SetRangeUser(0.5, 5e+3);
    HPeakMinusBaselineAnode31->GetXaxis()->SetRangeUser(min, max); 
  }
  HPeakMinusBaselineAnode31->GetXaxis()->SetTitle("Amplitude [mV]");
  HPeakMinusBaselineAnode31->SetFillColor(9);
  HPeakMinusBaselineAnode31->SetTitle("Anode 3 Waveform Spectra");
  HPeakMinusBaselineAnode31->Draw(""); 

  
  std::cout <<"got file 2" << std::endl;
  inputData2->cd();

  TH1D* HPeakMinusBaselineAnode32 = (TH1D*)inputData2->Get("HPeakMinusBaselineAnode3");
  
  HPeakMinusBaselineAnode32->Rebin(rebin);
  HPeakMinusBaselineAnode32->SetStats(false);
  //HPeakMinusBaselineAnode32->Add(HPeakMinusBaselineAnode32, -0.133);
  /*
  if(changeRange){
    HPeakMinusBaselineAnode32->GetYaxis()->SetRangeUser(0.5, 5e+4);
    HPeakMinusBaselineAnode32->GetXaxis()->SetRangeUser(min,max);
  }
  HPeakMinusBaselineAnode32->GetXaxis()->SetTitle("Amplitude [mV]");
  */

  // HPeakMinusBaselineAnode32->SetFillColor(54);
  HPeakMinusBaselineAnode32->Draw("same");

  TLegend* legPeakMinusBaseline = new TLegend(0.5, 0.6, 0.8, 0.8);
  legPeakMinusBaseline->AddEntry(HPeakMinusBaselineAnode31, "Background");
  legPeakMinusBaseline->AddEntry(HPeakMinusBaselineAnode32, "Sources");
  legPeakMinusBaseline->Draw("same");

  ampSpectraCanvas->Print((outDir+"/"+inputFileName+"_and_"+inputFile2Name+"_HPeakMinusBaselineAnode3."+outFileExtension).c_str());

  TCanvas* sMinusBgCanvas = new TCanvas("sMinusBgCanvas", "sMinusBgCanvas");
  sMinusBgCanvas->cd();
  sMinusBgCanvas->SetLogy();
  
  TH1D* HPeakMinusBaselineAnode3Diff = (TH1D*) HPeakMinusBaselineAnode31->Clone("HPeakMinusBaselineAnode3Diff");
  HPeakMinusBaselineAnode3Diff->Add(HPeakMinusBaselineAnode32, -1.);
  HPeakMinusBaselineAnode3Diff->Draw(""); 

  sMinusBgCanvas->Print((outDir+"/"+inputFileName+"_minus_"+inputFile2Name+"HPeakMinusBaselineAnode3."+outFileExtension).c_str());

  // cleaning up
  inputData->Close();
  inputData2->Close();

  return;
}

