
#include <string>
#include <math.h>
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

void plotPeakPositionPreampAdam(int i = 1, std::string plottoplot= "noplot", bool RealVolt = true,  double cut_here = 0.3, double min_cut = 0){
   TCanvas* c1 = new TCanvas("c1","Plots for peak position",300,50,800,700);
   // c1->SetGrid();
   std::vector<int> runNumbers;
   std::vector<std::string> resultFile;
   std::vector<std::string> SignalFile;
   std::vector<double> pulseSize;

   //Find files to plot    
   int j = 6; 

   std::string coup = "1";
   int couple = 1;

   /*   resultFile.push_back(Form("/scratch4/atarrant/hptpc_and_cmos_guerilla_code/plotting/AmplitudeAnode%ipeakschi_raptorr_tpcdata_gain_R1318034_noExpSmooth.root.dat",i));
   resultFile.push_back(Form("/scratch4/atarrant/hptpc_and_cmos_guerilla_code/plotting/AmplitudeAnode%ipeakschi_raptorr_tpcdata_gain_R1318035_noExpSmooth.root.dat",i));
   resultFile.push_back(Form("/scratch4/atarrant/hptpc_and_cmos_guerilla_code/plotting/AmplitudeAnode%ipeakschi_raptorr_tpcdata_gain_R1318036_noExpSmooth.root.dat",i));
   resultFile.push_back(Form("/scratch4/atarrant/hptpc_and_cmos_guerilla_code/plotting/AmplitudeAnode%ipeakschi_raptorr_tpcdata_gain_R1318037_noExpSmooth.root.dat",i));
   resultFile.push_back(Form("/scratch4/atarrant/hptpc_and_cmos_guerilla_code/plotting/AmplitudeAnode%ipeakschi_raptorr_tpcdata_gain_R1318038_noExpSmooth.root.dat",i));
   resultFile.push_back(Form("/scratch4/atarrant/hptpc_and_cmos_guerilla_code/plotting/AmplitudeAnode%ipeakschi_raptorr_tpcdata_gain_R1318039_noExpSmooth.root.dat",i));
   */

   std::vector<std::string> runNumber;
   runNumber.push_back("1319037");
   runNumber.push_back("1319037");
   runNumber.push_back("1319040");
   runNumber.push_back("1319041");
   runNumber.push_back("1319042");
   runNumber.push_back("1319043");
   runNumber.push_back("1319045");
   runNumber.push_back("1319046");
   runNumber.push_back("1319052");
   for(int runi = 0; runi < runNumber.size(); runi ++){ 
      stringstream filePath;
      filePath << "/scratch0/hrichie/hptpc_ritchie/HPTPC/data/preampData/2018/fitted_Spectra/AmplitudeAnode3parameters_raptorr_tpcdata_gain_R" << runNumber[runi] <<  "_expSmooth.root.dat";
      resultFile.push_back(filePath.str());
   }












   if(RealVolt==true){

     SignalFile.push_back("/scratch4/atarrant/hptpc_and_cmos_guerilla_code/plotting/AmplitudeAnode1peakschi_100mv.root.dat");
     SignalFile.push_back("/scratch4/atarrant/hptpc_and_cmos_guerilla_code/plotting/AmplitudeAnode1peakschi_150mv.root.dat");
     SignalFile.push_back("/scratch4/atarrant/hptpc_and_cmos_guerilla_code/plotting/AmplitudeAnode1peakschi_200mv.root.dat");
     SignalFile.push_back("/scratch4/atarrant/hptpc_and_cmos_guerilla_code/plotting/AmplitudeAnode1peakschi_250mv.root.dat");
     SignalFile.push_back("/scratch4/atarrant/hptpc_and_cmos_guerilla_code/plotting/AmplitudeAnode1peakschi_300mv.root.dat");
     SignalFile.push_back("/scratch4/atarrant/hptpc_and_cmos_guerilla_code/plotting/AmplitudeAnode1peakschi_350mv.root.dat");
     SignalFile.push_back("/scratch4/atarrant/hptpc_and_cmos_guerilla_code/plotting/AmplitudeAnode1peakschi_400mv.root.dat");
     SignalFile.push_back("/scratch4/atarrant/hptpc_and_cmos_guerilla_code/plotting/AmplitudeAnode1peakschi_450mv.root.dat");
     SignalFile.push_back("/scratch4/atarrant/hptpc_and_cmos_guerilla_code/plotting/AmplitudeAnode1peakschi_500mv.root.dat");
     SignalFile.push_back("/scratch4/atarrant/hptpc_and_cmos_guerilla_code/plotting/AmplitudeAnode1peakschi_550mv.root.dat");
     SignalFile.push_back("/scratch4/atarrant/hptpc_and_cmos_guerilla_code/plotting/AmplitudeAnode1peakschi_600mv.root.dat");
     SignalFile.push_back("/scratch4/atarrant/hptpc_and_cmos_guerilla_code/plotting/AmplitudeAnode1peakschi_700mv.root.dat");
     SignalFile.push_back("/scratch4/atarrant/hptpc_and_cmos_guerilla_code/plotting/AmplitudeAnode1peakschi_800mv.root.dat");

     std::vector<double> Peaks;
     std::vector<double> peakErrors;
     std::vector<double> Chis;
     std::vector<double> Ndfs;
     std::vector<double> ChisNdfs;
     std::vector<double> scaledpeakErrors;

     std::cout << "The signal Data" << std::endl;
     //Get peak parameters from file
     for(int iResults1 = 0; iResults1 < SignalFile.size(); iResults1++){
       std::ifstream file1;
       file1.open(SignalFile[iResults1].c_str());
       if(file1.is_open()){
	 double peak, peakerr, chi, ndf;
	 file1 >> peak >> peakerr >> chi >> ndf;
	 std::cout << peak << " " << peakerr << " " << chi << " " << ndf << std::endl;
	 pulseSize.push_back(peak);
	 peakErrors.push_back(peakerr);
	 Chis.push_back(chi);
	 Ndfs.push_back(ndf);
       }
       else{ std::cout << "Cannot open peak file "<< SignalFile[iResults] << std::endl;}
     }


     //Scale Errors by Chisquare/NDF
     for(int chisub = 0; chisub < Chis.size(); chisub++){
       for(int ndfsub = 0; ndfsub < Ndfs.size(); ndfsub++){
	 if(chisub == ndfsub){
	   if((!Ndfs[ndfsub]) > 0){
	     std::cout << "ndf is bad :(" << std::endl;

	     ChisNdfs.push_back(Chis[chisub]);
	   }
	   else{
	     ChisNdfs.push_back(Chis[chisub]/ Ndfs[chisub]);
	   }
	 }
       }
     }
     for(int errorsuba = 0; errorsuba < peakErrors.size(); errorsuba++){
       for(int errorsubb = 0; errorsubb < ChisNdfs.size(); errorsubb++){
	 if(errorsuba == errorsubb){
	   //scaledpeakErrors.push_back(peakErrors[errorsuba] * ChisNdfs[errorsubb]);
	   scaledpeakErrors.push_back((sqrt(pow(peakErrors[errorsuba],2)) * ChisNdfs[errorsubb])/10);
	 }
       }
     }
      
   }

   else{

     /*
     pulseSize.push_back(100e-03);
     pulseSize.push_back(200e-03);
     pulseSize.push_back(300e-03);
     pulseSize.push_back(400e-03);
     pulseSize.push_back(500e-03);
     pulseSize.push_back(600e-03);
     */  pulseSize.push_back(700e-03);
     pulseSize.push_back(800e-03);
     pulseSize.push_back(900e-03);
     pulseSize.push_back(1000e-03);
     pulseSize.push_back(1200e-03);
     pulseSize.push_back(1400e-03);
     pulseSize.push_back(1600e-03);
     pulseSize.push_back(1800e-03);
     
/*
     pulseSize.push_back(0.1);
     pulseSize.push_back(0.15);
     pulseSize.push_back(0.2);
     pulseSize.push_back(0.25);
     pulseSize.push_back(0.3);
     pulseSize.push_back(0.35);
     pulseSize.push_back(0.4);
     pulseSize.push_back(0.45);
     pulseSize.push_back(0.5);
     pulseSize.push_back(0.55);
     pulseSize.push_back(0.6);
     pulseSize.push_back(0.7);
     pulseSize.push_back(0.8);
     */
     for(int l = 0 ; l<j; l++){
       //       scaledpeakErrors.push_back(0);
     }
   }

   runNumbers.push_back(1312065);
   runNumbers.push_back(1312066);


// data storage
   // ... more vectors
   std::vector<double> Peaks1;
   std::vector<double> peakErrors1;
   std::vector<double> Chis1;
   std::vector<double> Ndfs1;
   std::vector<double> ChisNdfs4;
   std::vector<double> scaledpeakErrors1;
   std::cout << "The  amplified data" << std::endl;   

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
	 if((!Ndfs1[ndfsub1]) > 0){
	   std::cout << "ndf is bad :(" << std::endl;
	   
	   ChisNdfs4.push_back(Chis1[chisub1]); 
	 }
	 else{	   
	   ChisNdfs4.push_back(Chis1[chisub1]/ Ndfs1[chisub1]); 
	 }
       }
     }
   }
   for(int errorsub1a = 0; errorsub1a < peakErrors1.size(); errorsub1a++){
     for(int errorsub1b = 0; errorsub1b < ChisNdfs4.size(); errorsub1b++){
       if(errorsub1a == errorsub1b){

	 scaledpeakErrors1.push_back((sqrt(pow(peakErrors1[errorsub1a],2))* ChisNdfs4[errorsub1b])/10); //(peakErrors1[errorsub1a] * ChisNdfs4[errorsub1b]));
       }
     }
   }

   // create the TGraph, draw it, and save it

gr = new TGraphErrors(j,&(pulseSize[0]),&(Peaks1[0]),0,&(scaledpeakErrors1[0]));

   for(int m = 0; m<=j; m++){
     std::cout << "What it is plotting " << scaledpeakErrors1[m] << " " << pulseSize[m] << " " << Peaks1[m] << std::endl;   
   }

   stringstream plotname;
   plotname << "Signal Peak Amplitude verse Amplified Peak Ampltiude " << plottoplot;
   gr->SetTitle(plotname.str().c_str());
   c1->SetLeftMargin(0.12);
   c1->SetBottomMargin(0.125);
   gStyle->SetOptStat(0);

   gr->SetMarkerColor(4);
   gr->SetMarkerStyle(21);
 
   stringstream xaxisname;
   xaxisname << plottoplot << " (V)";
 
   gr->Fit("pol1","","", min_cut, cut_here);
  
   gr->SetTitle(Form("Signal Peak Amplitude verse Amplified Peak Ampltiude of HPTPC anode preamps. Coupled anode %i, recordered anode %i. Vessel Evacuated, N0 PREAMPS", couple, i) );
    //    gr->SetAxisRange(0.0, 2.00);
    //gr->GetXaxis()->SetLimits(0.0, 1.0);
   
   if(RealVolt == true || RealVolt == false){ 
     
     gr->GetXaxis()->SetTitle("Signal Peak Amplitude (V)");//xaxisname.str().c_str());
     gr->GetYaxis()->SetTitle("Amplifed Peak Amplitude (mV)");
     
   } 

   else{

     gr->GetXaxis()->SetTitle("Set Voltage of Signal Generator (V)");//xaxisname.str().c_str());
     gr->GetYaxis()->SetTitle("Mean Ampltuide (mV)");
   }

   gr->GetXaxis()->SetTitleSize(0.06);
   gr->GetXaxis()->SetTitleOffset(0.85);
   gr->GetYaxis()->SetTitleSize(0.06);
   gr->GetYaxis()->SetTitleOffset(0.95);

   gr->Draw("AP");

   // std::string couple = std::to_string(coup);
   c1->Print(("preamp_Cailbration_coupledAN"+coup+"_readAN_"+plottoplot+".pdf").c_str());
   c1->Print(("preamp_Cailbration_coupledAn"+coup+"_readAN_"+plottoplot+".root").c_str());
   c1->Update();

   }

