
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


Double_t Background(Double_t *x, Double_t *par)
// The background function: an exponential
{
  Double_t val = TMath::Exp(par[0] + par[1]*x[0]);
  return val;
}

Double_t Signal(Double_t *x, Double_t *par)
// The signal function: a gaussian                                                                                                                                                                         
{
  Double_t arg = 0;
  if (par[2]) arg = (x[0] - par[1])/par[2];

  Double_t sig = par[0]*TMath::Exp(-0.5*pow((x[0] - par[1])/par[2],2));
  return sig;
}

Double_t fitFunction(Double_t *x, Double_t *par) 
{
return Background(x,par) + Signal(x,&par[2]);
}


Double_t fitFunction9(Double_t *x, Double_t *par)
{
  return Background(x,par) + Signal(x,&par[2]) + Signal(x,&par[5]);
}


// directory with the raw data files
//#define DATA_DIR "$HPTPC_RESULTS/"
#define DATA_DIR "/scratch0/hrichie/hptpc_ritchie/HPTPC/data/haddData"
//#define DATA_DIR "/scratch0/hrichie/hptpc_ritchie/HPTPC/code/plotting/testData"

// directory for the output root files
#define RESULT_DIR "/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData"

// directory for the input root files
#define CUT_DIR "/scratch0/hrichie/hptpc_ritchie/HPTPC/data/cleanData"

/* the standard single page plots */
void plotAmplitudeSpectraNewFe55(std::string inputFileName = "test.root",bool debug = false, bool cuts = true, int rebin = 25, double min = 0, double max = 450, std::string outFileExtension = ".png", bool changeRange = true){
  // open file and catch, in case it doesn't exist
  std::string inDir;
  std::string outDir;

  if(!cuts){
    inDir = DATA_DIR;
    outDir = RESULT_DIR;  
  }
  else{
    inDir = CUT_DIR;
    outDir = RESULT_DIR;
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
  TF1 *fitFcn = new TF1("fitFcn",fitFunction,15,400,5);
  fitFcn->SetParameters(4,-0.01,140,140,50);

  TF1 *Bkgr = new TF1("Bkgr",Background,15,400,2);
  Bkgr->SetParameters(4,0.1);

  TF1 *fitFcn9 = new TF1("fitFcn9",fitFunction9,15,400,8);

  //Fit
  for(int i = 3; i <= 3; i++){
      stringstream histotofit;
      histotofit << "Anode" <<  i;
      stringstream plotTitle;
      plotTitle << "Waveform Amplitude Spectrum Anode " << i;
      stringstream newhistotofit;
      newhistotofit << "AmplitudeAnode" << i << " >> HPeakMinusBaselineAnode3";
      std::cout << "debug " << newhistotofit.str() << std::endl;

      TCanvas* ampSpectraCanvas = new TCanvas("ampSpectraCanvas", "ampSpectraCanvas");
      ampSpectraCanvas->cd();
      ampSpectraCanvas->SetLogy();

      //Get Histograms from root file
      TTree* chargeGainAnalysisTreea = (TTree*)inputData->Get("chargeGainAnalysisTree");

      TH1D* HPeakMinusBaselineAnode3 = new TH1D("HPeakMinusBaselineAnode3","HPeakMinusBaselineAnode3",202,-10,1010);
      chargeGainAnalysisTreea->Draw(newhistotofit.str().c_str(), "", "goff");


      HPeakMinusBaselineAnode3->GetXaxis()->SetTitle("Amplitude [mV]");


      //Initial fit
      Bkgr->SetLineColor(kGreen);
      HPeakMinusBaselineAnode3->SetTitle(plotTitle.str().c_str());
      HPeakMinusBaselineAnode3->SetFillColor(9);
      HPeakMinusBaselineAnode3->Fit(Bkgr, "R+");
      //HPeakMinusBaselineAnode3->Fit(Bkgr, "R+");

      Double_t bkgr01 = Bkgr->GetParameter(0);
      Double_t bkgr02 = Bkgr->GetParameter(1);

      //Find Initial peak parameters by fitting peaks individually
      fitFcn->SetParameters(bkgr01,bkgr02,140,140,75);
      fitFcn->SetLineColor(kBlue);
      HPeakMinusBaselineAnode3->Fit(fitFcn, "R+");
      if(!debug){
      HPeakMinusBaselineAnode3->Fit(fitFcn, "R");
      }
      else{
      HPeakMinusBaselineAnode3->Fit(fitFcn, "R+");
      }
      Double_t ipeak11 = fitFcn->GetParameter(2);
      Double_t ipeak12 = fitFcn->GetParameter(3);
      Double_t ipeak13 = fitFcn->GetParameter(4);
      fitFcn->SetParameters(bkgr01,bkgr02,ipeak11,ipeak12,ipeak13);
      HPeakMinusBaselineAnode3->Fit(fitFcn, "R+");
      HPeakMinusBaselineAnode3->Fit(fitFcn, "R+");


      //std::cin >> bkgr01;

      fitFcn->SetParameters(bkgr01,bkgr02,ipeak11,ipeak12,ipeak13);
      HPeakMinusBaselineAnode3->Fit(fitFcn, "R+");
      //HPeakMinusBaselineAnode3->Fit(fitFcn, "R+");
      
      
      //Iterative improvements to fitting (taking parameters from a fit and using them as initial parameters for successively more complex fits)
      Double_t bkgr11 = fitFcn->GetParameter(0);
      Double_t bkgr12 = fitFcn->GetParameter(1);
      Double_t peak11 = fitFcn->GetParameter(2);
      Double_t peak12 = fitFcn->GetParameter(3);
      Double_t peak13 = fitFcn->GetParameter(4);
      
      //Remove negative peaks
      
      //2nd Fit
      fitFcn9->SetParameters(bkgr11,bkgr12,peak11,peak12,peak13,150,150,40);
      fitFcn9->SetLineColor(kCyan);
      
      if(!debug){
	HPeakMinusBaselineAnode3->Fit(fitFcn9, "R");
      }
      else{
	HPeakMinusBaselineAnode3->Fit(fitFcn9, "R+");
      }

      //HPeakMinusBaselineAnode3->Fit(fitFcn9,"R+"):

      Double_t bkgr11 = fitFcn9->GetParameter(0);
      Double_t bkgr12 = fitFcn9->GetParameter(1);
      Double_t peak11 = fitFcn9->GetParameter(2);
      Double_t peak12 = fitFcn9->GetParameter(3);
      Double_t peak13 = fitFcn9->GetParameter(4);
      Double_t peak21 = fitFcn9->GetParameter(5);
      Double_t peak22 = fitFcn9->GetParameter(6);
      Double_t peak23 = fitFcn9->GetParameter(7);
      /*
      //Remove negative peaks
      if(peak11 <= 0){
        peak11 = 0;
	peak12 = 0;
	peak13 = 0;
      }
      if(peak31 <= 0){
        peak31 = 0;
	peak32 = 0;
	peak33 = 0;
      }
      if(peak21 <= 0){
        peak21 = 0;
	peak22 = 0;
	peak23 = 0;
      }
      if(peak41 <= 0){
        peak41 = 0;
	peak42 = 0;
	peak43 = 0;
      }   
 
      //Remove peaks that don't improve the fit
      Double_t chisqpeak1 = fitFcn->GetChisquare();
      Double_t ndfpeak1 = fitFcn->GetNDF();
      Double_t chisqpeak2 = fitFcn9->GetChisquare();
      Double_t ndfpeak2 = fitFcn9->GetNDF();     
      Double_t chisqpeak3 = fitFcn2->GetChisquare();
      Double_t ndfpeak3 = fitFcn2->GetNDF();
      Double_t chisqpeak4 = fitFcn8->GetChisquare();
      Double_t ndfpeak4 = fitFcn8->GetNDF();
      if (ndfpeak4 == 0){
	peak41 = 0;
	peak42 = 0;
	peak43 = 0;
      }
      if (ndfpeak3 == 0){
	peak31 = 0;
	peak32 = 0;
	peak33 = 0;
      }

      Double_t chisqndfpeak1 = 0;
      Double_t chisqndfpeak2 = 0;
      Double_t chisqndfpeak3 = 0;
      Double_t chisqndfpeak4 = 0;
      Double_t test = 0;
      /*
      
      //Peak 2
      if (ndfpeak2 == 0 || ndfpeak1 == 0){
	  Double_t test = 0;
	}
      else{
	chisqndfpeak2 = chisqpeak2 / ndfpeak2;
	chisqndfpeak1 = chisqpeak1 / ndfpeak1;	
	if (chisqndfpeak2 >= chisqndfpeak1){
	  peak21 = 0;
	  peak22 = 0;
	  peak23 = 0;
	}
      }
          
      /*
      //Peak 3
      if (ndfpeak2 == 0 || ndfpeak3 == 0){
	  test = 0;
	}
      else{
	chisqndfpeak2 = chisqpeak2 / ndfpeak2;
	chisqndfpeak3 = chisqpeak3 / ndfpeak3;	
	if (chisqndfpeak3 >= chisqndfpeak2){
	  peak31 = 0;
	  peak32 = 0;
	  peak33 = 0;
	}
      }
      */
      /*
      //Peak4
      if (ndfpeak4 == 0 || ndfpeak3 == 0){
	  test = 0;
	}
      else{	
	chisqndfpeak4 = chisqpeak4 / ndfpeak4;
	chisqndfpeak3 = chisqpeak3 / ndfpeak3;
	if (chisqndfpeak4 >= chisqndfpeak3){
	  peak41 = 0;
	  peak42 = 0;
	  peak43 = 0;
	}
      }
      */
      
      /*
      //Print ChiSquare/NDF
      std::cout << " chi2 / ndf " << Bkgr->GetChisquare() << " / " << Bkgr->GetNDF() << std::endl;
      std::cout << " chi2 / ndf " << fitFcn->GetChisquare() << " / " << fitFcn->GetNDF() << " = " << chisqndfpeak1 <<std::endl;
      std::cout << " chi2 / ndf " << fitFcn9->GetChisquare() << " / " << fitFcn9->GetNDF() << " = " << chisqndfpeak2 <<std::endl;
      std::cout << " chi2 / ndf " << fitFcn2->GetChisquare() << " / " << fitFcn2->GetNDF() << " = " << chisqndfpeak3 << std::endl;
      std::cout << " chi2 / ndf " << fitFcn8->GetChisquare() << " / " << fitFcn8->GetNDF() << " = " << chisqndfpeak4 << std::endl;

      //Print ChiSquare/NDF	
      std::cout << " chi2 / ndf " << Bkgr->GetChisquare() << " / " << Bkgr->GetNDF() << std::endl;
      std::cout << " chi2 / ndf " << fitFcn->GetChisquare() << " / " << fitFcn->GetNDF() <<std::endl;
      std::cout << " chi2 / ndf " << fitFcn9->GetChisquare() << " / " << fitFcn9->GetNDF() <<std::endl;
      std::cout << " chi2 / ndf " << fitFcn2->GetChisquare() << " / " << fitFcn2->GetNDF() <<  std::endl;
      std::cout << " chi2 / ndf " << fitFcn8->GetChisquare() << " / " << fitFcn8->GetNDF() << std::endl;


      // chisqndfpeak4 = chisqpeak4 / ndfpeak4;

      //Does the fit converge?
      //TFitResultPtr r = HPeakMinusBaselineAnode3->Fit(fitFcn8,"S");
      //Int_t fitStatus = r;
      // std::cout << "status " << fitStatus << std::endl;

      //Get final fit parameters
      Double_t bkgr11 = fitFcn8->GetParameter(0);
      Double_t bkgr12 = fitFcn8->GetParameter(1);
      Double_t peak11 = fitFcn8->GetParameter(2);
      Double_t peak12 = fitFcn8->GetParameter(3);
      Double_t peak13 = fitFcn8->GetParameter(4);
      Double_t peak21 = fitFcn8->GetParameter(5);
      Double_t peak22 = fitFcn8->GetParameter(6);
      Double_t peak23 = fitFcn8->GetParameter(7);
      Double_t peak31 = fitFcn8->GetParameter(8);
      Double_t peak32 = fitFcn8->GetParameter(9);
      Double_t peak33 = fitFcn8->GetParameter(10);
      Double_t peak41 = fitFcn8->GetParameter(11);
      Double_t peak42 = fitFcn8->GetParameter(12);
      Double_t peak43 = fitFcn8->GetParameter(13);
      Double_t pars8[14] = {bkgr11,bkgr12,peak11,peak12,peak13,peak21,peak22,peak23,peak31,peak32,peak33,peak41,peak42,peak43};
      
      Double_t peakerror12 = fitFcn8->GetParError(3);
      Double_t peakerror22 = fitFcn8->GetParError(6);
      Double_t peakerror32 = fitFcn8->GetParError(9);
      Double_t peakerror42 = fitFcn8->GetParError(12);

      Double_t chisqpeak1 = fitFcn->GetChisquare();
      Double_t ndfpeak1 = fitFcn->GetNDF();
      Double_t chisqpeak2 = fitFcn9->GetChisquare();
      Double_t ndfpeak2 = fitFcn9->GetNDF();     
      Double_t chisqpeak3 = fitFcn2->GetChisquare();
      Double_t ndfpeak3 = fitFcn2->GetNDF();
      Double_t chisqpeak4 = fitFcn8->GetChisquare();
      Double_t ndfpeak4 = fitFcn8->GetNDF();
      
      Double_t peaks[8] = {peak12,peakerror12,peak22,peakerror22,peak32,peakerror32,peak42,peakerror42}; 
      Double_t peakschi[10] = {peak12,peakerror12,peak22,peakerror22,peak32,peakerror32,peak42,peakerror42,chisqpeak4,ndfpeak4};
      Double_t peakschiall[16] = {peak12,peakerror12,peak22,peakerror22,peak32,peakerror32,peak42,peakerror42,chisqpeak1,ndfpeak1,chisqpeak2,ndfpeak2,chisqpeak3,ndfpeak3,chisqpeak4,ndfpeak4};
      */

      Double_t chisqpeak1 = fitFcn9->GetChisquare();                                                                                                                                                        
      Double_t ndfpeak1 = fitFcn9->GetNDF();   
      Double_t peakerror22 = fitFcn9->GetParError(6); 

      Double_t pars8[8] = {bkgr11,bkgr12,peak11,peak12,peak13,peak21,peak22,peak23};
      Double_t peaks[2] = {peak22,peakerror22}; 
      Double_t peakschi[4] = {peak22,peakerror22,chisqpeak1,ndfpeak1};

      //Save Fit Parameters
      std::string paramArray = outDir+"/parameters_"+inputFileName+".dat";
      std::ofstream paramFile;
      paramFile.open(paramArray.c_str());
      for(int i2 = 0; i2 < 8; i2++){
	paramFile << pars8[i2];
        if(i2 != 7) paramFile << "\t";
      }

      //Save Peaks + errors
      std::string peaksArray = outDir+"/peaks_"+inputFileName+".dat";
      std::ofstream peaksFile;
      peaksFile.open(peaksArray.c_str());
      for(int i3 = 0; i3 < 2; i3++){
	peaksFile << peaks[i3];
        if(i3 != 1) peaksFile << "\t";
      }
      
      //Save Peaks + errors + chisq + ndf
      std::string peakschiArray = outDir+"/peakschi_"+inputFileName+".dat";
      std::ofstream peakschiFile;
      peakschiFile.open(peakschiArray.c_str());
      for(int i4 = 0; i4 < 4; i4++){
	peakschiFile << peakschi[i4];
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
      HPeakMinusBaselineAnode3->Draw("");
      TLegend* legPeakMinusBaseline = new TLegend(0.5, 0.6, 0.8, 0.8);
      legPeakMinusBaseline->AddEntry(HPeakMinusBaselineAnode3, "anode 3");
      ampSpectraCanvas->Print((outDir+"/"+inputFileName+histotofit.str()+outFileExtension).c_str());
      /*
      //Print fit peak positions
      std::cout << "Peak 1 Position = " << peak12 << " Error = " << fitFcn8->GetParError(3) << std::endl;
      std::cout << "Peak 2 Position = " << peak22 << " Error = " << fitFcn8->GetParError(6) << std::endl;
      std::cout << "Peak 3 Position = " << peak32 << " Error = " << fitFcn8->GetParError(9) << std::endl;
      std::cout << "Peak 4 Position = " << peak42 << " Error = " << fitFcn8->GetParError(12) << std::endl;
      */
      //Converting Vpeak to energy deposited
      Double_t Vpeak = fitFcn->GetParameter(3);
      Double_t Vpeak2 = Vpeak*1e-3;
      Double_t SourceEnergy = 6e3 / (1.6e-19);
      Double_t WorkFunction = 27.4;
      Double_t Qe = 1.6e-19;
      Double_t PreampGain = 12e-12;

      Double_t EnergyDep = Vpeak2 / (Qe*(SourceEnergy/WorkFunction)*PreampGain);
  
      cout << "a" << endl;
      cout << "Vpeak is" << Vpeak << endl;
      cout << "Gas Gain is" << EnergyDep << endl; 

      

  }
 
  // cleaning up
  inputData->Close();
  return;
}

