
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TSystem.h"
#include "TH2.h"
#include "TH1.h"
#include "TGraph.h"

#include "TCanvas.h"
#include "TFrame.h"
#include "TBenchmark.h"
#include "TString.h"
#include "TF1.h"
#include "TH1.h"
#include "TFile.h"
#include "TROOT.h"
#include "TError.h"
#include "TInterpreter.h"
#include "TSystem.h"
#include "TPaveText.h"
#include "TMath.h"


Double_t Signal(Double_t *x, Double_t *par)
// The signal function: a gaussian
{
   Double_t arg = 0;
   if (par[2]) arg = (x[0] - par[1])/par[2];

   Double_t sig = par[0]*TMath::Exp(-0.5*pow((x[0] - par[1])/par[2],2));
   return sig;
}

// directory with the raw data files
#define DATA_DIR "$HPTPC_RESULTS/"

// directory for the output root files
#define RESULT_DIR "$HPTPC_RESULTS/"

// directory for the output root files
#define CUT_DIR "$HPTPC_RESULTS_CUT/"

/* the standard single page plots */
void plotAmplitudeSpectraPreamp(std::string inputFileName = "test.root", bool cuts = true, int rebin = 2, double min = -50, double max = 250, std::string outFileExtension = ".png", bool changeRange = true){
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
  
  // since we know the file's content, we get the required histograms by name
  std::cout << "got file" << std::endl;
  inputData->cd();


  //Define fits
  TF1 *Sgnl = new TF1("Sgnl",Signal,10,500,3);
  Sgnl->SetParameters(10,55,1);


  //Fit
  for(int i = 1; i <= 1; i++){
      stringstream histotofit;
      histotofit << "AmplitudeAnode" <<  i;
      std::cout << "Using Histogram " << histotofit.str() << std::endl;

      TCanvas* ampSpectraCanvas = new TCanvas("ampSpectraCanvas", "ampSpectraCanvas");
      ampSpectraCanvas->cd();
      ampSpectraCanvas->SetLogy();

      //Get histograms from root file
      TTree* chargeGainAnalysisTreea = (TTree*)inputData->Get("chargeGainAnalysisTree");

      TH1D* AmplitudeAnode3a = new TH1D("AmplitudeAnode3a", "AmplitudeAnode3a",101,-10,2010);
      chargeGainAnalysisTreea->Draw("AmplitudeAnode3 >> AmplitudeAnode3a", "", "goff");
      // chargeGainAnalysisTreea->Draw("histotofit.str().c_str()); >> AmplitudeAnode3a", "", "goff");

      TH1D* HPeakMinusBaselineAnode3 = (TH1D*)inputData->Get(histotofit.str().c_str());



      AmplitudeAnode3a->GetXaxis()->SetTitle("Amplitude [mV]");
      AmplitudeAnode3a->SetFillColor(9);
      AmplitudeAnode3a->SetTitle("Anode 3 Waveform Spectra");

      //Initial fit
      AmplitudeAnode3a->Fit(Sgnl, "R");

      Double_t Sgnlpeak11 = Sgnl->GetParameter(0);
      Double_t Sgnlpeak12 = Sgnl->GetParameter(1);
      Double_t Sgnlpeak13 = Sgnl->GetParameter(2);
      Double_t Sgnlpeakerror12 = Sgnl->GetParError(1);
      Double_t SgnlChisq = Sgnl->GetChisquare();
      Double_t SgnlNdf = Sgnl->GetNDF();
      Double_t Sgnlpars[3] = {Sgnlpeak11,Sgnlpeak12,Sgnlpeak13};
      Double_t SgnlPeaksChi[4] = {Sgnlpeak12, Sgnlpeak13, SgnlChisq, SgnlNdf};

      //Save Fit Parameters
      std::string paramArray = "parameters_"+inputFileName+".dat";
      std::ofstream paramFile;
      paramFile.open(paramArray.c_str());
      for(int i2 = 0; i2 < 3; i2++){
	paramFile << Sgnlpars[i2];
        if(i2 != 2) paramFile << "\t";
      }

      /*
      //Save Peaks + errors
      std::string peaksArray = "peaks_"+inputFileName+".dat";
      std::ofstream peaksFile;
      peaksFile.open(peaksArray.c_str());
      for(int i3 = 0; i3 < 8; i3++){
	peaksFile << peaks[i3];
        if(i3 != 7) peaksFile << "\t";
      }
      */
      
      //Save Peaks + errors + chisq + ndf
      std::string peakschiArray = "peakschi_"+inputFileName+".dat";
      std::ofstream peakschiFile;
      peakschiFile.open(peakschiArray.c_str());
      for(int i4 = 0; i4 < 4; i4++){
	peakschiFile << SgnlPeaksChi[i4];
        if(i4 != 3) peakschiFile << "\t";
      }
      
      /*
      //Save Peaks + errors + all chisq + ndf
      std::string peakschiallArray = "peakschiall_"+inputFileName+".dat";
      std::ofstream peakschiallFile;
      peakschiallFile.open(peakschiallArray.c_str());
      for(int i5 = 0; i5 < 16; i5++){
	peakschiallFile << peakschiall[i5];
        if(i5 != 15) peakschiallFile << "\t";
      }
      */

      //Draw Canvases & Save output
      AmplitudeAnode3a->Draw("");
      TLegend* legPeakMinusBaseline = new TLegend(0.5, 0.6, 0.8, 0.8);
      legPeakMinusBaseline->AddEntry(HPeakMinusBaselineAnode3, "anode 3");
      ampSpectraCanvas->Print((outDir+"/"+inputFileName+histotofit.str()+outFileExtension).c_str());



      /*
      //Print fit peak positions
      std::cout << "Peak 1 Position = " << peak12 << " Error = " << fitFcn8->GetParError(3) << std::endl;
      std::cout << "Peak 2 Position = " << peak22 << " Error = " << fitFcn8->GetParError(6) << std::endl;
      std::cout << "Peak 3 Position = " << peak32 << " Error = " << fitFcn8->GetParError(9) << std::endl;
      std::cout << "Peak 4 Position = " << peak42 << " Error = " << fitFcn8->GetParError(12) << std::endl;

      //Converting Vpeak to energy deposited
      Double_t Vpeak = fitFcn8->GetParameter(6);
      Double_t Vpeak2 = Vpeak*1e-3;
      Double_t SourceEnergy = 2200e3 / (1.6e-19);
      Double_t WorkFunction = 27.4;
      Double_t Qe = 1.6e-19;
      Double_t PreampGain = 1.2e-9;

      Double_t EnergyDep = Vpeak2 / (Qe*(SourceEnergy/WorkFunction)*PreampGain);
  
      cout << "a" << endl;
      cout << "Vpeak is" << Vpeak << endl;
      cout << "Gas Gain is" << EnergyDep << endl; 
      */
      

  }
 
  // cleaning up
  inputData->Close();
  return;
}

