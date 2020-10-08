#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include "TString.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TString.h"
#include "TStyle.h"


#include <cmath>
#include <math.h>
#include <TCut.h>
#include <TROOT.h>
#include <TFile.h>
#include <TProfile.h>
#include <TString.h>
#include <TH1.h>
#include <TH2.h>
#include <TH2D.h>
#include <TH3.h>
#include <TH3D.h>
#include <TList.h>
#include <TError.h>
#include <TVector3.h>
#include <TRandom.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <TSystemDirectory.h>
#include <TSystemFile.h>
#include <TStopwatch.h>
#include <TNtuple.h>
#include <TMinuit.h>
#include <TImage.h>
#include <TLine.h>
#include <TF1.h>
#include <TDatime.h>
#include <TRandom1.h>
#include <TMultiGraph.h>

#include "TXMLEngine.h"
#include "TSystem.h"
#include "TSQLRow.h"
#include "TSQLResult.h"
#include "TSQLServer.h"

using namespace std;

//fileToPlotPrefix is for example "SchemeB_Unclean_NoSparks_NoCuts", I can provide a full list of options here, and adding Scheme B as default for now
void LightVsCharge(TString Scheme = "C", TString ChargeType = "PeakPosition"){

  //TString ChargeType = "PeakPosition"; // Entries or PeakPosition
  //ChargeType = "Entries";  
  TString fin = "";
  TString listFileSuffix = "";

  std::string fileToPlotPrefix;
  std::stringstream fileToPlotPrefixStream;
  fileToPlotPrefixStream << "Scheme" << Scheme << "_Clean_NoSparks_TrigAll";
  fileToPlotPrefixStream >> fileToPlotPrefix;  
 
  /*                                                                                                             
  if (Scheme == "A"){}                                                                                           
  else if (Scheme == "B"){}                                                                                 
  else if (Scheme == "C"){}                                                                                   
  else {Fatal("LightVsCharge","No scheme specified");}                                                                            
  */

  if (Scheme == "A"){
    fin = "/home/zchenwis/s0/raptorr2/raptorr/src/dataproc/tpc/ccd/run_scripts/Output_sum_sim/sum_lightsumsquare_OUTPUT_SuperBias_SchemeA_box9x9_PLOT_SBA1.root";}
  if (Scheme == "B"){
    fin = "/home/zchenwis/s0/raptorr2/raptorr/src/dataproc/tpc/ccd/run_scripts/Output_sum_sim/sum_lightsumsquare_OUTPUT_SuperBias_SchemeB_box9x9_PLOT_SBB.root";
    listFileSuffix = "Mod";}
  if (Scheme == "C"){
    fin = "/home/zchenwis/s0/raptorr2/raptorr/src/dataproc/tpc/ccd/run_scripts/Output_sum_sim/sum_lightsumsquare_OUTPUT_SuperBias_SchemeC_box9x9_PLOT_SBB.root";}


  TFile *g = new TFile(fin);

  if (!g || g->IsZombie())
    Fatal("LightVsCharge","Can't open file");

  TTree *LightSumTree = (TTree*)g->Get("LightSumTree3");
  if (!LightSumTree)
    Fatal("LightVsCharge","Can't take LightSumTree from file");
  
  TTree *LightSumTree2 = 0;
  int tsource;
  double tfitssum, terror_on_mean, ta1v, ta2v, ta3v;
  string trunst;

  //LightSumTree->SetBranchAddress("Run_Number",           &trun);
  //LightSumTree->SetBranchAddress("Camera_Number",        &tcam);
  LightSumTree->SetBranchAddress("Source",               &tsource);
  LightSumTree->SetBranchAddress("Fit_Source_Sum",       &tfitssum);
  LightSumTree->SetBranchAddress("Error_on_Mean",        &terror_on_mean);
  //LightSumTree->SetBranchAddress("Events",               &tevents);
  //LightSumTree->SetBranchAddress("Cathode_Voltage",      &tcv);
  LightSumTree->SetBranchAddress("Anode1_Voltage",       &ta1v);
  LightSumTree->SetBranchAddress("Anode2_Voltage",       &ta2v);
  LightSumTree->SetBranchAddress("Anode3_Voltage",       &ta3v);
  //LightSumTree->SetBranchAddress("Pressure",             &tpressure);

  int count = 0;
  int nentries = LightSumTree->GetEntries();
  for (Int_t i = 0; i<nentries; i++) {
    LightSumTree->GetEntry(i);
    //if(tcam == 2 && tsource == 5){
    if(tsource == 5){   
      count++;
    }
  }
  int N = count;
  double entryIntADU[count];
  double entryIntADUError[count];
  double entryVoltage[count];
  double entryIntADU_Vorder[count];
  double entryIntADUError_Vorder[count];
  double entryVoltage_Vorder[count];
  
  count = 0;
  double voltage = 0.0;
  for (Int_t i = 0; i<nentries; i++) {
    LightSumTree->GetEntry(i);
    if(tsource == 5){
      entryIntADU[count] = tfitssum;
      entryIntADUError[count] = terror_on_mean;
      if (Scheme == "A"){entryVoltage[count] = ta3v;}                                                                                    
      else if (Scheme == "B"){entryVoltage[count] = ta3v - ta2v;}                                     
      else if (Scheme == "C"){entryVoltage[count] = ta2v - ta1v;}                                                  
      else {cout << "FATAL ERROR :(" << endl;}     	
      count++;
    }
  }
  
  if (Scheme == "A"){
    for (Int_t i = 0; i<N; i++) {
      entryIntADU_Vorder[i] = entryIntADU[i];
      entryIntADUError_Vorder[i] = entryIntADUError[i];
      entryVoltage_Vorder[i] = entryVoltage[i];}
  }
  else if (Scheme == "B"){
    for (Int_t i = 0; i<N; i++) {
      if (i == 0){
	entryIntADU_Vorder[i] = entryIntADU[2];
	entryIntADUError_Vorder[i] = entryIntADUError[2];
	entryVoltage_Vorder[i] = entryVoltage[2];}
      else if (i == 1){
	entryIntADU_Vorder[i] = entryIntADU[1];
	entryIntADUError_Vorder[i] = entryIntADUError[1];
	entryVoltage_Vorder[i] = entryVoltage[1];
      }
      else if (i == 2){
	entryIntADU_Vorder[i] = entryIntADU[0];
	entryIntADUError_Vorder[i] = entryIntADUError[0];
	entryVoltage_Vorder[i] = entryVoltage[0];
      }
      else{
	entryIntADU_Vorder[i] = entryIntADU[i];
	entryIntADUError_Vorder[i] = entryIntADUError[i];
	entryVoltage_Vorder[i] = entryVoltage[i];
      }
    }
  }
  else if (Scheme == "C"){
    for (Int_t i = 0; i<N; i++) {
      if (i == 0){
        entryIntADU_Vorder[i] = entryIntADU[8];
        entryIntADUError_Vorder[i] = entryIntADUError[8];
	entryVoltage_Vorder[i] = entryVoltage[8];}
      else if (i == 1){
        entryIntADU_Vorder[i] = entryIntADU[7];
        entryIntADUError_Vorder[i] = entryIntADUError[7];
	entryVoltage_Vorder[i] = entryVoltage[7];}
      else if (i == 2){
        entryIntADU_Vorder[i] = entryIntADU[6];
        entryIntADUError_Vorder[i] = entryIntADUError[6];
	entryVoltage_Vorder[i] = entryVoltage[6];}
      else if (i == 3){
        entryIntADU_Vorder[i] = entryIntADU[5];
        entryIntADUError_Vorder[i] = entryIntADUError[5];
	entryVoltage_Vorder[i] = entryVoltage[5];}
      else if (i == 4){
        entryIntADU_Vorder[i] = entryIntADU[4];
        entryIntADUError_Vorder[i] = entryIntADUError[4];
	entryVoltage_Vorder[i] = entryVoltage[4];}
      else{
        entryIntADU_Vorder[i] = entryIntADU[i-5];
        entryIntADUError_Vorder[i] = entryIntADUError[i-5];
	entryVoltage_Vorder[i] = entryVoltage[i-5];
      }
    }
  }
  else {Fatal("LightVsCharge","No scheme specified");} 

  for (Int_t i = 0; i < N; i++) {
    cout << "old: " << entryVoltage[i] << " : " << entryIntADU[i] << " : " << entryIntADUError[i] << endl;
  }
  
  for (Int_t i = 0; i < N; i++) {
    cout << entryVoltage_Vorder[i] << " : " << entryIntADU_Vorder[i] << " : " << entryIntADUError_Vorder[i] << endl;
  }

  //This part just makes the canvas look nice
  
  TCanvas* plotCanvas = new TCanvas("plotCanvas","plot of voltage vs event no", 200, 10, 500, 500);
  plotCanvas->SetLeftMargin(0.12);
  plotCanvas->SetRightMargin(0.12);
  plotCanvas->SetBottomMargin(0.12);
  
  //Vectors of what to plot
  std::vector<double> entryMeans;
  std::vector<double> entryStdDevs;

  //  std::vector<double> peakPosMeans;
  //std::vector<double> peakPosErrors;

  //Vector of file names to look at 
  std::vector<std::string> meanFiles;
  std::vector<std::string> peakPosFiles;

  //get data files to plot (sorry I'm doing this in the laziest way possible, I'll come back in and make this better at some point)
  if(Scheme == "A"){
    peakPosFiles.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/fitDataUNCERTAINSchemeA_Gain1200.dat");
    peakPosFiles.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/fitDataUNCERTAINSchemeA_Gain1400.dat");
    peakPosFiles.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/fitDataUNCERTAINSchemeA_Gain1600.dat");
    peakPosFiles.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/fitDataUNCERTAINSchemeA_Gain1800.dat");   
    peakPosFiles.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/fitDataUNCERTAINSchemeA_Gain2000.dat");
  }
  if(Scheme == "B"){
    peakPosFiles.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/fitDataUNCERTAINSchemeB_Gain0.dat");
    peakPosFiles.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/fitDataUNCERTAINSchemeB_Gain400.dat");
    peakPosFiles.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/fitDataUNCERTAINSchemeB_Gain800.dat");
    peakPosFiles.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/fitDataUNCERTAINSchemeB_Gain1200.dat");
    peakPosFiles.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/fitDataUNCERTAINSchemeB_Gain1400.dat");
    peakPosFiles.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/fitDataUNCERTAINSchemeB_Gain1600.dat");
    peakPosFiles.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/fitDataUNCERTAINSchemeB_Gain1800.dat");
    peakPosFiles.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/fitDataUNCERTAINSchemeB_Gain2000.dat");
    peakPosFiles.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/fitDataUNCERTAINSchemeB_Gain2200.dat");
    peakPosFiles.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/fitDataUNCERTAINSchemeB_Gain2400.dat");
    peakPosFiles.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/fitDataUNCERTAINSchemeB_Gain2600.dat");
  }
  if(Scheme == "C"){
    peakPosFiles.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/fitDataUNCERTAINSchemeC_Gain200.dat");
    peakPosFiles.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/fitDataUNCERTAINSchemeC_Gain400.dat");
    peakPosFiles.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/fitDataUNCERTAINSchemeC_Gain600.dat");
    peakPosFiles.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/fitDataUNCERTAINSchemeC_Gain800.dat");
    peakPosFiles.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/fitDataUNCERTAINSchemeC_Gain1000.dat");
    peakPosFiles.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/fitDataUNCERTAINSchemeC_Gain1200.dat");
    peakPosFiles.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/fitDataUNCERTAINSchemeC_Gain1400.dat");
    peakPosFiles.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/fitDataUNCERTAINSchemeC_Gain1600.dat");
    peakPosFiles.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/fitDataUNCERTAINSchemeC_Gain1800.dat");
   }

  //Examples here of adding data files to vector without a loop
  //meanFiles.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/scripts/bkgVsSignal/comparisonPlots/GroupedRuns_EntriesTestNew_meanAndStdDev.dat");
  //meanFiles.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/scripts/bkgVsSignal/comparisonPlots/IndivRuns_R2083022_to_R2083032_SchemeC_Unclean_meanAndStdDev.dat");

  //get data files to plot from list

  std::ifstream listFile;
  //Looks at a list of which configs you want to plot, right now the two options are runsToPlotSchemeBwith_to_Mod or runsToPlotSchemeCwith_to_   
  TString listFileName;
  std::stringstream listFileNameString;
  listFileNameString << "/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/scripts/bkgVsSignal/runsToPlotScheme"<< Scheme << "wi\
th_to_" << listFileSuffix;
  listFileNameString >> listFileName;
  listFile.open(listFileName);
  std::string fileLine;
  if(listFile.is_open()){
    std::cout << "listFile is open" << std:: endl;
    while (std::getline(listFile, fileLine)){
      std::stringstream entriesSS;
    std:string entriesString = "";
      entriesSS << "/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/scripts/bkgVsSignal/comparisonPlots/IndivRuns_" << fileLine << "_" << fileToPlotPrefix << "_meanAndStdDev.dat";
      entriesSS >> entriesString;
      meanFiles.push_back(entriesString);
    }
  }

  if(ChargeType == "Entries"){
    //Get Mean and Std from File (number of entries)                            
    int n = meanFiles.size();
                                                                      
    for(int iMeanFiles = 0; iMeanFiles < n; iMeanFiles++){
      std::cout << "File to get mean and stddev from is " << meanFiles[iMeanFiles] << std::endl;
      std::ifstream file;
      file.open(meanFiles[iMeanFiles].c_str());
      if(file.is_open()){
	double meanNoEntries, stdDevNoEntries;
	file >> meanNoEntries >> stdDevNoEntries;
	std::cout << "Mean No Entries is " << meanNoEntries << " and std Dev is " << stdDevNoEntries << std::endl;
	entryMeans.push_back(meanNoEntries);
	entryStdDevs.push_back(stdDevNoEntries);
        //cout << entryMeans[iMeanFiles] << endl;
      }
      else{
        std::cout << "Cannot open means file " << meanFiles[iMeanFiles] << std::endl;
      }
      file.close();
    }
  }
  else if(ChargeType == "PeakPosition"){
      //Get Mean and Error from file (peak position)
      int n = peakPosFiles.size();

      for(int iMeanFiles = 0; iMeanFiles < n; iMeanFiles++){
	std::cout << "File to get mean and error from is " << peakPosFiles[iMeanFiles] << std::endl;
	std::ifstream file;
	file.open(peakPosFiles[iMeanFiles].c_str());
	if(file.is_open()){
	  double meanPeakPos, errorPeakPos;
	  file >> meanPeakPos >> errorPeakPos;
	  std::cout << "Mean Peak Position is " << meanPeakPos << " and error is " << errorPeakPos << std::endl;
	  entryMeans.push_back(meanPeakPos);
	  entryStdDevs.push_back(errorPeakPos);
	}
	else{
	  std::cout << "Cannot open means file " << peakPosFiles[iMeanFiles] << std::endl;
	}
	file.close();
      }
  }
  else{std::cout << "YOU MUST Set Charge type as either PeakPosition or Entries" << std::endl;}

  if (Scheme == "A"){N = 5;}
  if (Scheme == "B"){N = 11;}
  if (Scheme == "C"){N = 9;}
 
  TGraphErrors* plotGraph = new TGraphErrors(N,&(entryIntADU_Vorder[0]),&(entryMeans[0]),&(entryIntADUError_Vorder[0]),&(entryStdDevs[0]));

  // if(ChargeType == "Entries"){}  
  //else if(ChargeType == "PeakPosition"){}
  //else{std::cout << "YOU MUST CONSTRUCT ADDITIONAL PYLONS (Set Charge type as either PeakPosition or Entries)" << std::endl;}


  if (Scheme == "A"){
    if(ChargeType == "Entries"){
      plotGraph->SetTitle("Intergrated ADU vs Number of Entries:Scheme A (V1:1200V->2000V)");
      plotGraph->GetYaxis()->SetTitle("Number of Entries per Event");}
    else if(ChargeType == "PeakPosition"){
      plotGraph->SetTitle("Intergrated ADU vs Gas Gain:Scheme A(V1:1200V->2000V) ");
      plotGraph->GetYaxis()->SetTitle("Gas Gain");}
    else{std::cout << "YOU MUST Set Charge type as either PeakPosition or Entries" << std::endl;}
  }
  else if (Scheme == "B"){
    if(ChargeType == "Entries"){
      plotGraph->SetTitle("Intergrated ADU vs Number of Entries:Scheme B (V3-V2:0V->2400V)");
      plotGraph->GetYaxis()->SetTitle("Number of Entries per Event");}
    else if(ChargeType == "PeakPosition"){
      plotGraph->SetTitle("Intergrated ADU vs Gas Gain:Scheme B (V3-V2:0V->2400V)");
      plotGraph->GetYaxis()->SetTitle("Gas Gain");}
    else{std::cout << "YOU MUST Set Charge type as either PeakPosition or Entries" << std::endl;}  
  }
  
  else if (Scheme == "C"){
    if(ChargeType == "Entries"){
      plotGraph->SetTitle("Intergrated ADU vs Number of Entries:Scheme C (V2-V1:200V->2800V)");
      plotGraph->GetYaxis()->SetTitle("Number of Entries per Event");}
    else if(ChargeType == "PeakPosition"){    
      plotGraph->SetTitle("Intergrated ADU vs Gas Gain:Scheme C (V2-V1:200V->2800V)");
      plotGraph->GetYaxis()->SetTitle("Gas Gain");}
    else{std::cout << "YOU MUST Set Charge type as either PeakPosition or Entries" << std::endl;}
  }
  else {Fatal("LightVsCharge","No scheme specified");}  
 
  plotGraph->GetXaxis()->SetTitle("Intergrated ADU (ADU)");                                                                            
  //plotGraph->Fit("pol1");
  plotGraph->Draw("AP");
  g->Close(); 
}
