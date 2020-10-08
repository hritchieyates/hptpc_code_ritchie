#include <iostream>
#include <memory>
#include <vector>

#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLegend.h"
#include "TPad.h"
#include "TStyle.h"
#include "TTree.h"

/** lower level functions used by the functions below **/ 

// allows to shift all bin contents in a histogram by a given number of bins (not yet tested and not used below)
TH1D* shiftBins(TH1D* input_histo, int bin_shift, int nb_of_bins, double x_min, double x_max){
  // define the output histogram
  TH1D* shiftedHiatogram = new TH1D("tmp", "tmp", nb_of_bins, x_min, x_max);

  //
  for(Int_t iFillBins = 0; iFillBins < input_histo->GetSize()-2-bin_shift; iFillBins++){
    double binContent = input_histo->GetBinContent(iFillBins);
    shiftedHiatogram->SetBinContent(iFillBins + bin_shift, binContent);
  }
  TString histoName = "%s_shift";
  shiftedHiatogram->SetName(Form(histoName, input_histo->GetName()));
  shiftedHiatogram->SetTitle(Form(histoName, input_histo->GetTitle()));

  return shiftedHiatogram;
}

// returns a histogram from a TTree - 1D version
TH1D* returnHistogram(TString histo_name, TString x_axis_name, TString y_axis_name, Int_t nb_of_bins, Double_t min, Double_t max){
  TH1D* newHisto = new TH1D(histo_name, histo_name, nb_of_bins, min, max);
  newHisto->GetYaxis()->SetTitleSize(0.055);
  newHisto->GetYaxis()->SetTitleOffset(0.95);
  newHisto->GetXaxis()->SetTitleSize(0.055);
  newHisto->GetXaxis()->SetTitleOffset(0.85);
  newHisto->GetXaxis()->SetTitle(x_axis_name);
  newHisto->GetYaxis()->SetTitle(y_axis_name);

  return newHisto;
}
// returns a histogram from a TTree - 2D version
TH2D* returnHistogram(TString histo_name, TString x_axis_name, TString y_axis_name, Int_t nb_of_x_bins, Double_t x_min, Double_t x_max, Int_t nb_of_y_bins, Double_t y_min, Double_t y_max){
  TH2D* newHisto = new TH2D(histo_name, histo_name, nb_of_x_bins, x_min, x_max, nb_of_y_bins, y_min, y_max);
  newHisto->GetYaxis()->SetTitleSize(0.055);
  newHisto->GetYaxis()->SetTitleOffset(0.95);
  newHisto->GetXaxis()->SetTitleSize(0.055);
  newHisto->GetXaxis()->SetTitleOffset(0.85);
  newHisto->GetXaxis()->SetTitle(x_axis_name);
  newHisto->GetYaxis()->SetTitle(y_axis_name);

  return newHisto;
}

// gets the number of runs and events in a run from the chargeGainAnalysisTree
void getRunNumberAndEventNumberLists(TFile* input_file, std::vector<int> & run_numbers, std::vector<int> & nb_of_events){
  run_numbers.clear();
  nb_of_events.clear();

  std::vector<int>* allRunNumbers = 0;
  std::vector<int>* allEventNumbers = 0;
  std::vector<int> tmpRunNumbers;
  std::vector<int> tmpEventNumbers;

  TTree* chargeAnalysisTree = (TTree*)input_file->Get("chargeGainAnalysisTree");
  if(chargeAnalysisTree){
    chargeAnalysisTree->SetBranchAddress("EventNo", &allEventNumbers);
    chargeAnalysisTree->SetBranchAddress("RunNo", &allRunNumbers);

    for(int iTreeEntries = 0; iTreeEntries < chargeAnalysisTree->GetEntries(); iTreeEntries++){
      chargeAnalysisTree->GetEvent(iTreeEntries);

      std::vector<int> runNumbersStillToCheck;
      std::vector<int> eventNumbersStillToCheck;

      runNumbersStillToCheck = *allRunNumbers;
      eventNumbersStillToCheck = *allEventNumbers;

      while (runNumbersStillToCheck.size() > 0){
        // copy:
        tmpRunNumbers.clear();
        tmpRunNumbers = runNumbersStillToCheck;
        tmpEventNumbers.clear();
        tmpEventNumbers = eventNumbersStillToCheck;
        // clear:
        runNumbersStillToCheck.clear();
        eventNumbersStillToCheck.clear();

        // check and fill again
        int currentRunNbToCheck = 0;
        int currentMaxEvent = 0;
        for(unsigned int iRunNumberId = 0; iRunNumberId < tmpRunNumbers.size(); iRunNumberId++){
          //std::cout << tmpRunNumbers.at(iRunNumberId) << " ";
          if (iRunNumberId == 0) {
            currentRunNbToCheck = tmpRunNumbers.at(0);
            currentMaxEvent = tmpEventNumbers.at(0);
          }
          else{
            if (currentRunNbToCheck == tmpRunNumbers.at(iRunNumberId)){
              if (currentMaxEvent  <  tmpEventNumbers.at(iRunNumberId)) {
              currentMaxEvent = tmpEventNumbers.at(iRunNumberId);
              }
            }
            else{
              runNumbersStillToCheck.push_back(tmpRunNumbers.at(iRunNumberId));
              eventNumbersStillToCheck.push_back(tmpEventNumbers.at(iRunNumberId));
            }
          }
        }
        //std::cout << std::endl;
        //std::cout << currentRunNbToCheck << "\t" << currentMaxEvent  << std::endl;

        // push back result
        run_numbers.push_back(currentRunNbToCheck);
        nb_of_events.push_back(currentMaxEvent);
      }// while search loop
    }//for loop over tree entries
  }//if tree exists


  // done 
  return;
}

// creates a vector of 1D plots from a given list of files - one plot for each file
std::vector<std::vector<TH1D*> > plotList(std::vector<TFile*> input_files, std::vector<TString> prefix_list, TString branch, TString cut, TString x_axis_name, TString y_axis_name, Int_t nb_of_bins, Double_t min, Double_t max){
  std::vector<std::vector<TH1D*> > returnHistograms;

  for(unsigned int iFile = 0; iFile < input_files.size(); iFile++){
    std::vector<TH1D*> currentSetOfHistograms;
    input_files[iFile]->cd();
  
    TH1D *sgl_amplitudeAnode1 = returnHistogram(prefix_list[iFile]+"_"+branch+Form("%i", 1), x_axis_name, y_axis_name, nb_of_bins, min, max);
    TH1D *sgl_amplitudeAnode2 = returnHistogram(prefix_list[iFile]+"_"+branch+Form("%i", 2), x_axis_name, y_axis_name, nb_of_bins, min, max);
    TH1D *sgl_amplitudeAnode3 = returnHistogram(prefix_list[iFile]+"_"+branch+Form("%i", 3), x_axis_name, y_axis_name, nb_of_bins, min, max);

    TTree* sgl_chargeAnalysisTree = (TTree*)input_files[iFile]->Get("chargeGainAnalysisTree");
    if(!sgl_chargeAnalysisTree){
      std::cout << "chargeGainAnalysisTree not found" << std::endl;
      input_files[iFile]->Close();
      return returnHistograms;
    }

    sgl_chargeAnalysisTree->Draw(branch+Form("%i >> ", 1)+prefix_list[iFile]+"_"+branch+Form("%i", 1), cut, "goff");
    //std::cout << branch+Form("%i >> ", 1)+prefix_list[iFile]+"_"+branch+Form("%i", 1) << std::endl;
    //std::cout << sgl_amplitudeAnode1->GetEntries() << std::endl;
    sgl_amplitudeAnode1->SetDirectory(0);
    currentSetOfHistograms.push_back(sgl_amplitudeAnode1);
    sgl_chargeAnalysisTree->Draw(branch+Form("%i >> ", 2)+prefix_list[iFile]+"_"+branch+Form("%i", 2), cut, "goff");
    //std::cout << branch+Form("%i >> ", 2)+prefix_list[iFile]+"_"+branch+Form("%i", 2) << std::endl;
    //std::cout << sgl_amplitudeAnode2->GetEntries() << std::endl;
    sgl_amplitudeAnode2->SetDirectory(0);
    currentSetOfHistograms.push_back(sgl_amplitudeAnode2);
    sgl_chargeAnalysisTree->Draw(branch+Form("%i >> ", 3)+prefix_list[iFile]+"_"+branch+Form("%i", 3), cut, "goff");
    //std::cout << branch+Form("%i >> ", 3)+prefix_list[iFile]+"_"+branch+Form("%i", 3) << std::endl;
    //std::cout << sgl_amplitudeAnode3->GetEntries() << std::endl;
    sgl_amplitudeAnode3->SetDirectory(0);
    currentSetOfHistograms.push_back(sgl_amplitudeAnode3);

    returnHistograms.push_back(currentSetOfHistograms);
  }

  // done
  return returnHistograms;
}

// creates a vector of 2D plots from a given list of files - data from the different files get summed to produce one plot
std::vector<TH2D*> addPlotList2D(std::vector<TFile*> input_files, std::vector<TString> prefix_list, TString x_branch, TString y_branch, TString cut, TString x_axis_name, TString y_axis_name, Int_t nb_of_x_bins, Double_t x_min, Double_t x_max, Int_t nb_of_y_bins, Double_t y_min, Double_t y_max){

  std::vector<TH2D*> returnHistograms;
  TH2D *plotAnode1vsAnode2 = returnHistogram(y_branch+Form("%i", 1)+"vs"+x_branch+Form("%i", 2), x_branch+x_axis_name, y_branch+y_axis_name, nb_of_x_bins, x_min, x_max, nb_of_y_bins, y_min, y_max);
  plotAnode1vsAnode2->SetDirectory(0);
  TH2D *plotAnode2vsAnode3 = returnHistogram(y_branch+Form("%i", 2)+"vs"+x_branch+Form("%i", 3), x_branch+x_axis_name, y_branch+y_axis_name, nb_of_x_bins, x_min, x_max, nb_of_y_bins, y_min, y_max);
  plotAnode2vsAnode3->SetDirectory(0);
  TH2D *plotAnode3vsAnode1 = returnHistogram(y_branch+Form("%i", 3)+"vs"+x_branch+Form("%i", 1), x_branch+x_axis_name, y_branch+y_axis_name, nb_of_x_bins, x_min, x_max, nb_of_y_bins, y_min, y_max);
  plotAnode3vsAnode1->SetDirectory(0);

  for(unsigned int iFile = 0; iFile < input_files.size(); iFile++){
    input_files[iFile]->cd();
    TString currentPlotName1vs2 = prefix_list[iFile]+"_"+x_branch+Form("%i", 1)+"vs"+y_branch+Form("%i", 2);
    TH2D *currentPlotAnode1vsAnode2 = returnHistogram(currentPlotName1vs2, x_axis_name, y_axis_name, nb_of_x_bins, x_min, x_max, nb_of_y_bins, y_min, y_max);
    TString currentPlotName2vs3 = prefix_list[iFile]+"_"+x_branch+Form("%i", 2)+"vs"+y_branch+Form("%i", 3);
    TH2D *currentPlotAnode2vsAnode3 = returnHistogram(currentPlotName2vs3, x_axis_name, y_axis_name, nb_of_x_bins, x_min, x_max, nb_of_y_bins, y_min, y_max);
    TString currentPlotName3vs1 = prefix_list[iFile]+"_"+x_branch+Form("%i", 3)+"vs"+y_branch+Form("%i", 1);
    TH2D *currentPlotAnode3vsAnode1 = returnHistogram(currentPlotName3vs1, x_axis_name, y_axis_name, nb_of_x_bins, x_min, x_max, nb_of_y_bins, y_min, y_max);
  
    TTree* chargeAnalysisTree = (TTree*)input_files[iFile]->Get("chargeGainAnalysisTree");
    if(!chargeAnalysisTree){
      std::cout << "chargeGainAnalysisTree not found" << std::endl;
      input_files[iFile]->Close();
      return returnHistograms;
    }

    chargeAnalysisTree->Draw(x_branch+Form("%i:", 1)+y_branch+Form("%i >> ", 2)+currentPlotName1vs2, cut, "goff");
    //std::cout << x_branch+Form("%i:", 1)+y_branch+Form("%i >> ", 2)+currentPlotName1vs2 << std::endl;
    //std::cout << currentPlotAnode1vsAnode2->GetEntries() << std::endl;
    currentPlotAnode1vsAnode2->SetDirectory(0);
    plotAnode1vsAnode2->Add(currentPlotAnode1vsAnode2);
    chargeAnalysisTree->Draw(x_branch+Form("%i:", 2)+y_branch+Form("%i >> ", 3)+currentPlotName2vs3, cut, "goff");
    //std::cout << x_branch+Form("%i:", 2)+y_branch+Form("%i >> ", 2)+currentPlotName1vs2 << std::endl;
    //std::cout << currentPlotAnode2vsAnode3->GetEntries() << std::endl;
    currentPlotAnode2vsAnode3->SetDirectory(0);
    plotAnode2vsAnode3->Add(currentPlotAnode2vsAnode3);
    chargeAnalysisTree->Draw(x_branch+Form("%i:", 3)+y_branch+Form("%i >> ", 1)+currentPlotName3vs1, cut, "goff");
    //std::cout << x_branch+Form("%i:", 3)+y_branch+Form("%i >> ", 2)+currentPlotName1vs2 << std::endl;
    //std::cout << currentPlotAnode3vsAnode1->GetEntries() << std::endl;
    currentPlotAnode3vsAnode1->SetDirectory(0);
    plotAnode3vsAnode1->Add(currentPlotAnode3vsAnode1);
  }

  returnHistograms.push_back(plotAnode1vsAnode2);
  returnHistograms.push_back(plotAnode2vsAnode3);
  returnHistograms.push_back(plotAnode3vsAnode1);

  // done
  return returnHistograms;
}

// open files and check for the number of runs and number of events in the different files
void returnFileAndNumberLists(std::vector<TString> prefix_list, std::vector<TString> legend_list, TString input_file_base, std::vector<TFile*> & input_files, std::vector<TString> & final_prefix_list, std::vector<TString> & final_legend_list, std::vector<int> & run_number_count, std::vector<int> & event_count){

  for(unsigned int iOpenFiles = 0; iOpenFiles < prefix_list.size(); iOpenFiles++){
    TString currentFileName = Form(input_file_base, (static_cast<std::string>(prefix_list[iOpenFiles])).c_str());
    TFile *currentFile = TFile::Open(currentFileName);
    if (currentFile) {
      std::cout << currentFileName << " open" << std::endl;
      TTree* chargeAnalysisTree = (TTree*)currentFile->Get("chargeGainAnalysisTree");
      if (chargeAnalysisTree) {
        final_prefix_list.push_back(prefix_list[iOpenFiles]);
        final_legend_list.push_back(legend_list[iOpenFiles]);
        input_files.push_back(currentFile);

        // check on the numbers of run per file and on the number of events
        std::vector<int> runNumbers;
        std::vector<int> eventNumbers;
        int eventCounter = 0;
        getRunNumberAndEventNumberLists(currentFile, runNumbers, eventNumbers);

        for(unsigned int iRunNumberAccounting = 0; iRunNumberAccounting < runNumbers.size(); iRunNumberAccounting++){
          // + 1 currently, because runs start with event number 0 
          eventCounter += eventNumbers[iRunNumberAccounting] + 1;
        }
        run_number_count.push_back(static_cast<int>(runNumbers.size()));
        event_count.push_back(eventCounter);
      }
      else{
          std::cout << " SORRY, " << prefix_list[iOpenFiles] << std::endl;
      }
    }
    else{
      std::cout << " one file not found, " << prefix_list[iOpenFiles] << std::endl;
    }
  }

  // done
  return;
}
// open files and check for the number of runs and number of events in the different files
// while doing so, background (bkg) and signal (sgl) lists are checked in parallel and information is only retained when bkg and sgl files exist
void returnFileAndNumberLists(std::vector<TString> prefix_list_bkg, std::vector<TString> legend_list_bkg, TString input_file_base_bkg, std::vector<TFile*> & input_files_bkg, std::vector<TString> & final_prefix_list_bkg, std::vector<TString> & final_legend_list_bkg, std::vector<int> & run_number_count_bkg, std::vector<int> & event_count_bkg, std::vector<TString> prefix_list_sgl, std::vector<TString> legend_list_sgl, TString input_file_base_sgl, std::vector<TFile*> & input_files_sgl, std::vector<TString> & final_prefix_list_sgl, std::vector<TString> & final_legend_list_sgl, std::vector<int> & run_number_count_sgl, std::vector<int> & event_count_sgl){

  for(unsigned int iOpenFiles = 0; iOpenFiles < prefix_list_sgl.size() && iOpenFiles < prefix_list_bkg.size(); iOpenFiles++){
    TString currentSglFileName = Form(input_file_base_sgl, (static_cast<std::string>(prefix_list_sgl[iOpenFiles])).c_str());
    TString currentBkgFileName = Form(input_file_base_bkg, (static_cast<std::string>(prefix_list_bkg[iOpenFiles])).c_str());

    TFile *currentSglFile = TFile::Open(currentSglFileName);
    TFile *currentBkgFile = TFile::Open(currentBkgFileName);
    if (currentSglFile && currentBkgFile) {
      TTree* sglChargeAnalysisTree = (TTree*)currentSglFile->Get("chargeGainAnalysisTree");
      TTree* bkgChargeAnalysisTree = (TTree*)currentBkgFile->Get("chargeGainAnalysisTree");
      if (sglChargeAnalysisTree && bkgChargeAnalysisTree) {
        final_prefix_list_bkg.push_back(prefix_list_bkg[iOpenFiles]);
        final_legend_list_bkg.push_back(legend_list_bkg[iOpenFiles]);
        input_files_bkg.push_back(currentBkgFile);

        final_prefix_list_sgl.push_back(prefix_list_sgl[iOpenFiles]);
        final_legend_list_sgl.push_back(legend_list_sgl[iOpenFiles]);
        input_files_sgl.push_back(currentSglFile);

        // check on the numbers of run per file and on the number of events
        // background
        std::vector<int> runNumbers;
        std::vector<int> eventNumbers;
        int eventCounter = 0;
        getRunNumberAndEventNumberLists(currentSglFile, runNumbers, eventNumbers);

        for(unsigned int iRunNumberAccounting = 0; iRunNumberAccounting < runNumbers.size(); iRunNumberAccounting++){
          // + 1 currently, because runs start with event number 0 
          eventCounter += eventNumbers[iRunNumberAccounting] + 1;
        }
        run_number_count_sgl.push_back(static_cast<int>(runNumbers.size()));
        event_count_sgl.push_back(eventCounter);
        // signal
        runNumbers.clear();
        eventNumbers.clear();
        eventCounter = 0;
        getRunNumberAndEventNumberLists(currentBkgFile, runNumbers, eventNumbers);

        for(unsigned int iRunNumberAccounting = 0; iRunNumberAccounting < runNumbers.size(); iRunNumberAccounting++){
          // + 1 currently, because runs start with event number 0 
          eventCounter += eventNumbers[iRunNumberAccounting] + 1;
        }
        run_number_count_bkg.push_back(static_cast<int>(runNumbers.size()));
        event_count_bkg.push_back(eventCounter);
      }
      else{
          std::cout << " SORRY (" << prefix_list_sgl[iOpenFiles] << " or " << prefix_list_bkg[iOpenFiles] << ")" << std::endl;
      }
    }
    else{
      std::cout << " one file not found (" << prefix_list_sgl[iOpenFiles] << " or " << prefix_list_bkg[iOpenFiles] << ")" <<  std::endl;
    }
  }

  // done
  return;
}

/** higher level functions using the low level functions **/

// creates a lot of 1D plots:
// amplitude, neg amplitude, waveform RMSs, baseline RMS, baseline mean
void createChargeGain1Dplots(std::vector<TString> prefix_list, std::vector<TString> legend_list, TString cut = "", TString file_name_flag = "", TString input_file_base = "raptorr_tpcdata_gain_%s_noExpSmooth_nowNeg.root"){

  /* Open files and get run numbers and event numbers */
  std::vector<TFile*> inputFiles;
  std::vector<TString> finalPrefixList;
  std::vector<TString> finalLegendList;
  std::vector<int> runNumberCount;
  std::vector<int> eventCount;

  returnFileAndNumberLists(prefix_list, legend_list, input_file_base, inputFiles, finalPrefixList, finalLegendList, runNumberCount, eventCount);
  std::cout << " got " << inputFiles.size() << " files (" << finalPrefixList.size() << "/" << finalLegendList.size() << "/" << runNumberCount.size() << "/" << eventCount.size() << ")" << std::endl;

  /* switches */
  bool lowAmplitudeBinning = false;


  /* binning */
  double amplitude_min[3] = { -25.0, -25.0, -25.0};
  double amplitude_max[3] = { 100.0, 350.0, 1000.0};
  double amplitude_count_min = -0.2;
  double amplitude_count_max = 2.0;
  double amplitude_count_min_log = 1E-4;
  double amplitude_count_max_log = 50.0;
  double neg_amplitude_count_min = -0.2;
  double neg_amplitude_count_max = 50.0;
  if(lowAmplitudeBinning) {
    amplitude_min[0] = -5.0;
    amplitude_min[1] = -5.0;
    amplitude_min[2] = -5.0;
    amplitude_max[0] = 100.0;
    amplitude_max[1] = 100.0;
    amplitude_max[2] = 100.0;
  }

  double baseline_min = -250.0;
  double baseline_max = 350.0;
  double baseline_RMS_min = -1.0;
  double baseline_RMS_max = 75.0;
  double baseline_count_min = 0.0;
  double baseline_count_max = 2.0;

  double RMS_min = -1.0;
  double RMS_max = 350.0;
  double RMS_count_min = 0.0;
  double RMS_count_max = 2.0;
  double RMS_count_min_log = 1E-3;
  double RMS_count_max_log = 10.0;  

  double RMS_over_amplitude_min = 0.0;
  double RMS_over_amplitude_max = 0.6;

  double binWidth_amplitude = 4;

  double binWidth_RMS = 0.4;
  double binWidth_RMS_over_amplitude = binWidth_RMS/100.0;


  /* amplitude histograms */
  std::vector< std::vector< TH1D*> > amplitudeHistogramList = plotList(inputFiles, finalPrefixList, "AmplitudeAnode", cut, "Amplitude [mV] ", "Counts ", static_cast<int>((amplitude_max[2]-amplitude_min[2])/binWidth_amplitude), amplitude_min[2], amplitude_max[2]);
  std::vector< std::vector< TH1D*> > baselineMeanHistogramList = plotList(inputFiles, finalPrefixList, "BaselineAnode", cut, "Baseline mean [mV] ", "Counts ", static_cast<int>((baseline_max-baseline_min)/binWidth_amplitude), baseline_min, baseline_max);
  std::vector< std::vector< TH1D*> > negAmplitudeHistogramList = plotList(inputFiles, finalPrefixList, "NegAmplitudeAnode", cut, "Amplitude (negative polarity) [mV] ", "Counts ", static_cast<int>((amplitude_max[2]-amplitude_min[2])/binWidth_amplitude), amplitude_min[2], amplitude_max[2]);


  /* RMS histograms */
  std::vector< std::vector< TH1D*> > RMSHistogramList = plotList(inputFiles, finalPrefixList, "WaveformRMSAnode", cut, "Waveform RMS [mV] ", "Counts ", static_cast<int>((RMS_max-RMS_min)/binWidth_RMS), RMS_min, RMS_max);

  std::vector< std::vector< TH1D*> > baselineRMSHistogramList = plotList(inputFiles, finalPrefixList, "BaselineRMSAnode", cut, "Baseline RMS [mV] ", "Counts ", static_cast<int>((baseline_RMS_max-baseline_RMS_min)/binWidth_RMS), baseline_RMS_min, baseline_RMS_max);


  /* shifting bins tests */
  //TH1D* bkg_waveformRMSAnode2_shifted = shiftBins(sgl_waveformRMSAnode2, 40, static_cast<int>((RMS_max-RMS_min)/binWidth_RMS), RMS_min, RMS_max);


  /* lets plot */
  for(int iAnode = 0; iAnode < 3; iAnode++){
    // Amplitude (positive polarity)
    TCanvas *amplitudeCanvas = new TCanvas(Form("AmplitudeCanvas_Anode%i", iAnode+1), Form("AmplitudeCanvas_Anode%i", iAnode+1), 800, 700);
    amplitudeCanvas->SetLeftMargin(0.12);
    amplitudeCanvas->SetRightMargin(0.12);
    amplitudeCanvas->SetBottomMargin(0.125);
    amplitudeCanvas->SetLogy();
    gStyle->SetOptStat(0);
    amplitudeCanvas->cd();

    TLegend* amplitudeLegend = new TLegend(0.6, 0.6, 0.9, 0.9);
  
    for(unsigned int iFile = 0; iFile < inputFiles.size(); iFile++){
      amplitudeHistogramList[iFile][iAnode]->Scale(1.0/static_cast<float>(eventCount[iFile]));
      amplitudeHistogramList[iFile][iAnode]->SetMarkerSize(0.6);
      amplitudeHistogramList[iFile][iAnode]->SetMinimum(amplitude_count_min_log);
      amplitudeHistogramList[iFile][iAnode]->SetMaximum(amplitude_count_max_log);
      if(iFile+1 < 10) { 
        amplitudeHistogramList[iFile][iAnode]->SetLineColor(iFile+1);
        amplitudeHistogramList[iFile][iAnode]->SetMarkerColor(iFile+1);
        amplitudeHistogramList[iFile][iAnode]->SetMarkerStyle(20);
      }
      else if(iFile-8 < 10){ 
        amplitudeHistogramList[iFile][iAnode]->SetLineColor(iFile-8);
        amplitudeHistogramList[iFile][iAnode]->SetMarkerColor(iFile-8);
        amplitudeHistogramList[iFile][iAnode]->SetLineStyle(2); 
        amplitudeHistogramList[iFile][iAnode]->SetMarkerStyle(21);
      }
      else {
        amplitudeHistogramList[iFile][iAnode]->SetLineColor(iFile-8*2-1);
        amplitudeHistogramList[iFile][iAnode]->SetMarkerColor(iFile-8*2-1);
        amplitudeHistogramList[iFile][iAnode]->SetLineStyle(2); 
        amplitudeHistogramList[iFile][iAnode]->SetMarkerStyle(22);
      }
      if (iFile == 0) { 
        amplitudeHistogramList[iFile][iAnode]->GetXaxis()->SetRangeUser(amplitude_min[iAnode], amplitude_max[iAnode]);
        amplitudeHistogramList[iFile][iAnode]->Draw("P0C"); 
      }
      else { amplitudeHistogramList[iFile][iAnode]->Draw("same"); }
      amplitudeLegend->AddEntry(amplitudeHistogramList[iFile][iAnode], finalLegendList[iFile], "lp");
    }

    amplitudeLegend->Draw("same");
    amplitudeCanvas->Update();
    if (!lowAmplitudeBinning) { amplitudeCanvas->Print(file_name_flag+Form("amplitudeCanvas_Anode%i_log.pdf", iAnode+1)); }
    else { amplitudeCanvas->Print(file_name_flag+Form("amplitudeCanvas_Anode%i_log_zoom.pdf", iAnode+1)); }

    amplitudeCanvas->SetLogy(0);
    gStyle->SetOptStat(0);

    for(unsigned int iFile = 0; iFile < inputFiles.size(); iFile++){
      if (iFile == 0) { 
        amplitudeHistogramList[iFile][iAnode]->GetXaxis()->SetRangeUser(amplitude_min[iAnode], amplitude_max[iAnode]);
        amplitudeHistogramList[iFile][iAnode]->Draw("P0C"); 
        amplitudeHistogramList[iFile][iAnode]->SetMinimum(amplitude_count_min);
        amplitudeHistogramList[iFile][iAnode]->SetMaximum(amplitude_count_max);
      }
      else { amplitudeHistogramList[iFile][iAnode]->Draw("same"); }
    }
    amplitudeLegend->Draw("same");
    amplitudeCanvas->Update();
    if (!lowAmplitudeBinning) { amplitudeCanvas->Print(file_name_flag+Form("amplitudeCanvas_Anode%i.pdf", iAnode+1)); }
    else { amplitudeCanvas->Print(file_name_flag+Form("amplitudeCanvas_Anode%i_zoom.pdf", iAnode+1)); }


    // negative polarity
    amplitudeCanvas->SetLogy();
    amplitudeCanvas->cd();

    for(unsigned int iFile = 0; iFile < inputFiles.size(); iFile++){
      negAmplitudeHistogramList[iFile][iAnode]->Scale(1.0/static_cast<float>(eventCount[iFile]));
      negAmplitudeHistogramList[iFile][iAnode]->SetMarkerSize(0.6);
      if(iFile+1 < 10) { 
        negAmplitudeHistogramList[iFile][iAnode]->SetLineColor(iFile+1);
        negAmplitudeHistogramList[iFile][iAnode]->SetMarkerColor(iFile+1);
        negAmplitudeHistogramList[iFile][iAnode]->SetMarkerStyle(20);
      }
      else if(iFile-8 < 10){ 
        negAmplitudeHistogramList[iFile][iAnode]->SetLineColor(iFile-8);
        negAmplitudeHistogramList[iFile][iAnode]->SetMarkerColor(iFile-8);
        negAmplitudeHistogramList[iFile][iAnode]->SetLineStyle(2); 
        negAmplitudeHistogramList[iFile][iAnode]->SetMarkerStyle(21);
      }
      else {
        negAmplitudeHistogramList[iFile][iAnode]->SetLineColor(iFile-8*2-1);
        negAmplitudeHistogramList[iFile][iAnode]->SetMarkerColor(iFile-8*2-1);
        negAmplitudeHistogramList[iFile][iAnode]->SetLineStyle(2); 
        negAmplitudeHistogramList[iFile][iAnode]->SetMarkerStyle(22);
      }

      if (iFile == 0) { 
        negAmplitudeHistogramList[iFile][iAnode]->GetXaxis()->SetRangeUser(amplitude_min[iAnode], amplitude_max[iAnode]);
        negAmplitudeHistogramList[iFile][iAnode]->Draw("P0C"); 
      }
      else { negAmplitudeHistogramList[iFile][iAnode]->Draw("same"); }
    }

    amplitudeLegend->Draw("same");
    amplitudeCanvas->Update();
    if (!lowAmplitudeBinning) { amplitudeCanvas->Print(file_name_flag+Form("negAmplitudeCanvas_Anode%i_log.pdf", iAnode+1)); }
    else { amplitudeCanvas->Print(file_name_flag+Form("negAmplitudeCanvas_Anode%i_log_zoom.pdf", iAnode+1)); }

    amplitudeCanvas->SetLogy(0);
    gStyle->SetOptStat(0);

    for(unsigned int iFile = 0; iFile < inputFiles.size(); iFile++){
      if (iFile == 0) { 
        negAmplitudeHistogramList[iFile][iAnode]->GetXaxis()->SetRangeUser(amplitude_min[iAnode], amplitude_max[iAnode]);
        negAmplitudeHistogramList[iFile][iAnode]->Draw("P0C"); 
        negAmplitudeHistogramList[iFile][iAnode]->SetMinimum(neg_amplitude_count_min);
        negAmplitudeHistogramList[iFile][iAnode]->SetMaximum(neg_amplitude_count_max);
      }
      else { negAmplitudeHistogramList[iFile][iAnode]->Draw("same"); }
    }
    amplitudeLegend->Draw("same");
    amplitudeCanvas->Update();
    if (!lowAmplitudeBinning) { amplitudeCanvas->Print(file_name_flag+Form("negAmplitudeCanvas_Anode%i.pdf", iAnode+1)); }
    else { amplitudeCanvas->Print(file_name_flag+Form("negAmplitudeCanvas_Anode%i_zoom.pdf", iAnode+1)); }


    // RMS
    TCanvas *waveformRMSCanvas = new TCanvas(Form("WaveformRMSCanvas_Anode%i", iAnode+1 ), Form("WaveformRMSCanvas_Anode%i", iAnode+1 ), 800, 700);
    waveformRMSCanvas->SetLeftMargin(0.12);
    waveformRMSCanvas->SetRightMargin(0.12);
    waveformRMSCanvas->SetBottomMargin(0.125);
    waveformRMSCanvas->SetLogy();

    for(unsigned int iFile = 0; iFile < inputFiles.size(); iFile++){
      RMSHistogramList[iFile][iAnode]->Scale(1.0/static_cast<float>(eventCount[iFile]));
      RMSHistogramList[iFile][iAnode]->SetMarkerSize(0.6);
      RMSHistogramList[iFile][iAnode]->SetMinimum(RMS_count_min_log);
      RMSHistogramList[iFile][iAnode]->SetMaximum(RMS_count_max_log);

      if(iFile+1 < 10) { 
        RMSHistogramList[iFile][iAnode]->SetLineColor(iFile+1);
        RMSHistogramList[iFile][iAnode]->SetMarkerColor(iFile+1);
        RMSHistogramList[iFile][iAnode]->SetMarkerStyle(20);
      }
      else if(iFile-8 < 10){ 
        RMSHistogramList[iFile][iAnode]->SetLineColor(iFile-8);
        RMSHistogramList[iFile][iAnode]->SetMarkerColor(iFile-8);
        RMSHistogramList[iFile][iAnode]->SetLineStyle(2); 
        RMSHistogramList[iFile][iAnode]->SetMarkerStyle(21);
      }
      else {
        RMSHistogramList[iFile][iAnode]->SetLineColor(iFile-8*2-1);
        RMSHistogramList[iFile][iAnode]->SetMarkerColor(iFile-8*2-1);
        RMSHistogramList[iFile][iAnode]->SetLineStyle(2); 
        RMSHistogramList[iFile][iAnode]->SetMarkerStyle(22);
      }

      if (iFile == 0) { RMSHistogramList[iFile][iAnode]->Draw("P0C"); }
      else { RMSHistogramList[iFile][iAnode]->Draw("same"); }
    }

    amplitudeLegend->Draw("same");
    waveformRMSCanvas->Update();
    waveformRMSCanvas->Print(file_name_flag+Form("waveformRMSCanvas_Anode%i_log.pdf", iAnode+1));

    waveformRMSCanvas->SetLogy(0);
    gStyle->SetOptStat(0);

    for(unsigned int iFile = 0; iFile < inputFiles.size(); iFile++){
      if (iFile == 0) { 
        RMSHistogramList[iFile][iAnode]->Draw("P0C"); 
        RMSHistogramList[iFile][iAnode]->SetMinimum(RMS_count_min);
        RMSHistogramList[iFile][iAnode]->SetMaximum(RMS_count_max);
      }
      else { RMSHistogramList[iFile][iAnode]->Draw("same"); }
    }

    amplitudeLegend->Draw("same");
    waveformRMSCanvas->Update();
    waveformRMSCanvas->Print(file_name_flag+Form("waveformRMSCanvas_Anode%i.pdf", iAnode+1));


    // baseline RMS
    TCanvas *baselineRMSCanvas = new TCanvas(Form("BaselineRMSCanvas_Anode%i", iAnode+1 ), Form("BaselineRMSCanvas_Anode%i", iAnode+1 ), 800, 700);
    baselineRMSCanvas->SetLeftMargin(0.12);
    baselineRMSCanvas->SetRightMargin(0.12);
    baselineRMSCanvas->SetBottomMargin(0.125);
    baselineRMSCanvas->SetLogy();

    for(unsigned int iFile = 0; iFile < inputFiles.size(); iFile++){
      baselineRMSHistogramList[iFile][iAnode]->Scale(1.0/static_cast<float>(eventCount[iFile]));
      baselineRMSHistogramList[iFile][iAnode]->SetMarkerSize(0.4);
      if(iFile+1 < 10) { 
        baselineRMSHistogramList[iFile][iAnode]->SetLineColor(iFile+1); 
        baselineRMSHistogramList[iFile][iAnode]->SetMarkerColor(iFile+1); 
        baselineRMSHistogramList[iFile][iAnode]->SetMarkerStyle(20);
      }
      else if(iFile-8 < 10){ 
        baselineRMSHistogramList[iFile][iAnode]->SetLineColor(iFile-8);
        baselineRMSHistogramList[iFile][iAnode]->SetMarkerColor(iFile-8);
        baselineRMSHistogramList[iFile][iAnode]->SetLineStyle(2); 
        baselineRMSHistogramList[iFile][iAnode]->SetMarkerStyle(21);
      }
      else {
        baselineRMSHistogramList[iFile][iAnode]->SetLineColor(iFile-8*2-1);
        baselineRMSHistogramList[iFile][iAnode]->SetMarkerColor(iFile-8*2-1);
        baselineRMSHistogramList[iFile][iAnode]->SetLineStyle(2); 
        baselineRMSHistogramList[iFile][iAnode]->SetMarkerStyle(22);
      }

      if (iFile == 0) { baselineRMSHistogramList[iFile][iAnode]->Draw("P0C"); }
      else { baselineRMSHistogramList[iFile][iAnode]->Draw("same"); }
    }

    amplitudeLegend->Draw("same");
    baselineRMSCanvas->Update();
    baselineRMSCanvas->Print(file_name_flag+Form("baselineRMSCanvas_Anode%i_log.pdf", iAnode+1));

    baselineRMSCanvas->SetLogy(0);
    gStyle->SetOptStat(0);

    for(unsigned int iFile = 0; iFile < inputFiles.size(); iFile++){
      if (iFile == 0) { 
        baselineRMSHistogramList[iFile][iAnode]->Draw("P0C"); 
        baselineRMSHistogramList[iFile][iAnode]->SetMinimum(RMS_count_min);
        baselineRMSHistogramList[iFile][iAnode]->SetMaximum(RMS_count_max);
      }
      else { baselineRMSHistogramList[iFile][iAnode]->Draw("same"); }
    }

    amplitudeLegend->Draw("same");
    baselineRMSCanvas->Update();
    baselineRMSCanvas->Print(file_name_flag+Form("baselineRMSCanvas_Anode%i.pdf", iAnode+1));
  

    // baseline 
    TCanvas *baselineMeanCanvas = new TCanvas(Form("BaselineMeanCanvas_Anode%i", iAnode+1 ), Form("BaselineMeanCanvas_Anode%i", iAnode+1 ), 800, 700);
    baselineMeanCanvas->SetLeftMargin(0.12);
    baselineMeanCanvas->SetRightMargin(0.12);
    baselineMeanCanvas->SetBottomMargin(0.125);
    baselineMeanCanvas->SetLogy();

    for(unsigned int iFile = 0; iFile < inputFiles.size(); iFile++){
      baselineMeanHistogramList[iFile][iAnode]->Scale(1.0/static_cast<float>(eventCount[iFile]));
      baselineMeanHistogramList[iFile][iAnode]->SetMarkerSize(0.6);
      if(iFile+1 < 10) { 
        baselineMeanHistogramList[iFile][iAnode]->SetLineColor(iFile+1); 
        baselineMeanHistogramList[iFile][iAnode]->SetMarkerColor(iFile+1); 
        baselineMeanHistogramList[iFile][iAnode]->SetMarkerStyle(20);
      }
      else if(iFile-8 < 10){ 
        baselineMeanHistogramList[iFile][iAnode]->SetLineColor(iFile-8);
        baselineMeanHistogramList[iFile][iAnode]->SetMarkerColor(iFile-8);
        baselineMeanHistogramList[iFile][iAnode]->SetLineStyle(2); 
        baselineMeanHistogramList[iFile][iAnode]->SetMarkerStyle(21);
      }
      else {
        baselineMeanHistogramList[iFile][iAnode]->SetLineColor(iFile-8*2-1);
        baselineMeanHistogramList[iFile][iAnode]->SetMarkerColor(iFile-8*2-1);
        baselineMeanHistogramList[iFile][iAnode]->SetLineStyle(2); 
        baselineMeanHistogramList[iFile][iAnode]->SetMarkerStyle(22);
      }

      if (iFile == 0) { baselineMeanHistogramList[iFile][iAnode]->Draw("P0C"); }
      else { baselineMeanHistogramList[iFile][iAnode]->Draw("same"); }
    }

    amplitudeLegend->Draw("same");
    baselineMeanCanvas->Update();
    baselineMeanCanvas->Print(file_name_flag+Form("baselineMeanCanvas_Anode%i_log.pdf", iAnode+1));

    baselineMeanCanvas->SetLogy(0);
    gStyle->SetOptStat(0);

    for(unsigned int iFile = 0; iFile < inputFiles.size(); iFile++){
      if (iFile == 0) { 
        baselineMeanHistogramList[iFile][iAnode]->Draw("P0C"); 
        baselineMeanHistogramList[iFile][iAnode]->SetMinimum(baseline_count_min);
        baselineMeanHistogramList[iFile][iAnode]->SetMaximum(baseline_count_max);
      }
      else { baselineMeanHistogramList[iFile][iAnode]->Draw("same"); }
    }

    amplitudeLegend->Draw("same");
    baselineMeanCanvas->Update();
    baselineMeanCanvas->Print(file_name_flag+Form("baselineMeanCanvas_Anode%i.pdf", iAnode+1));
  }

  // done
  return;
}
// the same as the above but for a number of signal and background files, signal minus background plots are created as well
void createChargeGain1Dplots(std::vector<TString> prefix_list_bkg, std::vector<TString> prefix_list_sgl, std::vector<TString> legend_list_bkg, std::vector<TString> legend_list_sgl, TString cut = "", TString file_name_flag = "", TString input_file_base = "raptorr_tpcdata_gain_%s_noExpSmooth_nowNeg.root"){

  /* Open files and get run numbers and event numbers */
  std::vector<TFile*> inputSglFiles;
  std::vector<TString> finalSglPrefixList;
  std::vector<TString> finalSglLegendList;
  std::vector<int> sglRunNumberCount;
  std::vector<int> sglEventCount;

  std::vector<TFile*> inputBkgFiles;
  std::vector<TString> finalBkgPrefixList;
  std::vector<TString> finalBkgLegendList;
  std::vector<int> bkgRunNumberCount;
  std::vector<int> bkgEventCount;

  returnFileAndNumberLists(prefix_list_bkg, legend_list_bkg, input_file_base, inputBkgFiles, finalBkgPrefixList, finalBkgLegendList, bkgRunNumberCount, bkgEventCount, prefix_list_sgl, legend_list_sgl, input_file_base, inputSglFiles, finalSglPrefixList, finalSglLegendList, sglRunNumberCount, sglEventCount);
  
  /* switches */
  bool lowAmplitudeBinning = false;


  /* binning */
  // here so far only the last element in these 3 slots arrays is used
  double amplitude_min[3] = { -25.0, -25.0, -25.0};
  double amplitude_max[3] = { 350.0, 350.0, 800.0};
  double amplitude_count_min = -1.0;
  double amplitude_count_max = 2.0;
  double amplitude_count_min_log = 1E-3;
  double amplitude_count_max_log = 10.0;
  double neg_amplitude_count_min = 0.1;
  double neg_amplitude_count_max = 2.0;
  if(lowAmplitudeBinning) {
    amplitude_min[0] = -5.0;
    amplitude_min[1] = -5.0;
    amplitude_min[2] = -5.0;
    amplitude_max[0] = 60.0;
    amplitude_max[1] = 60.0;
    amplitude_max[2] = 60.0;
  }

  double baseline_min = -250.0;
  double baseline_max = 350.0;
  double baseline_count_min = 0.0;
  double baseline_count_max = 2.0;
  double baseline_count_min_log = 1E-3;
  double baseline_count_max_log = 100;

  double RMS_min = -1.0;
  double RMS_max = 80.0;
  double RMS_count_min = -1.0;
  double RMS_count_max = 2.0;
  double RMS_count_min_log = 1E-3;
  double RMS_count_max_log = 10.0;  

  double RMS_over_amplitude_min = 0.0;
  double RMS_over_amplitude_max = 0.6;

  double binWidth_amplitude = 6;

  double binWidth_RMS = 0.6;
  double binWidth_RMS_over_amplitude = binWidth_RMS/100.0;


  /* amplitude histograms */
  // bkg
  std::vector< std::vector< TH1D*> > bkgAmplitudeHistogramList = plotList(inputBkgFiles, finalBkgPrefixList, "AmplitudeAnode", cut, "Amplitude [mV] ", "Counts ", static_cast<int>((amplitude_max[2]-amplitude_min[2])/binWidth_amplitude), amplitude_min[2], amplitude_max[2]);
  std::vector< std::vector< TH1D*> > bkgNegAmplitudeHistogramList = plotList(inputBkgFiles, finalBkgPrefixList, "NegAmplitudeAnode", cut, "Amplitude (negative polarity) [mV] ", "Counts ", static_cast<int>((amplitude_max[2]-amplitude_min[2])/binWidth_amplitude), amplitude_min[2], amplitude_max[2]);
  // sgl
  std::vector< std::vector< TH1D*> > sglAmplitudeHistogramList = plotList(inputSglFiles, finalSglPrefixList, "AmplitudeAnode", cut, "Amplitude [mV] ", "Counts ", static_cast<int>((amplitude_max[2]-amplitude_min[2])/binWidth_amplitude), amplitude_min[2], amplitude_max[2]);
  std::vector< std::vector< TH1D*> > sglNegAmplitudeHistogramList = plotList(inputSglFiles, finalSglPrefixList, "NegAmplitudeAnode", cut, "Amplitude (negative polarity) [mV] ", "Counts ", static_cast<int>((amplitude_max[2]-amplitude_min[2])/binWidth_amplitude), amplitude_min[2], amplitude_max[2]);

  /* baseline histograms */
  // bkg
  std::vector< std::vector< TH1D*> > bkgBaselineMeanHistogramList = plotList(inputBkgFiles, finalBkgPrefixList, "BaselineAnode", cut, "Baseline mean [mV] ", "Counts ", static_cast<int>((baseline_max-baseline_min)/binWidth_amplitude), baseline_min, baseline_max);
  // sgl
  std::vector< std::vector< TH1D*> > sglBaselineMeanHistogramList = plotList(inputSglFiles, finalSglPrefixList, "BaselineAnode", cut, "Baseline mean [mV] ", "Counts ", static_cast<int>((baseline_max-baseline_min)/binWidth_amplitude), baseline_min, baseline_max);


  /* RMS histograms */
  // bkg
  std::vector< std::vector< TH1D*> > bkgRMSHistogramList = plotList(inputBkgFiles, finalBkgPrefixList, "WaveformRMSAnode", cut, "Waveform RMS [mV] ", "Counts ", static_cast<int>((RMS_max-RMS_min)/binWidth_RMS), RMS_min, RMS_max);
  std::vector< std::vector< TH1D*> > bkgBaselineRMSHistogramList = plotList(inputBkgFiles, finalBkgPrefixList, "BaselineRMSAnode", cut, "Baseline RMS [mV] ", "Counts ", static_cast<int>((RMS_max-RMS_min)/binWidth_RMS), RMS_min, RMS_max);
  // sgl
  std::vector< std::vector< TH1D*> > sglRMSHistogramList = plotList(inputSglFiles, finalSglPrefixList, "WaveformRMSAnode", cut, "Waveform RMS [mV] ", "Counts ", static_cast<int>((RMS_max-RMS_min)/binWidth_RMS), RMS_min, RMS_max);
  std::vector< std::vector< TH1D*> > sglBaselineRMSHistogramList = plotList(inputSglFiles, finalSglPrefixList, "BaselineRMSAnode", cut, "Baseline RMS [mV] ", "Counts ", static_cast<int>((RMS_max-RMS_min)/binWidth_RMS), RMS_min, RMS_max);


  /* lets plot */
  // storage for signal minus background histograms
  std::vector< std::vector< TH1D*> > sglMinusBkgAmplitudeHistogramList;
  std::vector< std::vector< TH1D*> > sglMinusBkgNegAmplitudeHistogramList;
  std::vector< std::vector< TH1D*> > sglMinusBkgRMSHistogramListHistogramList;
  std::vector< std::vector< TH1D*> > sglMinusBkgBaselineRMSHistogramList;
  std::vector< std::vector< TH1D*> > sglMinusBkgBaselineMeanHistogramList;

  for(int iAnode = 0; iAnode < 3; iAnode++){
    std::cout << " anode " << iAnode << std::endl;
    // Amplitude (positive polarity)
    // signal and background in one messy canvas
    TCanvas *amplitudeSglAndBkgCanvas = new TCanvas(Form("AmplitudeSglAndBkgCanvas_Anode%i", iAnode+1), Form("AmplitudeSglAndBkgCanvas_Anode%i", iAnode+1), 800, 700);
    amplitudeSglAndBkgCanvas->SetLeftMargin(0.12);
    amplitudeSglAndBkgCanvas->SetRightMargin(0.12);
    amplitudeSglAndBkgCanvas->SetBottomMargin(0.125);
    amplitudeSglAndBkgCanvas->SetLogy();
    gStyle->SetOptStat(0);
    amplitudeSglAndBkgCanvas->cd();

    // signal minus background in one slightly less canvas
    TCanvas *amplitudeSglMinusBkgCanvas = new TCanvas(Form("AmplitudeSglMinusBkgCanvas_Anode%i", iAnode+1), Form("AmplitudeSglMinusBkgCanvas_Anode%i", iAnode+1), 800, 700);
    amplitudeSglMinusBkgCanvas->SetLeftMargin(0.12);
    amplitudeSglMinusBkgCanvas->SetRightMargin(0.12);
    amplitudeSglMinusBkgCanvas->SetBottomMargin(0.125);
    amplitudeSglMinusBkgCanvas->SetLogy();
    gStyle->SetOptStat(0);
    amplitudeSglMinusBkgCanvas->cd();

    // legends
    TLegend* sglAndBkgAmplitudeLegend = new TLegend(0.6, 0.6, 0.9, 0.9);
    TLegend* sglMinusBkgAmplitudeLegend = new TLegend(0.6, 0.6, 0.9, 0.9);
    // storage
    std::vector< TH1D*> currentSglMinusBkgAmplitudeHistogramList;
    std::vector< TH1D*> currentSglMinusBkgNegAmplitudeHistogramList;

    for(unsigned int iFile = 0; iFile < inputBkgFiles.size() && iFile < inputSglFiles.size(); iFile++){
      bkgAmplitudeHistogramList[iFile][iAnode]->Scale(1.0/static_cast<float>(bkgEventCount[iFile]));
      bkgAmplitudeHistogramList[iFile][iAnode]->SetMarkerSize(0.6);
      bkgAmplitudeHistogramList[iFile][iAnode]->SetMinimum(amplitude_count_min_log);
      bkgAmplitudeHistogramList[iFile][iAnode]->SetMaximum(amplitude_count_max_log);

      sglAmplitudeHistogramList[iFile][iAnode]->Scale(1.0/static_cast<float>(sglEventCount[iFile]));
      sglAmplitudeHistogramList[iFile][iAnode]->SetMarkerSize(0.6);
      sglAmplitudeHistogramList[iFile][iAnode]->SetMinimum(amplitude_count_min_log);
      sglAmplitudeHistogramList[iFile][iAnode]->SetMaximum(amplitude_count_max_log);

      TH1D* currentSglMinusBkgAmplitudeHistogram = static_cast<TH1D*>(sglAmplitudeHistogramList[iFile][iAnode]->Clone());
      currentSglMinusBkgAmplitudeHistogram->Add(bkgAmplitudeHistogramList[iFile][iAnode], -1);
      currentSglMinusBkgAmplitudeHistogram->SetMarkerSize(0.6);
      currentSglMinusBkgAmplitudeHistogram->SetMinimum(amplitude_count_min_log);
      currentSglMinusBkgAmplitudeHistogram->SetMaximum(amplitude_count_max_log);

      if(iFile+1 < 10) { 
        bkgAmplitudeHistogramList[iFile][iAnode]->SetLineColor(iFile+1);
        bkgAmplitudeHistogramList[iFile][iAnode]->SetMarkerColor(iFile+1);
        bkgAmplitudeHistogramList[iFile][iAnode]->SetMarkerStyle(22);

        sglAmplitudeHistogramList[iFile][iAnode]->SetLineColor(iFile+1);
        sglAmplitudeHistogramList[iFile][iAnode]->SetMarkerColor(iFile+1);
        sglAmplitudeHistogramList[iFile][iAnode]->SetMarkerStyle(23);

        currentSglMinusBkgAmplitudeHistogram->SetLineColor(iFile+1);
        currentSglMinusBkgAmplitudeHistogram->SetMarkerColor(iFile+1);
        currentSglMinusBkgAmplitudeHistogram->SetMarkerStyle(20);        
      }
      else if(iFile-8 < 10){ 
        bkgAmplitudeHistogramList[iFile][iAnode]->SetLineColor(iFile-8);
        bkgAmplitudeHistogramList[iFile][iAnode]->SetMarkerColor(iFile-8);
        bkgAmplitudeHistogramList[iFile][iAnode]->SetLineStyle(2); 
        bkgAmplitudeHistogramList[iFile][iAnode]->SetMarkerStyle(26);

        sglAmplitudeHistogramList[iFile][iAnode]->SetLineColor(iFile-8);
        sglAmplitudeHistogramList[iFile][iAnode]->SetMarkerColor(iFile-8);
        sglAmplitudeHistogramList[iFile][iAnode]->SetLineStyle(2); 
        sglAmplitudeHistogramList[iFile][iAnode]->SetMarkerStyle(32);

        currentSglMinusBkgAmplitudeHistogram->SetLineColor(iFile-8);
        currentSglMinusBkgAmplitudeHistogram->SetMarkerColor(iFile-8);
        currentSglMinusBkgAmplitudeHistogram->SetLineStyle(2); 
        currentSglMinusBkgAmplitudeHistogram->SetMarkerStyle(24);
      }
      else {
        bkgAmplitudeHistogramList[iFile][iAnode]->SetLineColor(iFile-8*2-1);
        bkgAmplitudeHistogramList[iFile][iAnode]->SetMarkerColor(iFile-8*2-1);
        bkgAmplitudeHistogramList[iFile][iAnode]->SetLineStyle(2); 
        bkgAmplitudeHistogramList[iFile][iAnode]->SetMarkerStyle(27);

        sglAmplitudeHistogramList[iFile][iAnode]->SetLineColor(iFile-8*2-1);
        sglAmplitudeHistogramList[iFile][iAnode]->SetMarkerColor(iFile-8*2-1);
        sglAmplitudeHistogramList[iFile][iAnode]->SetLineStyle(2); 
        sglAmplitudeHistogramList[iFile][iAnode]->SetMarkerStyle(33);

        currentSglMinusBkgAmplitudeHistogram->SetLineColor(iFile-8*2-1);
        currentSglMinusBkgAmplitudeHistogram->SetMarkerColor(iFile-8*2-1);
        currentSglMinusBkgAmplitudeHistogram->SetLineStyle(2); 
        currentSglMinusBkgAmplitudeHistogram->SetMarkerStyle(21);
      }

      if (iFile == 0) { 
        amplitudeSglAndBkgCanvas->cd();
        bkgAmplitudeHistogramList[iFile][iAnode]->Draw("P0C"); 
        sglAmplitudeHistogramList[iFile][iAnode]->Draw("same");
        amplitudeSglMinusBkgCanvas->cd();
        currentSglMinusBkgAmplitudeHistogram->Draw("P0C");
      }
      else { 
        amplitudeSglAndBkgCanvas->cd();
        bkgAmplitudeHistogramList[iFile][iAnode]->Draw("same");
        sglAmplitudeHistogramList[iFile][iAnode]->Draw("same");
        amplitudeSglMinusBkgCanvas->cd();
        currentSglMinusBkgAmplitudeHistogram->Draw("same");
      }

      currentSglMinusBkgAmplitudeHistogramList.push_back(currentSglMinusBkgAmplitudeHistogram);

      sglAndBkgAmplitudeLegend->AddEntry(sglAmplitudeHistogramList[iFile][iAnode], finalSglLegendList[iFile], "lp");
      sglAndBkgAmplitudeLegend->AddEntry(bkgAmplitudeHistogramList[iFile][iAnode], finalBkgLegendList[iFile], "lp");
      sglMinusBkgAmplitudeLegend->AddEntry(currentSglMinusBkgAmplitudeHistogram, finalSglLegendList[iFile]+" minus "+finalBkgLegendList[iFile], "lp");
    }

    amplitudeSglAndBkgCanvas->cd();
    sglAndBkgAmplitudeLegend->Draw("same");
    amplitudeSglAndBkgCanvas->Update();
    amplitudeSglAndBkgCanvas->Print(file_name_flag+Form("amplitudeSglAndBkgCanvas_Anode%i_log.pdf", iAnode+1));

    amplitudeSglMinusBkgCanvas->cd();
    sglMinusBkgAmplitudeLegend->Draw("same");
    amplitudeSglMinusBkgCanvas->Update();
    amplitudeSglMinusBkgCanvas->Print(file_name_flag+Form("amplitudeSglMinusBkgCanvas_Anode%i_log.pdf", iAnode+1));

    amplitudeSglAndBkgCanvas->SetLogy(0);
    gStyle->SetOptStat(0);

    amplitudeSglMinusBkgCanvas->SetLogy(0);
    gStyle->SetOptStat(0);

    for(unsigned int iFile = 0; iFile < inputBkgFiles.size() && iFile < inputSglFiles.size(); iFile++){
      if (iFile == 0) { 
        bkgAmplitudeHistogramList[iFile][iAnode]->SetMinimum(amplitude_count_min);
        bkgAmplitudeHistogramList[iFile][iAnode]->SetMaximum(amplitude_count_max);
        sglAmplitudeHistogramList[iFile][iAnode]->SetMinimum(amplitude_count_min);
        sglAmplitudeHistogramList[iFile][iAnode]->SetMaximum(amplitude_count_max);
        
        amplitudeSglAndBkgCanvas->cd();
        bkgAmplitudeHistogramList[iFile][iAnode]->Draw("P0C");
        sglAmplitudeHistogramList[iFile][iAnode]->Draw("same"); 

        amplitudeSglMinusBkgCanvas->cd();
        currentSglMinusBkgAmplitudeHistogramList[iFile]->SetMinimum(amplitude_count_min);
        currentSglMinusBkgAmplitudeHistogramList[iFile]->SetMaximum(amplitude_count_max);
        currentSglMinusBkgAmplitudeHistogramList[iFile]->Draw("P0C");
      }
      else { 
        amplitudeSglAndBkgCanvas->cd();
        bkgAmplitudeHistogramList[iFile][iAnode]->Draw("same");
        sglAmplitudeHistogramList[iFile][iAnode]->Draw("same"); 

        amplitudeSglMinusBkgCanvas->cd();
        currentSglMinusBkgAmplitudeHistogramList[iFile]->Draw("same");
      }
    }

    amplitudeSglAndBkgCanvas->cd();
    sglAndBkgAmplitudeLegend->Draw("same");
    amplitudeSglAndBkgCanvas->Update();
    amplitudeSglAndBkgCanvas->Print(file_name_flag+Form("amplitudeSglAndBkgCanvas_Anode%i.pdf", iAnode+1));

    amplitudeSglMinusBkgCanvas->cd();
    sglMinusBkgAmplitudeLegend->Draw("same");
    amplitudeSglMinusBkgCanvas->Update();
    amplitudeSglMinusBkgCanvas->Print(file_name_flag+Form("amplitudeSglMinusBkgCanvas_Anode%i.pdf", iAnode+1));

    sglMinusBkgAmplitudeHistogramList.push_back(currentSglMinusBkgAmplitudeHistogramList);


    // negative polarity
    amplitudeSglAndBkgCanvas->SetLogy();
    amplitudeSglMinusBkgCanvas->SetLogy();

    for(unsigned int iFile = 0; iFile < inputBkgFiles.size() && iFile < inputSglFiles.size(); iFile++){
      bkgNegAmplitudeHistogramList[iFile][iAnode]->Scale(1.0/static_cast<float>(bkgEventCount[iFile]));
      bkgNegAmplitudeHistogramList[iFile][iAnode]->SetMarkerSize(0.6);
      bkgNegAmplitudeHistogramList[iFile][iAnode]->SetMinimum(amplitude_count_min_log);
      bkgNegAmplitudeHistogramList[iFile][iAnode]->SetMaximum(amplitude_count_max_log);

      sglNegAmplitudeHistogramList[iFile][iAnode]->Scale(1.0/static_cast<float>(sglEventCount[iFile]));
      sglNegAmplitudeHistogramList[iFile][iAnode]->SetMarkerSize(0.6);
      sglNegAmplitudeHistogramList[iFile][iAnode]->SetMinimum(amplitude_count_min_log);
      sglNegAmplitudeHistogramList[iFile][iAnode]->SetMaximum(amplitude_count_max_log);

      TH1D* currentSglMinusBkgNegAmplitudeHistogram = static_cast<TH1D*>(sglNegAmplitudeHistogramList[iFile][iAnode]->Clone());
      currentSglMinusBkgNegAmplitudeHistogram->Add(bkgNegAmplitudeHistogramList[iFile][iAnode], -1);
      currentSglMinusBkgNegAmplitudeHistogram->SetMarkerSize(0.6);
      currentSglMinusBkgNegAmplitudeHistogram->SetMinimum(amplitude_count_min_log);
      currentSglMinusBkgNegAmplitudeHistogram->SetMaximum(amplitude_count_max_log);

      if(iFile+1 < 10) { 
        bkgNegAmplitudeHistogramList[iFile][iAnode]->SetLineColor(iFile+1);
        bkgNegAmplitudeHistogramList[iFile][iAnode]->SetMarkerColor(iFile+1);
        bkgNegAmplitudeHistogramList[iFile][iAnode]->SetMarkerStyle(22);

        sglNegAmplitudeHistogramList[iFile][iAnode]->SetLineColor(iFile+1);
        sglNegAmplitudeHistogramList[iFile][iAnode]->SetMarkerColor(iFile+1);
        sglNegAmplitudeHistogramList[iFile][iAnode]->SetMarkerStyle(23);

        currentSglMinusBkgNegAmplitudeHistogram->SetLineColor(iFile+1);
        currentSglMinusBkgNegAmplitudeHistogram->SetMarkerColor(iFile+1);
        currentSglMinusBkgNegAmplitudeHistogram->SetMarkerStyle(20);        
      }
      else if(iFile-8 < 10){ 
        bkgNegAmplitudeHistogramList[iFile][iAnode]->SetLineColor(iFile-8);
        bkgNegAmplitudeHistogramList[iFile][iAnode]->SetMarkerColor(iFile-8);
        bkgNegAmplitudeHistogramList[iFile][iAnode]->SetLineStyle(2); 
        bkgNegAmplitudeHistogramList[iFile][iAnode]->SetMarkerStyle(26);

        sglNegAmplitudeHistogramList[iFile][iAnode]->SetLineColor(iFile-8);
        sglNegAmplitudeHistogramList[iFile][iAnode]->SetMarkerColor(iFile-8);
        sglNegAmplitudeHistogramList[iFile][iAnode]->SetLineStyle(2); 
        sglNegAmplitudeHistogramList[iFile][iAnode]->SetMarkerStyle(32);

        currentSglMinusBkgNegAmplitudeHistogram->SetLineColor(iFile-8);
        currentSglMinusBkgNegAmplitudeHistogram->SetMarkerColor(iFile-8);
        currentSglMinusBkgNegAmplitudeHistogram->SetLineStyle(2); 
        currentSglMinusBkgNegAmplitudeHistogram->SetMarkerStyle(24);
      }
      else {
        bkgNegAmplitudeHistogramList[iFile][iAnode]->SetLineColor(iFile-8*2-1);
        bkgNegAmplitudeHistogramList[iFile][iAnode]->SetMarkerColor(iFile-8*2-1);
        bkgNegAmplitudeHistogramList[iFile][iAnode]->SetLineStyle(2); 
        bkgNegAmplitudeHistogramList[iFile][iAnode]->SetMarkerStyle(27);

        sglNegAmplitudeHistogramList[iFile][iAnode]->SetLineColor(iFile-8*2-1);
        sglNegAmplitudeHistogramList[iFile][iAnode]->SetMarkerColor(iFile-8*2-1);
        sglNegAmplitudeHistogramList[iFile][iAnode]->SetLineStyle(2); 
        sglNegAmplitudeHistogramList[iFile][iAnode]->SetMarkerStyle(33);

        currentSglMinusBkgNegAmplitudeHistogram->SetLineColor(iFile-8*2-1);
        currentSglMinusBkgNegAmplitudeHistogram->SetMarkerColor(iFile-8*2-1);
        currentSglMinusBkgNegAmplitudeHistogram->SetLineStyle(2); 
        currentSglMinusBkgNegAmplitudeHistogram->SetMarkerStyle(21);
      }

      if (iFile == 0) { 
        amplitudeSglAndBkgCanvas->cd();
        bkgNegAmplitudeHistogramList[iFile][iAnode]->Draw("P0C"); 
        sglNegAmplitudeHistogramList[iFile][iAnode]->Draw("same");
        amplitudeSglMinusBkgCanvas->cd();
        currentSglMinusBkgNegAmplitudeHistogram->Draw("P0C");
      }
      else { 
        amplitudeSglAndBkgCanvas->cd();
        bkgNegAmplitudeHistogramList[iFile][iAnode]->Draw("same");
        sglNegAmplitudeHistogramList[iFile][iAnode]->Draw("same");
        amplitudeSglMinusBkgCanvas->cd();
        currentSglMinusBkgNegAmplitudeHistogram->Draw("same");
      }

      currentSglMinusBkgNegAmplitudeHistogramList.push_back(currentSglMinusBkgNegAmplitudeHistogram);
    }

    amplitudeSglAndBkgCanvas->cd();
    sglAndBkgAmplitudeLegend->Draw("same");
    amplitudeSglAndBkgCanvas->Update();
    amplitudeSglAndBkgCanvas->Print(file_name_flag+Form("negAmplitudeSglAndBkgCanvas_Anode%i_log.pdf", iAnode+1));

    amplitudeSglMinusBkgCanvas->cd();
    sglMinusBkgAmplitudeLegend->Draw("same");
    amplitudeSglMinusBkgCanvas->Update();
    amplitudeSglMinusBkgCanvas->Print(file_name_flag+Form("negAmplitudeSglMinusBkgCanvas_Anode%i_log.pdf", iAnode+1));

    amplitudeSglAndBkgCanvas->SetLogy(0);
    gStyle->SetOptStat(0);

    amplitudeSglMinusBkgCanvas->SetLogy(0);
    gStyle->SetOptStat(0);

    for(unsigned int iFile = 0; iFile < inputBkgFiles.size() && iFile < inputSglFiles.size(); iFile++){
      if (iFile == 0) { 
        bkgNegAmplitudeHistogramList[iFile][iAnode]->SetMinimum(amplitude_count_min);
        bkgNegAmplitudeHistogramList[iFile][iAnode]->SetMaximum(amplitude_count_max);
        sglNegAmplitudeHistogramList[iFile][iAnode]->SetMinimum(amplitude_count_min);
        sglNegAmplitudeHistogramList[iFile][iAnode]->SetMaximum(amplitude_count_max);
        
        amplitudeSglAndBkgCanvas->cd();
        bkgNegAmplitudeHistogramList[iFile][iAnode]->Draw("P0C");
        sglNegAmplitudeHistogramList[iFile][iAnode]->Draw("same"); 

        amplitudeSglMinusBkgCanvas->cd();
        currentSglMinusBkgNegAmplitudeHistogramList[iFile]->SetMinimum(amplitude_count_min);
        currentSglMinusBkgNegAmplitudeHistogramList[iFile]->SetMaximum(amplitude_count_max);
        currentSglMinusBkgNegAmplitudeHistogramList[iFile]->Draw("P0C");
      }
      else { 
        amplitudeSglAndBkgCanvas->cd();
        bkgNegAmplitudeHistogramList[iFile][iAnode]->Draw("same");
        sglNegAmplitudeHistogramList[iFile][iAnode]->Draw("same"); 

        amplitudeSglMinusBkgCanvas->cd();
        currentSglMinusBkgNegAmplitudeHistogramList[iFile]->Draw("same");
      }
    }

    amplitudeSglAndBkgCanvas->cd();
    sglAndBkgAmplitudeLegend->Draw("same");
    amplitudeSglAndBkgCanvas->Update();
    amplitudeSglAndBkgCanvas->Print(file_name_flag+Form("negAmplitudeSglAndBkgCanvas_Anode%i.pdf", iAnode+1));

    amplitudeSglMinusBkgCanvas->cd();
    sglMinusBkgAmplitudeLegend->Draw("same");
    amplitudeSglMinusBkgCanvas->Update();
    amplitudeSglMinusBkgCanvas->Print(file_name_flag+Form("negAmplitudeSglMinusBkgCanvas_Anode%i.pdf", iAnode+1));

    sglMinusBkgNegAmplitudeHistogramList.push_back(currentSglMinusBkgNegAmplitudeHistogramList);

    // RMS
    // signal and background in one messy canvas
    TCanvas *waveformRMSSglAndBkgCanvas = new TCanvas(Form("waveformRMSSglAndBkgCanvas_Anode%i", iAnode+1), Form("waveformRMSSglAndBkgCanvas_Anode%i", iAnode+1), 800, 700);
    waveformRMSSglAndBkgCanvas->SetLeftMargin(0.12);
    waveformRMSSglAndBkgCanvas->SetRightMargin(0.12);
    waveformRMSSglAndBkgCanvas->SetBottomMargin(0.125);
    waveformRMSSglAndBkgCanvas->SetLogy();
    gStyle->SetOptStat(0);
    waveformRMSSglAndBkgCanvas->cd();

    // signal minus background in one slightly less canvas
    TCanvas *WaveformRMSSglMinusBkgCanvas = new TCanvas(Form("WaveformRMSSglMinusBkgCanvas_Anode%i", iAnode+1), Form("WaveformRMSSglMinusBkgCanvas_Anode%i", iAnode+1), 800, 700);
    WaveformRMSSglMinusBkgCanvas->SetLeftMargin(0.12);
    WaveformRMSSglMinusBkgCanvas->SetRightMargin(0.12);
    WaveformRMSSglMinusBkgCanvas->SetBottomMargin(0.125);
    WaveformRMSSglMinusBkgCanvas->SetLogy();
    gStyle->SetOptStat(0);
    WaveformRMSSglMinusBkgCanvas->cd();

    // storage
    std::vector< TH1D*> currentSglMinusBkgRMSHistogramList;

    for(unsigned int iFile = 0; iFile < inputBkgFiles.size() && iFile < inputSglFiles.size(); iFile++){
      bkgRMSHistogramList[iFile][iAnode]->Scale(1.0/static_cast<float>(bkgEventCount[iFile]));
      bkgRMSHistogramList[iFile][iAnode]->SetMarkerSize(0.6);
      bkgRMSHistogramList[iFile][iAnode]->SetMinimum(RMS_count_min_log);
      bkgRMSHistogramList[iFile][iAnode]->SetMaximum(RMS_count_max_log);

      sglRMSHistogramList[iFile][iAnode]->Scale(1.0/static_cast<float>(sglEventCount[iFile]));
      sglRMSHistogramList[iFile][iAnode]->SetMarkerSize(0.6);
      sglRMSHistogramList[iFile][iAnode]->SetMinimum(RMS_count_min_log);
      sglRMSHistogramList[iFile][iAnode]->SetMaximum(RMS_count_max_log);

      TH1D* currentSglMinusBkgRMSHistogram = static_cast<TH1D*>(sglRMSHistogramList[iFile][iAnode]->Clone());
      currentSglMinusBkgRMSHistogram->Add(bkgRMSHistogramList[iFile][iAnode], -1);
      currentSglMinusBkgRMSHistogram->SetMarkerSize(0.6);
      currentSglMinusBkgRMSHistogram->SetMinimum(RMS_count_min_log);
      currentSglMinusBkgRMSHistogram->SetMaximum(RMS_count_max_log);

      if(iFile+1 < 10) { 
        bkgRMSHistogramList[iFile][iAnode]->SetLineColor(iFile+1);
        bkgRMSHistogramList[iFile][iAnode]->SetMarkerColor(iFile+1);
        bkgRMSHistogramList[iFile][iAnode]->SetMarkerStyle(22);

        sglRMSHistogramList[iFile][iAnode]->SetLineColor(iFile+1);
        sglRMSHistogramList[iFile][iAnode]->SetMarkerColor(iFile+1);
        sglRMSHistogramList[iFile][iAnode]->SetMarkerStyle(23);

        currentSglMinusBkgRMSHistogram->SetLineColor(iFile+1);
        currentSglMinusBkgRMSHistogram->SetMarkerColor(iFile+1);
        currentSglMinusBkgRMSHistogram->SetMarkerStyle(20);        
      }
      else if(iFile-8 < 10){ 
        bkgRMSHistogramList[iFile][iAnode]->SetLineColor(iFile-8);
        bkgRMSHistogramList[iFile][iAnode]->SetMarkerColor(iFile-8);
        bkgRMSHistogramList[iFile][iAnode]->SetLineStyle(2); 
        bkgRMSHistogramList[iFile][iAnode]->SetMarkerStyle(26);

        sglRMSHistogramList[iFile][iAnode]->SetLineColor(iFile-8);
        sglRMSHistogramList[iFile][iAnode]->SetMarkerColor(iFile-8);
        sglRMSHistogramList[iFile][iAnode]->SetLineStyle(2); 
        sglRMSHistogramList[iFile][iAnode]->SetMarkerStyle(32);

        currentSglMinusBkgRMSHistogram->SetLineColor(iFile-8);
        currentSglMinusBkgRMSHistogram->SetMarkerColor(iFile-8);
        currentSglMinusBkgRMSHistogram->SetLineStyle(2); 
        currentSglMinusBkgRMSHistogram->SetMarkerStyle(24);
      }
      else {
        bkgRMSHistogramList[iFile][iAnode]->SetLineColor(iFile-8*2-1);
        bkgRMSHistogramList[iFile][iAnode]->SetMarkerColor(iFile-8*2-1);
        bkgRMSHistogramList[iFile][iAnode]->SetLineStyle(2); 
        bkgRMSHistogramList[iFile][iAnode]->SetMarkerStyle(27);

        sglRMSHistogramList[iFile][iAnode]->SetLineColor(iFile-8*2-1);
        sglRMSHistogramList[iFile][iAnode]->SetMarkerColor(iFile-8*2-1);
        sglRMSHistogramList[iFile][iAnode]->SetLineStyle(2); 
        sglRMSHistogramList[iFile][iAnode]->SetMarkerStyle(33);

        currentSglMinusBkgRMSHistogram->SetLineColor(iFile-8*2-1);
        currentSglMinusBkgRMSHistogram->SetMarkerColor(iFile-8*2-1);
        currentSglMinusBkgRMSHistogram->SetLineStyle(2); 
        currentSglMinusBkgRMSHistogram->SetMarkerStyle(21);
      }

      currentSglMinusBkgRMSHistogramList.push_back(currentSglMinusBkgRMSHistogram);

      if (iFile == 0) { 
        waveformRMSSglAndBkgCanvas->cd();
        bkgRMSHistogramList[iFile][iAnode]->Draw("P0C"); 
        sglRMSHistogramList[iFile][iAnode]->Draw("same");
        WaveformRMSSglMinusBkgCanvas->cd();
        currentSglMinusBkgRMSHistogram->Draw("P0C");
      }
      else { 
        waveformRMSSglAndBkgCanvas->cd();
        bkgRMSHistogramList[iFile][iAnode]->Draw("same");
        sglRMSHistogramList[iFile][iAnode]->Draw("same");
        WaveformRMSSglMinusBkgCanvas->cd();
        currentSglMinusBkgRMSHistogram->Draw("same");
      }
    }

    waveformRMSSglAndBkgCanvas->cd();
    sglAndBkgAmplitudeLegend->Draw("same");
    waveformRMSSglAndBkgCanvas->Update();
    waveformRMSSglAndBkgCanvas->Print(file_name_flag+Form("waveformRMSSglAndBkgCanvas_Anode%i_log.pdf", iAnode+1));

    WaveformRMSSglMinusBkgCanvas->cd();
    sglMinusBkgAmplitudeLegend->Draw("same");
    WaveformRMSSglMinusBkgCanvas->Update();
    WaveformRMSSglMinusBkgCanvas->Print(file_name_flag+Form("WaveformRMSSglMinusBkgCanvas_Anode%i_log.pdf", iAnode+1));

    waveformRMSSglAndBkgCanvas->SetLogy(0);
    gStyle->SetOptStat(0);

    WaveformRMSSglMinusBkgCanvas->SetLogy(0);
    gStyle->SetOptStat(0);

    for(unsigned int iFile = 0; iFile < inputBkgFiles.size() && iFile < inputSglFiles.size(); iFile++){
      if (iFile == 0) { 
        bkgRMSHistogramList[iFile][iAnode]->SetMinimum(RMS_count_min);
        bkgRMSHistogramList[iFile][iAnode]->SetMaximum(RMS_count_max);
        sglRMSHistogramList[iFile][iAnode]->SetMinimum(RMS_count_min);
        sglRMSHistogramList[iFile][iAnode]->SetMaximum(RMS_count_max);
        
        waveformRMSSglAndBkgCanvas->cd();
        bkgRMSHistogramList[iFile][iAnode]->Draw("P0C");
        sglRMSHistogramList[iFile][iAnode]->Draw("same"); 

        WaveformRMSSglMinusBkgCanvas->cd();
        currentSglMinusBkgRMSHistogramList[iFile]->SetMaximum(RMS_count_max);
        currentSglMinusBkgRMSHistogramList[iFile]->SetMinimum(RMS_count_min);
        currentSglMinusBkgRMSHistogramList[iFile]->Draw("P0C");
      }
      else { 
        waveformRMSSglAndBkgCanvas->cd();
        bkgRMSHistogramList[iFile][iAnode]->Draw("same");
        sglRMSHistogramList[iFile][iAnode]->Draw("same"); 

        WaveformRMSSglMinusBkgCanvas->cd();
        currentSglMinusBkgRMSHistogramList[iFile]->Draw("same");
      }
    }

    waveformRMSSglAndBkgCanvas->cd();
    sglAndBkgAmplitudeLegend->Draw("same");
    waveformRMSSglAndBkgCanvas->Update();
    waveformRMSSglAndBkgCanvas->Print(file_name_flag+Form("waveformRMSSglAndBkgCanvas_Anode%i.pdf", iAnode+1));

    WaveformRMSSglMinusBkgCanvas->cd();
    sglMinusBkgAmplitudeLegend->Draw("same");
    WaveformRMSSglMinusBkgCanvas->Update();
    WaveformRMSSglMinusBkgCanvas->Print(file_name_flag+Form("WaveformRMSSglMinusBkgCanvas_Anode%i.pdf", iAnode+1));

    sglMinusBkgRMSHistogramListHistogramList.push_back(currentSglMinusBkgRMSHistogramList);


    // baseline RMS
    // signal and background in one messy canvas
    TCanvas *baselineRMSSglAndBkgCanvas = new TCanvas(Form("baselineRMSSglAndBkgCanvas_Anode%i", iAnode+1), Form("baselineRMSSglAndBkgCanvas_Anode%i", iAnode+1), 800, 700);
    baselineRMSSglAndBkgCanvas->SetLeftMargin(0.12);
    baselineRMSSglAndBkgCanvas->SetRightMargin(0.12);
    baselineRMSSglAndBkgCanvas->SetBottomMargin(0.125);
    baselineRMSSglAndBkgCanvas->SetLogy();
    gStyle->SetOptStat(0);
    baselineRMSSglAndBkgCanvas->cd();

    // signal minus background in one slightly less canvas
    TCanvas *baselineRMSSglMinusBkgCanvas = new TCanvas(Form("baselineRMSSglMinusBkgCanvas_Anode%i", iAnode+1), Form("baselineRMSSglMinusBkgCanvas_Anode%i", iAnode+1), 800, 700);
    baselineRMSSglMinusBkgCanvas->SetLeftMargin(0.12);
    baselineRMSSglMinusBkgCanvas->SetRightMargin(0.12);
    baselineRMSSglMinusBkgCanvas->SetBottomMargin(0.125);
    baselineRMSSglMinusBkgCanvas->SetLogy();
    gStyle->SetOptStat(0);
    baselineRMSSglMinusBkgCanvas->cd();

    // storage
    std::vector< TH1D*> currentSglMinusBkgBaselineRMSHistogramList;

    for(unsigned int iFile = 0; iFile < inputBkgFiles.size() && iFile < inputSglFiles.size(); iFile++){
      bkgBaselineRMSHistogramList[iFile][iAnode]->Scale(1.0/static_cast<float>(bkgEventCount[iFile]));
      bkgBaselineRMSHistogramList[iFile][iAnode]->SetMarkerSize(0.6);
      bkgBaselineRMSHistogramList[iFile][iAnode]->SetMinimum(RMS_count_min_log);
      bkgBaselineRMSHistogramList[iFile][iAnode]->SetMaximum(RMS_count_max_log);

      sglBaselineRMSHistogramList[iFile][iAnode]->Scale(1.0/static_cast<float>(sglEventCount[iFile]));
      sglBaselineRMSHistogramList[iFile][iAnode]->SetMarkerSize(0.6);
      sglBaselineRMSHistogramList[iFile][iAnode]->SetMinimum(RMS_count_min_log);
      sglBaselineRMSHistogramList[iFile][iAnode]->SetMaximum(RMS_count_max_log);

      TH1D* currentSglMinusBkgBaselineRMSHistogram = static_cast<TH1D*>(sglBaselineRMSHistogramList[iFile][iAnode]->Clone());
      currentSglMinusBkgBaselineRMSHistogram->Add(bkgBaselineRMSHistogramList[iFile][iAnode], -1);
      currentSglMinusBkgBaselineRMSHistogram->SetMarkerSize(0.6);
      currentSglMinusBkgBaselineRMSHistogram->SetMinimum(RMS_count_min_log);
      currentSglMinusBkgBaselineRMSHistogram->SetMaximum(RMS_count_max_log);

      if(iFile+1 < 10) { 
        bkgBaselineRMSHistogramList[iFile][iAnode]->SetLineColor(iFile+1);
        bkgBaselineRMSHistogramList[iFile][iAnode]->SetMarkerColor(iFile+1);
        bkgBaselineRMSHistogramList[iFile][iAnode]->SetMarkerStyle(22);

        sglBaselineRMSHistogramList[iFile][iAnode]->SetLineColor(iFile+1);
        sglBaselineRMSHistogramList[iFile][iAnode]->SetMarkerColor(iFile+1);
        sglBaselineRMSHistogramList[iFile][iAnode]->SetMarkerStyle(23);

        currentSglMinusBkgBaselineRMSHistogram->SetLineColor(iFile+1);
        currentSglMinusBkgBaselineRMSHistogram->SetMarkerColor(iFile+1);
        currentSglMinusBkgBaselineRMSHistogram->SetMarkerStyle(20);        
      }
      else if(iFile-8 < 10){ 
        bkgBaselineRMSHistogramList[iFile][iAnode]->SetLineColor(iFile-8);
        bkgBaselineRMSHistogramList[iFile][iAnode]->SetMarkerColor(iFile-8);
        bkgBaselineRMSHistogramList[iFile][iAnode]->SetLineStyle(2); 
        bkgBaselineRMSHistogramList[iFile][iAnode]->SetMarkerStyle(26);

        sglBaselineRMSHistogramList[iFile][iAnode]->SetLineColor(iFile-8);
        sglBaselineRMSHistogramList[iFile][iAnode]->SetMarkerColor(iFile-8);
        sglBaselineRMSHistogramList[iFile][iAnode]->SetLineStyle(2); 
        sglBaselineRMSHistogramList[iFile][iAnode]->SetMarkerStyle(32);

        currentSglMinusBkgBaselineRMSHistogram->SetLineColor(iFile-8);
        currentSglMinusBkgBaselineRMSHistogram->SetMarkerColor(iFile-8);
        currentSglMinusBkgBaselineRMSHistogram->SetLineStyle(2); 
        currentSglMinusBkgBaselineRMSHistogram->SetMarkerStyle(24);
      }
      else {
        bkgBaselineRMSHistogramList[iFile][iAnode]->SetLineColor(iFile-8*2-1);
        bkgBaselineRMSHistogramList[iFile][iAnode]->SetMarkerColor(iFile-8*2-1);
        bkgBaselineRMSHistogramList[iFile][iAnode]->SetLineStyle(2); 
        bkgBaselineRMSHistogramList[iFile][iAnode]->SetMarkerStyle(27);

        sglBaselineRMSHistogramList[iFile][iAnode]->SetLineColor(iFile-8*2-1);
        sglBaselineRMSHistogramList[iFile][iAnode]->SetMarkerColor(iFile-8*2-1);
        sglBaselineRMSHistogramList[iFile][iAnode]->SetLineStyle(2); 
        sglBaselineRMSHistogramList[iFile][iAnode]->SetMarkerStyle(33);

        currentSglMinusBkgBaselineRMSHistogram->SetLineColor(iFile-8*2-1);
        currentSglMinusBkgBaselineRMSHistogram->SetMarkerColor(iFile-8*2-1);
        currentSglMinusBkgBaselineRMSHistogram->SetLineStyle(2); 
        currentSglMinusBkgBaselineRMSHistogram->SetMarkerStyle(21);
      }

      currentSglMinusBkgBaselineRMSHistogramList.push_back(currentSglMinusBkgBaselineRMSHistogram);

      if (iFile == 0) { 
        baselineRMSSglAndBkgCanvas->cd();
        bkgBaselineRMSHistogramList[iFile][iAnode]->Draw("P0C"); 
        sglBaselineRMSHistogramList[iFile][iAnode]->Draw("same");
        baselineRMSSglMinusBkgCanvas->cd();
        currentSglMinusBkgBaselineRMSHistogram->Draw("P0C");
      }
      else { 
        baselineRMSSglAndBkgCanvas->cd();
        bkgBaselineRMSHistogramList[iFile][iAnode]->Draw("same");
        sglBaselineRMSHistogramList[iFile][iAnode]->Draw("same");
        baselineRMSSglMinusBkgCanvas->cd();
        currentSglMinusBkgBaselineRMSHistogram->Draw("same");
      }
    }

    baselineRMSSglAndBkgCanvas->cd();
    sglAndBkgAmplitudeLegend->Draw("same");
    baselineRMSSglAndBkgCanvas->Update();
    baselineRMSSglAndBkgCanvas->Print(file_name_flag+Form("baselineRMSSglAndBkgCanvas_Anode%i_log.pdf", iAnode+1));

    baselineRMSSglMinusBkgCanvas->cd();
    sglMinusBkgAmplitudeLegend->Draw("same");
    baselineRMSSglMinusBkgCanvas->Update();
    baselineRMSSglMinusBkgCanvas->Print(file_name_flag+Form("baselineRMSSglMinusBkgCanvas_Anode%i_log.pdf", iAnode+1));

    baselineRMSSglAndBkgCanvas->SetLogy(0);
    gStyle->SetOptStat(0);

    baselineRMSSglMinusBkgCanvas->SetLogy(0);
    gStyle->SetOptStat(0);

    for(unsigned int iFile = 0; iFile < inputBkgFiles.size() && iFile < inputSglFiles.size(); iFile++){
      if (iFile == 0) { 
        bkgBaselineRMSHistogramList[iFile][iAnode]->SetMinimum(RMS_count_min);
        bkgBaselineRMSHistogramList[iFile][iAnode]->SetMaximum(RMS_count_max);
        sglBaselineRMSHistogramList[iFile][iAnode]->SetMinimum(RMS_count_min);
        sglBaselineRMSHistogramList[iFile][iAnode]->SetMaximum(RMS_count_max);
        
        baselineRMSSglAndBkgCanvas->cd();
        bkgBaselineRMSHistogramList[iFile][iAnode]->Draw("P0C");
        sglBaselineRMSHistogramList[iFile][iAnode]->Draw("same"); 

        baselineRMSSglMinusBkgCanvas->cd();
        currentSglMinusBkgBaselineRMSHistogramList[iFile]->SetMinimum(RMS_count_min);
        currentSglMinusBkgBaselineRMSHistogramList[iFile]->SetMaximum(RMS_count_max);
        currentSglMinusBkgBaselineRMSHistogramList[iFile]->Draw("P0C");
      }
      else { 
        baselineRMSSglAndBkgCanvas->cd();
        bkgBaselineRMSHistogramList[iFile][iAnode]->Draw("same");
        sglBaselineRMSHistogramList[iFile][iAnode]->Draw("same"); 

        baselineRMSSglMinusBkgCanvas->cd();
        currentSglMinusBkgBaselineRMSHistogramList[iFile]->Draw("same");
      }
    }

    baselineRMSSglAndBkgCanvas->cd();
    sglAndBkgAmplitudeLegend->Draw("same");
    baselineRMSSglAndBkgCanvas->Update();
    baselineRMSSglAndBkgCanvas->Print(file_name_flag+Form("baselineRMSSglAndBkgCanvas_Anode%i.pdf", iAnode+1));

    baselineRMSSglMinusBkgCanvas->cd();
    sglMinusBkgAmplitudeLegend->Draw("same");
    baselineRMSSglMinusBkgCanvas->Update();
    baselineRMSSglMinusBkgCanvas->Print(file_name_flag+Form("baselineRMSSglMinusBkgCanvas_Anode%i.pdf", iAnode+1));

    sglMinusBkgBaselineRMSHistogramList.push_back(currentSglMinusBkgBaselineRMSHistogramList);
  

    // baseline
    // signal and background in one messy canvas
    TCanvas *baselineSglAndBkgCanvas = new TCanvas(Form("baselineSglAndBkgCanvas_Anode%i", iAnode+1), Form("baselineSglAndBkgCanvas_Anode%i", iAnode+1), 800, 700);
    baselineSglAndBkgCanvas->SetLeftMargin(0.12);
    baselineSglAndBkgCanvas->SetRightMargin(0.12);
    baselineSglAndBkgCanvas->SetBottomMargin(0.125);
    baselineSglAndBkgCanvas->SetLogy();
    gStyle->SetOptStat(0);
    baselineSglAndBkgCanvas->cd();

    // signal minus background in one slightly less canvas
    TCanvas *baselineSglMinusBkgCanvas = new TCanvas(Form("baselineSglMinusBkgCanvas_Anode%i", iAnode+1), Form("baselineSglMinusBkgCanvas_Anode%i", iAnode+1), 800, 700);
    baselineSglMinusBkgCanvas->SetLeftMargin(0.12);
    baselineSglMinusBkgCanvas->SetRightMargin(0.12);
    baselineSglMinusBkgCanvas->SetBottomMargin(0.125);
    baselineSglMinusBkgCanvas->SetLogy();
    gStyle->SetOptStat(0);
    baselineSglMinusBkgCanvas->cd();

    // storage
    std::vector< TH1D*> currentSglMinusBkgBaselineMeanHistogramList;

    for(unsigned int iFile = 0; iFile < inputBkgFiles.size() && iFile < inputSglFiles.size(); iFile++){
      bkgBaselineMeanHistogramList[iFile][iAnode]->Scale(1.0/static_cast<float>(bkgEventCount[iFile]));
      bkgBaselineMeanHistogramList[iFile][iAnode]->SetMarkerSize(0.6);
      bkgBaselineMeanHistogramList[iFile][iAnode]->SetMinimum(baseline_count_min_log);
      bkgBaselineMeanHistogramList[iFile][iAnode]->SetMaximum(baseline_count_max_log);

      sglBaselineMeanHistogramList[iFile][iAnode]->Scale(1.0/static_cast<float>(sglEventCount[iFile]));
      sglBaselineMeanHistogramList[iFile][iAnode]->SetMarkerSize(0.6);
      sglBaselineMeanHistogramList[iFile][iAnode]->SetMinimum(baseline_count_min_log);
      sglBaselineMeanHistogramList[iFile][iAnode]->SetMaximum(baseline_count_max_log);

      TH1D* currentSglMinusBkgBaselineMeanHistogramListHistogram = static_cast<TH1D*>(sglBaselineMeanHistogramList[iFile][iAnode]->Clone());
      currentSglMinusBkgBaselineMeanHistogramListHistogram->Add(bkgBaselineMeanHistogramList[iFile][iAnode], -1);
      currentSglMinusBkgBaselineMeanHistogramListHistogram->SetMarkerSize(0.6);
      currentSglMinusBkgBaselineMeanHistogramListHistogram->SetMinimum(baseline_count_min_log);
      currentSglMinusBkgBaselineMeanHistogramListHistogram->SetMaximum(baseline_count_max_log);

      if(iFile+1 < 10) { 
        bkgBaselineMeanHistogramList[iFile][iAnode]->SetLineColor(iFile+1);
        bkgBaselineMeanHistogramList[iFile][iAnode]->SetMarkerColor(iFile+1);
        bkgBaselineMeanHistogramList[iFile][iAnode]->SetMarkerStyle(22);

        sglBaselineMeanHistogramList[iFile][iAnode]->SetLineColor(iFile+1);
        sglBaselineMeanHistogramList[iFile][iAnode]->SetMarkerColor(iFile+1);
        sglBaselineMeanHistogramList[iFile][iAnode]->SetMarkerStyle(23);

        currentSglMinusBkgBaselineMeanHistogramListHistogram->SetLineColor(iFile+1);
        currentSglMinusBkgBaselineMeanHistogramListHistogram->SetMarkerColor(iFile+1);
        currentSglMinusBkgBaselineMeanHistogramListHistogram->SetMarkerStyle(20);        
      }
      else if(iFile-8 < 10){ 
        bkgBaselineMeanHistogramList[iFile][iAnode]->SetLineColor(iFile-8);
        bkgBaselineMeanHistogramList[iFile][iAnode]->SetMarkerColor(iFile-8);
        bkgBaselineMeanHistogramList[iFile][iAnode]->SetLineStyle(2); 
        bkgBaselineMeanHistogramList[iFile][iAnode]->SetMarkerStyle(26);

        sglBaselineMeanHistogramList[iFile][iAnode]->SetLineColor(iFile-8);
        sglBaselineMeanHistogramList[iFile][iAnode]->SetMarkerColor(iFile-8);
        sglBaselineMeanHistogramList[iFile][iAnode]->SetLineStyle(2); 
        sglBaselineMeanHistogramList[iFile][iAnode]->SetMarkerStyle(32);

        currentSglMinusBkgBaselineMeanHistogramListHistogram->SetLineColor(iFile-8);
        currentSglMinusBkgBaselineMeanHistogramListHistogram->SetMarkerColor(iFile-8);
        currentSglMinusBkgBaselineMeanHistogramListHistogram->SetLineStyle(2); 
        currentSglMinusBkgBaselineMeanHistogramListHistogram->SetMarkerStyle(24);
      }
      else {
        bkgBaselineMeanHistogramList[iFile][iAnode]->SetLineColor(iFile-8*2-1);
        bkgBaselineMeanHistogramList[iFile][iAnode]->SetMarkerColor(iFile-8*2-1);
        bkgBaselineMeanHistogramList[iFile][iAnode]->SetLineStyle(2); 
        bkgBaselineMeanHistogramList[iFile][iAnode]->SetMarkerStyle(27);

        sglBaselineMeanHistogramList[iFile][iAnode]->SetLineColor(iFile-8*2-1);
        sglBaselineMeanHistogramList[iFile][iAnode]->SetMarkerColor(iFile-8*2-1);
        sglBaselineMeanHistogramList[iFile][iAnode]->SetLineStyle(2); 
        sglBaselineMeanHistogramList[iFile][iAnode]->SetMarkerStyle(33);

        currentSglMinusBkgBaselineMeanHistogramListHistogram->SetLineColor(iFile-8*2-1);
        currentSglMinusBkgBaselineMeanHistogramListHistogram->SetMarkerColor(iFile-8*2-1);
        currentSglMinusBkgBaselineMeanHistogramListHistogram->SetLineStyle(2); 
        currentSglMinusBkgBaselineMeanHistogramListHistogram->SetMarkerStyle(21);
      }

      currentSglMinusBkgBaselineMeanHistogramList.push_back(currentSglMinusBkgBaselineMeanHistogramListHistogram);

      if (iFile == 0) { 
        baselineSglAndBkgCanvas->cd();
        bkgBaselineMeanHistogramList[iFile][iAnode]->Draw("P0C"); 
        sglBaselineMeanHistogramList[iFile][iAnode]->Draw("same");
        baselineSglMinusBkgCanvas->cd();
        currentSglMinusBkgBaselineMeanHistogramListHistogram->Draw("P0C");
      }
      else { 
        baselineSglAndBkgCanvas->cd();
        bkgBaselineMeanHistogramList[iFile][iAnode]->Draw("same");
        sglBaselineMeanHistogramList[iFile][iAnode]->Draw("same");
        baselineSglMinusBkgCanvas->cd();
        currentSglMinusBkgBaselineMeanHistogramListHistogram->Draw("same");
      }
    }

    baselineSglAndBkgCanvas->cd();
    sglAndBkgAmplitudeLegend->Draw("same");
    baselineSglAndBkgCanvas->Update();
    baselineSglAndBkgCanvas->Print(file_name_flag+Form("baselineSglAndBkgCanvas_Anode%i_log.pdf", iAnode+1));

    baselineSglMinusBkgCanvas->cd();
    sglMinusBkgAmplitudeLegend->Draw("same");
    baselineSglMinusBkgCanvas->Update();
    baselineSglMinusBkgCanvas->Print(file_name_flag+Form("baselineSglMinusBkgCanvas_Anode%i_log.pdf", iAnode+1));

    baselineSglAndBkgCanvas->SetLogy(0);
    gStyle->SetOptStat(0);

    baselineSglMinusBkgCanvas->SetLogy(0);
    gStyle->SetOptStat(0);

    for(unsigned int iFile = 0; iFile < inputBkgFiles.size() && iFile < inputSglFiles.size(); iFile++){
      if (iFile == 0) { 
        bkgBaselineMeanHistogramList[iFile][iAnode]->SetMinimum(baseline_count_min);
        bkgBaselineMeanHistogramList[iFile][iAnode]->SetMaximum(baseline_count_max);
        sglBaselineMeanHistogramList[iFile][iAnode]->SetMinimum(baseline_count_min);
        sglBaselineMeanHistogramList[iFile][iAnode]->SetMaximum(baseline_count_max);
        
        baselineSglAndBkgCanvas->cd();
        bkgBaselineMeanHistogramList[iFile][iAnode]->Draw("P0C");
        sglBaselineMeanHistogramList[iFile][iAnode]->Draw("same"); 

        baselineSglMinusBkgCanvas->cd();
        currentSglMinusBkgBaselineMeanHistogramList[iAnode]->Draw("P0C");
      }
      else { 
        baselineSglAndBkgCanvas->cd();
        bkgBaselineMeanHistogramList[iFile][iAnode]->Draw("same");
        sglBaselineMeanHistogramList[iFile][iAnode]->Draw("same"); 

        baselineSglMinusBkgCanvas->cd();
        currentSglMinusBkgBaselineMeanHistogramList[iAnode]->Draw("same");
      }
    }

    baselineSglAndBkgCanvas->cd();
    sglAndBkgAmplitudeLegend->Draw("same");
    baselineSglAndBkgCanvas->Update();
    baselineSglAndBkgCanvas->Print(file_name_flag+Form("baselineSglAndBkgCanvas_Anode%i.pdf", iAnode+1));

    baselineSglMinusBkgCanvas->cd();
    sglMinusBkgAmplitudeLegend->Draw("same");
    baselineSglMinusBkgCanvas->Update();
    baselineSglMinusBkgCanvas->Print(file_name_flag+Form("baselineSglMinusBkgCanvas_Anode%i.pdf", iAnode+1));

    sglMinusBkgBaselineMeanHistogramList.push_back(currentSglMinusBkgBaselineMeanHistogramList);
  }

  // done
  return;
}

// creates 2D correlation plots, data from different files is summed
// amplitude vs amplitude, neg amplitude vs amplitude; amplitude vs neg amplitude, neg amplitude vs neg amplitude, waveform RMS vs waveform RMS
void createChargeGainSummed2Dplots(std::vector<TString> prefix_list, TString cut = "", TString file_name_flag = "", TString input_file_base = "raptorr_tpcdata_gain_%s_noExpSmooth_nowNeg.root"){

  /* Open files and get run numbers and event numbers */
  std::vector<TFile*> inputFiles;
  std::vector<TString> finalPrefixList;
  std::vector<TString> finalLegendList; // <- not really needed here, but we need this as an input parameter for a latter function
  std::vector<int> runNumberCount;
  std::vector<int> eventCount;

  returnFileAndNumberLists(prefix_list, prefix_list, input_file_base, inputFiles, finalPrefixList, finalLegendList, runNumberCount, eventCount);

  /* switches */
  bool lowAmplitudeBinning = false;


  /* binning */
  double amplitude_min = -25.0;
  double amplitude_max = 350.0;
  double amplitude_min_2D = -25.0;
  double amplitude_max_2D = 350.0;
  if(lowAmplitudeBinning) {
    amplitude_min = -5.0;
    amplitude_max = 60.0;
  }

  double RMS_min = -1.0;
  double RMS_max = 80.0;
  
  double RMS_over_amplitude_min = 0.0;
  double RMS_over_amplitude_max = 0.6;
  double RMS_over_amplitude_min_2D = 0.0;
  double RMS_over_amplitude_max_2D = 0.6;

  double binWidth_amplitude = 4;
  double binWidth_amplitude_2D = 5;

  double binWidth_RMS = 0.4;
  double binWidth_RMS_over_amplitude = binWidth_RMS/100.0;


  /* amplitude histograms */
  std::vector<TH2D*> ampVsAmpHistogramList = addPlotList2D(inputFiles, finalPrefixList, "AmplitudeAnode", "AmplitudeAnode", cut, " [mV] ", " [mV]", static_cast<int>((amplitude_max-amplitude_min)/binWidth_amplitude), amplitude_min, amplitude_max, static_cast<int>((amplitude_max-amplitude_min)/binWidth_amplitude), amplitude_min, amplitude_max);
  std::vector<TH2D*> negAmpVsAmpHistogramList = addPlotList2D(inputFiles, finalPrefixList, "NegAmplitudeAnode", "AmplitudeAnode", cut, " [mV] ", " [mV]", static_cast<int>((amplitude_max-amplitude_min)/binWidth_amplitude), amplitude_min, amplitude_max, static_cast<int>((amplitude_max-amplitude_min)/binWidth_amplitude), amplitude_min, amplitude_max);
  std::vector<TH2D*> ampVsNegAmpHistogramList = addPlotList2D(inputFiles, finalPrefixList, "AmplitudeAnode", "NegAmplitudeAnode", cut, " [mV] ", " [mV]", static_cast<int>((amplitude_max-amplitude_min)/binWidth_amplitude), amplitude_min, amplitude_max, static_cast<int>((amplitude_max-amplitude_min)/binWidth_amplitude), amplitude_min, amplitude_max);
  std::vector<TH2D*> negAmpVsNegAmpHistogramList = addPlotList2D(inputFiles, finalPrefixList, "NegAmplitudeAnode", "NegAmplitudeAnode", cut, " [mV] ", " [mV]", static_cast<int>((amplitude_max-amplitude_min)/binWidth_amplitude), amplitude_min, amplitude_max, static_cast<int>((amplitude_max-amplitude_min)/binWidth_amplitude), amplitude_min, amplitude_max);

  /* RMS histograms */
  std::vector< TH2D*> RMSHistogramList = addPlotList2D(inputFiles, finalPrefixList, "WaveformRMSAnode", "WaveformRMSAnode", cut, " [mV] ", " [mV]", static_cast<int>((RMS_max-RMS_min)/binWidth_RMS), RMS_min, RMS_max, static_cast<int>((RMS_max-RMS_min)/binWidth_RMS), RMS_min, RMS_max);




  /* lets plot */
  gStyle->SetOptStat(0);

  TCanvas *ampVsAmpCanvas = new TCanvas("AmplitudeVsAmplitudeCanvas", "AmplitudeVsAmplitudeCanvas", 400*ampVsAmpHistogramList.size(), 400);
  ampVsAmpCanvas->SetLeftMargin(0.12);
  ampVsAmpCanvas->SetRightMargin(0.12);
  ampVsAmpCanvas->SetBottomMargin(0.125);
  ampVsAmpCanvas->Divide(ampVsAmpHistogramList.size(), 1);

  TCanvas *negAmpVsAmpCanvas = new TCanvas("NegAmplitudeVsAmplitudeCanvas", "NegAmplitudeVsAmplitudeCanvas", 400*ampVsAmpHistogramList.size(), 400);
  negAmpVsAmpCanvas->SetLeftMargin(0.12);
  negAmpVsAmpCanvas->SetRightMargin(0.12);
  negAmpVsAmpCanvas->SetBottomMargin(0.125);
  negAmpVsAmpCanvas->Divide(negAmpVsAmpHistogramList.size(), 1);

  TCanvas *ampVsNegAmpCanvas = new TCanvas("AmplitudeVsNegAmplitudeCanvas", "AmplitudeVsNegAmplitudeCanvas", 400*ampVsAmpHistogramList.size(), 400);
  ampVsNegAmpCanvas->SetLeftMargin(0.12);
  ampVsNegAmpCanvas->SetRightMargin(0.12);
  ampVsNegAmpCanvas->SetBottomMargin(0.125);
  ampVsNegAmpCanvas->Divide(ampVsNegAmpHistogramList.size(), 1);

  TCanvas *negAmpVsNegAmpCanvas = new TCanvas("NegAmplitudeVsNegAmplitudeCanvas", "NegAmplitudeVsNegAmplitudeCanvas", 400*ampVsAmpHistogramList.size(), 400);
  negAmpVsNegAmpCanvas->SetLeftMargin(0.12);
  negAmpVsNegAmpCanvas->SetRightMargin(0.12);
  negAmpVsNegAmpCanvas->SetBottomMargin(0.125);
  negAmpVsNegAmpCanvas->Divide(negAmpVsNegAmpHistogramList.size(), 1);

  TCanvas *WFRMSVsWFRMSCanvas = new TCanvas("WaveformRMSVsWaveformRMSCanvas", "WaveformRMSVsWaveformRMSCanvas", 400*ampVsAmpHistogramList.size(), 400);
  WFRMSVsWFRMSCanvas->SetLeftMargin(0.12);
  WFRMSVsWFRMSCanvas->SetRightMargin(0.12);
  WFRMSVsWFRMSCanvas->SetBottomMargin(0.125);
  WFRMSVsWFRMSCanvas->Divide(RMSHistogramList.size(), 1);

  std::cout << " and now for the 2D plots " << std::endl;
  for(unsigned int iPlots = 0; iPlots < ampVsAmpHistogramList.size(); iPlots++){
    std::cout << "ampVsAmpCanvas " << iPlots << " " << ampVsAmpHistogramList.size() << std::endl;
    ampVsAmpCanvas->cd(iPlots+1);
    gPad->SetRightMargin(0.15);
    gPad->SetLeftMargin(0.175);
    gPad->SetBottomMargin(0.175);
    gPad->SetLogz();
    ampVsAmpHistogramList[iPlots]->Draw("colz");
    ampVsAmpCanvas->Update();

    std::cout << "negAmpVsAmpCanvas " << iPlots << " " << negAmpVsAmpHistogramList.size() << std::endl;
    negAmpVsAmpCanvas->cd(iPlots+1);
    gPad->SetRightMargin(0.15);
    gPad->SetLeftMargin(0.175);
    gPad->SetBottomMargin(0.175);
    gPad->SetLogz();
    negAmpVsAmpHistogramList[iPlots]->Draw("colz");
    negAmpVsAmpCanvas->Update();

    std::cout << "ampVsNegAmpCanvas " << iPlots << " " << ampVsNegAmpHistogramList.size() << std::endl;
    ampVsNegAmpCanvas->cd(iPlots+1);
    gPad->SetRightMargin(0.15);
    gPad->SetLeftMargin(0.175);
    gPad->SetBottomMargin(0.175);
    gPad->SetLogz();
    ampVsNegAmpHistogramList[iPlots]->Draw("colz");
    ampVsNegAmpCanvas->Update();
  
    std::cout << "negAmpVsNegAmpCanvas " << iPlots << " " << negAmpVsNegAmpHistogramList.size() << std::endl;
    negAmpVsNegAmpCanvas->cd(iPlots+1);
    gPad->SetRightMargin(0.15);
    gPad->SetLeftMargin(0.175);
    gPad->SetBottomMargin(0.175);
    gPad->SetLogz();
    negAmpVsNegAmpHistogramList[iPlots]->Draw("colz");
    negAmpVsNegAmpCanvas->Update();

    std::cout << "WFRMSVsWFRMSCanvas " << iPlots << " " << RMSHistogramList.size() << std::endl;
    WFRMSVsWFRMSCanvas->cd(iPlots+1);
    gPad->SetRightMargin(0.15);
    gPad->SetLeftMargin(0.175);
    gPad->SetBottomMargin(0.175);
    gPad->SetLogz();
    RMSHistogramList[iPlots]->Draw("colz");
    WFRMSVsWFRMSCanvas->Update();
  }

  ampVsAmpCanvas->Print(file_name_flag+"ampVsAmpCanvas.pdf");
  negAmpVsAmpCanvas->Print(file_name_flag+"negAmpVsAmpCanvas.pdf");
  ampVsNegAmpCanvas->Print(file_name_flag+"ampVsNegAmpCanvas.pdf");
  negAmpVsNegAmpCanvas->Print(file_name_flag+"negAmpVsNegAmpCanvas.pdf");
  WFRMSVsWFRMSCanvas->Print(file_name_flag+"WFRMSVsWFRMSCanvas.pdf");


  // the large plots
  TH2D* ampAnode1vsAmpAnode2_large = returnHistogram("ampAnode1vsAmpAnode2_large", "amplitude anode 2 [mv]", "amplitude anode 1 [mv]", static_cast<int>((amplitude_max-(-amplitude_max))/binWidth_amplitude), (-amplitude_max), amplitude_max, static_cast<int>((amplitude_max-(-amplitude_max))/binWidth_amplitude), (-amplitude_max), amplitude_max);
  TH2D* ampAnode2vsAmpAnode3_large = returnHistogram("ampAnode2vsAmpAnode3_large", "amplitude anode 3 [mv]", "amplitude anode 2 [mv]", static_cast<int>((amplitude_max-(-amplitude_max))/binWidth_amplitude), (-amplitude_max), amplitude_max, static_cast<int>((amplitude_max-(-amplitude_max))/binWidth_amplitude), (-amplitude_max), amplitude_max);
  TH2D* ampAnode3vsAmpAnode1_large = returnHistogram("ampAnode3vsAmpAnode1_large", "amplitude anode 1 [mv]", "amplitude anode 3 [mv]", static_cast<int>((amplitude_max-(-amplitude_max))/binWidth_amplitude), (-amplitude_max), amplitude_max, static_cast<int>((amplitude_max-(-amplitude_max))/binWidth_amplitude), (-amplitude_max), amplitude_max);
  std::vector<TH2D*> combinedAmplitudePlotList;
  combinedAmplitudePlotList.push_back(ampAnode1vsAmpAnode2_large);
  combinedAmplitudePlotList.push_back(ampAnode2vsAmpAnode3_large);
  combinedAmplitudePlotList.push_back(ampAnode3vsAmpAnode1_large);

  std::vector<TH2D*> ampVsAmpHistogramList_toCombine = addPlotList2D(inputFiles, finalPrefixList, "AmplitudeAnode", "AmplitudeAnode", cut, " [mV] ", " [mV]", static_cast<int>((amplitude_max-(-amplitude_max))/binWidth_amplitude), (-amplitude_max), amplitude_max, static_cast<int>((amplitude_max-(-amplitude_max))/binWidth_amplitude), (-amplitude_max), amplitude_max);
  std::vector<TH2D*> negAmpVsAmpHistogramList_toCombine = addPlotList2D(inputFiles, finalPrefixList, "-NegAmplitudeAnode", "AmplitudeAnode", cut, " [mV] ", " [mV]", static_cast<int>((amplitude_max-(-amplitude_max))/binWidth_amplitude), (-amplitude_max), amplitude_max, static_cast<int>((amplitude_max-(-amplitude_max))/binWidth_amplitude), (-amplitude_max), amplitude_max);
  std::vector<TH2D*> ampVsNegAmpHistogramList_toCombine = addPlotList2D(inputFiles, finalPrefixList, "AmplitudeAnode", "-NegAmplitudeAnode", cut, " [mV] ", " [mV]", static_cast<int>((amplitude_max-(-amplitude_max))/binWidth_amplitude), (-amplitude_max), amplitude_max, static_cast<int>((amplitude_max-(-amplitude_max))/binWidth_amplitude), (-amplitude_max), amplitude_max);
  std::vector<TH2D*> negAmpVsNegAmpHistogramList_toCombine = addPlotList2D(inputFiles, finalPrefixList, "-NegAmplitudeAnode", "-NegAmplitudeAnode", cut, " [mV] ", " [mV]", static_cast<int>((amplitude_max-(-amplitude_max))/binWidth_amplitude), (-amplitude_max), amplitude_max, static_cast<int>((amplitude_max-(-amplitude_max))/binWidth_amplitude), (-amplitude_max), amplitude_max);

  TCanvas *ampVsAmpCanvas_combinedPlot = new TCanvas("AmplitudeVsAmplitudeCanvas_combinedPlot", "AmplitudeVsAmplitudeCanvas_combinedPlot", 400*ampVsAmpHistogramList_toCombine.size(), 400);
  ampVsAmpCanvas_combinedPlot->SetLeftMargin(0.12);
  ampVsAmpCanvas_combinedPlot->SetRightMargin(0.12);
  ampVsAmpCanvas_combinedPlot->SetBottomMargin(0.125);
  ampVsAmpCanvas_combinedPlot->Divide(ampVsAmpHistogramList_toCombine.size(), 1);

  std::cout << " .. and the larger one " << std::endl;
  for(unsigned int iPlot = 0; iPlot < ampVsAmpHistogramList_toCombine.size(); iPlot++){
    std::cout << "AmplitudeVsAmplitudeCanvas_combinedPlot " << iPlot << " " << ampVsAmpHistogramList_toCombine.size() << std::endl;
    ampVsAmpCanvas_combinedPlot->cd(iPlot+1);
    gPad->SetRightMargin(0.15);
    gPad->SetLeftMargin(0.175);
    gPad->SetBottomMargin(0.175);
    gPad->SetLogz();

    combinedAmplitudePlotList[iPlot]->Add(ampVsAmpHistogramList_toCombine[iPlot]);
    combinedAmplitudePlotList[iPlot]->Add(negAmpVsAmpHistogramList_toCombine[iPlot]);
    combinedAmplitudePlotList[iPlot]->Add(ampVsNegAmpHistogramList_toCombine[iPlot]);
    combinedAmplitudePlotList[iPlot]->Add(negAmpVsNegAmpHistogramList_toCombine[iPlot]);

    combinedAmplitudePlotList[iPlot]->Draw("col");
    ampVsAmpCanvas_combinedPlot->Update();
  }

  ampVsAmpCanvas_combinedPlot->Print(file_name_flag+"amplitudeVsAmplitudeCanvas_combinedPlot.pdf");

  // done
  return;
}


/** the equivalent to main functions **/

// creates a series of control plots for a given list of runs
void compareDifferentRuns(TString cut = ""){
  /* files */
  std::vector<TString> prefixList;
  std::vector<TString> legendList;
  //prefixList.push_back("R1340169-R1341008");
  //legendList.push_back("SGL, normal: 1500  3600  7400");
  prefixList.push_back("R1352091-R1353014");
  legendList.push_back("BKG, swapped: 1500  3600  7400");
  prefixList.push_back("R1354007-R1354017");
  legendList.push_back("BKG, normal: 1500  3600  7400");
  
  prefixList.push_back("R1354001-R1354006");
  legendList.push_back("BKG, swapped: 1500  3600  7600");
  prefixList.push_back("R1354018-R1354029");
  legendList.push_back("BKG, normal: 1500  3600  7600");
  
  //prefixList.push_back("R1341081-R1341093");
  //legendList.push_back("SGL, normal: 1500  3600  7600");
  //prefixList.push_back("1341095 1341137");
  //legendList.push_back("SGL, normal: 1500  3600  7600");

  /*1D plots*/
  createChargeGain1Dplots(prefixList, legendList, cut);

  /*2D plots*/
  createChargeGainSummed2Dplots(prefixList, cut);

  // done
  return;
}

void bkgVsSignal(TString cut = ""){

  std::vector<TString> sglPrefixList;
  std::vector<TString> sglLegendList;

  std::vector<TString> bkgPrefixList;
  std::vector<TString> bkgLegendList;


  
  bkgPrefixList.push_back("R1352091-R1353014");
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
  bkgLegendList.push_back("BKG, swapped: 1500  3600  7400");
  bkgPrefixList.push_back("R1354007-R1354017");
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
  bkgLegendList.push_back("BKG, normal: 1500  3600  7400");
  bkgPrefixList.push_back("R1354001-R1354006");
  bkgLegendList.push_back("BKG, swapped: 1500  3600  7600");
  bkgPrefixList.push_back("R1354018-R1354029");
  /*
    1354020
    Good to know: 
    _______________
    Anode 1 Baseline: -0.207559 and RMS: 2.94913
    Anode 2 Baseline: -3.15337 and RMS: 6.72412
    Anode 3 Baseline: -1.24704 and RMS: 2.41672
  */
  bkgLegendList.push_back("BKG, normal: 1500  3600  7600");

  sglPrefixList.push_back("R1340169-R1341008");
  /*  1340171
    Good to know: 
    _______________
    Anode 1 Baseline: -1.11843 and RMS: 3.75743
    Anode 2 Baseline: -4.1761 and RMS: 7.3025
    Anode 3 Baseline: -1.45965 and RMS: 3.01661
  */
  sglLegendList.push_back("SGL, normal: 1500  3600  7400");
  sglPrefixList.push_back("R1340169-R1341008");
  sglLegendList.push_back("SGL, normal: 1500  3600  7400");
  sglPrefixList.push_back("R1341081-R1341093");
  sglLegendList.push_back("SGL, normal: 1500  3600  7600");
  sglPrefixList.push_back("R1341095-R1341137");
  sglLegendList.push_back("SGL, normal: 1500  3600  7600");

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
  createChargeGain1Dplots(sglPrefixList, sglLegendList, cut, "sgl_");
  createChargeGain1Dplots(bkgPrefixList, bkgLegendList, cut, "bkg_");
  createChargeGain1Dplots(bkgPrefixList, sglPrefixList, bkgLegendList, sglLegendList, cut, "bkgSgl_");

  /*2D plots*/
  createChargeGainSummed2Dplots(sglPrefixList, cut, "sgl_");
  createChargeGainSummed2Dplots(bkgPrefixList, cut, "bkg_");




  // done
  return;
}