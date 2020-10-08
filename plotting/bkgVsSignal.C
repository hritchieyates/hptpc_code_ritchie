#include <iostream>
#include <memory>
#include <vector>

#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TTree.h"


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

void getRunNumberAndEventNumberLists(TFile* input_file, std::vector<int> & run_numbers, std::vector<int> & nb_of_events){
  run_numbers.clear();
  nb_of_events.clear();

  std::vector<int>* allRunNumbers = 0;
  std::vector<int>* allEventNumbers = 0;
  std::vector<int> tmpRunNumbers;
  std::vector<int> tmpEventNumbers;
  
  std::vector<int> runNumbersStillToCheck;
  std::vector<int> eventNumbersStillToCheck;

  TTree* chargeAnalysisTree = (TTree*)input_file->Get("chargeGainAnalysisTree");
  if(chargeAnalysisTree){
    chargeAnalysisTree->SetBranchAddress("EventNo", &allEventNumbers);
    chargeAnalysisTree->SetBranchAddress("RunNo", &allRunNumbers);
    chargeAnalysisTree->GetEvent(0);

    runNumbersStillToCheck = *allRunNumbers;
    std::cout << runNumbersStillToCheck.size() << std::endl;
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
      for(int iRunNumberId = 0; iRunNumberId < tmpRunNumbers.size(); iRunNumberId++){
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
      //std::cout << currentRunNbToCheck << "\t" << currentMaxEvent  << std::endl;
      // push back result
      run_numbers.push_back(currentRunNbToCheck);
      nb_of_events.push_back(currentMaxEvent);
    }// while search loop
  }//if tree exists


  // done 
  return;
}

std::vector<std::vector<TH1D*> > plotList(std::vector<TFile*> input_files, std::vector<TString> prefix_list, TString branch, TString cut, TString x_axis_name, TString y_axis_name, Int_t nb_of_bins, Double_t min, Double_t max){
  std::vector<std::vector<TH1D*> > returnHistograms;

  for(int iFile = 0; iFile < input_files.size(); iFile++){
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
    std::cout << branch+Form("%i >> ", 1)+prefix_list[iFile]+"_"+branch+Form("%i", 1) << std::endl;
    std::cout << sgl_amplitudeAnode1->GetEntries() << std::endl;
    sgl_amplitudeAnode1->SetDirectory(0);
    currentSetOfHistograms.push_back(sgl_amplitudeAnode1);
    sgl_chargeAnalysisTree->Draw(branch+Form("%i >> ", 2)+prefix_list[iFile]+"_"+branch+Form("%i", 2), cut, "goff");
    std::cout << branch+Form("%i >> ", 2)+prefix_list[iFile]+"_"+branch+Form("%i", 2) << std::endl;
    std::cout << sgl_amplitudeAnode2->GetEntries() << std::endl;
    sgl_amplitudeAnode2->SetDirectory(0);
    currentSetOfHistograms.push_back(sgl_amplitudeAnode2);
    sgl_chargeAnalysisTree->Draw(branch+Form("%i >> ", 3)+prefix_list[iFile]+"_"+branch+Form("%i", 3), cut, "goff");
    std::cout << branch+Form("%i >> ", 3)+prefix_list[iFile]+"_"+branch+Form("%i", 3) << std::endl;
    std::cout << sgl_amplitudeAnode3->GetEntries() << std::endl;
    sgl_amplitudeAnode3->SetDirectory(0);
    currentSetOfHistograms.push_back(sgl_amplitudeAnode3);

    returnHistograms.push_back(currentSetOfHistograms);
  }

  // done
  return returnHistograms;
}


void bkgVsSignal(TString cut = ""){
  /* files */
  // these two are switched around ..
  //TFile *sglFile = new TFile("raptorr_tpcdata_gain_R1287003_to_1287023_noExpSmooth.root", "READ");
  //TFile *bkgFile = TFile::Open("raptorr_tpcdata_gain_R1266001_to_1266024_expSmooth.root");

  std::vector<TString> prefixList;
  /*prefixList.push_back("R1339001-R1339021");
  prefixList.push_back("R1339022-R1339043");
  prefixList.push_back("R1339044-R1339063");
  prefixList.push_back("R1339064-R1340004");
  prefixList.push_back("R1340006-R1340077");
  prefixList.push_back("R1340078-R1340095");
  prefixList.push_back("R1340096-R1340116");
  prefixList.push_back("R1340121-R1340141");
  prefixList.push_back("R1340144-R1340167");
  prefixList.push_back("R1340169-R1341008");
  prefixList.push_back("R1341005-R1341008");
  prefixList.push_back("R1341011-R1341077");
  prefixList.push_back("R1341081-R1341093");
  prefixList.push_back("R1341095-R1341137");
  prefixList.push_back("R1344029-R1344058");*/
  prefixList.push_back("R1347001-to-R1347014");

  std::vector<TFile*> inputFiles;
  std::vector<TString> finalPrefixList;

  for(int iOpenFiles = 0; iOpenFiles < prefixList.size(); iOpenFiles++){
    TFile *currentFile = TFile::Open(prefixList[iOpenFiles]+"_raptorr_tpcdata_gainDMTPC.root");
    if (currentFile) {
      TTree* chargeAnalysisTree = (TTree*)currentFile->Get("chargeGainAnalysisTree");
      if (chargeAnalysisTree) {
        finalPrefixList.push_back(prefixList[iOpenFiles]);
        inputFiles.push_back(currentFile);
      }
      else{
          std::cout << " SORRY, " << prefixList[iOpenFiles] << std::endl;
      }
    }
    else{
      std::cout << " one file not found, " << prefixList[iOpenFiles] << std::endl;
    }
  }

  std::vector<int> runNumbers;
  std::vector<int> eventNumbers;
  getRunNumberAndEventNumberLists(inputFiles[0], runNumbers, eventNumbers);
  for(int iPrintRunNumbers = 0; iPrintRunNumbers < runNumbers.size(); iPrintRunNumbers++){std::cout << runNumbers[iPrintRunNumbers] << "\t" << eventNumbers[iPrintRunNumbers] << std::endl; }


  /* switches */
  bool lowAmplitudeBinning = false;


  /* binning */
  double amplitude_min = -25.0;
  double amplitude_max = 350.0;
  double amplitude_count_min = 0.0;
  double amplitude_count_max = 1200.0;
  double neg_amplitude_count_min = 0.0;
  double neg_amplitude_count_max = 400.0;
  double amplitude_min_2D = -25.0;
  double amplitude_max_2D = 350.0;
  if(lowAmplitudeBinning) {
    amplitude_min = -5.0;
    amplitude_max = 60.0;
    amplitude_min_2D = -5.0;
    amplitude_max_2D = 60.0;
  }

  double baseline_min = -250.0;
  double baseline_max = 350.0;
  double baseline_count_min = 0.0;
  double baseline_count_max = 1200.0;

  double RMS_min = -1.0;
  double RMS_max = 80.0;
  double RMS_count_min = 0.0;
  double RMS_count_max = 800.0;
  double RMS_min_2D = -1.0;
  double RMS_max_2D = 100.0;
  
  double RMS_over_amplitude_min = 0.0;
  double RMS_over_amplitude_max = 0.6;
  double RMS_over_amplitude_min_2D = 0.0;
  double RMS_over_amplitude_max_2D = 0.6;

  double binWidth_amplitude = 4;
  double binWidth_amplitude_2D = 5;

  double binWidth_RMS = 0.4;
  double binWidth_RMS_over_amplitude = binWidth_RMS/100.0;
  double binWidth_RMS_2D = 0.01;
  double binWidth_RMS_over_amplitude_2D = binWidth_RMS/100.0;


  /* integration limits */
  double int_min_anode_1 = 0;
  double int_max_anode_1 = 0;
  double int_min_anode_2 = 0;
  double int_max_anode_2 = 0;
  double int_min_anode_3 = 0;
  double int_max_anode_3 = 100;


  /* amplitude histograms */
  std::vector< std::vector< TH1D*> > amplitudeHistogramList = plotList(inputFiles, finalPrefixList, "AmplitudeAnode", cut, "Amplitude [mV] ", "Counts ", static_cast<int>((amplitude_max-amplitude_min)/binWidth_amplitude), amplitude_min, amplitude_max);
  std::vector< std::vector< TH1D*> > baselineMeanHistogramList = plotList(inputFiles, finalPrefixList, "BaselineAnode", cut, "Baseline mean [mV] ", "Counts ", static_cast<int>((baseline_max-baseline_min)/binWidth_amplitude), baseline_min, baseline_max);
  std::vector< std::vector< TH1D*> > negAmplitudeHistogramList = plotList(inputFiles, finalPrefixList, "NegAmplitudeAnode", cut, "Amplitude (negative polarity) [mV] ", "Counts ", static_cast<int>((amplitude_max-amplitude_min)/binWidth_amplitude), amplitude_min, amplitude_max);


  /* RMS histograms */
  std::vector< std::vector< TH1D*> > RMSHistogramList = plotList(inputFiles, finalPrefixList, "WaveformRMSAnode", cut, "Waveform RMS [mV] ", "Counts ", static_cast<int>((RMS_max-RMS_min)/binWidth_RMS), RMS_min, RMS_max);

  std::vector< std::vector< TH1D*> > baselineRMSHistogramList = plotList(inputFiles, finalPrefixList, "BaselineRMSAnode", cut, "Baseline RMS [mV] ", "Counts ", static_cast<int>((RMS_max-RMS_min)/binWidth_RMS), RMS_min, RMS_max);


  /* shifting bins tests */
  //TH1D* bkg_waveformRMSAnode2_shifted = shiftBins(sgl_waveformRMSAnode2, 40, static_cast<int>((RMS_max-RMS_min)/binWidth_RMS), RMS_min, RMS_max);



  /* lets plot */
  for(int iAnode = 0; iAnode < 3; iAnode++){
    // Amplitude (positive polartiy)
    TCanvas *amplitdeCanvas = new TCanvas(Form("AmplitudeCanvas_Anode%i", iAnode+1), Form("AmplitudeCanvas_Anode%i", iAnode+1), 800, 700);
    amplitdeCanvas->SetLeftMargin(0.12);
    amplitdeCanvas->SetRightMargin(0.12);
    amplitdeCanvas->SetBottomMargin(0.125);
    amplitdeCanvas->SetLogy();
    amplitdeCanvas->cd();

    TLegend* amplitudeLegend = new TLegend(0.6, 0.6, 0.9, 0.9);
  
    for(int iFile = 0; iFile < inputFiles.size(); iFile++){
      if(iFile+1 < 10) { amplitudeHistogramList[iFile][iAnode]->SetLineColor(iFile+1); }
      else { 
        amplitudeHistogramList[iFile][iAnode]->SetLineColor(iFile-8);
        amplitudeHistogramList[iFile][iAnode]->SetLineStyle(2); 
      }
      if (iFile == 0) { amplitudeHistogramList[iFile][iAnode]->Draw(); }
      else { amplitudeHistogramList[iFile][iAnode]->Draw("same"); }
      amplitudeLegend->AddEntry(amplitudeHistogramList[iFile][iAnode], finalPrefixList[iFile], "l");
    }

    amplitudeLegend->Draw("same");
    amplitdeCanvas->Update();
    amplitdeCanvas->Print(Form("amplitdeCanvas_Anode%i_log.pdf", iAnode+1));

    amplitdeCanvas->SetLogy(0);
    gStyle->SetOptStat(0);

    for(int iFile = 0; iFile < inputFiles.size(); iFile++){
      if (iFile == 0) { 
        amplitudeHistogramList[iFile][iAnode]->Draw(); 
        amplitudeHistogramList[iFile][iAnode]->SetMinimum(amplitude_count_min);
        amplitudeHistogramList[iFile][iAnode]->SetMaximum(amplitude_count_max);
      }
      else { amplitudeHistogramList[iFile][iAnode]->Draw("same"); }
    }
    amplitudeLegend->Draw("same");
    amplitdeCanvas->Update();
    amplitdeCanvas->Print(Form("amplitdeCanvas_Anode%i.pdf", iAnode+1));


    // negative polarity
    amplitdeCanvas->SetLogy();
    amplitdeCanvas->cd();

    for(int iFile = 0; iFile < inputFiles.size(); iFile++){
      if(iFile+1 < 10) { negAmplitudeHistogramList[iFile][iAnode]->SetLineColor(iFile+1); }
      else { 
        negAmplitudeHistogramList[iFile][iAnode]->SetLineColor(iFile-8);
        negAmplitudeHistogramList[iFile][iAnode]->SetLineStyle(2); 
      }
      if (iFile == 0) { negAmplitudeHistogramList[iFile][iAnode]->Draw(); }
      else { negAmplitudeHistogramList[iFile][iAnode]->Draw("same"); }
    }

    amplitudeLegend->Draw("same");
    amplitdeCanvas->Update();
    amplitdeCanvas->Print(Form("negAmplitdeCanvas_Anode%i_log.pdf", iAnode+1));

    amplitdeCanvas->SetLogy(0);
    gStyle->SetOptStat(0);

    for(int iFile = 0; iFile < inputFiles.size(); iFile++){
      if (iFile == 0) { 
        negAmplitudeHistogramList[iFile][iAnode]->Draw(); 
        negAmplitudeHistogramList[iFile][iAnode]->SetMinimum(neg_amplitude_count_min);
        negAmplitudeHistogramList[iFile][iAnode]->SetMaximum(neg_amplitude_count_max);
      }
      else { negAmplitudeHistogramList[iFile][iAnode]->Draw("same"); }
    }
    amplitudeLegend->Draw("same");
    amplitdeCanvas->Update();
    amplitdeCanvas->Print(Form("negAmplitdeCanvas_Anode%i.pdf", iAnode+1));


    // RMS
    TCanvas *waveformRMSCanvas = new TCanvas(Form("WaveformRMSCanvas_Anode%i", iAnode+1 ), Form("WaveformRMSCanvas_Anode%i", iAnode+1 ), 800, 700);
    waveformRMSCanvas->SetLeftMargin(0.12);
    waveformRMSCanvas->SetRightMargin(0.12);
    waveformRMSCanvas->SetBottomMargin(0.125);
    waveformRMSCanvas->SetLogy();

    for(int iFile = 0; iFile < inputFiles.size(); iFile++){
      if(iFile+1 < 10) { RMSHistogramList[iFile][iAnode]->SetLineColor(iFile+1); }
      else { 
        RMSHistogramList[iFile][iAnode]->SetLineColor(iFile-8);
        RMSHistogramList[iFile][iAnode]->SetLineStyle(2); 
      }
      if (iFile == 0) { RMSHistogramList[iFile][iAnode]->Draw(); }
      else { RMSHistogramList[iFile][iAnode]->Draw("same"); }
    }

    amplitudeLegend->Draw("same");
    waveformRMSCanvas->Update();
    waveformRMSCanvas->Print(Form("waveformRMSCanvas_Anode%i_log.pdf", iAnode+1));

    waveformRMSCanvas->SetLogy(0);
    gStyle->SetOptStat(0);

    for(int iFile = 0; iFile < inputFiles.size(); iFile++){
      if (iFile == 0) { 
        RMSHistogramList[iFile][iAnode]->Draw(); 
        RMSHistogramList[iFile][iAnode]->SetMinimum(RMS_count_min);
        RMSHistogramList[iFile][iAnode]->SetMaximum(RMS_count_max);
      }
      else { RMSHistogramList[iFile][iAnode]->Draw("same"); }
    }

    waveformRMSCanvas->Print(Form("waveformRMSCanvas_Anode%i.pdf", iAnode+1));


    // baseline RMS
    TCanvas *baselineRMSCanvas = new TCanvas(Form("BaselineRMSCanvas_Anode%i", iAnode+1 ), Form("BaselineRMSCanvas_Anode%i", iAnode+1 ), 800, 700);
    baselineRMSCanvas->SetLeftMargin(0.12);
    baselineRMSCanvas->SetRightMargin(0.12);
    baselineRMSCanvas->SetBottomMargin(0.125);
    baselineRMSCanvas->SetLogy();

    for(int iFile = 0; iFile < inputFiles.size(); iFile++){
      if(iFile+1 < 10) { baselineRMSHistogramList[iFile][iAnode]->SetLineColor(iFile+1); }
      else { 
        baselineRMSHistogramList[iFile][iAnode]->SetLineColor(iFile-8);
        baselineRMSHistogramList[iFile][iAnode]->SetLineStyle(2); 
      }
      if (iFile == 0) { baselineRMSHistogramList[iFile][iAnode]->Draw(); }
      else { baselineRMSHistogramList[iFile][iAnode]->Draw("same"); }
    }

    amplitudeLegend->Draw("same");
    baselineRMSCanvas->Update();
    baselineRMSCanvas->Print(Form("baselineRMSCanvas_Anode%i_log.pdf", iAnode+1));

    baselineRMSCanvas->SetLogy(0);
    gStyle->SetOptStat(0);

    for(int iFile = 0; iFile < inputFiles.size(); iFile++){
      if (iFile == 0) { 
        baselineRMSHistogramList[iFile][iAnode]->Draw(); 
        baselineRMSHistogramList[iFile][iAnode]->SetMinimum(RMS_count_min);
        baselineRMSHistogramList[iFile][iAnode]->SetMaximum(RMS_count_max);
      }
      else { baselineRMSHistogramList[iFile][iAnode]->Draw("same"); }
    }

    amplitudeLegend->Draw("same");
    baselineRMSCanvas->Update();
    baselineRMSCanvas->Print(Form("baselineRMSCanvas_Anode%i.pdf", iAnode+1));
  

    // baseline 
    TCanvas *baselineMeanCanvas = new TCanvas(Form("BaselineMeanCanvas_Anode%i", iAnode+1 ), Form("BaselineMeanCanvas_Anode%i", iAnode+1 ), 800, 700);
    baselineMeanCanvas->SetLeftMargin(0.12);
    baselineMeanCanvas->SetRightMargin(0.12);
    baselineMeanCanvas->SetBottomMargin(0.125);
    baselineMeanCanvas->SetLogy();

    for(int iFile = 0; iFile < inputFiles.size(); iFile++){
      if(iFile+1 < 10) { baselineMeanHistogramList[iFile][iAnode]->SetLineColor(iFile+1); }
      else { 
        baselineMeanHistogramList[iFile][iAnode]->SetLineColor(iFile-8);
        baselineMeanHistogramList[iFile][iAnode]->SetLineStyle(2); 
      }
      if (iFile == 0) { baselineMeanHistogramList[iFile][iAnode]->Draw(); }
      else { baselineMeanHistogramList[iFile][iAnode]->Draw("same"); }
    }

    amplitudeLegend->Draw("same");
    baselineMeanCanvas->Update();
    baselineMeanCanvas->Print(Form("baselineMeanCanvas_Anode%i_log.pdf", iAnode+1));

    baselineMeanCanvas->SetLogy(0);
    gStyle->SetOptStat(0);

    for(int iFile = 0; iFile < inputFiles.size(); iFile++){
      if (iFile == 0) { 
        baselineMeanHistogramList[iFile][iAnode]->Draw(); 
        baselineMeanHistogramList[iFile][iAnode]->SetMinimum(baseline_count_min);
        baselineMeanHistogramList[iFile][iAnode]->SetMaximum(baseline_count_max);
      }
      else { baselineMeanHistogramList[iFile][iAnode]->Draw("same"); }
    }

    amplitudeLegend->Draw("same");
    baselineMeanCanvas->Update();
    baselineMeanCanvas->Print(Form("baselineMeanCanvas_Anode%i.pdf", iAnode+1));
  }

  // done
  return;
}
