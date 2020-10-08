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
  Voltage.push_back(0);
  Voltage.push_back(400);
  Voltage.push_back(800);
  Voltage.push_back(1200);
  Voltage.push_back(1400);
  Voltage.push_back(1600);
  Voltage.push_back(1800);
  Voltage.push_back(2000);
  Voltage.push_back(2200);
  Voltage.push_back(2400);
  Voltage.push_back(2600);

  NoEntries.push_back(670);
  NoEntries.push_back(676);
  NoEntries.push_back(719);
  NoEntries.push_back(735);
  NoEntries.push_back(762);
  NoEntries.push_back(774);
  NoEntries.push_back(785);
  NoEntries.push_back(817);
  NoEntries.push_back(835);
  NoEntries.push_back(866);
  NoEntries.push_back(882);

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
  
  plotGraph->GetXaxis()->SetTitle("Anode 3 Voltage - Anode 2 Voltage (V)");
  plotGraph->GetYaxis()->SetTitle("Number of Entries");
  
  //Draw and save it
  plotGraph->Draw("AP");
  plotCanvas->Print("VoltageVsEventNoSchemeBTest.pdf");
  plotCanvas->Update();
}
