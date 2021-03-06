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

//fileToPlotPrefix is for example "SchemeB_Unclean_NoSparks_NoCuts", I can provide a full list of options here, and adding Scheme B as default for now
void LightVsCharge(TString Scheme = "B"){

  TString fin = "";
  TString listFileSuffix = "";
  
  std::string fileToPlotPrefix;
  std::stringstream fileToPlotPrefixStream;
  fileToPlotPrefixStream << "Scheme" << Scheme << "_Unclean_NoSparks_NoCuts";
  fileToPlotPrefixStream >> fileToPlotPrefix;
  /*                                                                                                             
  if (Scheme == "A"){}                                                                                           
  else if (Scheme == "B"){}                                                                                 
  else if (Scheme == "C"){}                                                                                   
  else {cout << "FATAL ERROR :(" << endl;}                                                                            
  */

  if (Scheme == "A"){
    fin = "/home/zchenwis/s0/raptorr2/raptorr/src/dataproc/tpc/ccd/Sum_output_dir/sum_lightsumsquare_OUTPUT_SuperBias_SchemeA_box9x9.root";}
  if (Scheme == "B"){
    fin = "/home/zchenwis/s0/raptorr2/raptorr/src/dataproc/tpc/ccd/Sum_output_dir/sum_lightsumsquare_OUTPUT_SuperBias_SchemeB_box9x9.root";
    listFileSuffix = "Mod";
  }
  if (Scheme == "C"){
    fin = "/home/zchenwis/s0/raptorr2/raptorr/src/dataproc/tpc/ccd/Sum_output_dir/sum_lightsumsquare_OUTPUT_SuperBias_SchemeC_box9x9.root";}


  TFile *g = new TFile(fin);

  if (!g || g->IsZombie())
    Fatal("plot_lightsumsquare","Can't open file");

  TTree *LightSumTree = (TTree*)g->Get("LightSumTree2");
  if (!LightSumTree)
    Fatal("plot_lightsumsquare","Can't take LightSumTree from file");
  
  TTree *LightSumTree2 = 0;
  int trun, tcam, tsource, tevents, tnumbcut;
  double tfitssum, terror_on_mean, tcv, ta1v, ta2v, ta3v, tpressure;
  string trunst;

  LightSumTree->SetBranchAddress("Run_Number",           &trun);
  LightSumTree->SetBranchAddress("Camera_Number",        &tcam);
  LightSumTree->SetBranchAddress("Source",               &tsource);
  LightSumTree->SetBranchAddress("Fit_Source_Sum",       &tfitssum);
  LightSumTree->SetBranchAddress("Error_on_Mean",        &terror_on_mean);
  LightSumTree->SetBranchAddress("Events",               &tevents);
  LightSumTree->SetBranchAddress("Cut_Events",           &tnumbcut);
  LightSumTree->SetBranchAddress("Cathode_Voltage",      &tcv);
  LightSumTree->SetBranchAddress("Anode1_Voltage",       &ta1v);
  LightSumTree->SetBranchAddress("Anode2_Voltage",       &ta2v);
  LightSumTree->SetBranchAddress("Anode3_Voltage",       &ta3v);
  LightSumTree->SetBranchAddress("Pressure",             &tpressure);

  int count = 0;
  int nentries = LightSumTree->GetEntries();
  for (Int_t i = 0; i<nentries; i++) {
    LightSumTree->GetEntry(i);
    if(tcam == 2 && tsource == 5){
      count++;
    }
  }

  double entryIntADU[count];
  double entryIntADUError[count];
  double entryVoltage[count];
  double entryIntADU_Vorder[count];
  double entryIntADUError_Vorder[count];
  double entryVoltage_Vorder[count];
  
  count = 0;
  if (Scheme == "B"){
    for (Int_t i = 0; i<nentries; i++) {
      LightSumTree->GetEntry(i);
      if(tcam == 2 && tsource == 5){
	cout << count << " " << tfitssum << " " << terror_on_mean << endl;
	entryIntADU[count] = tfitssum;
	entryIntADUError[count] = terror_on_mean;
	if (Scheme == "A"){entryVoltage[i] = ta3v;}                                                                                    
	else if (Scheme == "B"){entryVoltage[i] = ta3v - ta2v;}                                     
	else if (Scheme == "C"){entryVoltage[i] = ta2v - ta1v;}                                                  
	else {cout << "FATAL ERROR :(" << endl;}     	
	count++;
      }
    }
  }
  else if (Scheme == "C"){cout << "FATAL ERROR: No Data on Scheme C" << endl;}
  else{cout << "FATAL ERROR: No Data on Scheme" << endl;}

  int N = count;

  if (Scheme == "A"){cout << "FATAL ERROR: No Data on Scheme A" << endl;}
  else if (Scheme == "B"){
    for (Int_t i = 0; i<N; i++) {
      if (i == 0){
	entryIntADU_Vorder[i] = entryIntADU[10];
	entryIntADUError_Vorder[i] = entryIntADUError[10];}
      else if (i == 1){
	entryIntADU_Vorder[i] = entryIntADU[9];
	entryIntADUError_Vorder[i] = entryIntADUError[9];}
      else if (i == 2){
	entryIntADU_Vorder[i] = entryIntADU[8];
	entryIntADUError_Vorder[i] = entryIntADUError[8];}
      else{
	entryIntADU_Vorder[i] = entryIntADU[i-3];
	entryIntADUError_Vorder[i] = entryIntADUError[i-3];
      }
    }
  }
  else if (Scheme == "C"){cout << "FATAL ERROR: No Data on Scheme C" << endl;}
  else{cout << "FATAL ERROR: No Data on Scheme" << endl;}
  for (Int_t i = 0; i < N; i++) {
    cout << entryIntADU_Vorder[i] << " : " << entryIntADU[i] << endl;
  }
  
  //This part just makes the canvas look nice
  TCanvas* plotCanvas = new TCanvas("plotCanvas","plot of voltage vs event no", 200, 10, 500, 500);
  plotCanvas->SetLeftMargin(0.12);
  plotCanvas->SetRightMargin(0.12);
  plotCanvas->SetBottomMargin(0.12);
  
  //Vectors of what to plot
  std::vector<double> entryMeans;
  std::vector<double> entryStdDevs;

  //Vector of file names to look at 
  std::vector<std::string> meanFiles;

  //Examples here of adding data files to vector without a loop
  //meanFiles.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/scripts/bkgVsSignal/comparisonPlots/GroupedRuns_EntriesTestNew_meanAndStdDev.dat");
  //meanFiles.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/scripts/bkgVsSignal/comparisonPlots/IndivRuns_R2083022_to_R2083032_SchemeC_Unclean_meanAndStdDev.dat");

  //get data files to plot from list
  std::ifstream listFile;
  //Looks at a list of which configs you want to plot, right now the two options are runsToPlotSchemeBwith_to_Mod or runsToPlotSchemeCwith_to_
  TString listFileName;
  std::stringstream listFileNameString;
  listFileNameString << "/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/scripts/bkgVsSignal/runsToPlotScheme"<< Scheme << "with_to_" << listFileSuffix;
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


  //Get Mean and Std from File
  for(int iMeanFiles = 0; iMeanFiles < meanFiles.size(); iMeanFiles++){
    std::cout << "File to get mean and stddev from is " << meanFiles[iMeanFiles] << std::endl;
    std::ifstream file;
    file.open(meanFiles[iMeanFiles].c_str());
    if(file.is_open()){
      double meanNoEntries, stdDevNoEntries;
      file >> meanNoEntries >> stdDevNoEntries;
      std::cout << "Mean No Entries is " << meanNoEntries << " and std Dev is " << stdDevNoEntries << std::endl;
      entryMeans.push_back(meanNoEntries);
      entryStdDevs.push_back(stdDevNoEntries);
    }
    else{
            std::cout << "Cannot open means file " << meanFiles[iMeanFiles] << std::endl;
    }
    file.close();
  }


  int n = entryMeans.size();                                                                                                                            
  TGraphErrors* plotGraph = new TGraphErrors(n,&(entryIntADU_Vorder[0]),&(entryMeans[0]),&(entryIntADUError_Vorder[0]),&(entryStdDevs[0]));
  
  plotGraph->SetTitle("Intergrated ADU vs Number of Entries:Scheme B"); 
  plotGraph->GetXaxis()->SetTitle("Intergrated ADU");                                                                            
  plotGraph->GetYaxis()->SetTitle("Number of Entries per Event");  
  plotGraph->Fit("pol1");
  
  //Don't worry about this bit, this is where it makes the plots, just leaving it in for posterity 
  /*
  //Time to Plot
  int n = entryMeans.size();
  TGraphErrors* plotGraph = new TGraphErrors(10,&(VoltageDoub[0]),&(entryMeans[0]),0,&(entryStdDevs[0]));
  plotGraph->SetTitle("Voltage vs Number of Entries");
  //plotGraph->Fit("pol1");
  
  //Make it pretty
  plotGraph->SetLineColor(38);
  plotGraph->SetMarkerColor(30);
  plotGraph->SetMarkerStyle(33);
  
  plotGraph->GetYaxis()->SetTitleSize(0.045);
  plotGraph->GetYaxis()->SetTitleOffset(0.95);
  plotGraph->GetXaxis()->SetTitleSize(0.045);
  plotGraph->GetXaxis()->SetTitleOffset(0.95);
  
  plotGraph->GetXaxis()->SetTitle("Anode 2 Voltage - Anode 1 Voltage (V)");
  plotGraph->GetYaxis()->SetTitle("Number of Entries");
  
  //Draw and save it
  plotGraph->Draw("AP");
  std::stringstream plotName;
  plotName << "VoltageVsNoEntries_" << fileToPlotPrefix << ".pdf";
  plotCanvas->Print(plotName.str().c_str());
  plotCanvas->Update();
  */
  
  plotGraph->Draw("AP");
  g->Close();

}
