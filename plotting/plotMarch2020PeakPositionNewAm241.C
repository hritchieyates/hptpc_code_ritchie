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

void plotPeakPositionNewAm241(std::string Scheme= "noScheme", bool gain = false){
   TCanvas* c1 = new TCanvas("c1","Plots for peak position",200,10,500,500);
   // c1->SetGrid();
   std::vector<int> runNumbers;
   std::vector<std::string> resultFile;
   std::vector<std::string> paramsFile;
   std::vector<std::string> peaksFile;

  std::ifstream listFile;

  //SchemeBModSpecialTrigAll
  /*resultFile.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/peakschifirst_raptorr_tpcdata_gain_R2084009-R2084018_true_TEST_Am241.root.dat");
  resultFile.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/peakschifirst_raptorr_tpcdata_gain_R2083126-R2084008_Am241.root.dat");
  resultFile.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/peakschifirst_raptorr_tpcdata_gain_R2083116-R2083125_Am241.root.dat");
  paramsFile.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/parameters_raptorr_tpcdata_gain_R2084009-R2084018_true_TEST_Am241.root.dat");
  paramsFile.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/parameters_raptorr_tpcdata_gain_R2083126-R2084008_Am241.root.dat");
  paramsFile.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/parameters_raptorr_tpcdata_gain_R2083116-R2083125_Am241.root.dat");
  */

  std::cout << "Scheme is " << Scheme << std::endl;

  //SchemeBModSpecialTrigAll, SchemeBMod, SchemeC, SchemeA
  std::stringstream listFileNameSS;
  std::string listFileName;  
  listFileNameSS << "/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/scripts/bkgVsSignal/runsToPlot" << Scheme;
  listFileNameSS >> listFileName;
  std::cout << "listFileName is" << listFileName << std::endl;
  listFile.open(listFileName.c_str());

  //listFile.open("/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/scripts/bkgVsSignal/runsToPlotSchemeA");


  //SchemeA -> _true_Am241                                                                                                                                                                                 
  //SchemeC -> _Am241                                                                                                                                                                                      
  //SchemeB -> _true_Am242_AllTrig (and include the weird ones above)  
  std::string listFileSuffix;
  std::string listFilePrefix;
  if(Scheme == "SchemeA"){  
    listFileSuffix = "_true_Am241";
    listFilePrefix = "peakschi";
  }
  if(Scheme == "SchemeC"){
    listFileSuffix = "_Am241";
    listFilePrefix = "peakschi";
  }
  if(Scheme == "SchemeBModSpecialTrigAll"){
    listFileSuffix = "_true_Am241_AllTrig";
    listFilePrefix = "peakschifirst";
    resultFile.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/peakschifirst_raptorr_tpcdata_gain_R2084009-R2084018_true_TEST_Am241.root.dat");
    resultFile.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/peakschifirst_raptorr_tpcdata_gain_R2083126-R2084008_Am241.root.dat");
    resultFile.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/peakschifirst_raptorr_tpcdata_gain_R2083116-R2083125_Am241.root.dat");
    paramsFile.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/parameters_raptorr_tpcdata_gain_R2084009-R2084018_true_TEST_Am241.root.dat");
    paramsFile.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/parameters_raptorr_tpcdata_gain_R2083126-R2084008_Am241.root.dat");
    paramsFile.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/parameters_raptorr_tpcdata_gain_R2083116-R2083125_Am241.root.dat");
  }

  std::string fileLine;
  if(listFile.is_open()){
    std::cout << "listFile is open" << std:: endl;
    while (std::getline(listFile, fileLine)){
      std::stringstream resultsSS;
      std::stringstream paramsSS;
      std::stringstream peaksSS;
      std::string resultsString = "";
      std::string paramsString = "";
      std::string peaksString = "";
      std::cout << "Get Line is " << fileLine << std::endl;
      resultsSS << "/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/" << listFilePrefix << "_raptorr_tpcdata_gain_" << fileLine << listFileSuffix << ".root.dat";
      paramsSS << "/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/parameters_raptorr_tpcdata_gain_" << fileLine << listFileSuffix << ".root.dat"; 
      peaksSS << "/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/peaks_raptorr_tpcdata_gain_" << fileLine << listFileSuffix << ".root.dat"; 
      resultsSS >> resultsString;
      paramsSS >> paramsString;
      peaksSS >> peaksString;
      resultFile.push_back(resultsString);
      paramsFile.push_back(paramsString);
      peaksFile.push_back(peaksString);
    }
  }
  //SchemeA -> _true_Am241
  //SchemeC -> _Am241
  //SchemeB -> _true_Am242_AllTrig


   // data storage
   std::vector<double> schemeACustomVoltage;
   std::vector<double> schemeBCustomVoltage;
   std::vector<double> schemeCCustomVoltage;
   std::vector<double> customVoltage;
   // ... more vectors
   std::vector<double> Peaks1;
   std::vector<double> Peaks2;
   std::vector<double> peakErrors1;
   std::vector<double> peakErrors2;
   std::vector<double> Chis1;
   std::vector<double> Chis4;
   std::vector<double> Ndfs1;
   std::vector<double> Ndfs4;
   std::vector<double> ChisNdfs4;
   std::vector<double> scaledpeakErrors1;
   std::vector<double> scaledpeakErrors2;
   std::vector<double> peakWidth1;
   std::vector<double> peakWidth2;
   std::vector<double> scaledpeakWidth1;
   std::vector<double> scaledpeakWidth2;


   //Scheme C
   schemeCCustomVoltage.push_back(200);
   schemeCCustomVoltage.push_back(400);
   schemeCCustomVoltage.push_back(600);
   schemeCCustomVoltage.push_back(800);
   schemeCCustomVoltage.push_back(1000);
   schemeCCustomVoltage.push_back(1200);
   schemeCCustomVoltage.push_back(1400);
   schemeCCustomVoltage.push_back(1600);
   schemeCCustomVoltage.push_back(1800);
   

   //Scheme B
   schemeBCustomVoltage.push_back(0);
   schemeBCustomVoltage.push_back(400);
   schemeBCustomVoltage.push_back(800);
   schemeBCustomVoltage.push_back(1200);
   schemeBCustomVoltage.push_back(1400);
   schemeBCustomVoltage.push_back(1600);
   schemeBCustomVoltage.push_back(1800);
   schemeBCustomVoltage.push_back(2000);
   schemeBCustomVoltage.push_back(2200);
   schemeBCustomVoltage.push_back(2400);
   schemeBCustomVoltage.push_back(2600);
   

   //Scheme A
   schemeACustomVoltage.push_back(1200);
   schemeACustomVoltage.push_back(1400);
   schemeACustomVoltage.push_back(1600);
   schemeACustomVoltage.push_back(1800);
   schemeACustomVoltage.push_back(2000);
   


   //Get peak parameters from file
   for(int iResults = 0; iResults < resultFile.size(); iResults++){
     std::cout << "File to get peak from is " << resultFile[iResults] << std::endl;
     std::ifstream file;
     file.open(resultFile[iResults].c_str());
     if(file.is_open()){
       double peak2, peak2err, chi1, ndf1;
       file >> peak2 >> peak2err >> chi1 >> ndf1;
       std::cout << "Peak Position in mV is " << peak2 << " error is " << peak2err << std::endl;
       Peaks2.push_back(peak2);
       peakErrors2.push_back(peak2err);
       Chis4.push_back(chi1);
       Ndfs4.push_back(ndf1);
       }
     else{ std::cout << "Cannot open peak file" << resultFile[iResults] << std::endl;}
   }
   
   //Get peak parameters from file
   for(int iParams = 0; iParams < paramsFile.size(); iParams++){
     std::ifstream file2;
     file2.open(paramsFile[iParams].c_str());
     if(file2.is_open()){
     double bkgr11,bkgr12,peak11,peak12,peak13,peak21,peak22,peak23;
     file2 >> bkgr11 >> bkgr12 >> peak11 >> peak12 >> peak13 >> peak21 >> peak22 >> peak23;
     //peakWidth1.push_back(abs(peak13));
     std::cout << "Peak Position in mV is " << peak12 << " width is " << peak13 << std::endl;
     peakWidth2.push_back(abs(peak13));
     }
     else{ std::cout << "Cannot open params file" << paramsFile[iParams] << std::endl;}
   }

   //Scale Errors by Chisquare/NDF
   for(int chisub1 = 0; chisub1 < Chis4.size(); chisub1++){
     for(int ndfsub1 = 0; ndfsub1 < Ndfs4.size(); ndfsub1++){
       if(chisub1 == ndfsub1){
	 if(!(Ndfs4[ndfsub1] > 0)){
	   std::cout << "ndf is bad :(" << std::endl;
	   ChisNdfs4.push_back(1);
	 }
	 else{	   
	   ChisNdfs4.push_back(sqrt(Chis4[chisub1] / Ndfs4[chisub1])); 
	 }
       }
     }
   }
    for(int errorsub2a = 0; errorsub2a < peakErrors2.size(); errorsub2a++){
     for(int errorsub2b = 0; errorsub2b < ChisNdfs4.size(); errorsub2b++){
       if(errorsub2a == errorsub2b){
	 scaledpeakErrors2.push_back(peakErrors2[errorsub2a] * ChisNdfs4[errorsub2b]);
	 scaledpeakWidth2.push_back(peakWidth2[errorsub2a] / 2);
       }
     }
   }
 

   //Constants for gain measurement
   std::vector<double> Gains2;
   std::vector<double> Gains1;
   std::vector<double> gainErrors2;
   std::vector<double> gainErrors1;
   std::vector<double> gainWidth1;
   std::vector<double> gainWidth2;
   Double_t gPreamp = 12e12;
   Double_t eDep512 = 512e3;
   Double_t eDep661 = 612e3;
   Double_t eDep1174 = 1174e3;
   Double_t eDep60 = 60e3;
   Double_t workFunc = 27.4;
   Double_t qE = 1.6e-19;
   //   Double_t gCalcConst = gPreamp * qE / workFunc;
   Double_t gCalcConst = workFunc / (gPreamp * eDep60 * qE); 

   //Turn peaks into gain
    for(int gcalcj = 0; gcalcj < Peaks2.size(); gcalcj++){
     Gains2.push_back(Peaks2[gcalcj] * gCalcConst);
     gainErrors2.push_back(scaledpeakErrors2[gcalcj] * gCalcConst);
     gainWidth2.push_back(scaledpeakWidth2[gcalcj] * gCalcConst);
     std::cout << "Peak is " << Peaks2[gcalcj] << ", gain is " << Gains2[gcalcj] << std::endl;
    }

    std::vector<double> xplot;
    std::vector<double> yplot;
    std::vector<double> yploterrors;
    stringstream plotTitle;
    std::string xAxisName;
    std::cout << "Okay time to do the bad if bit" << std::endl;
    
    if(gain){
      yplot = Gains2;
      yploterrors = gainErrors2;
      plotTitle << "Gas Gain vs ";
    }
    else{
      yplot = Peaks2;
      yploterrors = scaledpeakErrors2;
      plotTitle << "Peak Postition vs ";
    }
   
    //Choose which graph to plot based on input
    if(Scheme == "SchemeA"){
      std::cout << "Scheme is still Schema A" << std::endl;
      xplot = schemeACustomVoltage;
      TGraphErrors* peakGraph2 = new TGraphErrors(5,&(xplot[0]),&(yplot[0]),0,&(yploterrors[0]));
      plotTitle << "Anode 1";
      xAxisName = "Anode1 Voltage (V)";
      peakGraph2->SetMarkerColor(3);
      peakGraph2->SetMarkerStyle(33);
      peakGraph2->SetLineColor(3);
    }    
    if(Scheme == "SchemeC"){
      xplot = schemeCCustomVoltage;
      TGraphErrors* peakGraph2 = new TGraphErrors(9,&(xplot[0]),&(yplot[0]),0,&(yploterrors[0]));
      plotTitle << "Anode 2 - Anode 1";    
      xAxisName = "Anode 2 - Anode 1 (V)";
      peakGraph2->SetMarkerColor(2);
      peakGraph2->SetMarkerStyle(33);
      peakGraph2->SetLineColor(2);
    }
    if(Scheme == "SchemeBMod" || Scheme == "SchemeBModSpecialTrigAll"){
      xplot = schemeBCustomVoltage;
      //TGraphErrors* peakGraph2 = new TGraphErrors(11,&(xplot[0]),&(Peaks2[0]),0,&(scaledpeakErrors2[0]));
      TGraphErrors* peakGraph2 = new TGraphErrors(11,&(xplot[0]),&(yplot[0]),0,&(yploterrors[0]));
      plotTitle << "Anode 3 - Anode 2";
      xAxisName = "Anode 3 - Anode 2 (V)";
      peakGraph2->SetMarkerColor(4);
      peakGraph2->SetMarkerStyle(33);
      peakGraph2->SetLineColor(4);
    }
    
    //    else{std::cout << "Looks like you've got some kind of a scheme problem" << std::endl;}

    std::cout << "You got past the bad if bit" << std::endl;

   // create the TGraph, draw it, and save it
    /*
   if(gain == false){
     TGraphErrors* peakGraph2 = new TGraphErrors(5,&(xplot[0]),&(Peaks2[0]),0,&(scaledpeakErrors2[0]));
     //TGraphErrors* peakGraph2 = new TGraphErrors(11,&(xplot[0]),&(Peaks2[0]),0,&(scaledpeakWidth2[0]));
   }

   if(gain == true){
     TGraphErrors* peakGraph2 = new TGraphErrors(5,&(xplot[0]),&(Gains2[0]),0,&(gainWidth2[0]));
   }
    */

    //GainString
    std::string GainString;
    if(gain){GainString = "Gain";}
    else{GainString = "Anode3PeakmV";}

    std::string SchemeName;
    if(Scheme == "SchemeA"){SchemeName = "SchemeA";}
    if(Scheme == "SchemeC"){SchemeName = "SchemeC";}
    if(Scheme == "SchemeBModSpecialTrigAll"){SchemeName = "SchemeB";}

    //Save Plot values to files for use in LightVsCharge.cxx
   for(int iZack = 0; iZack < 5; iZack++){
     std::stringstream zackFileName;
     std::stringstream voltageForName;
     voltageForName << xplot[iZack];
     zackFileName << "/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/fitData" << SchemeName << "_" << GainString << voltageForName.str() << ".dat";
     std::ofstream zackFile;
     zackFile.open(zackFileName.str().c_str());
     zackFile << yplot[iZack];
     zackFile << "\t";
     zackFile << yploterrors[iZack];
     zackFile.close();
     std::cout << zackFileName.str() <<  " created" << std::endl;
   }

      
   /*   stringstream plotname;
   if(gain == false){
     plotname << "Peak Position vs Anode 1";
   }
   if(gain == true){
     plotname << "Gain vs " << plottoplot;
   } 
   */
 
   peakGraph2->SetTitle(plotTitle.str().c_str());
   peakGraph2->SetTitle(plotTitle.str().c_str());
   peakGraph2->GetYaxis()->SetTitleSize(0.045);
   peakGraph2->GetYaxis()->SetTitleOffset(0.95);
   peakGraph2->GetXaxis()->SetTitleSize(0.045);
   peakGraph2->GetXaxis()->SetTitleOffset(0.85);
   peakGraph2.Draw("AP");
   //peakGraph2->GetXaxis()->SetTitle("Anode1 (V)");   
   peakGraph2->GetXaxis()->SetTitle(xAxisName.c_str());

   //gr2->Fit("pol1");
   //   TMultiGraph* mg = new TMultiGraph();
   //mg->Add(gr, "P");
   //mg->Add(gr2, "P");
   //mg->Add(gr4, "P");
   // mg->Add(peakGraph2, "P");
   //mg->Draw("A");


   
   //   int noRuns = runNumbers.size();
   //plottitle <<  runNumbers[0] << "_to_" << runNumbers[noRuns - 1];


   if(gain == false){
   peakGraph2->GetYaxis()->SetTitle("Anode 3 Peak Position (mV)");
   c1->Print(("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/"+SchemeName+"_Anode3PeakmV_"+"Custom"+".pdf").c_str());
   }

   if(gain == true){ 
   peakGraph2->GetYaxis()->SetTitle("Gas Gain");
   c1->Print(("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/"+SchemeName+"_peakGain_"+"Custom"+".pdf").c_str());
   }

   c1->Update();
}

