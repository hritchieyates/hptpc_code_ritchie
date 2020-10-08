#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include "TSQLServer.h"
#include "TString.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TSQLResult.h"
#include "TString.h"
#include "TSQLRow.h"
#include "TStyle.h"

void plotPeakPositionPreampNew(std::string preampNo = "2", std::string plottoplot= "noplot"){
   TCanvas* c1 = new TCanvas("c1","Plots for peak position",200,10,500,500);
   c1->SetLeftMargin(0.12);
   c1->SetRightMargin(0.12);
   c1->SetBottomMargin(0.125);   

// c1->SetGrid();
   //  std::vector<int> runNumbers;
   std::vector<std::string> runNumber;
   std::vector<std::string> resultFile;
   std::vector<double> pulseSize;
   /*
   pulseSize.push_back(10);
   pulseSize.push_back(9);
   //   pulseSize.push_back(8);
   pulseSize.push_back(7);
   pulseSize.push_back(6);
   //   pulseSize.push_back(5);
   pulseSize.push_back(4);
   pulseSize.push_back(3);
   pulseSize.push_back(2);
   //pulseSize.push_back(1);

   //runNumbers.push_back(1312065);
   //runNumbers.push_back(1312066);

   std::vector<std::string> runNumber;
   runNumber.push_back("1313046");
   runNumber.push_back("1313047");
   //runNumber.push_back("1313048");
   runNumber.push_back("1313049");
   runNumber.push_back("1313053");
   //runNumber.push_back("1313054");
   runNumber.push_back("1313057");
   runNumber.push_back("1313058");
   runNumber.push_back("1313060");
   // runNumber.push_back("1312082");
   
   for(int runi = 0; runi < runNumber.size(); runi ++){
     stringstream filePath;
     filePath << "/scratch0/hrichie/hptpc_ritchie/HPTPC/data/preampData/2018/fitted_Spectra/AmplitudeAnode2parameters_raptorr_tpcdata_gain_R" << runNumber[runi] <<  "_expSm\
ooth.root.dat";
     resultFile.push_back(filePath.str());
   }
  */




   //   resultFile.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/code/waveformData/testData/peakschi_raptorr_tpcdata_gain_R1253048_expSmooth05.root.dat");
   //resultFile.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/code/waveformData/testData/peakschi_raptorr_tpcdata_gain_R1253048_expSmooth05.root.dat");

   // data storage
   // ... more vectors
   std::vector<double> Peaks1;
   std::vector<double> peakErrors1;
   std::vector<double> Chis1;
   std::vector<double> Ndfs1;
   std::vector<double> ChisNdfs4;
   std::vector<double> scaledpeakErrors1;

   //Get Pulse + run Number from File
   //std::string runsFile = "preamp2CR112Runs.dat";
   std::stringstream runsFileSS;
   std::string chipNo = "CR113";
   runsFileSS << "preamp" << preampNo << chipNo << "Runs.dat";
   std::string runsFile = runsFileSS.str(); 
   std::ifstream file2;
   file2.open(runsFile.c_str());
   std::string fileLine;
   std::vector<std::string> fileData;
   if(file2.is_open()){
     std::cout << runsFile << " is open" << std::endl;
     std::string runNo, pulseNo;
     while (std::getline(file2, fileLine, ' ')){  
       if(fileLine.size() > 0){
	 std::cout << "fileLine = "  << fileLine << std::endl;
	 fileData.push_back(fileLine);
       }
       else{
	 std::cout << "why have you done this" << std::endl;
	 break;
       }
     
     }
     for(int runGet = 0; runGet < fileData.size(); runGet++){
       std::cout << "filedata run get ="  << fileData[runGet] << std::endl;
       if(fileData[runGet].size() > 3){
	 runNumber.push_back(fileData[runGet]);
       }
       else{
	double pulseDoub = 0.0;
	std::stringstream ss;
	ss << fileData[runGet];
	ss >> pulseDoub;
	std::cout << pulseDoub << std::endl;
	pulseSize.push_back(pulseDoub);
       }
         //fileLine >> runNo >> pulseNo;
         //std::cout << runNo << " " << pulseNo << std::endl;
         //runNumber.push_back(runNo);
         //pulseSize.push_back(pulseNo);          
     }
   }
   else {std::cout << "Cannot open runs file" << runsFile << std::endl;}


   for(int runi = 0; runi < runNumber.size(); runi ++){
     stringstream filePath;
     filePath << "/scratch0/hrichie/hptpc_ritchie/HPTPC/data/preampData/2018/fitted_Spectra/AmplitudeAnode" << preampNo << "peakschi_raptorr_tpcdata_gain_R" << runNumber[runi] <<  "_expSmooth.root.dat";
     resultFile.push_back(filePath.str());
   }


   
   //Get peak parameters from file
   for(int iResults = 0; iResults < resultFile.size(); iResults++){
     std::ifstream file;
     file.open(resultFile[iResults].c_str());
     /*if(file.is_open()){
       double peak1, peak1err, chi1, ndf1;
       file >> peak1 >> peak1err >> chi1 >> ndf1;
       std::cout << peak1 << " " << peak1err << " " << chi1 << " " << ndf1 << std::endl;
       Peaks1.push_back(peak1);
       peakErrors1.push_back(peak1err/10);
       Chis1.push_back(chi1);
       Ndfs1.push_back(ndf1);
     }*/

     if(file.is_open()){
       double peak1, peak1err, chi1;
       file >> peak1 >> peak1err >> chi1;
       std::cout << peak1 << " " << peak1err << " " << chi1 << std::endl;
       Peaks1.push_back(peak1);
       peakErrors1.push_back(peak1err);
       Chis1.push_back(chi1);
       // Ndfs1.push_back(ndf1);
     }

     else{ std::cout << "Cannot open peak file "<< resultFile[iResults] << std::endl;}
   }
  
   
   //Scale Errors by Chisquare/NDF
   for(int chisub1 = 0; chisub1 < Chis1.size(); chisub1++){
     for(int ndfsub1 = 0; ndfsub1 < Ndfs1.size(); ndfsub1++){
       if(chisub1 == ndfsub1){
	 if(!Ndfs1[ndfsub1] > 0){
	   std::cout << "ndf is bad :(" << std::endl;
	   ChisNdfs4.push_back(Chis1[chisub1]); 
	 }
	 else{	   
	   ChisNdfs4.push_back(Chis1[chisub1] / Ndfs1[chisub1]); 
	 }
       }
     }
   }
   for(int errorsub1a = 0; errorsub1a < peakErrors1.size(); errorsub1a++){
     for(int errorsub1b = 0; errorsub1b < ChisNdfs4.size(); errorsub1b++){
       if(errorsub1a == errorsub1b){
	 scaledpeakErrors1.push_back(peakErrors1[errorsub1a] * ChisNdfs4[errorsub1b]); 
       }
     }
   }

   // create the TGraph, draw it, and save it

   // gr = new TGraphErrors(2,&(pulseSize[0]),&(Peaks1[0]),0,&(peakErrors1[0]));
   
   //   hpx = new TH2F("hpx","Zoomed Graph Test",10,0,10,10,0.0,80.0);
   //hpx->Draw();

   int n = pulseSize.size();

   gr = new TGraphErrors(n,&(pulseSize[0]),&(Peaks1[0]),0,&(peakErrors1[0]));


   //   stringstream plotname;
   //   plotname << "Preamp Calibration Anode 2";
   std::stringstream plotTitle;
   plotTitle << "Preamp Calibration Anode " << preampNo << " " << chipNo;
   gr->SetTitle(plotTitle.str().c_str());
   //   gr->SetMarkerColor(4);
   // gr->SetMarkerStyle(21);
   //stringstream xaxisname;
   //xaxisname << "Input Pulse" << " (V)";

   gr->SetLineColor(38);
   gr->SetMarkerColor(30);
   gr->SetMarkerStyle(33);
 
   gr->Fit("pol1");
  
   //   gr->SetTitle(plotname.str().c_str());

   gr->GetYaxis()->SetTitleSize(0.045);
   gr->GetYaxis()->SetTitleOffset(0.95);
   gr->GetXaxis()->SetTitleSize(0.045);
   gr->GetXaxis()->SetTitleOffset(0.85);
  
   gr->GetXaxis()->SetTitle("Input Pulse (V)");
   gr->GetYaxis()->SetTitle("Peak Position (mV)");



   gr->Draw("AP");
   
   std::stringstream plotName;
   plotName << "preampPeakPositionVsInputPulsePreamp" << preampNo << chipNo << ".png"; 
   c1->Print(plotName.str().c_str());
   c1->Update();
}

