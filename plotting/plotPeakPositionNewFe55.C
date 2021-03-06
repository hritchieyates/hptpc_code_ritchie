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

void plotpeakposition12(std::string plottoplot= "noplot", bool gain = false){
   TCanvas* c1 = new TCanvas("c1","Plots for peak position",200,10,500,500);
   // c1->SetGrid();
   std::vector<int> runNumbers;
   std::vector<std::string> resultFile;
   std::vector<std::string> paramsFile;
   std::vector<std::string> peaksFile;
   // some function should generate the list...
   

  runNumbers.push_back(1339001);
  runNumbers.push_back(1339022);
  runNumbers.push_back(1339044);
  runNumbers.push_back(1339064);
  runNumbers.push_back(1340121);
  runNumbers.push_back(1340144);
  runNumbers.push_back(1340169);
  runNumbers.push_back(1341011);
 
  //nameStream << "/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/scripts/bkgVsSignal/comparisonPlots/GroupedRuns_" << file \
  //NameSuffix << "_";
  //nameStream >> fileNameFlag;
  std::ifstream listFile;
  /*  std::stringstream resultsSS = "";
  std::stringstream paramsSS = "";
  std::stringstream peaksSS = "";
  std::string resultsString = "";
  std::string paramsString = "";
  std::string peaksString = "";*/
  listFile.open("/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/scripts/bkgVsSignal/runsToPlotSglNewTo");
  std::string fileLine;
  if(listFile.is_open()){
    std::cout << "listFile is open" << std:: endl;
    while (std::getline(listFile, fileLine)){
      std::stringstream resultsSS = "";
      std::stringstream paramsSS = "";
      std::stringstream peaksSS = "";
      std::string resultsString = "";
      std::string paramsString = "";
      std::string peaksString = "";
      std::cout << "Get Line is " << fileLine << std::endl;
      resultsSS << "/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/peakschi_" << fileLine << "_raptorr_tpcdata_gain_PeakTime2_true.root.dat";
      paramsSS << "/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/parameters_" << fileLine << "_raptorr_tpcdata_gain_PeakTime2_true.root.dat"; 
      peaksSS << "/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/peaks_" << fileLine << "_raptorr_tpcdata_gain_PeakTime2_true.root.dat"; 
      resultsSS >> resultsString;
      paramsSS >> paramsString;
      peaksSS >> peaksString;
      resultFile.push_back(resultsString);
      paramsFile.push_back(paramsString);
      peaksFile.push_back(peaksString);
    }
  }







   /*
   
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1328009-to-R1328063_waveformAnalysis_gain_bintest.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1328124-to-R1328134_waveformAnalysis_gain_bintest.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1329011-to-R1329024_waveformAnalysis_gain_bintest.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1330016-to-R1330054_waveformAnalysis_gain_bintest.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1330100-to-R1330109_waveformAnalysis_gain_bintest.root.dat");
      
   paramsFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/parameters_R1328009-to-R1328063_waveformAnalysis_gain_bintest.root.dat");
   paramsFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/parameters_R1328124-to-R1328134_waveformAnalysis_gain_bintest.root.dat");
   paramsFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/parameters_R1329011-to-R1329024_waveformAnalysis_gain_bintest.root.dat");
   paramsFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/parameters_R1330016-to-R1330054_waveformAnalysis_gain_bintest.root.dat");
   paramsFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/parameters_R1330100-to-R1330109_waveformAnalysis_gain_bintest.root.dat");
   */

   // data storage
   std::vector<double> cVoltage;
   std::vector<double> a1Voltage;
   std::vector<double> a2Voltage;
   std::vector<double> a3Voltage;
   std::vector<double> a3a2Voltage;
   std::vector<double> a3a1Voltage;
   std::vector<double> a2a1Voltage;
   std::vector<double> suma2a1Voltage;
   std::vector<double> sumVoltage;
   std::vector<double> pressure;
   std::vector<double> absPressure;
   std::vector<double> a1VP;
   std::vector<double> a1VPtest;
   std::vector<double> a2VP;
   std::vector<double> a3VP;
   std::vector<double> a3a2VP;
   std::vector<double> a3a1VP;
   std::vector<double> a2a1VP;
   std::vector<double> sumVP;
   std::vector<double> cVP;
   std::vector<std::vector<double> > vectorVP;
   std::vector<std::vector<double> > vectorVoltage;

   // ... more vectors
   std::vector<double> Peaks1;
   std::vector<double> Peaks2;
   std::vector<double> Peaks3;
   std::vector<double> Peaks4;
   std::vector<double> peakErrors1;
   std::vector<double> peakErrors2;
   std::vector<double> peakErrors3;
   std::vector<double> peakErrors4;
   std::vector<double> Chis1;
   std::vector<double> Chis2;
   std::vector<double> Chis3;
   std::vector<double> Chis4;
   std::vector<double> Ndfs1;
   std::vector<double> Ndfs2;
   std::vector<double> Ndfs3;
   std::vector<double> Ndfs4;
   std::vector<double> ChisNdfs4;
   std::vector<double> scaledpeakErrors1;
   std::vector<double> scaledpeakErrors2;
   std::vector<double> scaledpeakErrors3;
   std::vector<double> scaledpeakErrors4;
   std::vector<double> peakWidth1;
   std::vector<double> peakWidth2;
   std::vector<double> peakWidth3;
   std::vector<double> peakWidth4;
   std::vector<double> scaledpeakWidth1;
   std::vector<double> scaledpeakWidth2;
   std::vector<double> scaledpeakWidth3;
   std::vector<double> scaledpeakWidth4;


   // ... more vectors

   // list of information to be extracted from the XML data base
   TString mysqlFieldsToQuery[13] = {"run_number",      "start_time",      "end_time",        "mean_cathode_V",  "mean_anode1_V",   "mean_anode2_V",   "mean_anode3_V",   "mean_pressure",   "anode1_sparks",  "anode2_sparks",   "anode3_sparks",        "sparking_good",   "reasonable_voltages"};
 
   for(int run = 0; run < runNumbers.size(); run++){
     // connect to a mysql server
     TSQLServer *xmlInformationDatabase = TSQLServer::Connect("mysql://dmdbserv.pp.rhul.ac.uk/GOOD_RUNS", "dmatter", "seedark");
     if( xmlInformationDatabase->IsError() ) { std::cout << "There is an Error with the Database" << std::endl; }
 
     // we construct the xml query which will give us all the information we need for the particular run
     TString mysql_query = "SELECT ";
     for(Int_t iBuildFullQuerry = 0; iBuildFullQuerry < 13; iBuildFullQuerry++){
       if(iBuildFullQuerry !=0) {mysql_query+=", "+mysqlFieldsToQuery[iBuildFullQuerry];}
       else{ mysql_query+=" "+mysqlFieldsToQuery[iBuildFullQuerry]; }
     }
     TString runNumberString;
     runNumberString.Form("%i", runNumbers.at(run));
     mysql_query+=" FROM run_info WHERE start_time>='2019-01-01 00:00:00' AND run_number="+runNumberString;
     //Now execute the query and put it in a TSQLResult object 
     TSQLResult *mysql_result = xmlInformationDatabase->Query(mysql_query);
     if (!mysql_result) { 
       std::cout << "Error when Querying Database" << std::endl;
       std::cout << "You probaly don't want this runs data " << std::endl;
       continue;
     }

     // the result is one row of data (in case nothing went wrong)
     if(mysql_result->GetRowCount() != 1) {
       if(mysql_result->GetRowCount() == 0) { 
	 std::cout << "Error, no xml information found for run " << runNumberString << std::endl;
	 // abort
	 continue;
       }
     }
     
     TSQLRow *mysql_row = mysql_result->Next();

     std::cout << "C \t"  <<  atof(mysql_row->GetField(3)) << std::endl;
     std::cout << "A1\t" <<  atof(mysql_row->GetField(4)) << std::endl;
     std::cout << "A2\t" <<  atof(mysql_row->GetField(5)) << std::endl;
     std::cout << "A3\t" <<  atof(mysql_row->GetField(6)) << std::endl;
     std::cout << "Pressure \t" << atof(mysql_row->GetField(7)) << std::endl;
     cVoltage.push_back(atof(mysql_row->GetField(3)));
     a1Voltage.push_back(atof(mysql_row->GetField(4)));
     a2Voltage.push_back(atof(mysql_row->GetField(5)));
     a3Voltage.push_back(atof(mysql_row->GetField(6)));
     pressure.push_back(atof(mysql_row->GetField(7)));
  }

   //Do some mathsy things with the voltages
   for(int voltsuba = 0; voltsuba < a3Voltage.size(); voltsuba++){
     for(int voltsubb = 0; voltsubb < a2Voltage.size(); voltsubb++){
       if(voltsuba == voltsubb){
	 a3a2Voltage.push_back(a3Voltage[voltsuba] - a2Voltage[voltsuba]);	 
       }
     }
   }

   for(int voltsubc = 0; voltsubc < a3Voltage.size(); voltsubc++){
     for(int voltsubd = 0; voltsubd < a1Voltage.size(); voltsubd++){
       if(voltsubc == voltsubd){
	 a3a1Voltage.push_back(a3Voltage[voltsubc] - a1Voltage[voltsubc]); 
       }
     }
   }

   for(int voltsube = 0; voltsube < a2Voltage.size(); voltsube++){
     for(int voltsubf = 0; voltsubf < a1Voltage.size(); voltsubf++){
       if(voltsube == voltsubf){
	 a2a1Voltage.push_back(a2Voltage[voltsube] - a1Voltage[voltsube]); 
       }
     }
   }

   for(int voltsuma = 0; voltsuma < a2Voltage.size(); voltsuma++){
     for(int voltsumb = 0; voltsumb < a1Voltage.size(); voltsumb++){
       if(voltsuma == voltsumb){
	 suma2a1Voltage.push_back(a2Voltage[voltsuma] + a1Voltage[voltsumb]); 
       }
     }
   }
   for(int voltsumc = 0; voltsumc < suma2a1Voltage.size(); voltsumc++){
     for(int voltsumd = 0; voltsumd < a3Voltage.size(); voltsumd++){
       if(voltsumc == voltsumd){
	 sumVoltage.push_back(suma2a1Voltage[voltsumc] + a3Voltage[voltsumd]); 
       }
     }
   }
   
   //Test of V/P for just one voltage
   /*
   for(int a1vpib = 0; a1vpib < a1Voltage.size(); a1vpib++){
     for(int a1vpjb = 0; a1vpjb < pressure.size(); a1vpjb++){
       if(a1vpib == a1vpjb){
	 a1VPtest.push_back(a1Voltage[a1vpib] / pressure[a1vpib]); 
       }
     }
   }
   */

   //Pressure to absolute pressure (assuming pRoon = 1 until we get proper pressure measurements)
   for(int presi = 0; presi < pressure.size(); presi++){
     if(pressure[presi] > 0){
       absPressure.push_back(pressure[presi] + 1);
     }
     else{
       absPressure.push_back(pressure[presi - 1] + 1);
     }
   }
   
   //Divide voltage by pressure
   vectorVoltage.push_back(a1Voltage);
   vectorVoltage.push_back(a2Voltage);
   vectorVoltage.push_back(a3Voltage);
   vectorVoltage.push_back(a3a2Voltage);
   vectorVoltage.push_back(a3a1Voltage);
   vectorVoltage.push_back(a2a1Voltage);
   vectorVoltage.push_back(sumVoltage);
   vectorVoltage.push_back(cVoltage);
   vectorVP.push_back(a1VP);
   vectorVP.push_back(a2VP);
   vectorVP.push_back(a3VP);
   vectorVP.push_back(a3a2VP);
   vectorVP.push_back(a3a1VP);
   vectorVP.push_back(a2a1VP);
   vectorVP.push_back(sumVP);
   vectorVP.push_back(cVP);
   for(int VPi = 0; VPi < vectorVP.size(); VPi++){
     for(int a1vpi = 0; a1vpi < vectorVoltage[VPi].size(); a1vpi++){
       for(int a1vpj = 0; a1vpj < absPressure.size(); a1vpj++){
	 if(a1vpi == a1vpj){
	   vectorVP[VPi].push_back(vectorVoltage[VPi][a1vpi] / absPressure[a1vpi]);
	 }
       }
     }
   }
    

   //Get peak parameters from file
   for(int iResults = 0; iResults < resultFile.size(); iResults++){
     std::cout << "File to get peak from is " << resultFile[iResults] << std::endl;
     std::ifstream file;
     file.open(resultFile[iResults].c_str());
     if(file.is_open()){
       double peak2, peak2err, chi1, ndf1;
       file >> peak2 >> peak2err >> chi1 >> ndf1;
       std::cout << "Peak Position in mV is " << peak2 << std::endl;
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
     peakWidth2.push_back(abs(peak23));
     }
     else{ std::cout << "Cannot open params file" << resultFile[iParams] << std::endl;}
   }

   //Scale Errors by Chisquare/NDF
   for(int chisub1 = 0; chisub1 < Chis4.size(); chisub1++){
     for(int ndfsub1 = 0; ndfsub1 < Ndfs4.size(); ndfsub1++){
       if(chisub1 == ndfsub1){
	 if(!Ndfs4[ndfsub1] > 0){
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
	 scaledpeakWidth2.push_back(peakWidth2[errorsub2a] * ChisNdfs4[errorsub2b]);
       }
     }
   }
 

   //Constants for gain measurement
   std::vector<double> Gains3;
   std::vector<double> Gains2;
   std::vector<double> Gains4;
   std::vector<double> Gains1;
   std::vector<double> gainErrors3;
   std::vector<double> gainErrors2;
   std::vector<double> gainErrors4;
   std::vector<double> gainErrors1;
   std::vector<double> gainWidth1;
   std::vector<double> gainWidth2;
   std::vector<double> gainWidth3;
   std::vector<double> gainWidth4;
   Double_t gPreamp = 12e12;
   Double_t eDep512 = 512e3;
   Double_t eDep661 = 612e3;
   Double_t eDep1174 = 1174e3;
   Double_t eDep60 = 60e3;
   Double_t workFunc = 27.4;
   Double_t qE = 1.6e-19;
   Double_t gCalcConst = gPreamp * qE / workFunc;
 
   //Turn peaks into gain
    for(int gcalcj = 0; gcalcj < Peaks2.size(); gcalcj++){
     Gains2.push_back(Peaks2[gcalcj] / (gCalcConst * eDep512));
     gainErrors2.push_back(scaledpeakErrors2[gcalcj] / (gCalcConst * eDep512));
     gainWidth2.push_back(scaledpeakWidth2[gcalcj] / (gCalcConst * eDep512));
    }

    std::vector<double> xplot;
    /*
    std::vector<double> yplot4;
    std::vector<double> yplot4err;
    std::vector<double> yplot3;
    std::vector<double> yplot3err;
    std::vector<double> yplot2;
    std::vector<double> yplot2err;
    std::vector<double> yplot1;
    std::vector<double> yplot1err;

    yplot4 = Peaks4;
    yplot4err = peakErrors4;
    yplot3 = Peaks3; 
    yplot3err = peakErrors3;
    yplot2 = Peaks2;
    yplot2err = peakErrors2;
    yplot1 = Peaks1;
    yplot1err = peakErrors1;
    */
   
    //Choose which graph to plot based on input
    if(plottoplot == "Anode1"){
      xplot = a1Voltage;
    }
    if(plottoplot == "Anode2"){
      xplot = a2Voltage;
    }
    if(plottoplot == "Anode3"){
      xplot = a3Voltage;
    }

    if(plottoplot == "Anode3minusAnode1"){
      xplot = a3a1Voltage;
    }
    if(plottoplot == "Anode3minusAnode2"){
      xplot = a3a2Voltage;
    }
    if(plottoplot == "Anode2minusAnode1"){
      xplot = a2a1Voltage;
    }

    if(plottoplot == "Cathode"){
      xplot = cVoltage;
    }
    if(plottoplot == "Pressure"){
      xplot = pressure;
    }
    if(plottoplot == "AnodeSum"){
      xplot = sumVoltage;
    }
    
    bool isVP = false;

    if(plottoplot == "Anode1VP"){
      xplot = vectorVP[0];
      isVP = true;
    }
    if(plottoplot == "Anode2VP"){
      xplot = vectorVP[1];
      isVP = true;
    }
    if(plottoplot == "Anode3VP"){
      xplot = vectorVP[2];
      isVP = true;
    }
    if(plottoplot == "Anode3minusAnode2VP"){
      xplot = vectorVP[3];
      isVP = true;
    }
    if(plottoplot == "Anode3minusAnode1VP"){
      xplot = vectorVP[4];
      isVP = true;
    }
    if(plottoplot == "Anode2minusAnode1VP"){
      xplot = vectorVP[5];
      isVP = true;
    }
    if(plottoplot == "AnodeSumVP"){
      xplot = vectorVP[6];
      isVP = true;
    }
    if(plottoplot == "CathodeVP"){
      xplot = vectorVP[7];
      isVP = true;
    }

 
   // create the TGraph, draw it, and save it

    TGraphErrors* gr;
    TGraphErrors* gr2;
    TGraphErrors* gr4;
    TGraphErrors* gr1;

    /* 
   if(gain == false){
     gr = new TGraphErrors(11,&(xplot[0]),&(Peaks3[0]),0,&(scaledpeakErrors3[0]));
     gr2 = new TGraphErrors(11,&(xplot[0]),&(Peaks2[0]),0,&(scaledpeakErrors2[0]));
     gr4 = new TGraphErrors(11,&(xplot[0]),&(Peaks4[0]),0,&(scaledpeakErrors4[0]));
     gr1 = new TGraphErrors(11,&(xplot[0]),&(Peaks1[0]),0,&(scaledpeakErrors1[0]));    
   }
    */
   /* 
   if(gain == false){
     gr = new TGraphErrors(11,&(xplot[0]),&(Peaks3[0]),0,&(peakWidth3[0]));
     gr2 = new TGraphErrors(11,&(xplot[0]),&(Peaks2[0]),0,&(peakWidth2[0]));
     gr4 = new TGraphErrors(11,&(xplot[0]),&(Peaks4[0]),0,&(peakWidth4[0]));
     gr1 = new TGraphErrors(11,&(xplot[0]),&(Peaks1[0]),0,&(peakWidth1[0]));
   }
   */
   
   if(gain == false){
     peakGraph2 = new TGraphErrors(5,&(xplot[0]),&(Peaks2[0]),0,&(scaledpeakWidth2[0]));
   }
   
   /*       
   if(gain == true){
     peakGraph2 = new TGraphErrors(5,&(xplot[0]),&(Gains2[0]),0,&(gainErrors2[0]));
   }
   */

   if(gain == true){
     peakGraph2 = new TGraphErrors(5,&(xplot[0]),&(Gains2[0]),0,&(gainWidth2[0]));
   }
   
   
   stringstream plotname;
   if(gain == false){
     plotname << "Peak Position vs " << plottoplot;
   }
   if(gain == true){
     plotname << "Gain vs " << plottoplot;
   }  
   peakGraph2->SetTitle(plotname.str().c_str());
   peakGraph2->SetMarkerColor(4);
   peakGraph2->SetMarkerStyle(21);
   
   stringstream xaxisname;
   if(isVP == true){
   xaxisname << plottoplot << " (V/barA)";
   }
   if(isVP == false){
   xaxisname << plottoplot << " (V)";
   }
   //gr2->Fit("pol1");
   TMultiGraph* mg = new TMultiGraph();
   //mg->Add(gr, "P");
   //mg->Add(gr2, "P");
   //mg->Add(gr4, "P");
   mg->Add(peakGraph2, "P");
   mg->SetTitle(plotname.str().c_str());
   mg->Draw("A");
   mg->GetXaxis()->SetTitle(xaxisname.str().c_str());
   int noRuns = runNumbers.size();
   ostringstream plottitle;
   plottitle <<  runNumbers[0] << "_to_" << runNumbers[noRuns - 1];
   //gr->Draw("AP");
   if(gain == false){
   mg->GetYaxis()->SetTitle("Peak Position (mV)");
   c1->Print(("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData"+plottitle.str()+"_peakmV_"+plottoplot+".png").c_str());
   }
   if(gain == true){ 
   mg->GetYaxis()->SetTitle("Gain");
   c1->Print(("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData"+plottitle.str()+"_peakGain_"+plottoplot+".png").c_str());
   }
   c1->Update();
}

