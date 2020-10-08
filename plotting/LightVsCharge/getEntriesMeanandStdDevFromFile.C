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
void plotNoEntries(std::string fileToPlotPrefix = "SchemeB_Unclean_NoSparks_NoCuts"){
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
  listFile.open("/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/scripts/bkgVsSignal/runsToPlotSchemeBwith_to_Mod");
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
  }



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
}
