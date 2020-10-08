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

void plotNoEntries(std::string fileToPlotPrefix = "", std::string Scheme = ""){
  TCanvas* plotCanvas = new TCanvas("plotCanvas","plot of voltage vs event no", 200, 10, 500, 500);
  plotCanvas->SetLeftMargin(0.12);
  plotCanvas->SetRightMargin(0.12);
  plotCanvas->SetBottomMargin(0.12);
  
  //Vectors of what to plot
  std::vector<int> Voltage;
  std::vector<double> VoltageDoub;
  std::vector<int> NoEntries;

  std::vector<double> entryMeans;
  std::vector<double> entryStdDevs;

  std::vector<std::string> meanFiles;

  //  meanFiles.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/scripts/bkgVsSignal/comparisonPlots/GroupedRuns_EntriesTestNew_meanAndStdDev.dat");

  //meanFiles.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/scripts/bkgVsSignal/comparisonPlots/IndivRuns_R2083022_to_R2083032_SchemeC_Unclean_meanAndStdDev.dat");

  std::cout << "Scheme is " << Scheme << std::endl;

  //Voltages to plot                                                                        
  //SchemeC
  if(Scheme == "SchemeC"){
    VoltageDoub.push_back(200.);
    VoltageDoub.push_back(400.);
    VoltageDoub.push_back(600.);
    VoltageDoub.push_back(800.);
    VoltageDoub.push_back(1000.);
    VoltageDoub.push_back(1200.);
    VoltageDoub.push_back(1400.);
    VoltageDoub.push_back(1600.);
    VoltageDoub.push_back(1800.);
  }
  //SchemeB
  if(Scheme == "SchemeB"){
    VoltageDoub.push_back(0.);
    VoltageDoub.push_back(400.);
    VoltageDoub.push_back(800.);
    VoltageDoub.push_back(1200.);
    VoltageDoub.push_back(1400.);
    VoltageDoub.push_back(1600.);
    VoltageDoub.push_back(1800.);
    VoltageDoub.push_back(2000.);
    VoltageDoub.push_back(2200.);
    VoltageDoub.push_back(2400.);
  }
  if(Scheme =="SchemeA"){
    VoltageDoub.push_back(1200.);
    VoltageDoub.push_back(1400.);
    VoltageDoub.push_back(1600.);
    VoltageDoub.push_back(1800.);
    VoltageDoub.push_back(2000.);
  }
  if(Scheme == "SchemeGas"){
    VoltageDoub.push_back(19.);
    VoltageDoub.push_back(20.);
    VoltageDoub.push_back(21.);
    VoltageDoub.push_back(22.);
    VoltageDoub.push_back(23.);
  }


  //get data files to plot from list
  std::ifstream listFile;
  if(Scheme == "SchemeB"){
  listFile.open("/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/scripts/bkgVsSignal/runsToPlotSchemeBwith_to_Mod");
  }
  if(Scheme == "SchemeC"){
    listFile.open("/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/scripts/bkgVsSignal/runsToPlotSchemeCwith_to_");
  }
  if(Scheme == "SchemeA"){
    listFile.open("/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/scripts/bkgVsSignal/runsToPlotSchemeAwith_to_");
  }
  if(Scheme == "SchemeGas"){
    listFile.open("/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/scripts/bkgVsSignal/runsToPlotSchemeGaswith_to_");
  }
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

  //Time to Plot
  int n = entryMeans.size();
  if(Scheme == "SchemeB"){
  TGraphErrors* plotGraph = new TGraphErrors(10,&(VoltageDoub[0]),&(entryMeans[0]),0,&(entryStdDevs[0]));

  plotGraph->SetTitle("Voltage vs Number of Entries");
  //Make it pretty                                                                                                                                                                                          
  plotGraph->SetLineColor(4);
  plotGraph->SetMarkerColor(4);
  plotGraph->SetMarkerStyle(33);

  plotGraph->GetYaxis()->SetTitleSize(0.045);
  plotGraph->GetYaxis()->SetTitleOffset(1.25);
  plotGraph->GetXaxis()->SetTitleSize(0.045);
  plotGraph->GetXaxis()->SetTitleOffset(0.95);

  plotGraph->GetXaxis()->SetTitle("Anode 3 Voltage - Anode 2 Voltage (V)");
  plotGraph->GetYaxis()->SetTitle("Number of Entries");

  plotGraph->Draw("AP");
  }
  if(Scheme == "SchemeC"){
  TGraphErrors* plotGraph = new TGraphErrors(9,&(VoltageDoub[0]),&(entryMeans[0]),0,&(entryStdDevs[0]));

  plotGraph->SetTitle("Voltage vs Number of Entries");
  plotGraph->SetLineColor(2);
  plotGraph->SetMarkerColor(2);
  plotGraph->SetMarkerStyle(33);

  plotGraph->GetYaxis()->SetTitleSize(0.045);
  plotGraph->GetYaxis()->SetTitleOffset(1.25);
  plotGraph->GetXaxis()->SetTitleSize(0.045);
  plotGraph->GetXaxis()->SetTitleOffset(0.95);

  plotGraph->GetXaxis()->SetTitle("Anode 2 Voltage - Anode 1 Voltage (V)");
  plotGraph->GetYaxis()->SetTitle("Number of Entries");

  plotGraph->Draw("AP");
  }
  if(Scheme == "SchemeA"){
    TGraphErrors* plotGraph = new TGraphErrors(5,&(VoltageDoub[0]),&(entryMeans[0]),0,&(entryStdDevs[0]));

    plotGraph->SetTitle("Voltage vs Number of Entries");
    plotGraph->SetLineColor(kGreen+2);
    plotGraph->SetMarkerColor(kGreen+2);
    plotGraph->SetMarkerStyle(33);

    plotGraph->GetYaxis()->SetTitleSize(0.045);
    plotGraph->GetYaxis()->SetTitleOffset(1.25);
    plotGraph->GetXaxis()->SetTitleSize(0.045);
    plotGraph->GetXaxis()->SetTitleOffset(0.95);

    plotGraph->GetXaxis()->SetTitle("Anode 1 Voltage (V)");
    plotGraph->GetYaxis()->SetTitle("Number of Entries");

    plotGraph->Draw("AP");
  }
  
  if(Scheme == "SchemeGas"){
    TGraphErrors* plotGraph = new TGraphErrors(5,&(VoltageDoub[0]),&(entryMeans[0]),0,&(entryStdDevs[0]));

    plotGraph->SetTitle("Voltage vs Date");
    plotGraph->SetLineColor(9);
    plotGraph->SetMarkerColor(9);
    plotGraph->SetMarkerStyle(33);

    plotGraph->GetYaxis()->SetTitleSize(0.045);
    plotGraph->GetYaxis()->SetTitleOffset(1.25);
    plotGraph->GetXaxis()->SetTitleSize(0.045);
    plotGraph->GetXaxis()->SetTitleOffset(0.95);

    plotGraph->GetXaxis()->SetTitle("Date");
    plotGraph->GetYaxis()->SetTitle("Number of Entries");

    plotGraph->Draw("AP");
  }


  //Draw and save it
  std::stringstream plotName;
  plotName << "VoltageVsNoEntries_" << fileToPlotPrefix << ".pdf";
  plotCanvas->Print(plotName.str().c_str());
  plotCanvas->Update();
}
