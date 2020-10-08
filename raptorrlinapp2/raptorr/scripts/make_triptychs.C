#include "TFile.h"
#include "TString.h"
#include "TRandom3.h"
#include "TTree.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TAxis.h"
#include <sstream>
#include "vector"
#include "iostream"

int main(int argc, char** argv){

  TString region = "1";
  if(argc==2){
   std::cout << "Argc is " << argc << std::endl;
   region = argv[1];
   std::cout << "Region is now set to " << region << std::endl;
  }

  //Get the inputs files, one for each anode
  //TFile *input_file_anode1 = new TFile("output_test_new_print_region_"+region+"_1266001_anode_1.root", "READ");
  //TFile *input_file_anode2 = new TFile("output_test_new_print_region_"+region+"_1266001_anode_2.root", "READ");
  //  TFile *input_file_anode3 = new TFile("output_test_new_print_region_"+region+"_1266001_anode_3.root", "READ");
  TString runNumber = "R1284015";
  TString fileSuffix = "_preampCalibration_Rough_WF";
  std::stringstream fileSS;
  TString fileToUse;
  //fileSS << "raptorr_tpcdata_gain_R1354003_WaveformTest63to70" << fileSuffix << ".root";
  //fileSS << "/scratch0/hrichie/hptpc_ritchie/HPTPC/data/2020/03/raptorr_waveform-Analysis/GasAgeingWaveforms/raptorr_tpcdata_gain_" << runNumber << "_Am241_Waveforms25_AllCuts_PeakOnly.root";
  //fileSS << "/scratch0/hrichie/hptpc_ritchie/HPTPC/data/2020/03/raptorr_waveform-Analysis/R2080166-R2080186/raptorr_tpcdata_gain_" << runNumber << fileSuffix << ".root";
  fileSS << "/scratch0/hrichie/hptpc_ritchie/HPTPC/data/preampData/biasInjection/raptorr_tpcdata_gain_" << runNumber\
	 << fileSuffix << ".root";
  fileSS >> fileToUse;
  std::cout << fileToUse << std::endl;

  //TFile *input_file_anode1 = new TFile("raptorr_tpcdata_gain_R1354003_WaveformTest63to70_OHBABYBABY.root", "READ");
  TFile *input_file_anode1 = new TFile(fileToUse, "READ");   
  TFile *input_file_anode2 = new TFile(fileToUse, "READ");
  TFile *input_file_anode3 = new TFile(fileToUse, "READ");
  //  TFile *input_file_anode3 = new TFile("raptorr_tpcdata_gain_R1352095_WaveformTest8.root", "READ");

  /* TFile *input_file_anode1 = new TFile("raptorr_tpcdata_gain_R1354004_WaveformTest63to70_BL3030.root", "READ");
  TFile *input_file_anode2 = new TFile("raptorr_tpcdata_gain_R1354004_WaveformTest63to70_BL3030.root", "READ");
  TFile *input_file_anode3 = new TFile("raptorr_tpcdata_gain_R1354004_WaveformTest63to70_BL3030.root", "READ");*/

  std::vector<Int_t> *saved_waveform_IDs = 0;
  std::vector<Int_t> *saved_events = 0;

  TGraph *waveform_anode1 = 0;
  TGraph *waveform_anode2 = 0;
  TGraph *waveform_anode3 = 0;
  TMultiGraph *anode_1 = new TMultiGraph();
  TMultiGraph *anode_2 = new TMultiGraph();
  TMultiGraph *anode_3 = new TMultiGraph();

  TCanvas *c1 = new TCanvas("c1", "c1", 7012, 2200);
  c1->Divide(3,1);
  TString pdf_name = "Tryptic_region2_"+region+".pdf";

  TCanvas *c2 = new TCanvas("c2", "c2", 7012, 2200);
  c2->Divide(3,1);
  TString pdf_name_2 = "Tryptic"+fileSuffix+"_"+runNumber+"_one_waveform_per_page2.pdf";
  c2->Print(pdf_name_2+"[");
  c2->cd();

  TTree *gain_analysis = 0;
  
  input_file_anode2->GetObject("chargeGainAnalysisTree", gain_analysis);
  //the main "analysis"
  
  if(gain_analysis){
    std::cout << "gain analysis is" << std::endl;
    gain_analysis->SetBranchStatus("*", 0);
    gain_analysis->SetBranchStatus("PrintWaveformIDAnode2", 1);
    gain_analysis->SetBranchStatus("PrintEventAnode2", 1);
    gain_analysis->SetBranchAddress("PrintWaveformIDAnode2", &saved_waveform_IDs);
    gain_analysis->SetBranchAddress("PrintEventAnode2", &saved_events);

    	gain_analysis->GetEntry(0);

	//if(saved_waveform_IDs){
	 std::cout << "Number of saved waveforms is " << saved_waveform_IDs->size() << std::endl;
	std::cout << "Now going to randomly pick values until we have 20 waveforms " << std::endl;
	std::cout << "I make it this far" <<std::endl;
	  TRandom3 *rand = new TRandom3();
	  rand->SetSeed(0);
	  rand->GetSeed();

	  int printed_waveforms = 0;
	  std::cout << "I make it This far" <<std::endl;
	  while(printed_waveforms < 1){

		//Get a random number
	    	double random_val = rand->Uniform(0,1);
		std::cout << "I make it THIS far" <<std::endl;
		//Convert this random number to an entry in the array
		int random_entry = (random_val*saved_waveform_IDs->size()) - 1;

		std::cout << "Random value is " << random_val << std::endl;
		std::cout << "Random entry is " << random_entry << std::endl;

		int waveform_ID = (*saved_waveform_IDs)[random_entry];
		int event = (*saved_events)[random_entry];
		std::cout << "Waveform ID is " << waveform_ID << std::endl;
		std::cout << " and event " << event << std::endl;

		/*
		TString graph_name_anode1 = Form("Anode1Waveform%d_Event%d", waveform_ID, event);
		input_file_anode1->GetObject(graph_name_anode1, waveform_anode1);
		TString graph_name_anode2 = Form("Anode2Waveform%d_Event%d", waveform_ID, event);
		input_file_anode2->GetObject(graph_name_anode2, waveform_anode2);
		TString graph_name_anode3 = Form("Anode3Waveform%d_Event%d", waveform_ID, event);
		input_file_anode3->GetObject(graph_name_anode3, waveform_anode3);
		*/

		TString graph_name_anode1 = "Anode1Waveform24_Event4";
                input_file_anode1->GetObject(graph_name_anode1, waveform_anode1);
                TString graph_name_anode2 = "Anode2Waveform24_Event4";
                input_file_anode2->GetObject(graph_name_anode2, waveform_anode2);
		TString graph_name_anode3 = "Anode3Waveform24_Event4";
                input_file_anode3->GetObject(graph_name_anode3, waveform_anode3);
		



		TGraph *copy_anode_1 = 0;
		TGraph *copy_anode_2 = 0;
		TGraph *copy_anode_3 = 0;
	
		if(waveform_anode1 && waveform_anode2 && waveform_anode3){
		  std::cout << "soo another if statement" << std::endl;

		  copy_anode_1 = (TGraph*)waveform_anode1->Clone();
		  copy_anode_3 = (TGraph*)waveform_anode3->Clone();
		  copy_anode_2 = (TGraph*)waveform_anode2->Clone();

		  //Draw the waveforms
		  c2->cd(1);
		  gPad->Clear();
		  copy_anode_1->GetXaxis()->SetTitle("Time (us)");
		  copy_anode_1->GetYaxis()->SetTitle("Amplitude (mV)");
		  copy_anode_1->SetTitle("Anode 1");
		  copy_anode_1->Draw();

		  c2->cd(2);
		  gPad->Clear();
		  copy_anode_2->GetXaxis()->SetTitle("Time (us)");
		  copy_anode_2->GetYaxis()->SetTitle("Amplitude (mV)");
		  copy_anode_2->SetTitle("Anode 2");
		  copy_anode_2->Draw();

		  c2->cd(3);
		  gPad->Clear();
		  copy_anode_3->GetXaxis()->SetTitle("Time (us)");
		  copy_anode_3->GetYaxis()->SetTitle("Amplitude (mV)");
		  copy_anode_3->SetTitle("Anode 3");
		  copy_anode_3->Draw();

		  // anode_1->Add(waveform_anode1);
		  // anode_2->Add(waveform_anode3);
		  // anode_3->Add(waveform_anode2);

		  c2->Print(pdf_name_2);

		  printed_waveforms++;
		}
	  }
	  std::cout << "we made it out of the while loop" << std::endl;
	  c2->Print(pdf_name_2+"]");
	  /*	  std::cout << "we made a pdf" << std::endl;
	  c1->cd(1);
	  std::cout << "time to write some titles here i go hope i dont seg fault... OH SHIT" << std::endl;
	  //anode_1->GetXaxis()->SetTitle("Time (ms)");
	  //anode_1->GetYaxis()->SetTitle("Amplitude (mV)");
	  anode_1->SetTitle("Anode 1 Sample waveforms");
	  std::cout << "wrote titles lol??" << std::endl;
      anode_1->Draw("A");
	  std::cout << "Drawn multigraph anode 1" << std::endl;

	  c1->cd(2);
	  //	  anode_2->GetXaxis()->SetTitle("Time (ms)");
	  //anode_2->GetYaxis()->SetTitle("Amplitude (mV)");
	  anode_2->SetTitle("Anode 2 Sample waveforms");
	  anode_2->Draw("A");
	  std::cout << "Drawn multigraph anode 2" << std::endl;

	  c1->cd(3);
	  // anode_3->GetXaxis()->SetTitle("Time (ms)");
	  //anode_3->GetYaxis()->SetTitle("Amplitude (mV)");
	  anode_3->SetTitle("Anode 3 Sample waveforms");
	  anode_3->Draw("A");
	  std::cout << "Drawn multigraph anode 3" << std::endl;
	
	  c1->Print(pdf_name);
	  std::cout << "we made it to the end" << std::endl;	
	  */
  }
  
return 0;

}
