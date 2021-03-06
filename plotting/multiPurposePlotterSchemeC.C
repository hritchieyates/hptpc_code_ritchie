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

void multiPurposePlotter(){
  TCanvas* plotCanvas = new TCanvas("plotCanvas","plot of voltage vs event no", 200, 10, 500, 500);
  plotCanvas->SetLeftMargin(0.12);
  plotCanvas->SetRightMargin(0.12);
  plotCanvas->SetBottomMargin(0.12);
  
  //Vectors of what to plot
  std::vector<int> Voltage;
  std::vector<int> NoEntries;

  //Values to plot
  Voltage.push_back(200);
  Voltage.push_back(400);
  Voltage.push_back(600);
  Voltage.push_back(800);
  Voltage.push_back(1000);
  Voltage.push_back(1200);
  Voltage.push_back(1400);
  Voltage.push_back(1600);
  Voltage.push_back(1800);

  NoEntries.push_back(662);
  NoEntries.push_back(674);
  NoEntries.push_back(687);
  NoEntries.push_back(701);
  NoEntries.push_back(713);
  NoEntries.push_back(740);
  NoEntries.push_back(759);
  NoEntries.push_back(792);
  NoEntries.push_back(826);

  //Time to Plot
  int n = NoEntries.size();
  plotGraph = new TGraph(n,&(Voltage[0]),&(NoEntries[0]));
  plotGraph->SetTitle("Voltage vs Number of Events");
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
  plotCanvas->Print("VoltageVsEventNoSchemeCTest.pdf");
  plotCanvas->Update();
}
