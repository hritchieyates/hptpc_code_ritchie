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

void plotPeakPositionPreampAdam(std::string plottoplot= "noplot"){
   TCanvas* c1 = new TCanvas("c1","Plots for peak position",200,10,500,500);
   // c1->SetGrid();
   std::vector<int> runNumbers;
   std::vector<std::string> resultFile;
   std::vector<double> pulseSize;

   pulseSize.push_back(7);
   pulseSize.push_back(8);

   runNumbers.push_back(1312065);
   runNumbers.push_back(1312066);

   resultFile.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/code/waveformData/testData/peakschi_raptorr_tpcdata_gain_R1253048_expSmooth05.root.dat");
   resultFile.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/code/waveformData/testData/peakschi_raptorr_tpcdata_gain_R1253048_expSmooth05.root.dat");

   // data storage
   // ... more vectors
   std::vector<double> Peaks1;
   std::vector<double> peakErrors1;
   std::vector<double> Chis1;
   std::vector<double> Ndfs1;
   std::vector<double> ChisNdfs4;
   std::vector<double> scaledpeakErrors1;
   
   //Get peak parameters from file
   for(int iResults = 0; iResults < resultFile.size(); iResults++){
     std::ifstream file;
     file.open(resultFile[iResults].c_str());
     if(file.is_open()){
       double peak1, peak1err, chi1, ndf1;
       file >> peak1 >> peak1err >> chi1 >> ndf1;
       std::cout << peak1 << " " << peak1err << " " << chi1 << " " << ndf1 << std::endl;
       Peaks1.push_back(peak1);
       peakErrors1.push_back(peak1err);
       Chis1.push_back(chi1);
       Ndfs1.push_back(ndf1);
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
   
   gr = new TGraphErrors(2,&(pulseSize[0]),&(Peaks1[0]),0,&(peakErrors1[0]));


   stringstream plotname;
   plotname << "Peak Position vs " << plottoplot;
   gr->SetTitle(plotname.str().c_str());
   gr->SetMarkerColor(4);
   gr->SetMarkerStyle(21);
   stringstream xaxisname;
   xaxisname << plottoplot << " (V)";
 
   gr->Fit("pol1");
  
   gr->SetTitle(plotname.str().c_str());
  
   gr->GetXaxis()->SetTitle(xaxisname.str().c_str());
   gr->GetYaxis()->SetTitle("Peak Position (mV)");
   gr->Draw("AP");
   c1->Print(("preamppeakpositionvs"+plottoplot+".png").c_str());
   c1->Update();
}

