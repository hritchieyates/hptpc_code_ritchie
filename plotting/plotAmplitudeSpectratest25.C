
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

Double_t Background2(Double_t *x, Double_t *par)
// The background function: an exponential
{
  Double_t val = TMath::Exp(Background(x,par));
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

Double_t fitFunction2(Double_t *x, Double_t *par) 
{
return Background(x,par) + Signal(x,&par[2]) + Signal(x,&par[5]) + Signal(x,&par[8]);
}

Double_t fitFunction5(Double_t *x, Double_t *par) 
{
return Background(x,par) + Signal(x,&par[2]) + Signal(x,&par[5]);
}

Double_t fitFunction6(Double_t *x, Double_t *par) 
{
  return fitFunction(x,par) + Signal(x,&par[5]);
}

Double_t fitFunction8(Double_t *x, Double_t *par) 
{
  return Background(x,par) + Signal(x,&par[2]) + Signal(x,&par[5]) + Signal(x,&par[8]) + Signal(x,&par[11]);
}


// directory with the raw data files
#define DATA_DIR "$HPTPC_RESULTS/"

// directory for the output root files
#define RESULT_DIR "$HPTPC_RESULTS/"

// directory for the output root files
#define CUT_DIR "$HPTPC_RESULTS_CUT/"

/* the standard single page plots */
void plotAmplitudeSpectratest24(std::string inputFileName = "test.root",bool debug = false, bool cuts = true, int rebin = 25, double min = 0, double max = 450, std::string outFileExtension = ".png", bool changeRange = true){
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
  TF1 *expogausfit = new TF1("expogausfit","expo(3)+gaus(0)",10,500);
  expogausfit->SetParameters(1,-2,200,200,1);

  TF1 *expofit = new TF1("expofit","expo(3)",10,500);
  expofit->SetParameters(1,-2);

  TF1 *gausfit = new TF1("gausfit","gaus(0)",10,500);
  gausfit->SetParameters(200,200,1);

  TF1 *fitFcn = new TF1("fitFcn",fitFunction,175,400,5);
  fitFcn->SetParameters(4,-0.01,200,200,10);

  TF1 *fitFcn3 = new TF1("fitFcn3",fitFunction,600,1000,5);
  fitFcn3->SetParameters(4,-0.01,30,350,10);

  TF1 *fitFcn4 = new TF1("fitFcn4",fitFunction,950,1300,5);
  fitFcn4->SetParameters(4,-0.01,30,450,10);

  TF1 *Sgnl4 = new TF1("Sgnl4",Signal,400,500,3);
  Sgnl4->SetParameters(30,400,10);

  TF1 *fitFcn7 = new TF1("fitFcn7",fitFunction,1400,1700,5);
  fitFcn7->SetParameters(4,-0.01,10,600,5);

  TF1 *Bkgr = new TF1("Bkgr",Background,175,2000,2);
  Bkgr->SetParameters(4,0.1);

  TF1 *Bkgr2 = new TF1("Bkgr2",Background2,10,500,2);
  Bkgr2->SetParameters(6,-0.015);

  TF1 *Sgnl = new TF1("Sgnl",Signal,10,500,3);
  Sgnl->SetParameters(200,200,1);

  // TF1 *fitFcn2 = new TF1("fitFcn9",fitFunction9,100,800,8);
  //fitFcn9->SetParameters(4,-0.01,200,200,10,30,350,10);

  TF1 *fitFcn2 = new TF1("fitFcn2",fitFunction2,175,2000,11);
  fitFcn2->SetParameters(4,-0.01,200,200,10,30,350,10,30,450,10);

  TF1 *fitFcn5 = new TF1("fitFcn5",fitFunction5,10,700,8);
  fitFcn5->SetParameters(4,-0.01,200,200,10,30,350,10);

  TF1 *fitFcn6 = new TF1("fitFcn6",fitFunction6,10,500,8);
  fitFcn6->SetParameters(4,-0.01,200,200,10,30,350,10);

  TF1 *fitFcn8 = new TF1("fitFcn8",fitFunction8,175,2000,14);
  Double_t pars8[14] = {4,-0.01,200,200,10,30,350,10,30,450,10,10,600,5};
  //fitFcn8->SetParameters(pars8);

  TF1 *fitFcn9 = new TF1("fitFcn9",fitFunction5,175,2000,8);
  //fitFcn9->SetParameters(4,-0.01,200,200,10,30,350,10);


  //Fit
  for(int i = 1; i <= 3; i++){
      stringstream histotofit;
      histotofit << "HPeakMinusBaselineAnode" <<  i;
      std::cout << "debug " << histotofit.str() << std::endl;

      TCanvas* ampSpectraCanvas = new TCanvas("ampSpectraCanvas", "ampSpectraCanvas");
      ampSpectraCanvas->cd();
      ampSpectraCanvas->SetLogy();

      //Get histograms from root file
      TH1D* HPeakMinusBaselineAnode3 = (TH1D*)inputData->Get(histotofit.str().c_str());

      HPeakMinusBaselineAnode3->Rebin(rebin);
      HPeakMinusBaselineAnode3->SetStats(false);
      if(changeRange){ 
	HPeakMinusBaselineAnode3->GetYaxis()->SetRangeUser(0.5, 5e+4);
	HPeakMinusBaselineAnode3->GetXaxis()->SetRangeUser(min, max); 
      }
      HPeakMinusBaselineAnode3->GetXaxis()->SetTitle("Amplitude [mV]");


      //Initial fit
      Bkgr->SetLineColor(kGreen);
      HPeakMinusBaselineAnode3->SetFillColor(45);
      HPeakMinusBaselineAnode3->Fit(Bkgr, "R+");
      //HPeakMinusBaselineAnode3->Fit(Bkgr, "R+");

      Double_t bkgr01 = Bkgr->GetParameter(0);
      Double_t bkgr02 = Bkgr->GetParameter(1);

      //Find Initial peak parameters by fitting peaks individually
      fitFcn->SetParameters(bkgr01,bkgr02,600,250,75);
      fitFcn3->SetParameters(bkgr01,bkgr02,200,850,100);
      fitFcn4->SetParameters(bkgr01,bkgr02,200,1100,100);
      fitFcn7->SetParameters(bkgr01,bkgr02,200,1600,100);

      HPeakMinusBaselineAnode3->Fit(fitFcn, "R+")
      HPeakMinusBaselineAnode3->Fit(fitFcn3, "R+");
      HPeakMinusBaselineAnode3->Fit(fitFcn4, "R+");
      HPeakMinusBaselineAnode3->Fit(fitFcn7, "R+");

      Double_t ipeak11 = fitFcn->GetParameter(2);
      Double_t ipeak12 = fitFcn->GetParameter(3);
      Double_t ipeak13 = fitFcn->GetParameter(4);
      Double_t ipeak21 = fitFcn3->GetParameter(2);
      Double_t ipeak22 = fitFcn3->GetParameter(3);
      Double_t ipeak23 = fitFcn3->GetParameter(4);
      Double_t ipeak31 = fitFcn4->GetParameter(2);
      Double_t ipeak32 = fitFcn4->GetParameter(3);
      Double_t ipeak33 = fitFcn4->GetParameter(4);
      Double_t ipeak41 = fitFcn7->GetParameter(2);
      Double_t ipeak42 = fitFcn7->GetParameter(3);
      Double_t ipeak43 = fitFcn7->GetParameter(4);


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
      fitFcn9->SetParameters(bkgr11,bkgr12,peak11,peak12,peak13,ipeak21,ipeak22,ipeak23);
      fitFcn9->SetLineColor(kCyan);
      HPeakMinusBaselineAnode3->Fit(fitFcn9,"R+");
      //HPeakMinusBaselineAnode3->Fit(fitFcn9,"R+"):

      Double_t bkgr11 = fitFcn9->GetParameter(0);
      Double_t bkgr12 = fitFcn9->GetParameter(1);
      Double_t peak11 = fitFcn9->GetParameter(2);
      Double_t peak12 = fitFcn9->GetParameter(3);
      Double_t peak13 = fitFcn9->GetParameter(4);
      Double_t peak21 = fitFcn9->GetParameter(5);
      Double_t peak22 = fitFcn9->GetParameter(6);
      Double_t peak23 = fitFcn9->GetParameter(7);

      //Remove negative peaks

      //3rd Fit
      fitFcn2->SetParameters(bkgr11,bkgr12,peak11,peak12,peak13,peak21,peak22,peak23,ipeak31,ipeak32,ipeak33);
      HPeakMinusBaselineAnode3->Fit(fitFcn2,"R+");
      //HPeakMinusBaselineAnode3->Fit(fitFcn2,"R+");

      Double_t bkgr11 = fitFcn2->GetParameter(0);
      Double_t bkgr12 = fitFcn2->GetParameter(1);
      Double_t peak11 = fitFcn2->GetParameter(2);
      Double_t peak12 = fitFcn2->GetParameter(3);
      Double_t peak13 = fitFcn2->GetParameter(4);
      Double_t peak21 = fitFcn2->GetParameter(5);
      Double_t peak22 = fitFcn2->GetParameter(6);
      Double_t peak23 = fitFcn2->GetParameter(7);
      Double_t peak31 = fitFcn2->GetParameter(8);
      Double_t peak32 = fitFcn2->GetParameter(9);
      Double_t peak33 = fitFcn2->GetParameter(10);

      //Remove negative peaks

      //4th Fit
      Double_t pars8[14] = {bkgr11,bkgr12,peak11,peak12,peak13,peak21,peak22,peak23,peak31,peak32,peak33,ipeak41,ipeak42,ipeak43};
      fitFcn8->SetParameters(pars8);
      fitFcn8->SetLineColor(kBlue);
      HPeakMinusBaselineAnode3->Fit(fitFcn8,"R+");
      HPeakMinusBaselineAnode3->Fit(fitFcn8,"R+");
      HPeakMinusBaselineAnode3->Fit(fitFcn8,"R+");
      
      Double_t bkgr11 = fitFcn8->GetParameter(0);
      Double_t bkgr12 = fitFcn8->GetParameter(1);
      Double_t peak11 = fitFcn8->GetParameter(2);
      Double_t peak12 = fitFcn8->GetParameter(3);
      //Double_t peak11 = 0;   
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

      //Print ChiSquare/NDF
      std::cout << " chi2 / ndf " << Bkgr->GetChisquare() << " / " << Bkgr->GetNDF() << std::endl;
      std::cout << " chi2 / ndf " << fitFcn->GetChisquare() << " / " << fitFcn->GetNDF() << " = " << chisqndfpeak1 <<std::endl;
      std::cout << " chi2 / ndf " << fitFcn9->GetChisquare() << " / " << fitFcn9->GetNDF() << " = " << chisqndfpeak2 <<std::endl;
      std::cout << " chi2 / ndf " << fitFcn2->GetChisquare() << " / " << fitFcn2->GetNDF() << " = " << chisqndfpeak3 << std::endl;
      std::cout << " chi2 / ndf " << fitFcn8->GetChisquare() << " / " << fitFcn8->GetNDF() << " = " << chisqndfpeak4 << std::endl;
      
      //Final Fit
      Double_t pars8[14] = {bkgr11,bkgr12,peak11,peak12,peak13,peak21,peak22,peak23,peak31,peak32,peak33,peak41,peak42,peak43};
      fitFcn8->SetParameters(pars8);
      if(debug){
      HPeakMinusBaselineAnode3->Fit(fitFcn8,"R+");
      }
      else{
      HPeakMinusBaselineAnode3->Fit(fitFcn8,"R");
      }
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

      //Save Fit Parameters
      std::string paramArray = "parameters_"+inputFileName+".dat";
      std::ofstream paramFile;
      paramFile.open(paramArray.c_str());
      for(int i2 = 0; i2 < 14; i2++){
	paramFile << pars8[i2];
        if(i2 != 13) paramFile << "\t";
      }

      //Save Peaks + errors
      std::string peaksArray = "peaks_"+inputFileName+".dat";
      std::ofstream peaksFile;
      peaksFile.open(peaksArray.c_str());
      for(int i3 = 0; i3 < 8; i3++){
	peaksFile << peaks[i3];
        if(i3 != 7) peaksFile << "\t";
      }
      /*
      //Save Peaks + errors + chisq + ndf
      std::string peakschiArray = "peakschi_"+inputFileName+".dat";
      std::ofstream peakschiFile;
      peakschiFile.open(peakschiArray.c_str());
      for(int i4 = 0; i4 < 10; i4++){
	peakschiFile << peakschi[i4];
        if(i4 != 9) peakschiFile << "\t";
      }
      */
      //Save Peaks + errors + all chisq + ndf
      std::string peakschiallArray = "peakschiall_"+inputFileName+".dat";
      std::ofstream peakschiallFile;
      peakschiallFile.open(peakschiallArray.c_str());
      for(int i5 = 0; i5 < 16; i5++){
	peakschiallFile << peakschiall[i5];
        if(i5 != 15) peakschiallFile << "\t";
      }

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

      

  }
 
  // cleaning up
  inputData->Close();
  return;
}

