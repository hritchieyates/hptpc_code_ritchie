//  STL
#include <iostream>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

// ROOT
#include "TROOT.h"

// self made
#include "bkgVsSignal.hxx"




/** the equivalent to main functions **/

// creates a series of control plots for a given list of runs
void compareDifferentRuns(TString cut = "", TString fileNameSuffix = "", int firstRun = 0, int lastRun = 0, TString TreeSuffix = "_anode3"){
  /* files */
  std::vector<TString> prefixList;
  std::vector<TString> legendList;
  TString fileNameFlag;
  bool UseSpecificRuns = true;
  bool UseConsecutiveRuns = false;
  bool UseRunList = false;


  std::stringstream nameStream;

  //Select Specific Runs to plot                             
  if(UseSpecificRuns){
    nameStream << "/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/scripts/bkgVsSignal/comparisonPlots/SpecificRuns_" << fileNameSuffix << "_";
    nameStream >> fileNameFlag;


    //Fe55 Bkg Vs Signal
    prefixList.push_back("R1354007-R1354017");                                                                                                                                     
    legendList.push_back("Fe55 Bkg");
    prefixList.push_back("R1340169-R1341008");                                                                                                                                     
    legendList.push_back("Fe55 Sgl"); 



    //Propaganda...
    //prefixList.push_back("R2081070-R2081084");
    //legendList.push_back("A1: 1200V, A2: 2400V, A3: 3600V, C: 6000V");
    //...of the sparky kind
    //prefixList.push_back("R2083056-R2083065");
    //legendList.push_back("C4: 1200V, A2: 3000V, A3: 4200V, C: 6000V");

    //Scheme A1 split
    /*prefixList.push_back("R2081070-R2081076");                                                                                                                                                                                                                         
    legendList.push_back("R2081070-R2081076");
    prefixList.push_back("R2081077-R2081084");
    legendList.push_back("R2081077-R2081084");
    */
    
    //Scheme A1 split b
    // prefixList.push_back("R2081070-R2081074");
    //legendList.push_back("R2081070-R2081074");
    //prefixList.push_back("R2081080-R2081084");                                     
    //legendList.push_back("R2081080-R2081084");                                                                                                                                                                                                                            
    //Smooth Vs UnSmooth?
    /*prefixList.push_back("R2083022-R2083032");                                                                                                                             
    legendList.push_back("No Smooth");  
    prefixList.push_back("R2083022-R2083031");
    legendList.push_back("Smooth");*/
    //Smooth Vs UnSmooth?                                                                                                                                                           
    /*    prefixList.push_back("R2082009-R2082024");
    legendList.push_back("No Smooth");
    prefixList.push_back("R2082009-R2082023");
    legendList.push_back("Smooth");
    */
    //Scheme A1 Indivs
    /* prefixList.push_back("R2081070");                                                                                                                                                                                                                          
    legendList.push_back("R2081070");
    prefixList.push_back("R2081077");
    legendList.push_back("R2081077");
    prefixList.push_back("R2081084");
    legendList.push_back("R2081084");
    */

    //Bias Frame Subtraction
    /*prefixList.push_back("R2081183-R2081211");
    legendList.push_back("OV runs");
    prefixList.push_back("R2081070-R2081084");
    legendList.push_back("Scheme A Config 1");
    */

    //Cathode Test
    /*    prefixList.push_back("R1322012-R1322032");
    legendList.push_back("A1 1025, A2 2025, A3 3200, C -3800"); 
    prefixList.push_back("R1323002-R1323022");
    legendList.push_back("A1 1025, A2 2275, A3 3350, C -5150"); 
    */

    //Cathode Test 2
    /*    prefixList.push_back("R2079057-R2079062");
    legendList.push_back("A1 1200, A2 2400, A3 3600, C -4600");
    prefixList.push_back("R2079066-R2080007");
    legendList.push_back("A1 1750, A2 2450, A3 3650, C -6000"); 
    */

    //Gas Ageing Day 1
    /*prefixList.push_back("R2079024-R2079034");
    legendList.push_back("R2079024-R2079034");
    prefixList.push_back("R2079034-R2079044");
    legendList.push_back("R2079034-R2079044");
    prefixList.push_back("R2079044-R2079054");
    legendList.push_back("R2079044-R2079054");
    */

    //Am241 Vs Leaky Bkg
    /*    prefixList.push_back("R1098010-R1098052");
    legendList.push_back("No Source 1200/2400/3600/6000");
    prefixList.push_back("R2081070-R2081084");
    legendList.push_back("Am241 1200/2400/3600/6000");
    */
    
    //Am241 Vs Am241 + Cs137
    /*    prefixList.push_back("R1355038-R1355078");
    legendList.push_back("Am241 + Cs137 1750/2350/3700/6800");
    prefixList.push_back("R2079067-R2080001");
    legendList.push_back("Am241 1750/2450/3650/6000");
    */
    //Gas Ageing Again
    /*    prefixList.push_back("R2080010-R2080027");
    legendList.push_back("19th 1750/2950/4150");
    prefixList.push_back("R2081129-R2081155");
    legendList.push_back("21st 1800/3000/4200");
    */

    //Am241 vs Fe55
    /*    prefixList.push_back("R1226033-R1227081");
    legendList.push_back("Fe55, 1180/2800/4150/6500V");

    prefixList.push_back("R2083046-R2083055B");
    legendList.push_back("Am241, 1200/2800/4000/6000V");
    */

    //Gas Ageing
    /*        prefixList.push_back("R2079057-R2079062");
    legendList.push_back("19th March");
    prefixList.push_back("R2080141-R2080165");
    legendList.push_back("20th March");
    prefixList.push_back("R2081070-R2081084");
    legendList.push_back("21st March");
    prefixList.push_back("R2082009-R2082024");
    legendList.push_back("22nd March");
    prefixList.push_back("R2083022-R2083032");
    legendList.push_back("23rd March");
    */

    //SchemeA
    /*      prefixList.push_back("R2081070-R2081084");
    legendList.push_back("A1 = 1200V");
    prefixList.push_back("R2081085-R2081111");
    legendList.push_back("A1 = 1400V");
    prefixList.push_back("R2081114-R2081128");
    legendList.push_back("A1 = 1600V");
    prefixList.push_back("R2081129-R2081155");
    legendList.push_back("A1 = 1800V");
    prefixList.push_back("R2081165-R2081181");
    legendList.push_back("A1 = 2000V");
    */
    //Scheme B
    /*prefixList.push_back("R2084009-R2084018");
      legendList.push_back("A3 - A2 = 0V");
      prefixList.push_back("R2083126-R2084008");
      legendList.push_back("A3 - A2 = 400V");
      prefixList.push_back("R2083116-R2083125");
      legendList.push_back("A3 - A2 = 800V");
      prefixList.push_back("R2082009-R2082024");
      legendList.push_back("A3 - A2 = 1200V");
      prefixList.push_back("R2082025-R2082039");
      legendList.push_back("A3 - A2 = 1400V");
      prefixList.push_back("R2082040-R2082054");
      legendList.push_back("A3 - A2 = 1600V");
      prefixList.push_back("R2082055-R2082069");
      legendList.push_back("A3 - A2 = 1800V");
      prefixList.push_back("R2082070-R2082084");
      legendList.push_back("A3 - A2 = 2000V");
      prefixList.push_back("R2082085-R2082094");
      legendList.push_back("A3 - A2 = 2200V");
      prefixList.push_back("R2082095-R2082104");
      legendList.push_back("A3 - A2 = 2400V");
      prefixList.push_back("R2082105-R2083001");
      legendList.push_back("A3 - A2 = 2600V");
    */

    //Scheme C
    // prefixList.push_back("R2083106-R2083115");
    // prefixList.push_back("R2083096-R2083105");
    // prefixList.push_back("R2083086-R2083095");
    //    prefixList.push_back("R2083076-R2083085");
    //prefixList.push_back("R2083066-R2083075");
    //        prefixList.push_back("R2083022-R2083032");
    //        prefixList.push_back("R2083035-R2083045");
    //   prefixList.push_back("R2083046-R2083055");
    // prefixList.push_back("R2083056-R2083065");
    //legendList.push_back("A2 - A1 = 200V");
    //legendList.push_back("A2 - A1 = 400V");
    //legendList.push_back("A2 - A1 = 600V");
    //legendList.push_back("A2 - A1 = 800V");
    //legendList.push_back("A2 - A1 = 1000V");
    //legendList.push_back("A2 - A1 = 1200V");
    //legendList.push_back("A2 - A1 = 1400V");
    //legendList.push_back("A2 - A1 = 1600V");
    //legendList.push_back("A2 - A1 = 1800V");
  }

  //Select individual consecutive runs 
  if(UseConsecutiveRuns){
    nameStream << "/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/scripts/bkgVsSignal/comparisonPlots/IndivRuns_R" << firstRun << "_to_R" << lastRun << "_" << fileNameSuffix << "_";
    nameStream >> fileNameFlag;
    int ilastRun = lastRun + 1;
    for(int iRunList = firstRun; iRunList < ilastRun; iRunList ++){
       std::string runNoString;
       std::stringstream runNo;
       runNo << "R" << iRunList << "";
       //std::cout << iRunList << std::endl;
       runNo >> runNoString;
       std::cout << runNoString << std::endl;
       prefixList.push_back(runNoString.c_str());
       legendList.push_back(runNoString.c_str());
    }
  }
  
  //Select runs from a list in a file
  if(UseRunList){
    nameStream << "/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/scripts/bkgVsSignal/comparisonPlots/GroupedRuns_" << fileNameSuffix << "_";
    nameStream >> fileNameFlag; 
    std::ifstream listFile;
    listFile.open("/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/scripts/bkgVsSignal/runsToPlotSchemeA");
    std::string fileLine;
    if(listFile.is_open()){
      std::cout << "listFile is open" << std:: endl;
      while (std::getline(listFile, fileLine)){
	prefixList.push_back(fileLine);
	legendList.push_back(fileLine);
      }
    }
  }
  
  
  //prefixList.push_back("R1341081-R1341093");
  //legendList.push_back("SGL, normal: 1500  3600  7600");
  //prefixList.push_back("1341095 1341137");
  //legendList.push_back("SGL, normal: 1500  3600  7600");

  /*1D plots*/

  //createChargeGain1Dplots(prefixList, legendList, cut, fileNameFlag,"raptorr_tpcdata_gain_%s_noExpSmooth_nowNeg_clean.root");
  //createChargeGain1Dplots(prefixList, legendList, cut, fileNameFlag,"%s_raptorr_tpcdata_gain_PeakTime2_true.root");
  //createChargeGain1Dplots(prefixList, legendList, cut, fileNameFlag,"%s_raptorr_tpcdata_gain_2018_noExpSmooth_nowNeg.root");
  createChargeGain1Dplots(prefixList, legendList, cut, fileNameFlag, "raptorr_tpcdata_gain_%s_noExpSmooth_nowNeg_clean.root", TreeSuffix);  

  //createChargeGain1Dplots(prefixList, legendList, cut, fileNameFlag,"raptorr_tpcdata_gain_%s_Am241.root", TreeSuffix);

  //createChargeGain1Dplots(prefixList, legendList, cut, fileNameFlag,"raptorr_tpcdata_gain_%s_true_Am241.root", TreeSuffix);
  
//createChargeGain1Dplots(prefixList, legendList, cut, fileNameFlag,"raptorr_tpcdata_gain_%s_Am241_CathodeTest_true.root", TreeSuffix);

  //checkMeanAndRMSOfParametersInSingleRuns(prefixList, cut, fileNameFlag, "raptorr_tpcdata_gain_%s_noExpSmooth_nowNeg.root");
  //  checkMeanAndRMSOfParametersInSingleRuns(prefixList, cut, fileNameFlag, "raptorr_tpcdata_gain_%s_Am241.root", "");
  
   std::cout << "Tree Suffix is " << TreeSuffix << std::endl;
   //         checkMeanNoEntriesInSingleRuns(prefixList, cut, fileNameFlag, "raptorr_tpcdata_gain_%s_true_Am241.root", TreeSuffix);  
  
/*2D plots*/
  //createChargeGainSummed2Dplots(prefixList, cut, fileNameFlag, "%s_raptorr_tpcdata_gain_PeakTime2_true.root");
   //createChargeGainSummed2Dplots(prefixList, cut, fileNameFlag, "raptorr_tpcdata_gain_%s_true_Am241.root", TreeSuffix);

  /*more correlations*/
  TString nameOfSomething = "BaselineAnode";
  TString labelOfSomething = "baseline anode %i [mV]";
  double somethingMin = -400; 
  double somethingMax = 1600;
  double binWidthSomething = 1; 
  Bool_t crossCorrelate = false;

  //correlatedSomethingWithAmpsAndNegAmps(nameOfSomething, labelOfSomething, somethingMin, somethingMax, binWidthSomething, crossCorrelate, prefixList, cut);



  // done
  return;
}













// actual bkg vs signal
void bkgVsSignal(TString cut = "", TString fileNameSuffix = "", TString runsToPlotBkg = "", TString runsToPlotSgl = "", TString TreeSuffix = "_anode3"){
  std::vector<TString> sglPrefixList;
  std::vector<TString> sglLegendList;

  std::vector<TString> bkgPrefixList;
  std::vector<TString> bkgLegendList;

  TString fileNameFlag;
  std::stringstream nameStream;

  bool UseRunList = false;
  bool UseSpecificRuns = true;

  if(UseSpecificRuns){
    nameStream << "/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/scripts/bkgVsSignal/comparisonPlots/BkgVsSglSpecificRuns_" << fileNameSuffix << "_";
    nameStream >> fileNameFlag;    


    //    bkgPrefixList.push_back("R1226033-R1227081");
    //bkgLegendList.push_back("Fe55");
    //sglPrefixList.push_back("R2083046-R2083055");
    //sglLegendList.push_back("Am241");

    //Bias Frame Subtraction                                                                                                                       
    bkgPrefixList.push_back("R2081183-R2081211");
    bkgLegendList.push_back("OV runs");
    sglPrefixList.push_back("R2081070-R2081084");
    sglLegendList.push_back("Scheme A Config 1");



  }




  //Select runs from a list in a file                                                                                                                  
  if(UseRunList){
    nameStream << "/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/scripts/bkgVsSignal/comparisonPlots/BkgVsSglGroupedRuns_" << fileNameSuffix << "_";
    nameStream >> fileNameFlag;
    std::ifstream listFileBkg;
    std::stringstream fileNameStreamBkg;
    std::string fileNameBkg;
    fileNameStreamBkg << "/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/scripts/bkgVsSignal/runstoPlot" << runsToPlotBkg;
    fileNameStreamBkg >> fileNameBkg;
    std::cout << fileNameBkg << std::endl; 
    listFileBkg.open(fileNameBkg);
    std::string fileLineBkg;
    if(listFileBkg.is_open()){
      std::cout << "listFileBkg is open" << std:: endl;
      while (std::getline(listFileBkg, fileLineBkg)){
        bkgPrefixList.push_back(fileLineBkg);
        bkgLegendList.push_back(runsToPlotBkg);
      }
    }
    std::ifstream listFileSgl;
    std::stringstream fileNameStreamSgl;
    std::string fileNameSgl;
    fileNameStreamSgl << "/scratch0/hrichie/hptpc_ritchie/HPTPC/code/raptorrlinapp2/raptorr/scripts/bkgVsSignal/runstoPlot" << runsToPlotSgl;
    fileNameStreamSgl >> fileNameSgl;
    std::cout << fileNameSgl <<std::endl;
    listFileSgl.open(fileNameSgl);
    std::string fileLineSgl;
    if(listFileSgl.is_open()){
      std::cout << "listFileSgl is open" << std:: endl;
      while (std::getline(listFileSgl, fileLineSgl)){
        sglPrefixList.push_back(fileLineSgl);
        sglLegendList.push_back(runsToPlotSgl);
      }
    }
    else{
      std::cout << "File is not open everything is terrible I hate you why have you done this I can't believe you did this to me!" << std::endl; 
    }
  }

  
  //bkgPrefixList.push_back("R1352091-R1353014");
  /*
    1352095
    Good to know: 
    _______________
    Anode 1 Baseline: 0.619263 and RMS: 3.67559
    Anode 2 Baseline: -3.15966 and RMS: 3.14929
    Anode 3 Baseline: -0.674549 and RMS: 7.02353
    -> 50% signals (17/30) and 50% (8/17) of that negative signals in anode 2
    -> anode 2 and anode 3 baseline centred around zero
    -> oscillations about 20 mVpp (anode 2 - 3 in the direct recorded waveforms, but swapped)  and 5 mVpp (anode 3 - 2 in the direct recorded waveforms, but swapped)
  */
  //bkgLegendList.push_back("BKG, swapped: 1500  3600  7400");
  //bkgPrefixList.push_back("R1354007-R1354017");
  /*
    1354009
    Good to know: 
    _______________
    Anode 1 Baseline: -1.2877 and RMS: 3.14567
    Anode 2 Baseline: -4.66731 and RMS: 7.75475
    Anode 3 Baseline: -1.31762 and RMS: 3.01437

    -> a bit more than 1/3 signals (12/30) no negative amplitudes
    -> anode 2 and anode 3 baseline centred around zero
    -> oscillations about 20 mVpp (...)  and 5 mVpp (anode 3)
  */
  //bkgLegendList.push_back("BKG, normal: 1500  3600  7400");
  //bkgPrefixList.push_back("R1354001-R1354006");
  //bkgLegendList.push_back("BKG, swapped: 1500  3600  7600");
  //bkgPrefixList.push_back("R1354018-R1354029");
  /*
    1354020
    Good to know: 
    _______________
    Anode 1 Baseline: -0.207559 and RMS: 2.94913
    Anode 2 Baseline: -3.15337 and RMS: 6.72412
    Anode 3 Baseline: -1.24704 and RMS: 2.41672
  */
  //bkgLegendList.push_back("BKG, normal: 1500  3600  7600");

  // sglPrefixList.push_back("R1340169-R1341008");
  /*  1340171
    Good to know: 
    _______________
    Anode 1 Baseline: -1.11843 and RMS: 3.75743
    Anode 2 Baseline: -4.1761 and RMS: 7.3025
    Anode 3 Baseline: -1.45965 and RMS: 3.01661
  */
  /*sglLegendList.push_back("SGL, normal: 1500  3600  7400");
  sglPrefixList.push_back("R1340169-R1341008");
  sglLegendList.push_back("SGL, normal: 1500  3600  7400");
  sglPrefixList.push_back("R1341081-R1341093");
  sglLegendList.push_back("SGL, normal: 1500  3600  7600");
  sglPrefixList.push_back("R1341095-R1341137");
  sglLegendList.push_back("SGL, normal: 1500  3600  7600");
  */
  /*
  bkgPrefixList.push_back("R1351004-R1351034");
  bkgLegendList.push_back("R1351004-R1351034 1500/3600/6800");
  bkgPrefixList.push_back("R1351059-R1351079");
  bkgLegendList.push_back("R1351059-R1351079 1500/3600/6900");
  bkgPrefixList.push_back("R1351081-R1352013");
  bkgLegendList.push_back("R1351081-R1352013 1500/3600/7000");
  bkgPrefixList.push_back("R1352014-R1352037");
  bkgLegendList.push_back("R1352014-R1352037 1500/3600/7100");
  bkgPrefixList.push_back("R1352014-R1352037");
  bkgLegendList.push_back("R1352014-R1352037 1500/3600/7100");
  bkgPrefixList.push_back("R1351039-R1351059");
  bkgLegendList.push_back("R1351039-R1351059 1500/3600/7200");

  sglPrefixList.push_back("R1339001-R1339021");
  sglLegendList.push_back("R1339001-R1339021 1500/3600/6800 ^{55}Fe");
  sglPrefixList.push_back("R1339022-R1339043");
  sglLegendList.push_back("R1339022-R1339043 1500/3600/6900 ^{55}Fe");
  sglPrefixList.push_back("R1339044-R1339063");
  sglLegendList.push_back("R1339044-R1339063 1500/3600/7000 ^{55}Fe");
  sglPrefixList.push_back("R1339064-R1340004");
  sglLegendList.push_back("R1339064-R1340004 1500/3600/7100 ^{55}Fe");
  sglPrefixList.push_back("R1344029-R1344058");
  sglLegendList.push_back("R1344029-R1344058 1500/3600/7100 ^{55}Fe");
  sglPrefixList.push_back("R1340121-R1340141");
  sglLegendList.push_back("R1340121-R1340141 1500/3600/7200 ^{55}Fe");*/

  /*1D plots*/
  // createChargeGain1Dplots(sglPrefixList, sglLegendList, cut, "sgl_");
  //createChargeGain1Dplots(bkgPrefixList, bkgLegendList, cut, "bkg_");
  //  createChargeGain1Dplots(bkgPrefixList, sglPrefixList, bkgLegendList, sglLegendList, cut, fileNameFlag,"raptorr_tpcdata_gain_%s_noExpSmooth_nowNeg_clean.root");
  createChargeGain1Dplots(bkgPrefixList, sglPrefixList, bkgLegendList, sglLegendList, cut, fileNameFlag,"raptorr_tpcdata_gain_%s_true_Am241.root", TreeSuffix);
  std::cout << "Tree Suffix is " << TreeSuffix << std::endl;
  /*2D plots*/
  //createChargeGainSummed2Dplots(sglPrefixList, cut, fileNameFlag);
  //createChargeGainSummed2Dplots(bkgPrefixList, cut, fileNameFlag);




  // done
  return;
}
