#include <TFile.h>
#include <TString.h>
#include <TSQLServer.h>
#include <TSQLRow.h>
#include <TSQLResult.h>
#include <iostream>
#include <TGraph.h>
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>
#include <TTimeStamp.h>
#include <TMultiGraph.h>
#include <TCanvas.h>
#include "TROOT.h"
#include "TLegend.h"

TTimeStamp convert_to_timestamp(TString time_string){

  TString time_to_convert = time_string;
  char* time =  (char*)time_to_convert.Data();

  //Get various time parameters from the char that the query spits out  
  UInt_t year = atoi(strtok(time, "-"));
  std::cout << "year is " << year << std::endl;
  UInt_t month = atoi(strtok(NULL, "-"));
  std::cout << "month is " << month << std::endl;
  UInt_t day = atoi(strtok(NULL, " "));
  std::cout << "day is " << day << std::endl;
  UInt_t hour = atoi(strtok(NULL, ":"));
  UInt_t min = atoi(strtok(NULL, ":"));
  UInt_t sec = atoi(strtok(NULL, ""));

  TTimeStamp time_stamp = TTimeStamp(year, month, day, hour, min, sec, 0, "kFALSE", 0);

  return time_stamp;
} 

/*TTimeStamp convert_to_timestamp(TString time){

  char* start_char =  (char*)time.Data();

  std::cout << start_char << std::endl; 
  // Get various time parameters from the char that the query spits out  
  UInt_t start_year = atoi(strtok(start_char, "-"));
  UInt_t start_month = atoi(strtok(NULL, "-"));
  UInt_t start_day = atoi(strtok(NULL, " "));
  UInt_t start_hour = atoi(strtok(NULL, ":"));
  UInt_t start_min = atoi(strtok(NULL, ":"));
  UInt_t start_sec = atoi(strtok(NULL, ""));

  TTimeStamp timestamp = TTimeStamp(start_year, start_month, start_day, start_hour, start_min, start_sec, 0, "kFALSE", 0);

  return timestamp;

}*/ 

void get_cathode_info(TSQLServer *db, TString search_start_time, TString search_end_time, TGraph* Cathode_voltage, TGraph *Cathode_current, TGraph *r_vs_t){
  /////////////////////////////////////////////////////////////////////////////
  // Get Cathode information for time period stated 			     // 
  //									     //
  /////////////////////////////////////////////////////////////////////////////
  TString cathode_select = "SELECT caen_brd0_ch3_hv.value, caen_brd0_ch3_hv.timestamp, caen_brd0_ch3_i.value FROM caen_brd0_ch3_hv";
  TString cathode_join = " JOIN caen_brd0_ch3_i ON caen_brd0_ch3_hv.timestamp=caen_brd0_ch3_i.timestamp"; 
  TString cathode_hist_where = " WHERE caen_brd0_ch3_hv.timestamp>='"+ search_start_time + "' AND caen_brd0_ch3_hv.timestamp<='"+ search_end_time + "'";
  TString cathode_spark_query_hist = cathode_select + cathode_join + cathode_hist_where;
  TSQLResult *cathode_sparking_for_hist = db->Query(cathode_spark_query_hist);

  std::cout << "Done cathode query" << std::endl;  
  std::cout << "Query is " << cathode_spark_query_hist << std::endl;

  TString search_start_time_holder = search_start_time; 
  // Covert start_time to Timestamp to do resistance vs time plot

  TTimeStamp start_timestamp = convert_to_timestamp(search_start_time); 
  std::cout << "Found " << cathode_sparking_for_hist->GetRowCount() << " results " << std::endl;

  // Fill histogram of resistance to get sigma of resistance
  for (int i_row = 0; i_row < cathode_sparking_for_hist->GetRowCount(); i_row++)
  {
	TSQLRow *row_cathode_hist = cathode_sparking_for_hist->Next();

	double voltage = atof(row_cathode_hist->GetField(0));
	char* time = (char*)row_cathode_hist->GetField(1);
	double current = atof(row_cathode_hist->GetField(2));//*10**(-6);
	double resistance = voltage/current;

	delete row_cathode_hist;

	// Get various time parameters from the char that the query spits out  
	UInt_t year = atoi(strtok(time, "-"));
	UInt_t month = atoi(strtok(NULL, "-"));
	UInt_t day = atoi(strtok(NULL, " "));
	UInt_t hour = atoi(strtok(NULL, ":"));
	UInt_t min = atoi(strtok(NULL, ":"));
	UInt_t sec = atoi(strtok(NULL, ":"));

	//initialise timestamp object with info above
	TTimeStamp timestamp = TTimeStamp(year, month, day, hour, min, sec, 0, kFALSE, 0);

	double rel_t = (timestamp.GetSec() - start_timestamp.GetSec())/(60.0*60.0);

	r_vs_t->SetPoint(i_row, rel_t, resistance);
	Cathode_voltage->SetPoint(i_row, rel_t, voltage);
	Cathode_current->SetPoint(i_row, rel_t, current);
  }

  delete cathode_sparking_for_hist;

  return;
}

int main(int argc, char** argv){

//'  gROOT->ProcessLine(".L ~/rootlogon.C");
//  gROOT->ProcessLine("rootlogon()");

  TString test_start = argv[1];
  TString test_end = argv[1];
  TString plot_title = argv[3];
  TString outfile_name = argv[4];

  if(argc != 5){std::cout << "You didn't give me the correct number of input arguments, you gave me " << argc << std::endl; exit(1);}
  // Runs information, probably will get time
  TString search_start_time = argv[1];//"2018-08-15T00:00:00";
  TString search_end_time = argv[2];//"2018-09-18T19:00:00";

  TString dodgey_time = "2018-09-08 19:00:00";

  TTimeStamp Bad_power_time = convert_to_timestamp(dodgey_time); 

  //Open SQL server
  TSQLServer *db = TSQLServer::Connect("mysql://dmdbserv.pp.rhul.ac.uk/HPTPC_TEST", "dmatter", "seedark"); //mysql://hpslow.cern.ch/HPTPC_TEST

  TGraph *Cathode_voltage = new TGraph();
  TGraph *Cathode_current = new TGraph();
  TGraph *r_vs_t = new TGraph();

  get_cathode_info(db, search_start_time, search_end_time, Cathode_voltage, Cathode_current, r_vs_t);

  TTimeStamp start_timestamp = convert_to_timestamp(search_start_time); 

  /////////////////////////////////////////////////////////////////////////////
  ///////////// Do a query getting anode current information //////////////////
  /////////////////////////////////////////////////////////////////////////////

  TGraph *anode0_current = new TGraph();
  TGraph *anode1_current = new TGraph();
  TGraph *anode2_current = new TGraph();

  TString anode0_current_query_for_hist = "SELECT value, timestamp FROM caen_brd0_ch0_i WHERE caen_brd0_ch0_i.timestamp>='" + search_start_time + "' AND caen_brd0_ch0_i.timestamp<='" + search_end_time + "'";
  TString anode1_current_query_for_hist = "SELECT value, timestamp FROM caen_brd0_ch1_i WHERE caen_brd0_ch1_i.timestamp>='" + search_start_time + "' AND caen_brd0_ch1_i.timestamp<='" + search_end_time + "'";
  TString anode2_current_query_for_hist = "SELECT value, timestamp FROM caen_brd0_ch2_i WHERE caen_brd0_ch2_i.timestamp>='" + search_start_time + "' AND caen_brd0_ch2_i.timestamp<='" + search_end_time + "'";
  TSQLResult *anode0_current_for_hist = db->Query(anode0_current_query_for_hist);
  TSQLResult *anode1_current_for_hist = db->Query(anode1_current_query_for_hist);
  TSQLResult *anode2_current_for_hist = db->Query(anode2_current_query_for_hist);

  std::cout << "Got anode current query" << std::endl;
  std::cout << "Anode 0 query " << anode0_current_query_for_hist << std::endl;
  std::cout << "Anode 1 query " << anode1_current_for_hist->GetRowCount() << std::endl;
  std::cout << "Anode 2 query " << anode2_current_for_hist->GetRowCount() << std::endl;

  int anode0_queries = anode0_current_for_hist->GetRowCount();
  std::cout << "Current of anode 0 has " << anode0_current_for_hist->GetRowCount() << std::endl;
  for(int current_row0 = 0 ; current_row0 < anode0_current_for_hist->GetRowCount() ; current_row0++)
  {
	TSQLRow* row_anode0_current = anode0_current_for_hist->Next();
	double current = atof(row_anode0_current->GetField(0));
	char* time = (char*)row_anode0_current->GetField(1);

	UInt_t year = atoi(strtok(time, "-"));
	UInt_t month = atoi(strtok(NULL, "-"));
	UInt_t day = atoi(strtok(NULL, " "));
	UInt_t hour = atoi(strtok(NULL, ":"));
	UInt_t min = atoi(strtok(NULL, ":"));
	UInt_t sec = atoi(strtok(NULL, ":"));

	//initialise timestamp object with info above
	TTimeStamp timestamp = TTimeStamp(year, month, day, hour, min, sec, 0, kFALSE, 0);

	double rel_t = (timestamp.GetSec() - start_timestamp.GetSec())/(60.0*60.0);

	anode0_current->SetPoint(current_row0, timestamp, current);
	delete row_anode0_current;
  }

  int anode1_queries = anode1_current_for_hist->GetRowCount();
  std::cout << "Current of anode 1 has " << anode1_current_for_hist->GetRowCount() << std::endl;
  for(int current_row1 = 0 ; current_row1 < anode1_current_for_hist->GetRowCount() ; current_row1++)
  {
	TSQLRow* row_anode1_current = anode1_current_for_hist->Next();
	double current = atof(row_anode1_current->GetField(0));
	char* time = (char*)row_anode1_current->GetField(1);

	UInt_t year = atoi(strtok(time, "-"));
	UInt_t month = atoi(strtok(NULL, "-"));
	UInt_t day = atoi(strtok(NULL, " "));
	UInt_t hour = atoi(strtok(NULL, ":"));
	UInt_t min = atoi(strtok(NULL, ":"));
	UInt_t sec = atoi(strtok(NULL, ":"));

	//initialise timestamp object with info above
	TTimeStamp timestamp = TTimeStamp(year, month, day, hour, min, sec, 0, kFALSE, 0);

	double rel_t = (timestamp.GetSec() - start_timestamp.GetSec())/(60.0*60.0);

	anode1_current->SetPoint(current_row1, timestamp, current);

	delete row_anode1_current;
  }

  int anode2_queries = anode2_current_for_hist->GetRowCount();
  std::cout << "Current of anode 2 has " << anode2_current_for_hist->GetRowCount() << std::endl;
  for(int current_row2 = 0 ; current_row2 < anode2_current_for_hist->GetRowCount() ; current_row2++)
  {
	TSQLRow* row_anode2_current = anode2_current_for_hist->Next();
	double current = atof(row_anode2_current->GetField(0));

	char* time = (char*)row_anode2_current->GetField(1);

	UInt_t year = atoi(strtok(time, "-"));
	UInt_t month = atoi(strtok(NULL, "-"));
	UInt_t day = atoi(strtok(NULL, " "));
	UInt_t hour = atoi(strtok(NULL, ":"));
	UInt_t min = atoi(strtok(NULL, ":"));
	UInt_t sec = atoi(strtok(NULL, ":"));

	//initialise timestamp object with info above
	TTimeStamp timestamp = TTimeStamp(year, month, day, hour, min, sec, 0, kFALSE, 0);

	double rel_t = (timestamp.GetSec() - start_timestamp.GetSec())/(60.0*60.0);

	anode2_current->SetPoint(current_row2, timestamp, current);

	delete row_anode2_current;
  }

  //delete hanode0_current;
  delete anode0_current_for_hist;

  std::cout << "Got anode current information" << std::endl;
  TString start_time_tstring = search_start_time;
/*
  //Convert start_time TString to time stamp to compare times later
  char* start_time_char =  (char*)start_time_tstring.Data();

  // Get various time parameters from the char that the query spits out  
  UInt_t start_time_year = atoi(strtok(start_time_char, "-"));
  UInt_t start_time_month = atoi(strtok(NULL, "-"));
  UInt_t start_time_day = atoi(strtok(NULL, "T"));
  UInt_t start_time_hour = atoi(strtok(NULL, ":"));
  UInt_t start_time_min = atoi(strtok(NULL, ":"));
  UInt_t start_time_sec = atoi(strtok(NULL, ""));
*/
  TTimeStamp start_time_timestamp = convert_to_timestamp(start_time_tstring);

  TString anode0_voltage_query;
  TString anode1_voltage_query;
  TString anode2_voltage_query;

  TGraph *anode0_voltage = new TGraph();
  TGraph *anode1_voltage = new TGraph();
  TGraph *anode2_voltage = new TGraph();

  if(start_time_timestamp.GetSec() < Bad_power_time.GetSec()){ 
	//Do some anode voltage check by getting mean for each run
	anode0_voltage_query = "SELECT value, timestamp FROM caen_brd1_ch0_set_hv WHERE caen_brd1_ch0_set_hv.timestamp>='" + search_start_time + "' AND caen_brd1_ch0_set_hv.timestamp<='" + search_end_time + "'";// AND value>1"; 
	anode1_voltage_query = "SELECT value, timestamp FROM caen_brd1_ch1_set_hv WHERE caen_brd1_ch1_set_hv.timestamp>='" + search_start_time + "' AND caen_brd1_ch1_set_hv.timestamp<='" + search_end_time + "'";//AND value>1";
	anode2_voltage_query = "SELECT value, timestamp FROM caen_brd1_ch2_set_hv WHERE caen_brd1_ch2_set_hv.timestamp>='" + search_start_time + "' AND caen_brd1_ch2_set_hv.timestamp<='" + search_end_time + "'";// AND value>1";// AND value<=0";
  }
  else{
	anode0_voltage_query = "SELECT value, timestamp FROM caen_brd0_ch0_set_hv WHERE caen_brd0_ch0_set_hv.timestamp>='" + search_start_time + "' AND caen_brd0_ch0_set_hv.timestamp<='" + search_end_time + "'";// AND value>1"; 
	anode1_voltage_query = "SELECT value, timestamp FROM caen_brd0_ch1_set_hv WHERE caen_brd0_ch1_set_hv.timestamp>='" + search_start_time + "' AND caen_brd0_ch1_set_hv.timestamp<='" + search_end_time + "'";//AND value>1";
	anode2_voltage_query = "SELECT value, timestamp FROM caen_brd0_ch2_set_hv WHERE caen_brd0_ch2_set_hv.timestamp>='" + search_start_time + "' AND caen_brd0_ch2_set_hv.timestamp<='" + search_end_time + "'";// AND value>1";// AND value<=0";
  }                                                                                                                                                               

  TSQLResult *anode0_voltage_result = db->Query(anode0_voltage_query);
  TSQLResult *anode1_voltage_result = db->Query(anode1_voltage_query);
  TSQLResult *anode2_voltage_result = db->Query(anode2_voltage_query);

  std::cout << "anode0_voltage_query is " << anode0_voltage_query << std::endl;

  for(int anode0_row_i = 0 ; anode0_row_i < anode0_voltage_result->GetRowCount() ; anode0_row_i++)
  {
	TSQLRow *v_row_anode0 = anode0_voltage_result->Next();
	double voltage_anode0 = atof(v_row_anode0->GetField(0));
	char* time = (char*)v_row_anode0->GetField(1);

	UInt_t year = atoi(strtok(time, "-"));
	UInt_t month = atoi(strtok(NULL, "-"));
	UInt_t day = atoi(strtok(NULL, " "));
	UInt_t hour = atoi(strtok(NULL, ":"));
	UInt_t min = atoi(strtok(NULL, ":"));
	UInt_t sec = atoi(strtok(NULL, ":"));

	//initialise timestamp object with info above
	TTimeStamp timestamp = TTimeStamp(year, month, day, hour, min, sec, 0, kFALSE, 0);

	double rel_t = (timestamp.GetSec() - start_timestamp.GetSec())/(60.0*60.0);

	//	std::cout << "Anode 0: Voltage is " << voltage_anode0 << " and time is " << timestamp << std::endl;

	anode0_voltage->SetPoint(anode0_row_i, timestamp, voltage_anode0);
	delete v_row_anode0;
  } 	  

  std::cout << "number of rows in anode0_voltage query is " << anode0_voltage_result->GetRowCount() << std::endl;


  for(int anode1_row_i = 0 ; anode1_row_i < anode1_voltage_result->GetRowCount() ; anode1_row_i++)
  {
	TSQLRow* v_row_anode1 = anode1_voltage_result->Next();
	double voltage_anode1 = atof(v_row_anode1->GetField(0));
	char* time = (char*)v_row_anode1->GetField(1);

	UInt_t year = atoi(strtok(time, "-"));
	UInt_t month = atoi(strtok(NULL, "-"));
	UInt_t day = atoi(strtok(NULL, " "));
	UInt_t hour = atoi(strtok(NULL, ":"));
	UInt_t min = atoi(strtok(NULL, ":"));
	UInt_t sec = atoi(strtok(NULL, ":"));

	//initialise timestamp object with info above
	TTimeStamp timestamp = TTimeStamp(year, month, day, hour, min, sec, 0, kFALSE, 0);

	double rel_t = (timestamp.GetSec() - start_timestamp.GetSec())/(60.0*60.0);
	//	std::cout << "Anode 1: Voltage is " << voltage_anode1 << " and time is " << timestamp << std::endl;

	anode1_voltage->SetPoint(anode1_row_i, timestamp, voltage_anode1);
	delete v_row_anode1;
  } 

  std::cout << "number of rows in anode1_voltage query is " << anode1_voltage_result->GetRowCount() << std::endl;

  for(int anode2_row_i = 0 ; anode2_row_i < anode2_voltage_result->GetRowCount() ; anode2_row_i++)
  {
	TSQLRow* v_row_anode2 = anode2_voltage_result->Next();
	double voltage_anode2 = atof(v_row_anode2->GetField(0));
	char* time = (char*)v_row_anode2->GetField(1);

	UInt_t year = atoi(strtok(time, "-"));
	UInt_t month = atoi(strtok(NULL, "-"));
	UInt_t day = atoi(strtok(NULL, " "));
	UInt_t hour = atoi(strtok(NULL, ":"));
	UInt_t min = atoi(strtok(NULL, ":"));
	UInt_t sec = atoi(strtok(NULL, ":"));

	//initialise timestamp object with info above
	TTimeStamp timestamp = TTimeStamp(year, month, day, hour, min, sec, 0, kFALSE, 0);

	double rel_t = (timestamp.GetSec() - start_timestamp.GetSec())/(60.0*60.0);

//	std::cout << "Anode2: Voltage is " << voltage_anode2 << " and time is " << timestamp << std::endl;
	anode2_voltage->SetPoint(anode2_row_i, timestamp, voltage_anode2);//rel_t, voltage_anode2);
	delete v_row_anode2;

  } 
  std::cout << "number of rows in anode2_voltage query is " << anode2_voltage_result->GetRowCount() << std::endl;

  /////////////////////////////////////////////////////////////////////////////
  ////////////////////// PRESSURE QUERY TEST //////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////

  //Pressure change query
  TString pressure_stability_query = "SELECT value, timestamp FROM chamber_pressure_cdg WHERE timestamp>='"+ search_start_time+"' AND timestamp<='"+ search_end_time+"'";
  //turbo_pressure_cdg

  TSQLResult *pressure_stability = db->Query(pressure_stability_query);
  TGraph *Pressure_plot = new TGraph();

  for(int i_row = 0; i_row < pressure_stability->GetRowCount(); i_row++)
  {
	TSQLRow *row_pressure = pressure_stability->Next();
	double pressure = atof(row_pressure->GetField(0));
	//std::cout << "Pressure is " << pressure << std::endl;

	char *time = (char*)row_pressure->GetField(1);

	// Get various time parameters from the char that the query spits out  
	UInt_t year = atoi(strtok(time, "-"));
	UInt_t month = atoi(strtok(NULL, "-"));
	UInt_t day = atoi(strtok(NULL, " "));
	UInt_t hour = atoi(strtok(NULL, ":"));
	UInt_t min = atoi(strtok(NULL, ":"));
	UInt_t sec = atoi(strtok(NULL, ":"));

	//initialise timestamp object with info above
	TTimeStamp timestamp = TTimeStamp(year, month, day, hour, min, sec, 0, kFALSE, 0);

	//double rel_t = (timestamp.GetSec() - start_timestamp.GetSec())/(60.0*60.0*24);
	//std::cout << "Setting pressure point with " << pressure << std::endl;


//	std::cout << "Filling and " << i_row << std::endl;
	Pressure_plot->SetPoint(i_row, timestamp, pressure);


	delete row_pressure;
  }

  //p_vs_t->SetPoint(j, rel_t, pressure_mean);

  //delete row_pressure;
  //p_vs_t->GetXaxis()->SetTitle("Time since 2nd of August (s)");
  //p_vs_t->GetYaxis()->SetTitle("Pressure (BarG)");
  //p_vs_t->Draw("A*");

  //Some fitting and drawing of plots done here. Not too important other than to makes figures for presentations 
  //r_vs_t->GetXaxis()->SetTitle("Time since 2nd of August (s)");
  //r_vs_t->GetYaxis()->SetRangeUser(35,50);
  //r_vs_t->GetYaxis()->SetTitle("Resistance (Mega Ohms)");
  //r_vs_t->SetTitle("Cathode Resistance for the Beam Test");
  //  r_vs_t->Draw("A*");
  // v_vs_t->SetMarkerColor(4);
  // hresistance->Fit("f1", "CR");

  //  C_cathode->Update();//Force stats box to be drawn
  //hresistance->GetXaxis()->SetRangeUser(41,44);
  // hresistance->Fit("f2", "CR+");
  // TPaveStats *p2 = (TPaveStats*)hresistance->FindObject("stats");
  //  C_cathode->Modified();
  //delete hresistance, f2;
  //  C_cathode->Print("Resistance_for_beam_test.png");
  //v_vs_t->Draw("*SAMES");
  //std::cout << "Number of rows " << anode1_sparking->GetRowCount() << std::endl;

  //TString file_name = Form("cathode_voltage_vs_current_%s_to_%s.root",date1.Data(), date2.Data());
  //TFile *file = new TFile(file_name,"RECREATE");
  //g_v_vs_i->Write();
  //file->Close();

  //delete row_run_times;

  // delete cathode_sparking;

  //  delete anode0_voltage;
  //  delete anode1_voltage;
  //  delete anode2_voltage;

  //delete cathode_sparking; 

  //"+search_start_time+"_to_"+search_end_time+
  TString outname = "Slow_control_plot_for_.root";
  TFile *output_file = new TFile(outfile_name+".root", "RECREATE");
  output_file->cd(); 

  TCanvas *c2 = new TCanvas("c2", "c2", 800,600);
  c2->cd();

  r_vs_t->GetXaxis()->SetTitle("Time (hours)");
  r_vs_t->GetYaxis()->SetTitle("Resistance (MegaOhms)");
  r_vs_t->SetTitle("Resistance vs time during beam test");
  r_vs_t->SetName("resistance_vs_t");
  r_vs_t->Write();

  Cathode_voltage->GetXaxis()->SetTitle("Time (hours)");
  Cathode_voltage->GetYaxis()->SetTitle("Voltage (V)");
  Cathode_voltage->SetTitle("Cathode voltage vs time");
  Cathode_voltage->SetName("Cathode_voltage");
  Cathode_voltage->Write();

  Cathode_current->GetXaxis()->SetTitle("Time (hours)");
  Cathode_current->GetYaxis()->SetTitle("Current (MicroAmps)");
  Cathode_current->SetTitle("Cathode current vs time");
  Cathode_current->SetName("Cathode_current");
  Cathode_current->Write();

  anode0_current->GetXaxis()->SetTitle("Time (hours)");
  anode0_current->GetYaxis()->SetTitle("Current (MicroAmps)");
  anode0_current->GetXaxis()->SetTimeFormat("%H:%M");
  anode0_current->SetTitle("Anode 0 "+plot_title);
  anode0_current->SetName("anode0_current");
  anode0_current->Write();

  anode1_current->GetXaxis()->SetTitle("Time (hours)");
  anode1_current->GetYaxis()->SetTitle("Current (MicroAmps)");
  anode1_current->GetXaxis()->SetTimeFormat("%H:%M");
  anode1_current->SetTitle("Anode 1 "+plot_title);
  anode1_current->SetName("anode1_current");
  anode1_current->Write();

  anode2_current->GetXaxis()->SetTitle("time (hours)");
  anode2_current->GetYaxis()->SetTitle("current (microamps)");
  anode2_current->GetXaxis()->SetTimeFormat("%H:%M");
  anode2_current->SetTitle("anode 2 "+plot_title);
  anode2_current->SetName("anode2_current");
  anode2_current->Write();

  TCanvas *c1 = new TCanvas("c1", "c1", 300, 300);
  c1->cd();
  anode0_current->SetLineColor(kRed);
  anode1_current->SetLineColor(kBlack);
  anode2_current->SetLineColor(kBlue);
  std::cout << "Now I will try to draw the current graphs" << std::endl;
  TMultiGraph *graph = new TMultiGraph();
  TLegend *legend = new TLegend(0.6, 0.7, 0.9, 0.9);
  if(anode2_queries > 0){anode2_current->Draw("ALP*");graph->Add(anode2_current); legend->AddEntry(anode2_current, "Anode 3", "l");}//anode2_current->Draw("ALP*");} 
  if(anode1_queries > 0){anode1_current->Draw("ALP*");graph->Add(anode1_current); legend->AddEntry(anode1_current, "Anode 2", "l");}//anode1_current->Draw("ALP*");}
  if(anode0_queries > 0){anode0_current->Draw("ALP*");graph->Add(anode0_current); legend->AddEntry(anode0_current, "Anode 1", "l");}//anode0_current->Draw("ALP*");}
  
  graph->Draw();
//  graph->GetYaxis()->SetRangeUser(0,2);
//  graph->Draw();
  legend->Draw();
  c1->Print(outfile_name+".pdf");

  anode0_voltage->GetXaxis()->SetTitle("Time (hours)");
  anode0_voltage->GetYaxis()->SetTitle("Voltage (V)");
  anode0_voltage->GetXaxis()->SetTimeDisplay(1);
  anode0_voltage->GetXaxis()->SetTimeFormat("%H:%M");
  anode0_voltage->SetTitle("anode 0 Voltage vs Time");
  anode0_voltage->SetName("anode0_voltage");
  anode0_voltage->Write();

  anode1_voltage->GetXaxis()->SetTitle("Time (hours)");
  anode1_voltage->GetYaxis()->SetTitle("Voltage (V)");
  anode1_voltage->GetXaxis()->SetTimeDisplay(1);
  anode1_voltage->GetXaxis()->SetTimeFormat("%H:%M");
  anode1_voltage->SetTitle("anode 1 Voltage vs Time");
  anode1_voltage->SetName("anode1_voltage");
  anode1_voltage->Write();

  anode2_voltage->GetXaxis()->SetTitle("Time (hours)");
  anode2_voltage->GetYaxis()->SetTitle("Voltage (V)");
  anode2_voltage->GetXaxis()->SetTimeDisplay(1);
  anode2_voltage->GetXaxis()->SetTimeFormat("%H:%M");
  anode2_voltage->SetTitle("anode 2 Voltage vs Time");
  anode2_voltage->SetName("anode2_voltage");
  anode2_voltage->Write();

  Pressure_plot->GetXaxis()->SetTitle("Date");
  Pressure_plot->GetYaxis()->SetTitle("Pressure (BarG)");
  Pressure_plot->GetXaxis()->SetTimeDisplay(1);
  Pressure_plot->GetXaxis()->SetTimeFormat("%d\/%m");
  Pressure_plot->SetTitle("Pressure vs Time");
  Pressure_plot->SetName("Pressure_plot");
  Pressure_plot->Write();

  //Cathode_current->Draw("AP*");
  //Pressure_plot->Draw("AP*");
  //anode0_voltage->Draw("AP*");
  //anode_cuts->Draw("AP*");
  //hanode0_current->GetXaxis()->SetTitle("Current (microAmps)");
  //hanode0_current->GetYaxis()->SetTitle("Entries");
  //hanode0_current->SetTitle("Current distribution for anode 3");
  //hanode0_current->Draw();
  //hresistance->GetXaxis()->SetTitle("Resistance (MegaOhms)");
  //hresistance->GetYaxis()->SetTitle("Entries");
  //hresistance->SetTitle("Cathode Resistance for beam test");
  //hresistance->Draw();
  //hvi->SetTitle("Current vs voltage");
  //hvi->GetXaxis()->SetTitle("Voltage (V)");
  //hvi->GetYaxis()->SetTitle("Current (microAmps)");
  //hvi->GetZaxis()->SetRangeUser(0,400);
  //hvi->Draw("colz");
  //p_vs_t->SetTitle("TPC Pressure during Beam test");
  //p_vs_t->Draw("AP*");

  //std::cout << "Runs with broken pressure reading" << broken << std::endl;
  //std::cout << "Number of runs where all anode votlage entries have entries" << n_all/number_of_runs << std::endl;
  //c2->Update();
  output_file->Close();
  //c2->Print("Test_output.pdf");

  return 0;
}
