
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

   Double_t sig = par[0]*TMath::Exp(-0.5*pow((x[0] - par[1]),2)/pow(par[2],2));
   return sig;
}

// directory with the raw data files
//#define DATA_DIR "/scratch4/atarrant/hptpc_and_cmos_guerilla_code/raptorr/"
//#define DATA_DIR "/scratch4/adeistin/2019/11/raptorr_waveform-Analysis_test-pulses/"
#define DATA_DIR "/scratch0/hrichie/hptpc_ritchie/HPTPC/data/preampData/2018/raptorr_waveform-Analysis"

// directory for the output root files
#define RESULT_DIR "/scratch0/hrichie/hptpc_ritchie/HPTPC/data/preampData/2018/fitted_Spectra"
#define CUT_DIR "/scratch0/hrichie/hptpc_ritchie/HPTPC/data/preampData/2018/fitted_Spectra"

/* the standard single page plots */
void plotAmplitudeSpectraPreamp(std::string inputFileName = "1V.root", int anode = 1, int hist_cut = 0, std::string outFileExtension = ".png", bool cuts = false, int rebin = 2, double min = -50, double max = 250, bool changeRange = true){
  // open file and catch, in case it doesn't exist
  std::string inDir;
  std::string outDir;

  double mini, maxi, meani, riseRMS, rise_size; 

  std::vector<Double_t> risetimeN;



  if(!cuts){
    inDir = DATA_DIR;
    outDir = "/scratch0/hrichie/hptpc_ritchie/HPTPC/data/preampData/2018/fitted_Spectra";  
  }
  else{
    inDir = CUT_DIR;
    outDir = CUT_DIR;
  }

  //double mini = 3.5;
  //double maxi = 3.7;
  //double meani = 3.6;

  TFile* inputData = new TFile((inDir+"/"+inputFileName).c_str());
  if(!inputData->IsOpen()){
    std::cout << inputFileName << " could not be opened" << std::endl;
    return;
  } 
  
  // since we know the file's content, we get the required histograms by name
  std::cout << "got file" << std::endl;
  inputData->cd();


  //Define fits
  //TF1 *Sgnl = new TF1("Sgnl",Signal,mini,maxi,3);
  //Sgnl->SetParameters((maxi+mini)/2,meani,0.1);


  //Fit
  for(int i = anode; i <= anode; i++){ //Change int i = 1 back  
     stringstream histotofit;
     histotofit << "AmplitudeAnode" <<  i;
     std::cout << "Using Histogram " << histotofit.str() << std::endl;
     stringstream histoFitter;
     histoFitter << histotofit.str() << Form(" >> AmplitudeAnode%ia", i);

     //Get rise time 
     /*stringstream histotofit1;
     histotofit << "RisetimeAnode%ia" <<  i;
     std::cout << "Using Histogram " << histotofit1.str() << std::endl;
     stringstream histoFitter1;
     histoFitter1 << histotofit1.str() << Form(" >> RisetimeAnode%ia", i);
     std::cout << "debug " << histoFitter.str() << std::endl;
     */
     TCanvas* ampSpectraCanvas = new TCanvas("ampSpectraCanvas", "ampSpectraCanvas");
     ampSpectraCanvas->cd();
     ampSpectraCanvas->SetLogy();

     //Get histograms from root file
     TTree* chargeGainAnalysisTreea = (TTree*)inputData->Get("chargeGainAnalysisTree");
     
     //TH1D* RisetimeAnode = new TH1D(Form("RisetimeAnode%ia", i), Form("RisetimeAnode%ia", i), 100, 0, 1e-06);
     //chargeGainAnalysisTreea->Draw(histoFitter.str().c_str(), "", "goff");
     /*
     riseRMS = RisetimeAnode->GetRMS(); //how to apply cut 
     rise_size = RisetimeAnode->GetEntries();
     std::cout << " The rise RMS " << riseRMS << " the number of entries are " << rise_size << std::endl;  

     // risetimeN = RisetimeAnode->GetEntries();
     */
     TH1D* AmplitudeAnode3a = new TH1D(Form("AmplitudeAnode%ia", i), Form("AmplitudeAnode%ia", i),101, 0, 100);

          TString amplitudeCut = Form("AmplitudeAnode%i > %i", i, hist_cut);
      std::cout << " THE CUT " << amplitudeCut << std::endl;

     chargeGainAnalysisTreea->Draw(histoFitter.str().c_str(), amplitudeCut, "goff");
     //chargeGainAnalysisTreea->Draw(histoFitter.str().c_str(), c1, "goff");

     std::cout << Form("AmplitudeAnode%ia", i) << std::endl;

     mini = AmplitudeAnode3a->GetMaximum();
     maxi = AmplitudeAnode3a->GetMinimum();
     rise_size = AmplitudeAnode3a->GetEntries();

     //     TH1D* AmplitudeAnode3a = new TH1D(Form("AmplitudeAnode%ia", i), Form("AmplitudeAnode%ia", i),50, mini, maxi);
     
     //     for(int eventNum =0; eventNum < rise_size; eventNum++){
       
     //std::cout << "the number of events " << rise_size << std::endl; 
     //}

     chargeGainAnalysisTreea->Draw(histoFitter.str().c_str(), amplitudeCut, "goff");
     
     

     meani = AmplitudeAnode3a->GetMean();
     
     // chargeGainAnalysisTreea->Draw("histotofit.str().c_str() >> AmplitudeAnode3a", "", "goff");

     AmplitudeAnode3a->GetXaxis()->SetTitle("Amplitude [mV]");
     AmplitudeAnode3a->SetFillColor(9);
     AmplitudeAnode3a->SetTitle(Form("Anode %i Waveform Spectra", i));
     
     //Define fits
     TF1 *Sgnl = new TF1("Sgnl",Signal,mini,maxi,3);
     Sgnl->SetParameters(10, meani,1);

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

     std::cout << "The parameters " << Sgnlpeak12 << " " << Sgnlpeak13 << " " << SgnlChisq << " " << SgnlNdf << std::endl; 


     //Save Fit Parameters
     std::string paramArray = outDir+"/"+histotofit.str()+"parameters_"+inputFileName+".dat";
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
     std::string peakschiArray = outDir+"/"+histotofit.str()+"peakschi_"+inputFileName+".dat";
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
     legPeakMinusBaseline->AddEntry(AmplitudeAnode3a, Form("anode %i", i));
     ampSpectraCanvas->Print((outDir+"/"+inputFileName+histotofit.str()+outFileExtension).c_str());
     //ampSpectraCanvas->Print(("fit_"+i+".root").c_str());

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

