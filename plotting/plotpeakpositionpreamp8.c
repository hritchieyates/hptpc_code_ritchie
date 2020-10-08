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

void plotpeakpositionpreamp8(std::string plottoplot= "noplot"){
   TCanvas* c1 = new TCanvas("c1","Plots for peak position",200,10,500,500);
   // c1->SetGrid();
   std::vector<int> runNumbers;
   std::vector<std::string> resultFile;

   // some function should generate the list...
   


   /*
   runNumbers.push_back(1336094);
   runNumbers.push_back(1337009);
   runNumbers.push_back(1337052);
   //runNumbers.push_back(1338001);
   //runNumbers.push_back(1338085);
   runNumbers.push_back(1339015);
   
   runNumbers.push_back(1349104);
   runNumbers.push_back(1349110);
   runNumbers.push_back(1349116);
   runNumbers.push_back(1349152);
   runNumbers.push_back(1350028);
   
   
   // some function should generate this list...
   
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peaks_R1336094-to-R1336108_waveformAnalysis_gain_all.root.dat");  
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peaks_R1337009-to-R1337034_waveformAnalysis_gain_all.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peaks_R1337052-to-R1337061_waveformAnalysis_gain_all.root.dat");
   // resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peaks_R1338001-to-R1338082_waveformAnalysis_gain_all.root.dat");
   //resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peaks_R1338085-to-R1338109_waveformAnalysis_gain_all.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peaks_R1339015-to-R1339042_waveformAnalysis_gain_all.root.dat");
   
   
   
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1349104-to-R1349107_waveformAnalysis_gain_clean.root.dat");  
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1349110-to-R1349113_waveformAnalysis_gain_clean.root.dat");  
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1349116-to-R1349121_waveformAnalysis_gain_clean.root.dat");  
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1349152-to-R1350015_waveformAnalysis_gain_clean.root.dat");  
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1350028-to-R1350037_waveformAnalysis_gain_clean.root.dat");  
   

   
   runNumbers.push_back(1351065);
   runNumbers.push_back(1351001);
   runNumbers.push_back(1351080);
   runNumbers.push_back(1351094);
   runNumbers.push_back(1351113);
   runNumbers.push_back(1352027);
   runNumbers.push_back(1353009);

   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1351065-to-R1351075_waveformAnalysis_gain_all.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1351001-to-R1351059_waveformAnalysis_gain_clean.root.dat");  
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1351080-to-R1351089_waveformAnalysis_gain_all.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1351094-to-R1351110_waveformAnalysis_gain_all.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1351113-to-R1351125_waveformAnalysis_gain_all.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1352049-to-R1352064_waveformAnalysis_gain_clean.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1353009-to-R1353019_waveformAnalysis_gain_clean_fixed.root.dat");
   
   */
   
   /*
   //Leaky 5bar data
   runNumbers.push_back(1341085);
   runNumbers.push_back(1342001);
   runNumbers.push_back(1342012);
   runNumbers.push_back(1342028);
   runNumbers.push_back(1342055);
   runNumbers.push_back(1342127);
   runNumbers.push_back(1342154);

   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1341065-to-R1341087_waveformAnalysis_gain_all.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1342001-to-R1342006_waveformAnalysis_gain_all.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1342012-to-R1342026_waveformAnalysis_gain_clean_fixed.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1342028-to-R1342044_waveformAnalysis_gain_clean_fixed.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1342055-to-R1342078_waveformAnalysis_gain_all.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1342127-to-R1342149_waveformAnalysis_gain_clean_fixed.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1342154-to-R1342184_waveformAnalysis_gain_clean.root.dat");
   */

   runNumbers.push_back(1312065);
   runNumbers.push_back(1312066);
   runNumbers.push_back(1312068);
   runNumbers.push_back(1312069);
   runNumbers.push_back(1312070);
   runNumbers.push_back(1312071);
   runNumbers.push_back(1312076);
   runNumbers.push_back(1312080);
   runNumbers.push_back(1312082);

   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschi_waveformAnalysis_R1312065_gain_all.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschi_waveformAnalysis_R1312066_gain_all.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschi_waveformAnalysis_R1312068_gain_all.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschi_waveformAnalysis_R1312069_gain_all.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschi_waveformAnalysis_R1312070_gain_all.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschi_waveformAnalysis_R1312071_gain_all.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschi_waveformAnalysis_R1312076_gain_all.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschi_waveformAnalysis_R1312080_gain_all.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschi_waveformAnalysis_R1312082_gain_all.root.dat");



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

   std::vector<double> pulseSize;
   pulseSize.push_back(7);
   pulseSize.push_back(8);
   pulseSize.push_back(9);
   pulseSize.push_back(10);
   pulseSize.push_back(6);
   pulseSize.push_back(5);
   pulseSize.push_back(4);
   pulseSize.push_back(3);
   pulseSize.push_back(2);  

   // ... more vectors

   // list of information to be extracted from the XML data base
   TString mysqlFieldsToQuery[13] = {"run_number",      "start_time",      "end_time",        "mean_cathode_V",  "mean_anode1_V",   "mean_anode2_V",   "mean_anode3_V",   "mean_pressure",   "anode1_sparks",  "anode2_sparks",   "anode3_sparks",        "sparking_good",   "reasonable_voltages"};
 
   for(int run = 0; run < runNumbers.size(); run++){
     // connect to a mysql server
     TSQLServer *xmlInformationDatabase = TSQLServer::Connect("mysql://dmdbserv.pp.rhul.ac.uk/GOOD_RUNS", "dmatter", "seedark");
     if( xmlInformationDatabase->IsError() ) { std::cout << " In case you were looking for an error: You found one! " << std::endl; }
 
     // we construct the xml query which will give us all the information we need for the particular run
     TString mysql_query = "SELECT ";
     for(Int_t iBuildFullQuerry = 0; iBuildFullQuerry < 13; iBuildFullQuerry++){
       if(iBuildFullQuerry !=0) {mysql_query+=", "+mysqlFieldsToQuery[iBuildFullQuerry];}
       else{ mysql_query+=" "+mysqlFieldsToQuery[iBuildFullQuerry]; }
     }
     TString runNumberString;
     runNumberString.Form("%i", runNumbers.at(run));
     mysql_query+=" FROM run_info WHERE run_number="+runNumberString;
     //Now execute the query and put it in a TSQLResult object 
     TSQLResult *mysql_result = xmlInformationDatabase->Query(mysql_query);
     if (!mysql_result) { 
       std::cout << "Error: Everything is on fire" << std::endl;
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
     cVoltage.push_back(atof(mysql_row->GetField(3)));
     a1Voltage.push_back(atof(mysql_row->GetField(4)));
     a2Voltage.push_back(atof(mysql_row->GetField(5)));
     a3Voltage.push_back(atof(mysql_row->GetField(6)));
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

   //Get peak parameters from file
   for(int iResults = 0; iResults < resultFile.size(); iResults++){
     std::ifstream file;
     file.open(resultFile[iResults].c_str());
     if(file.is_open()){
       // double peak1, peak1err, peak2, peak2err, peak3, peak3err, peak4, peak4err, chi1, ndf1, chi2, ndf2, chi3, ndf3, chi4, ndf4;
       double peak1, peak1err, chi1, ndf1;
       //file >> peak1 >> peak1err >> peak2 >> peak2err >> peak3 >> peak3err >> peak4 >> peak4err >> chi1 >> ndf1 >> chi2 >> ndf2 >> chi3 >> ndf3 >> chi4 >> ndf4;
       file >> peak1 >> peak1err >> chi1 >> ndf1;
       std::cout << peak1 << " " << peak1err << " " << chi1 << " " << ndf1 << std::endl;
       Peaks1.push_back(peak1);
       //Peaks2.push_back(peak2);
       //Peaks3.push_back(peak3);
       //Peaks4.push_back(peak4);
       peakErrors1.push_back(peak1err);
       //peakErrors2.push_back(peak2err);
       //peakErrors3.push_back(peak3err);
       //peakErrors4.push_back(peak4err);
       Chis1.push_back(chi1);
       //Chis2.push_back(chi2);
       //Chis3.push_back(chi3);
       //Chis4.push_back(chi4);
       Ndfs1.push_back(ndf1);
       //Ndfs2.push_back(ndf2);
       //Ndfs3.push_back(ndf3);
       //Ndfs4.push_back(ndf4);
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
   /*
   for(int errorsub2a = 0; errorsub2a < peakErrors2.size(); errorsub2a++){
     for(int errorsub2b = 0; errorsub2b < ChisNdfs4.size(); errorsub2b++){
       if(errorsub2a == errorsub2b){
	 scaledpeakErrors2.push_back(peakErrors2[errorsub2a] * ChisNdfs4[errorsub2b]); 
       }
     }
   }
   for(int errorsub3a = 0; errorsub3a < peakErrors3.size(); errorsub3a++){
     for(int errorsub3b = 0; errorsub3b < ChisNdfs4.size(); errorsub3b++){
       if(errorsub3a == errorsub3b){
	 scaledpeakErrors3.push_back(peakErrors3[errorsub3a] * ChisNdfs4[errorsub3b]); 
       }
     }
   }
   for(int errorsub4a = 0; errorsub4a < peakErrors4.size(); errorsub4a++){
     for(int errorsub4b = 0; errorsub4b < ChisNdfs4.size(); errorsub4b++){
       if(errorsub4a == errorsub4b){
	 scaledpeakErrors4.push_back(peakErrors4[errorsub4a] * ChisNdfs4[errorsub4b]); 
       }
     }
   }
   */

   // create the TGraph, draw it, and save it
   if(plottoplot == "Anode1"){
     gr = new TGraphErrors(7,&(a1Voltage[0]),&(Peaks1[0]),0,&(peakErrors1[0]));
     // gr2 = new TGraphErrors(7,&(a1Voltage[0]),&(Peaks1[0]),0,&(peakErrors1[0]));
   }
   if(plottoplot == "Anode2"){
     gr = new TGraphErrors(7,&(a2Voltage[0]),&(Peaks1[0]),0,&(peakErrors1[0]));
     // gr2 = new TGraphErrors(7,&(a2Voltage[0]),&(Peaks2[0]),0,&(peakErrors1[0]));
   }
   if(plottoplot == "runset"){
     gr = new TGraphErrors(7,&(a2Voltage[0]),&(Peaks1[0]),0,&(peakErrors1[0]));
   }
   if(plottoplot == "Pulse"){
     gr = new TGraphErrors(9,&(pulseSize[0]),&(Peaks1[0]),0,&(peakErrors1[0]));
   }
   if(plottoplot == "Anode3"){
     gr = new TGraphErrors(7,&(a3Voltage[0]),&(Peaks3[0]),0,&(peakErrors1[0]));
     //gr2 = new TGraphErrors(7,&(a3Voltage[0]),&(Peaks2[0]),0,&(peakErrors2[0]));
   }
   if(plottoplot == "Cathode"){
     gr = new TGraphErrors(7,&(cVoltage[0]),&(Peaks1[0]),0,&(peakErrors1[0]));
     //gr2 = new TGraphErrors(7,&(cVoltage[0]),&(Peaks2[0]),0,&(peakErrors2[0]));
   }
   if(plottoplot == "Anode3 - Anode2"){
     gr = new TGraphErrors(7,&(a3a2Voltage[0]),&(Peaks1[0]),0,&(peakErrors1[0]));
     // gr2 = new TGraphErrors(7,&(a3a2Voltage[0]),&(Peaks1[0]),0,&(peakErrors1[0]));
   }
   if(plottoplot == "Anode3 - Anode1"){
     gr = new TGraphErrors(7,&(a3a1Voltage[0]),&(Peaks1[0]),0,&(peakErrors1[0])); 
     //  gr2 = new TGraphErrors(7,&(a3a1Voltage[0]),&(Peaks2[0]),0,&(peakErrors2[0]));
   }
   if(plottoplot == "Anode2 - Anode1"){
     gr = new TGraphErrors(7,&(a2a1Voltage[0]),&(Peaks1[0]),0,&(peakErrors1[0]));
     //gr2 = new TGraphErrors(7,&(a2a1Voltage[0]),&(Peaks2[0]),0,&(peakErrors2[0]));
   }
   if(plottoplot == "Anode Sum"){
     gr = new TGraphErrors(7,&(sumVoltage[0]),&(Peaks1[0]),0,&(peakErrors1[0]));
     // gr2 = new TGraphErrors(7,&(sumVoltage[0]),&(Peaks2[0]),0,&(peakErrors2[0]));     
   }


   stringstream plotname;
   plotname << "Peak Position vs " << plottoplot;
   gr->SetTitle(plotname.str().c_str());
   gr->SetMarkerColor(4);
   gr->SetMarkerStyle(21);
   //gr2->SetMarkerColor(5);
   //gr2->SetMarkerStyle(21);
   stringstream xaxisname;
   xaxisname << plottoplot << " (V)";
 
   gr->Fit("pol1");
   //mg = new TMultiGraph();
   //mg->Add(gr, "P");
   //mg->Add(gr2, "P");
   gr->SetTitle(plotname.str().c_str());
   //mg->Draw("A");
   gr->GetXaxis()->SetTitle(xaxisname.str().c_str());
   gr->GetYaxis()->SetTitle("Peak Position (mV)");
   gr->Draw("AP");
   c1->Print(("preamppeakpositionvs"+plottoplot+".png").c_str());
   c1->Update();
}

