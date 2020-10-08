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

void plotPeakPositionNewAm241(std::string Scheme= "noScheme", bool gain = true){
   TCanvas* c1 = new TCanvas("c1","Plots for peak position",200,10,500,500);
   // c1->SetGrid();
   std::vector<int> runNumbers;
   std::vector<std::string> resultFile;
   std::vector<std::string> paramsFile;
   std::vector<std::string> peaksFile;
   std::vector<std::string> bkgrFile;
   std::vector<double> runDate;
   std::vector<double> runDateTime;
   std::ifstream listFile;


  //SchemeBModSpecialTrigAll
  /*resultFile.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/peakschifirst_raptorr_tpcdata_gain_R2084009-R2084018_true_TEST_Am241.root.dat");
  resultFile.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/peakschifirst_raptorr_tpcdata_gain_R2083126-R2084008_Am241.root.dat");
  resultFile.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/peakschifirst_raptorr_tpcdata_gain_R2083116-R2083125_Am241.root.dat");
  paramsFile.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/parameters_raptorr_tpcdata_gain_R2084009-R2084018_true_TEST_Am241.root.dat");
  paramsFile.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/parameters_raptorr_tpcdata_gain_R2083126-R2084008_Am241.root.dat");
  paramsFile.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/parameters_raptorr_tpcdata_gain_R2083116-R2083125_Am241.root.dat");
  */


  if(Scheme == "GasAgeing"){
    //GasAgeing
    //SchemeA
    resultFile.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/peakschi_raptorr_tpcdata_gain_R2081070-R2081084_true_Am241.root.dat");
    paramsFile.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/parameters_raptorr_tpcdata_gain_R2081070-R2081084_true_Am241.root.dat");
    //SchemeB
    resultFile.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/peakschifirst_raptorr_tpcdata_gain_R2082009-R2082024_true_Am241_AllTrig.root.dat");
    paramsFile.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/parameters_raptorr_tpcdata_gain_R2082009-R2082024_true_Am241_AllTrig.root.dat");
    //SchemeC
    resultFile.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/peakschi_raptorr_tpcdata_gain_R2083022-R2083032_Am241.root.dat");
    paramsFile.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/parameters_raptorr_tpcdata_gain_R2083022-R2083032_Am241.root.dat");
    //SchemeZ
    resultFile.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/peakschi_raptorr_tpcdata_gain_R2080141-R2080165_true_Am241.root.dat");
    paramsFile.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/parameters_raptorr_tpcdata_gain_R2080141-R2080165_true_Am241.root.dat");
    //SchemeY
    resultFile.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/peakschi_raptorr_tpcdata_gain_R2079057-R2079062_true_Am241.root.dat");
    paramsFile.push_back("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/parameters_raptorr_tpcdata_gain_R2079057-R2079062_true_Am241.root.dat");

    
    //runNumbers.push_back(2080149);
    runNumbers.push_back(2081073);
    runNumbers.push_back(2082012);
    runNumbers.push_back(2083025);
    runNumbers.push_back(2080147);
    runNumbers.push_back(2079060);
    //runNumbers.push_back(2079060);
    runDate.push_back(21);
    runDate.push_back(22);
    runDate.push_back(23);
    runDate.push_back(20);
    runDate.push_back(19);
    runDateTime.push_back(21.797);
    runDateTime.push_back(22.36373);
    runDateTime.push_back(23.33407);
    runDateTime.push_back(20.68969);
    runDateTime.push_back(19.89427);
  }

  std::cout << "Scheme is " << Scheme << std::endl;

  //SchemeBModSpecialTrigAll, SchemeBMod2, SchemeC, SchemeA
  std::stringstream listFileNameSS;
  std::string listFileName;  
  listFileNameSS << "/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/scripts/bkgVsSignal/runsToPlot" << Scheme;
  listFileNameSS >> listFileName;
  std::cout << "listFileName is" << listFileName << std::endl;
  listFile.open(listFileName.c_str());

  //listFile.open("/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/scripts/bkgVsSignal/runsToPlotSchemeA");


  //SchemeA -> _true_Am241                                                                                                                                                                                 
  //SchemeC -> _Am241                                                                                                                                                                                      
  //SchemeB -> _true_Am241_AllTrig (and include the weird ones above)  
  std::string listFileSuffix;
  std::string listFilePrefix;
  if(Scheme == "SchemeA"){  
    listFileSuffix = "_true_Am241";
    listFilePrefix = "peakschi";
    runNumbers.push_back(2081073);
    runNumbers.push_back(2081109);
    runNumbers.push_back(2081118);
    runNumbers.push_back(2081132);
    runNumbers.push_back(2081168);
  }
  if(Scheme == "SchemeC"){
    listFileSuffix = "_true_Am241";
    listFilePrefix = "peakschi";
    runNumbers.push_back(2083107);
    runNumbers.push_back(2083097);
    runNumbers.push_back(2083087);
    runNumbers.push_back(2083077);
    runNumbers.push_back(2083067);
    runNumbers.push_back(2083023);
    runNumbers.push_back(2083036);
    runNumbers.push_back(2083047);
    runNumbers.push_back(2083057);    
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
    runNumbers.push_back(2084011);
  }
  if(Scheme == "SchemeBMod2"){
    listFileSuffix = "_true_Am241";
    listFilePrefix = "peakschi";
    runNumbers.push_back(2084011);
  }



  std::string fileLine;
  if(listFile.is_open()){
    std::cout << "listFile is open" << std:: endl;
    while (std::getline(listFile, fileLine)){
      std::stringstream resultsSS;
      std::stringstream paramsSS;
      std::stringstream peaksSS;
      std::stringstream bkgrSS;
      std::string resultsString = "";
      std::string paramsString = "";
      std::string peaksString = "";
      std::string bkgrString = "";
      std::cout << "Get Line is " << fileLine << std::endl;
      resultsSS << "/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/bkgFits/" << listFilePrefix << "_raptorr_tpcdata_gain_" << fileLine << listFileSuffix << ".root.dat";
      paramsSS << "/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/bkgFits/parameters_raptorr_tpcdata_gain_" << fileLine << listFileSuffix << ".root.dat"; 
      peaksSS << "/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/bkgFits/peaks_raptorr_tpcdata_gain_" << fileLine << listFileSuffix << ".root.dat"; 
      bkgrSS << "/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/bkgFits/bkgrparams_raptorr_tpcdata_gain_" << fileLine << listFileSuffix << ".root.dat";
      resultsSS >> resultsString;
      paramsSS >> paramsString;
      peaksSS >> peaksString;
      bkgrSS >> bkgrString;
      resultFile.push_back(resultsString);
      paramsFile.push_back(paramsString);
      peaksFile.push_back(peaksString);
      bkgrFile.push_back(bkgrString);
    }
  }
  //SchemeA -> _true_Am241
  //SchemeC -> _Am241
  //SchemeB -> _true_Am242_AllTrig


   // data storage
   std::vector<double> schemeACustomVoltage;
   std::vector<double> schemeBCustomVoltage;
   std::vector<double> schemeCCustomVoltage;
   std::vector<double> gasAgeingCustomVoltage;
   std::vector<double> customVoltage;

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
   std::vector<double> Bkg1;
   std::vector<double> Bkg2;
   std::vector<double> Bkgrs1;
   std::vector<double> Bkgrs2;
   std::vector<double> BkgrErrors1;
   std::vector<double> BkgrErrors2;

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
   //   schemeBCustomVoltage.push_back(0);
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
   
   //GasAgeing
   gasAgeingCustomVoltage.push_back(1200);
   gasAgeingCustomVoltage.push_back(1200);
   gasAgeingCustomVoltage.push_back(1200);
   gasAgeingCustomVoltage.push_back(1200);
   gasAgeingCustomVoltage.push_back(1200);

   //Get voltages from database

   // list of information to be extracted from the XML data base                                                                                                                     
   TString mysqlFieldsToQuery[7] = {"run_number",      "start_time",      "end_time",        "mean_cathode_V",  "mean_anode1_V",   "mean_anode2_V",   "mean_anode3_V"};

   for(int run = 0; run < runNumbers.size(); run++){
     // connect to a mysql server                                                                                                                                                    
     TSQLServer *xmlInformationDatabase = TSQLServer::Connect("mysql://dmdbserv.pp.rhul.ac.uk/GOOD_RUNS", "dmatter", "seedark");
     if( xmlInformationDatabase->IsError() ) { std::cout << "There is an Error with the Database" << std::endl; }

     // we construct the xml query which will give us all the information we need for the particular run                                                                             
     TString mysql_query = "SELECT ";
     for(Int_t iBuildFullQuerry = 0; iBuildFullQuerry < 7; iBuildFullQuerry++){
       if(iBuildFullQuerry !=0) {mysql_query+=", "+mysqlFieldsToQuery[iBuildFullQuerry];}
       else{ mysql_query+=" "+mysqlFieldsToQuery[iBuildFullQuerry]; }
     }
     TString runNumberString;
     runNumberString.Form("%i", runNumbers.at(run));
     mysql_query+=" FROM run_info_lite WHERE start_time>='2019-01-01 00:00:00' AND run_number="+runNumberString;
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
     //     std::cout << "Start Time\t" << atof_l(mysql_row->GetField(1), "\t") << std::endl;
     //std::cout << "Pressure \t" << atof(mysql_row->GetField(7)) << std::endl;       
     cVoltage.push_back(atof(mysql_row->GetField(3)));
     a1Voltage.push_back(atof(mysql_row->GetField(4)));
     a2Voltage.push_back(atof(mysql_row->GetField(5)));
     a3Voltage.push_back(atof(mysql_row->GetField(6)));
     //     pressure.push_back(atof(mysql_row->GetField(7)));                                                                    
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

   //Get bkgr parameters from file                                                                                                                              
   for(int iBkgr = 0; iBkgr < bkgrFile.size(); iBkgr++){
     std::cout << "File to get peak from is " << bkgrFile[iBkgr] << std::endl;
     std::ifstream file;
     file.open(bkgrFile[iBkgr].c_str());
     if(file.is_open()){
       double bkgr11, bkgr12, bkgrError11, bkgrError12;
       file >> bkgr11 >> bkgr12 >> bkgrError11 >> bkgrError12;
       std::cout << "bkgr11 is  " << bkgr11 << " error is " << bkgrError11 << std::endl;
       Bkgrs1.push_back(bkgr11);
       Bkgrs2.push_back(bkgr12);
       BkgrErrors1.push_back(bkgrError11);
       BkgrErrors2.push_back(bkgrError12);
     }
     else{ std::cout << "Cannot open bkgr file" << bkgrFile[iBkgr] << std::endl;}
   }

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
     Bkg1.push_back(bkgr11);
     Bkg2.push_back(bkgr12);
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
      yplot = Bkgrs1;
      yploterrors = BkgrErrors1;
      plotTitle << "Bkgr1 vs ";
    }
    else{
      yplot = Bkgrs2;
      yploterrors = BkgrErrors2;
      plotTitle << "Bkgr2 vs ";
    }
    

  
    //Choose which graph to plot based on input
    if(Scheme == "SchemeA"){
      std::cout << "Scheme is still Schema A" << std::endl;
      xplot = a1Voltage;
      //xplot = schemeACustomVoltage
      TGraphErrors* peakGraph2 = new TGraphErrors(5,&(xplot[0]),&(yplot[0]),0,&(yploterrors[0]));
      plotTitle << "Anode 1";
      xAxisName = "Anode1 Voltage (V)";
      peakGraph2->SetMarkerColor(3);
      peakGraph2->SetMarkerStyle(33);
      peakGraph2->SetLineColor(3);
    }    
    if(Scheme == "SchemeC"){
      //xplot = schemeCCustomVoltage;
      xplot = a2a1Voltage;
      TGraphErrors* peakGraph2 = new TGraphErrors(7,&(xplot[0]),&(yplot[0]),0,&(yploterrors[0]));
      plotTitle << "Anode 2 - Anode 1";    
      xAxisName = "Anode 2 - Anode 1 (V)";
      peakGraph2->SetMarkerColor(2);
      peakGraph2->SetMarkerStyle(33);
      peakGraph2->SetLineColor(2);
    }
    if(Scheme == "SchemeBMod2" || Scheme == "SchemeBModSpecialTrigAll"){
      xplot = schemeBCustomVoltage;
      //xplot = schemeBCustomVoltage
      //TGraphErrors* peakGraph2 = new TGraphErrors(11,&(xplot[0]),&(Peaks2[0]),0,&(scaledpeakErrors2[0]));
      TGraphErrors* peakGraph2 = new TGraphErrors(10,&(xplot[0]),&(yplot[0]),0,&(yploterrors[0]));
      plotTitle << "Anode 3 - Anode 2";
      xAxisName = "Anode 3 - Anode 2 (V)";
      peakGraph2->SetMarkerColor(4);
      peakGraph2->SetMarkerStyle(33);
      peakGraph2->SetLineColor(4);
    }

    if(Scheme == "GasAgeing"){
      //xplot = gasAgeingCustomVoltage;
      //a1Voltage.push_back(1200);
      xplot = runDateTime;
      TGraphErrors* peakGraph2 = new TGraphErrors(5,&(xplot[0]),&(yplot[0]),0,&(yploterrors[0]));
      plotTitle << "Run Start Time";
      xAxisName = "Run Start Time (Date)";
      peakGraph2->SetMarkerColor(9);
      peakGraph2->SetMarkerStyle(33);
      peakGraph2->SetLineColor(9);
      peakGraph2->GetXaxis()->SetDecimals(false);
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
    if(Scheme == "SchemeBModSpecialTrigAll" || Scheme == "SchemeBMod2"){SchemeName = "SchemeB";}
    if(Scheme == "GasAgeing"){SchemeName = "GasAgeing";}

    //Save Plot values to files for use in LightVsCharge.cxx
    /*for(int iZack = 0; iZack < 5; iZack++){
     std::stringstream zackFileName;
     std::stringstream voltageForName;
     voltageForName << xplot[iZack];
     zackFileName << "/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/fitDataVOLTAGETEST" << SchemeName << "_" << GainString << voltageForName.str() << ".dat";
     std::ofstream zackFile;
     zackFile.open(zackFileName.str().c_str());
     zackFile << yplot[iZack];
     zackFile << "\t";
     zackFile << yploterrors[iZack];
     zackFile.close();
     std::cout << zackFileName.str() <<  " created" << std::endl;
   }
    */
      
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

   peakGraph2->Fit("pol1");
   //   TMultiGraph* mg = new TMultiGraph();
   //mg->Add(gr, "P");
   //mg->Add(gr2, "P");
   //mg->Add(gr4, "P");
   // mg->Add(peakGraph2, "P");
   //mg->Draw("A");


   
   //   int noRuns = runNumbers.size();
   //plottitle <<  runNumbers[0] << "_to_" << runNumbers[noRuns - 1];


   if(gain == false){
   peakGraph2->GetYaxis()->SetTitle("Bkgr12");
   c1->Print(("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/BackgroundsTEST"+SchemeName+"_Anode3Bkgr1_"+"Custom"+".pdf").c_str());
   }

   if(gain == true){ 
   peakGraph2->GetYaxis()->SetTitle("Bkgr11");
   c1->Print(("/scratch0/hrichie/hptpc_ritchie/HPTPC/data/fitData/BackgroundsTEST"+SchemeName+"_Anode3Bkgr2_"+"Custom"+".pdf").c_str());
   }

   c1->Update();
}

