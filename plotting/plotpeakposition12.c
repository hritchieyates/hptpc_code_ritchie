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

void plotpeakposition12(std::string plottoplot= "noplot", bool gain = true){
   TCanvas* c1 = new TCanvas("c1","Plots for peak position",200,10,500,500);
   // c1->SetGrid();
   std::vector<int> runNumbers;
   std::vector<std::string> resultFile;
   std::vector<std::string> paramsFile;
   std::vector<std::string> peaksFile;
   // some function should generate the list...
   


   /*
   runNumbers.push_back(1336094);
   runNumbers.push_back(1337009);
   runNumbers.push_back(1337052);
   //runNumbers.push_back(1338001);
   //runNumbers.push_back(1338085);
   runNumbers.push_back(1339015);
   */
   /*
   runNumbers.push_back(1349104);
   runNumbers.push_back(1349110);
   runNumbers.push_back(1349116);
   runNumbers.push_back(1349152);
   runNumbers.push_back(1350028);
   */
   
   // some function should generate this list...
   /*
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peaks_R1336094-to-R1336108_waveformAnalysis_gain_all.root.dat");  
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peaks_R1337009-to-R1337034_waveformAnalysis_gain_all.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peaks_R1337052-to-R1337061_waveformAnalysis_gain_all.root.dat");
   // resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peaks_R1338001-to-R1338082_waveformAnalysis_gain_all.root.dat");
   //resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peaks_R1338085-to-R1338109_waveformAnalysis_gain_all.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peaks_R1339015-to-R1339042_waveformAnalysis_gain_all.root.dat");
   */
   
   /*
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
   //runNumbers.push_back(1353009);

   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1351065-to-R1351075_waveformAnalysis_gain_all.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1351001-to-R1351059_waveformAnalysis_gain_clean.root.dat");  
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1351080-to-R1351089_waveformAnalysis_gain_all.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1351094-to-R1351110_waveformAnalysis_gain_all.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1351113-to-R1351125_waveformAnalysis_gain_all.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1352049-to-R1352064_waveformAnalysis_gain_clean.root.dat");
   //resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1353009-to-R1353019_waveformAnalysis_gain_clean_fixed.root.dat");
   */
   
   
   
   //Leaky 5bar data
   /*
   runNumbers.push_back(1341085);
   runNumbers.push_back(1342001);
   runNumbers.push_back(1342012);
   runNumbers.push_back(1342028);
   //runNumbers.push_back(1342055);
   // runNumbers.push_back(1342127);
   // runNumbers.push_back(1342154);

   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1341065-to-R1341087_waveformAnalysis_gain_all.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1342001-to-R1342006_waveformAnalysis_gain_all.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1342012-to-R1342026_waveformAnalysis_gain_clean_fixed.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1342028-to-R1342044_waveformAnalysis_gain_clean_fixed.root.dat");
   //  resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1342055-to-R1342078_waveformAnalysis_gain_all.root.dat");
   //   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1342127-to-R1342149_waveformAnalysis_gain_clean_fixed.root.dat");
   //  resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1342154-to-R1342184_waveformAnalysis_gain_clean.root.dat");
   */
  
   //Ar-CO2-N2 3.34barG 11th-12th Dec
   /*
   runNumbers.push_back(1345020);
   runNumbers.push_back(1345026);
   runNumbers.push_back(1345032);
   runNumbers.push_back(1345044);

   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1345020-to-R1345025_waveformAnalysis_gain_clean.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1345026-to-R1345029_waveformAnalysis_gain_clean.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1345032-to-R1345035_waveformAnalysis_gain_all.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1345044-to-R1345047_waveformAnalysis_gain_clean.root.dat");
  
   paramsFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/parameters_R1345020-to-R1345025_waveformAnalysis_gain_clean.root.dat");
   paramsFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/parameters_R1345026-to-R1345029_waveformAnalysis_gain_clean.root.dat");
   paramsFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/parameters_R1345032-to-R1345035_waveformAnalysis_gain_all.root.dat");
   paramsFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/parameters_R1345044-to-R1345047_waveformAnalysis_gain_clean.root.dat");
   */
   
   //Ar-293barG Nov 19th
   //runNumbers.push_back(1327033);
   /* runNumbers.push_back(1328009);
   runNumbers.push_back(1328124);
   runNumbers.push_back(1329011);
   runNumbers.push_back(1330016);
   runNumbers.push_back(1330100);
   //runNumbers.push_back(1331002);
   //runNumbers.push_back(1331008);
   */
   /*
   //resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1327033-to-R1327049_waveformAnalysis_gain_clean.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1328009-to-R1328063_waveformAnalysis_gain_clean_fixed.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1328124-to-R1328134_waveformAnalysis_gain_all.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1329011-to-R1329024_waveformAnalysis_gain_all.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1330016-to-R1330054_waveformAnalysis_gain_clean_fixed.root.dat");
   resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1330100-to-R1330109_waveformAnalysis_gain_clean_fixed.root.dat");
   //resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1331002-to-R1331036_waveformAnalysis_gain_clean.root.dat");
   //resultFile.push_back("/home/hrichie/hptpc-daq/DmtpcChargeAnalysis/peakschiall_R1331008-to-R1331168_waveformAnalysis_gain_clean.root.dat");
   */

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
  std::stringstream resultsSS = "";
  std::stringstream paramsSS = "";
  std::stringstream peaksSS = "";
  listFile.open("/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/scripts/bkgVsSignal/runsToSglNewTo");
  std::string fileLine;
  if(listFile.is_open()){
    std::cout << "listFile is open" << std:: endl;
    while (std::getline(listFile, fileLine)){
      resultsSS << "peakschi_" << fileLine << "_raptorr_tpcdata_gain_PeakTime2_true.root.dat";
      paramsSS << "parameters_" << fileLine << "_raptorr_tpcdata_gain_PeakTime2_true.root.dat"; 
      peaksSS << "peaks_" << fileLine << "_raptorr_tpcdata_gain_PeakTime2_true.root.dat"; 
      resultFile.push_back(resultsSS.str().c_str());
      paramsList.push_back(paramsSS.str().c_str());
      peaksList.push_back(peaksSS.str().c_str());
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
     mysql_query+=" FROM run_info WHERE run_number="+runNumberString;
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
     std::ifstream file;
     file.open(resultFile[iResults].c_str());
     if(file.is_open()){
       double peak1, peak1err, peak2, peak2err, peak3, peak3err, peak4, peak4err, chi1, ndf1, chi2, ndf2, chi3, ndf3, chi4, ndf4;
       file >> peak1 >> peak1err >> peak2 >> peak2err >> peak3 >> peak3err >> peak4 >> peak4err >> chi1 >> ndf1 >> chi2 >> ndf2 >> chi3 >> ndf3 >> chi4 >> ndf4;
       std::cout << peak1 << std::endl;
       Peaks1.push_back(peak1);
       Peaks2.push_back(peak2);
       Peaks3.push_back(peak3);
       Peaks4.push_back(peak4);
       peakErrors1.push_back(peak1err);
       peakErrors2.push_back(peak2err);
       peakErrors3.push_back(peak3err);
       peakErrors4.push_back(peak4err);
       Chis1.push_back(chi1);
       Chis2.push_back(chi2);
       Chis3.push_back(chi3);
       Chis4.push_back(chi4);
       Ndfs1.push_back(ndf1);
       Ndfs2.push_back(ndf2);
       Ndfs3.push_back(ndf3);
       Ndfs4.push_back(ndf4);
     }
     else{ std::cout << "Cannot open peak file" << resultFile[iResults] << std::endl;}
   }
   
   //Get peak parameters from file
   for(int iParams = 0; iParams < paramsFile.size(); iParams++){
     std::ifstream file2;
     file2.open(paramsFile[iParams].c_str());
     if(file2.is_open()){
     double bkgr11,bkgr12,peak11,peak12,peak13,peak21,peak22,peak23,peak31,peak32,peak33,peak41,peak42,peak43;
     file2 >> bkgr11 >> bkgr12 >> peak11 >> peak12 >> peak13 >> peak21 >> peak22 >> peak23 >> peak31 >> peak32 >> peak33 >> peak41 >> peak42 >> peak43;
     peakWidth1.push_back(abs(peak13));
     peakWidth2.push_back(abs(peak23));
     peakWidth3.push_back(abs(peak33));
     peakWidth4.push_back(abs(peak43));
     std::cout << "Peak 3 Width " << peak33 << std::endl;
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
   for(int errorsub1a = 0; errorsub1a < peakErrors1.size(); errorsub1a++){
     for(int errorsub1b = 0; errorsub1b < ChisNdfs4.size(); errorsub1b++){
       if(errorsub1a == errorsub1b){
	 scaledpeakErrors1.push_back(peakErrors1[errorsub1a] * ChisNdfs4[errorsub1b]); 
	 scaledpeakWidth1.push_back(peakWidth1[errorsub1a] * ChisNdfs4[errorsub1b]);
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
   for(int errorsub3a = 0; errorsub3a < peakErrors3.size(); errorsub3a++){
     for(int errorsub3b = 0; errorsub3b < ChisNdfs4.size(); errorsub3b++){
       if(errorsub3a == errorsub3b){
	 scaledpeakErrors3.push_back(peakErrors3[errorsub3a] * ChisNdfs4[errorsub3b]);
	 scaledpeakWidth3.push_back(peakWidth3[errorsub3a] * ChisNdfs4[errorsub3b]);
       }
     }
   }
   for(int errorsub4a = 0; errorsub4a < peakErrors4.size(); errorsub4a++){
     for(int errorsub4b = 0; errorsub4b < ChisNdfs4.size(); errorsub4b++){
       if(errorsub4a == errorsub4b){
	 scaledpeakErrors4.push_back(peakErrors4[errorsub4a] * ChisNdfs4[errorsub4b]);
	 scaledpeakWidth4.push_back(peakWidth4[errorsub4a] * ChisNdfs4[errorsub4b]);
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
   for(int gcalci = 0; gcalci < Peaks3.size(); gcalci++){
     Gains3.push_back(Peaks3[gcalci] / (gCalcConst * eDep661));
     gainErrors3.push_back(scaledpeakErrors3[gcalci] / (gCalcConst * eDep661));
     gainWidth3.push_back(scaledpeakWidth3[gcalci] / (gCalcConst * eDep661));
   }
   for(int gcalcj = 0; gcalcj < Peaks2.size(); gcalcj++){
     Gains2.push_back(Peaks2[gcalcj] / (gCalcConst * eDep512));
     gainErrors2.push_back(scaledpeakErrors2[gcalcj] / (gCalcConst * eDep512));
     gainWidth2.push_back(scaledpeakWidth2[gcalcj] / (gCalcConst * eDep512));
   }
   for(int gcalck = 0; gcalck < Peaks4.size(); gcalck++){
     Gains4.push_back(Peaks4[gcalck] / (gCalcConst * eDep1174));
     gainErrors4.push_back(scaledpeakErrors4[gcalck] / (gCalcConst * eDep1174));
     gainWidth4.push_back(scaledpeakWidth4[gcalck] / (gCalcConst * eDep1174));
   }
   for(int gcalcl = 0; gcalcl < Peaks1.size(); gcalcl++){
     Gains1.push_back(Peaks1[gcalcl] / (gCalcConst * eDep60));
     gainErrors1.push_back(scaledpeakErrors1[gcalcl] / (gCalcConst * eDep60));
     gainWidth1.push_back(scaledpeakWidth1[gcalcl] / (gCalcConst * eDep60));
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
     gr = new TGraphErrors(5,&(xplot[0]),&(Peaks3[0]),0,&(scaledpeakWidth3[0]));
     gr2 = new TGraphErrors(5,&(xplot[0]),&(Peaks2[0]),0,&(scaledpeakWidth2[0]));
     gr4 = new TGraphErrors(5,&(xplot[0]),&(Peaks4[0]),0,&(scaledpeakWidth4[0]));
     gr1 = new TGraphErrors(5,&(xplot[0]),&(Peaks1[0]),0,&(scaledpeakWidth1[0]));
   }
   
   /*       
   if(gain == true){
     gr = new TGraphErrors(5,&(xplot[0]),&(Gains3[0]),0,&(gainErrors3[0]));
     gr2 = new TGraphErrors(5,&(xplot[0]),&(Gains2[0]),0,&(gainErrors2[0]));
     gr4 = new TGraphErrors(5,&(xplot[0]),&(Gains4[0]),0,&(gainErrors4[0]));
     gr1 = new TGraphErrors(5,&(xplot[0]),&(Gains1[0]),0,&(gainErrors1[0]));
   }
   */

   if(gain == true){
     gr = new TGraphErrors(5,&(xplot[0]),&(Gains3[0]),0,&(gainWidth3[0]));
     gr2 = new TGraphErrors(5,&(xplot[0]),&(Gains2[0]),0,&(gainWidth2[0]));
     gr4 = new TGraphErrors(5,&(xplot[0]),&(Gains4[0]),0,&(gainWidth4[0]));
     gr1 = new TGraphErrors(5,&(xplot[0]),&(Gains1[0]),0,&(gainWidth1[0]));
   }
   
   
   stringstream plotname;
   if(gain == false){
     plotname << "Peak Position vs " << plottoplot;
   }
   if(gain == true){
     plotname << "Gain vs " << plottoplot;
   }  
   gr->SetTitle(plotname.str().c_str());
   gr->SetMarkerColor(4);
   gr->SetMarkerStyle(21);
   gr2->SetMarkerColor(5);
   gr2->SetMarkerStyle(21);
   gr4->SetMarkerColor(3);
   gr4->SetMarkerStyle(21);
   gr1->SetMarkerColor(2);
   gr1->SetMarkerStyle(21);
   stringstream xaxisname;
   if(isVP == true){
   xaxisname << plottoplot << " (V/barA)";
   }
   if(isVP == false){
   xaxisname << plottoplot << " (V)";
   }
   //gr->Fit("pol1");
   //gr2->Fit("pol1");
   //gr4->Fit("pol1");
   TMultiGraph* mg = new TMultiGraph();
   //mg->Add(gr, "P");
   //mg->Add(gr2, "P");
   //mg->Add(gr4, "P");
   mg->Add(gr, "P");
   mg->SetTitle(plotname.str().c_str());
   mg->Draw("A");
   mg->GetXaxis()->SetTitle(xaxisname.str().c_str());
   int noRuns = runNumbers.size();
   ostringstream plottitle;
   plottitle <<  runNumbers[0] << "_to_" << runNumbers[noRuns - 1];
   //gr->Draw("AP");
   if(gain == false){
   mg->GetYaxis()->SetTitle("Peak Position (mV)");
   c1->Print(("/scratch2/hrichie/gainPlots/"+plottitle.str()+"_3bpeak_"+plottoplot+".png").c_str());
   }
   if(gain == true){ 
   mg->GetYaxis()->SetTitle("Gain");
   c1->Print(("/scratch2/hrichie/gainPlots/"+plottitle.str()+"_3bgain_"+plottoplot+".png").c_str());
   }
   c1->Update();
}

