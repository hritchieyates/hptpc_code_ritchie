#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <vector>

#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLegend.h"
#include "TProfile.h"
#include "TROOT.h"
#include "TTree.h"
#include "TVectorD.h"


/* Definitions for data storage */ 

// struct to store and pass on analysis results of an anode analysis
/*
struct AnodeVectorCollection{
  // vectors to store actual analysis results in
  std::vector<Float_t> fBaselineVec;
  std::vector<Float_t> fBaselineRMSVec;
  std::vector<Float_t> fPeakTimeVec;
  std::vector<Float_t> fNegPeakTimeVec;
  std::vector<Float_t> fMaxValueVec;
  std::vector<Float_t> fMinValueVec;
  std::vector<Float_t> fAmplitudeVec;
  std::vector<Float_t> fNegAmplitudeVec;
  std::vector<Float_t> fWaveformMeanVec;
  std::vector<Float_t> fWaveformRMSVec;
  std::vector<Float_t> fRisetimeVec;
  std::vector<Float_t> fNegRisetimeVec;
  std::vector<Float_t> fFalltimeVec;
  std::vector<Float_t> fNegFalltimeVec;
  std::vector<Float_t> fIntegralVec;
  std::vector<Float_t> fNegIntegralVec;
  std::vector<Int_t> fGetSparkVec;
  std::vector<Int_t> fAbsPosAmpLargerThanAbsNegAmp;
  // Parameters calculated (and returned) when a waveform fit is used
  std::vector<Float_t> fFitAmplitudeVec;
  std::vector<Float_t> fFitBaselineEstimateVec;
  std::vector<Float_t> fFitFirstExponentEstimateVec;
  std::vector<Float_t> fFitSecondExponentEstimateVec;
  std::vector<Float_t> fFitOffsetEstimateVec;
  std::vector<Float_t> fFitChi2Vec;
  std::vector<Float_t> fFitNdfVec;
  // Parameters calculated (and returned) when a baseline fit is used
  std::vector<Float_t> fSinNoiseFitAmplitudeVec;
  std::vector<Float_t> fSinNoiseFitXOffsetVec;
  std::vector<Float_t> fSinNoiseFitYOffsetVec;
  std::vector<Float_t> fSinNoiseFitFrequencyVec;
  std::vector<Float_t> fSinNoiseFitChi2Vec;
  std::vector<Float_t> fSinNoiseFitNdfVec;
  // vectors to store the digitser configuration in
  std::vector<TString> fGainVec;
  std::vector<Float_t> fVMinVec;
  std::vector<Float_t> fVMaxVec;
  std::vector<Float_t> fBaselineOffsetVec;
  std::vector<Float_t> fTriggerThresholdVec;
  // debugging: vectors with waveforms
  std::vector<TGraph> fWaveformVec;
  std::vector<std::unique_ptr<TF1> > fSinNoiseFitFunctionVec;
};
*/

/* small helper function */

// helper function to create a vector containing data from a branch
template <typename S>
void moveBranchToVector(TTree* dataTree, std::string branchName, std::string cutString, std::vector<S> *dataVec){
  dataVec->clear();

  // get the branch from the three
  int sampleSize = dataTree->Draw(branchName.c_str(), cutString.c_str(), "goff");
  TVectorD branchData(dataTree->GetSelectedRows(), dataTree->GetV1());
  for(int iFillData = 0; iFillData < sampleSize; iFillData++) { dataVec->push_back(branchData[iFillData]); }

  // done
  return;
}

// helper function to create a vector with TStrings from a vector of Int_t s
std::vector<TString> createStringVec(std::vector<Int_t> dataVec){
  std::vector<TString> dataAsString;

  for(UInt_t iDataSample = 0; iDataSample < dataVec.size(); iDataSample++){
    TString sampleAsString;
    sampleAsString.Form("%i",  dataVec.at(iDataSample));
    dataAsString.push_back(sampleAsString);
  }

  // done
  return dataAsString;
}


/* manipulating trees */

// This functions returns a sub-tree holding only the elements which fulfil the cutString condition
// ToDo: Update the listOfAllBranchNames so it fits the current analysis tree in raptorr (in case there is something missing someone would like)
TTree* ICanNotBeliveThereIsNoRootToolToDoThis(TTree* fullTree, std::string cutString, std::string treeName, std::string treeTitle){
  std::cout << "In case you are processing a large data sample this may take a while. " << std::endl;

  /* create a vector for the data of each branch */
  std::vector<Float_t> fBaselineAnode1Vec, fBaselineAnode2Vec, fBaselineAnode3Vec;
  std::vector<Float_t> fBaselineRMSAnode1Vec, fBaselineRMSAnode2Vec, fBaselineRMSAnode3Vec;
  std::vector<Float_t> fWaveformMeanAnode1Vec, fWaveformMeanAnode2Vec, fWaveformMeanAnode3Vec;
  std::vector<Float_t> fWaveformRMSAnode1Vec, fWaveformRMSAnode2Vec, fWaveformRMSAnode3Vec;
  std::vector<Float_t> fPeakTimeAnode1Vec, fPeakTimeAnode2Vec, fPeakTimeAnode3Vec;
  std::vector<Float_t> fNegPeakTimeAnode1Vec, fNegPeakTimeAnode2Vec, fNegPeakTimeAnode3Vec;
  std::vector<Float_t> fMaxValueAnode1Vec, fMaxValueAnode2Vec, fMaxValueAnode3Vec;
  std::vector<Float_t> fMinValueAnode1Vec, fMinValueAnode2Vec, fMinValueAnode3Vec;
  std::vector<Float_t> fAmplitudeAnode1Vec, fAmplitudeAnode2Vec, fAmplitudeAnode3Vec;
  std::vector<Float_t> fNegAmplitudeAnode1Vec, fNegAmplitudeAnode2Vec, fNegAmplitudeAnode3Vec;
  std::vector<Float_t> fRisetimeAnode1Vec, fRisetimeAnode2Vec, fRisetimeAnode3Vec;
  std::vector<Float_t> f10PercentRisingEdgeAnode1Vec, f90PercentRisingEdgeAnode1Vec;
  std::vector<Float_t> f10PercentRisingEdgeAnode2Vec, f90PercentRisingEdgeAnode2Vec;
  std::vector<Float_t> f10PercentRisingEdgeAnode3Vec, f90PercentRisingEdgeAnode3Vec;
  std::vector<Float_t> fNeg10PercentRisingEdgeAnode1Vec, fNeg90PercentRisingEdgeAnode1Vec;
  std::vector<Float_t> fNeg10PercentRisingEdgeAnode2Vec, fNeg90PercentRisingEdgeAnode2Vec;
  std::vector<Float_t> fNeg10PercentRisingEdgeAnode3Vec, fNeg90PercentRisingEdgeAnode3Vec;
  std::vector<Float_t> f90PercentFallingEdgeAnode1Vec, f10PercentFallingEdgeAnode1Vec;
  std::vector<Float_t> f90PercentFallingEdgeAnode2Vec, f10PercentFallingEdgeAnode2Vec;
  std::vector<Float_t> f90PercentFallingEdgeAnode3Vec, f10PercentFallingEdgeAnode3Vec;
  std::vector<Float_t> fNeg90PercentFallingEdgeAnode1Vec, fNeg10PercentFallingEdgeAnode1Vec;
  std::vector<Float_t> fNeg90PercentFallingEdgeAnode2Vec, fNeg10PercentFallingEdgeAnode2Vec;
  std::vector<Float_t> fNeg90PercentFallingEdgeAnode3Vec, fNeg10PercentFallingEdgeAnode3Vec;
  std::vector<Float_t> fNegRisetimeAnode1Vec, fNegRisetimeAnode2Vec, fNegRisetimeAnode3Vec;
  std::vector<Float_t> fFalltimeAnode1Vec, fFalltimeAnode2Vec, fFalltimeAnode3Vec;
  std::vector<Float_t> fNegFalltimeAnode1Vec, fNegFalltimeAnode2Vec, fNegFalltimeAnode3Vec;
  std::vector<Float_t> fIntegralAnode1Vec, fIntegralAnode2Vec, fIntegralAnode3Vec;
  std::vector<Float_t> fNegIntegralAnode1Vec, fNegIntegralAnode2Vec, fNegIntegralAnode3Vec;
  std::vector<Int_t> fGetSparkAnode1Vec, fGetSparkAnode2Vec, fGetSparkAnode3Vec;
  std::vector<Int_t> fAbsPosAmpLargerThanAbsNegAmpAnode1Vec, fAbsPosAmpLargerThanAbsNegAmpAnode2Vec, fAbsPosAmpLargerThanAbsNegAmpAnode3Vec;
  // Parameters calculated (and returned) when a waveform fit is used
  std::vector<Float_t> fFitAmplitudeAnode1Vec, fFitAmplitudeAnode2Vec, fFitAmplitudeAnode3Vec;
  std::vector<Float_t> fFitBaselineEstimateAnode1Vec, fFitBaselineEstimateAnode2Vec, fFitBaselineEstimateAnode3Vec;
  std::vector<Float_t> fFitFirstExponentEstimateAnode1Vec, fFitFirstExponentEstimateAnode2Vec, fFitFirstExponentEstimateAnode3Vec;
  std::vector<Float_t> fFitSecondExponentEstimateAnode1Vec, fFitSecondExponentEstimateAnode2Vec, fFitSecondExponentEstimateAnode3Vec;
  std::vector<Float_t> fFitOffsetEstimateAnode1Vec, fFitOffsetEstimateAnode2Vec, fFitOffsetEstimateAnode3Vec;
  std::vector<Float_t> fFitChi2Anode1Vec, fFitChi2Anode2Vec, fFitChi2Anode3Vec;
  std::vector<Float_t> fFitNdfAnode1Vec, fFitNdfAnode2Vec, fFitNdfAnode3Vec;
  // timing
  std::vector<Float_t> waveformTriggerT0SecVec;
  std::vector<Float_t> waveformTriggerT0NanoSecVec;
  // these are only set if data is analysed with the Beam_testApp (in some future they should be always filled)
  std::vector<Float_t> beamTriggerTimeSeconds;
  std::vector<Float_t> beamTriggerTimeNanoSeconds;
  // These are not used here, but exist in raptorr
  // baseline + RMS, peak-time, minVal and amplitude of the beam trigger signal
  std::vector<Float_t> baselineBeamTriggerVec, baselineRMSBeamTriggerVec, peakTimeBeamTriggerVec, minValueBeamTriggerVec, amplitudeBeamTriggerVec;
  // vectors to store the digitser configuration in
  std::vector<TString> fAnode1GainVec, fAnode2GainVec, fAnode3GainVec;
  std::vector<Float_t> fAnode1VMinVec, fAnode1VMaxVec, fAnode1BaselineOffsetVec, fAnode1TriggerThresholdVec;
  std::vector<Float_t> fAnode2VMinVec, fAnode2VMaxVec, fAnode2BaselineOffsetVec, fAnode2TriggerThresholdVec;
  std::vector<Float_t> fAnode3VMinVec, fAnode3VMaxVec, fAnode3BaselineOffsetVec, fAnode3TriggerThresholdVec;  
  std::vector<Float_t> postTriggerFractionVec, acquisitionTimeVec;
  // book keeping: event number & run number
  std::vector<Int_t> eventNoVec, runNoVec;
  std::vector<TString> fRunNoStringVec;

  /* create new (return) TTree and assign vectors to branches */
  TTree* treeToReturn = new TTree(treeName.c_str(), treeTitle.c_str());

  // connect vectors and branches
  // analysis results:
  treeToReturn->Branch("BaselineAnode1", &fBaselineAnode1Vec);
  treeToReturn->Branch("BaselineAnode2", &fBaselineAnode2Vec);
  treeToReturn->Branch("BaselineAnode3", &fBaselineAnode3Vec);
  treeToReturn->Branch("BaselineRMSAnode1", &fBaselineRMSAnode1Vec);
  treeToReturn->Branch("BaselineRMSAnode2", &fBaselineRMSAnode2Vec);
  treeToReturn->Branch("BaselineRMSAnode3", &fBaselineRMSAnode3Vec);
  treeToReturn->Branch("WaveformMeanAnode1", &fWaveformMeanAnode1Vec);
  treeToReturn->Branch("WaveformMeanAnode2", &fWaveformMeanAnode2Vec);
  treeToReturn->Branch("WaveformMeanAnode3", &fWaveformMeanAnode3Vec);
  treeToReturn->Branch("WaveformRMSAnode1", &fWaveformRMSAnode1Vec);
  treeToReturn->Branch("WaveformRMSAnode2", &fWaveformRMSAnode2Vec);
  treeToReturn->Branch("WaveformRMSAnode3", &fWaveformRMSAnode3Vec);
  treeToReturn->Branch("PeakTimeAnode1", &fPeakTimeAnode1Vec);
  treeToReturn->Branch("PeakTimeAnode2", &fPeakTimeAnode2Vec);
  treeToReturn->Branch("PeakTimeAnode3", &fPeakTimeAnode3Vec);
  treeToReturn->Branch("MaxValueAnode1", &fMaxValueAnode1Vec);
  treeToReturn->Branch("MaxValueAnode2", &fMaxValueAnode2Vec);
  treeToReturn->Branch("MaxValueAnode3", &fMaxValueAnode3Vec);
  treeToReturn->Branch("AmplitudeAnode1", &fAmplitudeAnode1Vec);
  treeToReturn->Branch("AmplitudeAnode2", &fAmplitudeAnode2Vec);
  treeToReturn->Branch("AmplitudeAnode3", &fAmplitudeAnode3Vec);
  treeToReturn->Branch("RisetimeAnode1", &fRisetimeAnode1Vec);
  treeToReturn->Branch("TenPercentRisingEdgeAnode1", &f10PercentRisingEdgeAnode1Vec);
  treeToReturn->Branch("NtyPercentRisingEdgeAnode1", &f90PercentRisingEdgeAnode1Vec);
  treeToReturn->Branch("RisetimeAnode2", &fRisetimeAnode2Vec);
  treeToReturn->Branch("TenPercentRisingEdgeAnode2", &f10PercentRisingEdgeAnode2Vec);
  treeToReturn->Branch("NtyPercentRisingEdgeAnode2", &f90PercentRisingEdgeAnode2Vec);
  treeToReturn->Branch("RisetimeAnode3", &fRisetimeAnode3Vec);
  treeToReturn->Branch("TenPercentRisingEdgeAnode3", &f10PercentRisingEdgeAnode3Vec);
  treeToReturn->Branch("NtyPercentRisingEdgeAnode3", &f90PercentRisingEdgeAnode3Vec);
  treeToReturn->Branch("FalltimeAnode1", &fFalltimeAnode1Vec);
  treeToReturn->Branch("TenPercentFallingEdgeAnode1", &f10PercentFallingEdgeAnode1Vec);
  treeToReturn->Branch("NtyPercentFallingEdgeAnode1", &f90PercentFallingEdgeAnode1Vec);
  treeToReturn->Branch("FalltimeAnode2", &fFalltimeAnode2Vec);
  treeToReturn->Branch("TenPercentFallingEdgeAnode2", &f10PercentFallingEdgeAnode2Vec);
  treeToReturn->Branch("NtyPercentFallingEdgeAnode2", &f90PercentFallingEdgeAnode2Vec);
  treeToReturn->Branch("FalltimeAnode3", &fFalltimeAnode3Vec);
  treeToReturn->Branch("TenPercentFallingEdgeAnode3", &f10PercentFallingEdgeAnode3Vec);
  treeToReturn->Branch("NtyPercentFallingEdgeAnode3", &f90PercentFallingEdgeAnode3Vec);
  treeToReturn->Branch("IntegralAnode1", &fIntegralAnode1Vec);
  treeToReturn->Branch("IntegralAnode2", &fIntegralAnode2Vec);
  treeToReturn->Branch("IntegralAnode3", &fIntegralAnode3Vec);
  treeToReturn->Branch("NegPeakTimeAnode1", &fNegPeakTimeAnode1Vec);
  treeToReturn->Branch("NegPeakTimeAnode2", &fNegPeakTimeAnode2Vec);
  treeToReturn->Branch("NegPeakTimeAnode3", &fNegPeakTimeAnode3Vec);
  treeToReturn->Branch("MinValueAnode1", &fMinValueAnode1Vec);
  treeToReturn->Branch("MinValueAnode2", &fMinValueAnode2Vec);
  treeToReturn->Branch("MinValueAnode3", &fMinValueAnode3Vec);
  treeToReturn->Branch("NegAmplitudeAnode1", &fNegAmplitudeAnode1Vec);
  treeToReturn->Branch("NegAmplitudeAnode2", &fNegAmplitudeAnode2Vec);
  treeToReturn->Branch("NegAmplitudeAnode3", &fNegAmplitudeAnode3Vec);
  treeToReturn->Branch("NegRisetimeAnode1", &fNegRisetimeAnode1Vec);
  treeToReturn->Branch("NegRisetimeAnode2", &fNegRisetimeAnode2Vec);
  treeToReturn->Branch("NegRisetimeAnode3", &fNegRisetimeAnode3Vec);
  treeToReturn->Branch("NegFalltimeAnode1", &fNegFalltimeAnode1Vec);
  treeToReturn->Branch("NegFalltimeAnode2", &fNegFalltimeAnode2Vec);
  treeToReturn->Branch("NegFalltimeAnode3", &fNegFalltimeAnode3Vec);
  treeToReturn->Branch("NegIntegralAnode1", &fNegIntegralAnode1Vec);
  treeToReturn->Branch("NegIntegralAnode2", &fNegIntegralAnode2Vec);
  treeToReturn->Branch("NegIntegralAnode3", &fNegIntegralAnode3Vec);
  treeToReturn->Branch("GetSparkAnode1", &fGetSparkAnode1Vec);
  treeToReturn->Branch("GetSparkAnode2", &fGetSparkAnode2Vec);
  treeToReturn->Branch("GetSparkAnode3", &fGetSparkAnode3Vec);
  // Parameters calculated (and returned) when a waveform fit is used
  treeToReturn->Branch("FitAmplitudeAnode1", &fFitAmplitudeAnode1Vec);
  treeToReturn->Branch("FitAmplitudeAnode2", &fFitAmplitudeAnode2Vec);
  treeToReturn->Branch("FitAmplitudeAnode3", &fFitAmplitudeAnode3Vec);
  treeToReturn->Branch("FitBaselineEstimateAnode1", &fFitBaselineEstimateAnode1Vec);
  treeToReturn->Branch("FitBaselineEstimateAnode2", &fFitBaselineEstimateAnode2Vec);
  treeToReturn->Branch("FitBaselineEstimateAnode3", &fFitBaselineEstimateAnode3Vec);
  treeToReturn->Branch("FitFirstExponentEstimateAnode1", &fFitFirstExponentEstimateAnode1Vec);
  treeToReturn->Branch("FitFirstExponentEstimateAnode2", &fFitFirstExponentEstimateAnode2Vec);
  treeToReturn->Branch("FitFirstExponentEstimateAnode3", &fFitFirstExponentEstimateAnode3Vec);
  treeToReturn->Branch("FitSecondExponentEstimateAnode1", &fFitSecondExponentEstimateAnode1Vec);
  treeToReturn->Branch("FitSecondExponentEstimateAnode2", &fFitSecondExponentEstimateAnode2Vec);
  treeToReturn->Branch("FitSecondExponentEstimateAnode3", &fFitSecondExponentEstimateAnode3Vec);
  treeToReturn->Branch("FitOffsetEstimateAnode1", &fFitOffsetEstimateAnode1Vec);
  treeToReturn->Branch("FitOffsetEstimateAnode2", &fFitOffsetEstimateAnode2Vec);
  treeToReturn->Branch("FitOffsetEstimateAnode3", &fFitOffsetEstimateAnode3Vec);
  treeToReturn->Branch("FitChi2Anode1", &fFitChi2Anode1Vec);
  treeToReturn->Branch("FitChi2Anode2", &fFitChi2Anode2Vec);
  treeToReturn->Branch("FitChi2Anode3", &fFitChi2Anode3Vec);
  treeToReturn->Branch("FitNdfAnode1", &fFitNdfAnode1Vec);
  treeToReturn->Branch("FitNdfAnode2", &fFitNdfAnode2Vec);
  treeToReturn->Branch("FitNdfAnode3", &fFitNdfAnode3Vec);
  // waveform trigger (all waveforms) and most recent beam trigger for each waveform
  treeToReturn->Branch("WaveformTriggerT0Sec", &waveformTriggerT0SecVec);
  treeToReturn->Branch("WaveformTriggerT0NanoSec", &waveformTriggerT0NanoSecVec);
  treeToReturn->Branch("BeamTriggerTimeSeconds", &beamTriggerTimeSeconds);
  treeToReturn->Branch("BeamTriggerTimeNanoSeconds", &beamTriggerTimeNanoSeconds);
  // beam trigger amplitude information
  // not used so far
  treeToReturn->Branch("BaselineBeamTrigger", &baselineBeamTriggerVec); 
  treeToReturn->Branch("BaselineRMSBeamTrigger", &baselineRMSBeamTriggerVec); 
  treeToReturn->Branch("PeakTimeBeamTrigger", &peakTimeBeamTriggerVec); 
  treeToReturn->Branch("MinValueBeamTrigger", &minValueBeamTriggerVec); 
  treeToReturn->Branch("AmplitudeBeamTrigger", &amplitudeBeamTriggerVec);
  // digitiser configuration
  treeToReturn->Branch("Anode1VMin", &fAnode1VMinVec);
  treeToReturn->Branch("Anode1VMax", &fAnode1VMaxVec);
  treeToReturn->Branch("Anode1Gain", &fAnode1GainVec);
  treeToReturn->Branch("Anode1BaselineOffset", &fAnode1BaselineOffsetVec);
  treeToReturn->Branch("Anode1TriggerThreshold", &fAnode1TriggerThresholdVec); 
  treeToReturn->Branch("Anode2VMin", &fAnode2VMinVec);
  treeToReturn->Branch("Anode2VMax", &fAnode2VMaxVec);
  treeToReturn->Branch("Anode2Gain", &fAnode2GainVec);
  treeToReturn->Branch("Anode2BaselineOffset", &fAnode2BaselineOffsetVec);
  treeToReturn->Branch("Anode2TriggerThreshold", &fAnode2TriggerThresholdVec);
  treeToReturn->Branch("Anode3VMin", &fAnode3VMinVec);
  treeToReturn->Branch("Anode3VMax", &fAnode3VMaxVec);
  treeToReturn->Branch("Anode3Gain", &fAnode3GainVec);
  treeToReturn->Branch("Anode3BaselineOffset", &fAnode3BaselineOffsetVec);
  treeToReturn->Branch("Anode3TriggerThreshold", &fAnode3TriggerThresholdVec);
  treeToReturn->Branch("Anode3BaselineOffset", &fAnode3BaselineOffsetVec);
  treeToReturn->Branch("Anode3TriggerThreshold", &fAnode3TriggerThresholdVec); 
  treeToReturn->Branch("PostTriggerFraction", &postTriggerFractionVec);
  treeToReturn->Branch("AcquisitionTime", &acquisitionTimeVec);
  // book keeping
  treeToReturn->Branch("EventNo", &eventNoVec);
  treeToReturn->Branch("RunNoString", &fRunNoStringVec);
  treeToReturn->Branch("RunNo", &runNoVec);


  /* get the data from the original tree */
  moveBranchToVector(fullTree, "BaselineAnode1", cutString, &fBaselineAnode1Vec);
  moveBranchToVector(fullTree, "BaselineAnode2", cutString, &fBaselineAnode2Vec);
  moveBranchToVector(fullTree, "BaselineAnode3", cutString, &fBaselineAnode3Vec);
  moveBranchToVector(fullTree, "BaselineRMSAnode1", cutString, &fBaselineRMSAnode1Vec);
  moveBranchToVector(fullTree, "BaselineRMSAnode2", cutString, &fBaselineRMSAnode2Vec);
  moveBranchToVector(fullTree, "BaselineRMSAnode3", cutString, &fBaselineRMSAnode3Vec);
  moveBranchToVector(fullTree, "WaveformMeanAnode1", cutString, &fWaveformMeanAnode1Vec);
  moveBranchToVector(fullTree, "WaveformMeanAnode2", cutString, &fWaveformMeanAnode2Vec);
  moveBranchToVector(fullTree, "WaveformMeanAnode3", cutString, &fWaveformMeanAnode3Vec);
  moveBranchToVector(fullTree, "WaveformRMSAnode1", cutString, &fWaveformRMSAnode1Vec);
  moveBranchToVector(fullTree, "WaveformRMSAnode2", cutString, &fWaveformRMSAnode2Vec);
  moveBranchToVector(fullTree, "WaveformRMSAnode3", cutString, &fWaveformRMSAnode3Vec);
  moveBranchToVector(fullTree, "PeakTimeAnode1", cutString, &fPeakTimeAnode1Vec);
  moveBranchToVector(fullTree, "PeakTimeAnode2", cutString, &fPeakTimeAnode2Vec);
  moveBranchToVector(fullTree, "PeakTimeAnode3", cutString, &fPeakTimeAnode3Vec);
  moveBranchToVector(fullTree, "MaxValueAnode1", cutString, &fMaxValueAnode1Vec);
  moveBranchToVector(fullTree, "MaxValueAnode2", cutString, &fMaxValueAnode2Vec);
  moveBranchToVector(fullTree, "MaxValueAnode3", cutString, &fMaxValueAnode3Vec);
  moveBranchToVector(fullTree, "AmplitudeAnode1", cutString, &fAmplitudeAnode1Vec);
  moveBranchToVector(fullTree, "AmplitudeAnode2", cutString, &fAmplitudeAnode2Vec);
  moveBranchToVector(fullTree, "AmplitudeAnode3", cutString, &fAmplitudeAnode3Vec);
  moveBranchToVector(fullTree, "RisetimeAnode1", cutString, &fRisetimeAnode1Vec);
  moveBranchToVector(fullTree, "RisetimeAnode1", cutString, &fRisetimeAnode1Vec);
  moveBranchToVector(fullTree, "TenPercentRisingEdgeAnode1", cutString, &f10PercentRisingEdgeAnode1Vec);
  moveBranchToVector(fullTree, "NtyPercentRisingEdgeAnode1", cutString, &f90PercentRisingEdgeAnode1Vec);
  moveBranchToVector(fullTree, "RisetimeAnode2", cutString, &fRisetimeAnode2Vec);
  moveBranchToVector(fullTree, "TenPercentRisingEdgeAnode2", cutString, &f10PercentRisingEdgeAnode2Vec);
  moveBranchToVector(fullTree, "NtyPercentRisingEdgeAnode2", cutString, &f90PercentRisingEdgeAnode2Vec);
  moveBranchToVector(fullTree, "RisetimeAnode3", cutString, &fRisetimeAnode3Vec);
  moveBranchToVector(fullTree, "TenPercentRisingEdgeAnode3", cutString, &f10PercentRisingEdgeAnode3Vec);
  moveBranchToVector(fullTree, "NtyPercentRisingEdgeAnode3", cutString, &f90PercentRisingEdgeAnode3Vec);  
  moveBranchToVector(fullTree, "FalltimeAnode1", cutString, &fFalltimeAnode1Vec);
  moveBranchToVector(fullTree, "TenPercentFallingEdgeAnode1", cutString, &f10PercentFallingEdgeAnode1Vec);
  moveBranchToVector(fullTree, "NtyPercentFallingEdgeAnode1", cutString, &f90PercentFallingEdgeAnode1Vec);
  moveBranchToVector(fullTree, "FalltimeAnode2", cutString, &fFalltimeAnode2Vec);
  moveBranchToVector(fullTree, "TenPercentFallingEdgeAnode2", cutString, &f10PercentFallingEdgeAnode2Vec);
  moveBranchToVector(fullTree, "NtyPercentFallingEdgeAnode2", cutString, &f90PercentFallingEdgeAnode2Vec);
  moveBranchToVector(fullTree, "FalltimeAnode3", cutString, &fFalltimeAnode3Vec);
  moveBranchToVector(fullTree, "TenPercentFallingEdgeAnode3", cutString, &f10PercentFallingEdgeAnode3Vec);
  moveBranchToVector(fullTree, "NtyPercentFallingEdgeAnode3", cutString, &f90PercentFallingEdgeAnode3Vec);
  moveBranchToVector(fullTree, "IntegralAnode2", cutString, &fIntegralAnode2Vec);
  moveBranchToVector(fullTree, "IntegralAnode3", cutString, &fIntegralAnode3Vec);
  moveBranchToVector(fullTree, "NegPeakTimeAnode1", cutString, &fPeakTimeAnode1Vec);
  moveBranchToVector(fullTree, "NegPeakTimeAnode2", cutString, &fPeakTimeAnode2Vec);
  moveBranchToVector(fullTree, "NegPeakTimeAnode3", cutString, &fPeakTimeAnode3Vec);
  moveBranchToVector(fullTree, "MinValueAnode1", cutString, &fMinValueAnode1Vec);
  moveBranchToVector(fullTree, "MinValueAnode2", cutString, &fMinValueAnode2Vec);
  moveBranchToVector(fullTree, "MinValueAnode3", cutString, &fMinValueAnode3Vec);
  moveBranchToVector(fullTree, "NegAmplitudeAnode1", cutString, &fNegAmplitudeAnode1Vec);
  moveBranchToVector(fullTree, "NegAmplitudeAnode2", cutString, &fNegAmplitudeAnode2Vec);
  moveBranchToVector(fullTree, "NegAmplitudeAnode3", cutString, &fNegAmplitudeAnode3Vec);
  moveBranchToVector(fullTree, "NegRisetimeAnode1", cutString, &fNegRisetimeAnode1Vec);
  moveBranchToVector(fullTree, "NegRisetimeAnode2", cutString, &fNegRisetimeAnode2Vec);
  moveBranchToVector(fullTree, "NegRisetimeAnode3", cutString, &fNegRisetimeAnode3Vec);
  moveBranchToVector(fullTree, "NegFalltimeAnode1", cutString, &fNegFalltimeAnode1Vec);
  moveBranchToVector(fullTree, "NegFalltimeAnode2", cutString, &fNegFalltimeAnode2Vec);
  moveBranchToVector(fullTree, "NegFalltimeAnode3", cutString, &fNegFalltimeAnode3Vec);
  moveBranchToVector(fullTree, "NegIntegralAnode1", cutString, &fNegIntegralAnode1Vec);
  moveBranchToVector(fullTree, "NegIntegralAnode2", cutString, &fNegIntegralAnode2Vec);
  moveBranchToVector(fullTree, "NegIntegralAnode3", cutString, &fNegIntegralAnode3Vec);
  moveBranchToVector(fullTree, "GetSparkAnode1", cutString, &fGetSparkAnode1Vec);
  moveBranchToVector(fullTree, "GetSparkAnode2", cutString, &fGetSparkAnode2Vec);
  moveBranchToVector(fullTree, "GetSparkAnode3", cutString, &fGetSparkAnode3Vec);
  // waveform trigger (all waveforms) and most recent beam trigger for each waveform
  moveBranchToVector(fullTree, "WaveformTriggerT0Sec", cutString, &waveformTriggerT0SecVec);
  moveBranchToVector(fullTree, "WaveformTriggerT0NanoSec", cutString, &waveformTriggerT0NanoSecVec);
  moveBranchToVector(fullTree, "BeamTriggerTimeSeconds", cutString, &beamTriggerTimeSeconds);
  moveBranchToVector(fullTree, "BeamTriggerTimeNanoSeconds", cutString, &beamTriggerTimeNanoSeconds);
  // Parameters calculated (and returned) when a waveform fit is used
  moveBranchToVector(fullTree, "FitAmplitudeAnode1", cutString, &fFitAmplitudeAnode1Vec);
  moveBranchToVector(fullTree, "FitAmplitudeAnode2", cutString, &fFitAmplitudeAnode2Vec);
  moveBranchToVector(fullTree, "FitAmplitudeAnode3", cutString, &fFitAmplitudeAnode3Vec);
  moveBranchToVector(fullTree, "FitBaselineEstimateAnode1", cutString, &fFitBaselineEstimateAnode1Vec);
  moveBranchToVector(fullTree, "FitBaselineEstimateAnode2", cutString, &fFitBaselineEstimateAnode2Vec);
  moveBranchToVector(fullTree, "FitBaselineEstimateAnode3", cutString, &fFitBaselineEstimateAnode3Vec);
  moveBranchToVector(fullTree, "FitFirstExponentEstimateAnode1", cutString, &fFitFirstExponentEstimateAnode1Vec);
  moveBranchToVector(fullTree, "FitFirstExponentEstimateAnode2", cutString, &fFitFirstExponentEstimateAnode2Vec);
  moveBranchToVector(fullTree, "FitFirstExponentEstimateAnode3", cutString, &fFitFirstExponentEstimateAnode3Vec);
  moveBranchToVector(fullTree, "FitSecondExponentEstimateAnode1", cutString, &fFitSecondExponentEstimateAnode1Vec);
  moveBranchToVector(fullTree, "FitSecondExponentEstimateAnode2", cutString, &fFitSecondExponentEstimateAnode2Vec);
  moveBranchToVector(fullTree, "FitSecondExponentEstimateAnode3", cutString, &fFitSecondExponentEstimateAnode3Vec);
  moveBranchToVector(fullTree, "FitOffsetEstimateAnode1", cutString, &fFitOffsetEstimateAnode1Vec);
  moveBranchToVector(fullTree, "FitOffsetEstimateAnode2", cutString, &fFitOffsetEstimateAnode2Vec);
  moveBranchToVector(fullTree, "FitOffsetEstimateAnode3", cutString, &fFitOffsetEstimateAnode3Vec);
  moveBranchToVector(fullTree, "FitChi2Anode1", cutString, &fFitChi2Anode1Vec);
  moveBranchToVector(fullTree, "FitChi2Anode2", cutString, &fFitChi2Anode2Vec);
  moveBranchToVector(fullTree, "FitChi2Anode3", cutString, &fFitChi2Anode3Vec);
  moveBranchToVector(fullTree, "FitNdfAnode1", cutString, &fFitNdfAnode1Vec);
  moveBranchToVector(fullTree, "FitNdfAnode2", cutString, &fFitNdfAnode2Vec);
  moveBranchToVector(fullTree, "FitNdfAnode3", cutString, &fFitNdfAnode3Vec);
  // beam trigger amplitude information
  //moveBranchToVector(fullTree, "BaselineBeamTrigger", cutString, &baselineBeamTriggerVec);
  //moveBranchToVector(fullTree, "BaselineRMSBeamTrigger", cutString, &baselineRMSBeamTriggerVec);
  //moveBranchToVector(fullTree, "PeakTimeBeamTrigger", cutString, &peakTimeBeamTriggerVec);
  //moveBranchToVector(fullTree, "MinValueBeamTrigger", cutString, &minValueBeamTriggerVec);
  //moveBranchToVector(fullTree, "AmplitudeBeamTrigger", cutString, &amplitudeBeamTriggerVec);
  // digitiser configuration
  moveBranchToVector(fullTree, "Anode1VMin", cutString, &fAnode1VMinVec);
  moveBranchToVector(fullTree, "Anode1VMax", cutString, &fAnode1VMaxVec);
  moveBranchToVector(fullTree, "Anode1Gain", cutString, &fAnode1GainVec);
  moveBranchToVector(fullTree, "Anode1BaselineOffset", cutString, &fAnode1BaselineOffsetVec); 
  moveBranchToVector(fullTree, "Anode1TriggerThreshold", cutString, &fAnode1TriggerThresholdVec);
  moveBranchToVector(fullTree, "Anode2VMin", cutString, &fAnode2VMinVec);
  moveBranchToVector(fullTree, "Anode2VMax", cutString, &fAnode2VMaxVec);
  moveBranchToVector(fullTree, "Anode2Gain", cutString, &fAnode2GainVec);
  moveBranchToVector(fullTree, "Anode2BaselineOffset", cutString, &fAnode2BaselineOffsetVec); 
  moveBranchToVector(fullTree, "Anode2TriggerThreshold", cutString, &fAnode2TriggerThresholdVec);
  moveBranchToVector(fullTree, "Anode3VMin", cutString, &fAnode3VMinVec);
  moveBranchToVector(fullTree, "Anode3VMax", cutString, &fAnode3VMaxVec);
  moveBranchToVector(fullTree, "Anode3Gain", cutString, &fAnode3GainVec);
  moveBranchToVector(fullTree, "Anode3BaselineOffset", cutString, &fAnode3BaselineOffsetVec); 
  moveBranchToVector(fullTree, "Anode3TriggerThreshold", cutString, &fAnode3TriggerThresholdVec);
  moveBranchToVector(fullTree, "PostTriggerFraction", cutString, &postTriggerFractionVec);
  moveBranchToVector(fullTree, "AcquisitionTime", cutString, &acquisitionTimeVec);
  // book keeping
  moveBranchToVector(fullTree, "EventNo", cutString, &eventNoVec);
  moveBranchToVector(fullTree, "RunNo", cutString, &runNoVec);
  fRunNoStringVec = createStringVec(runNoVec);


  /* fill */
  treeToReturn->Fill();


  /* return */
  return treeToReturn;
}


// helper function to turn a single branch into a graph
TGraph* branchDataAsGraph(TTree* fullTree, std::string branchName, std::string cutString, Double_t *dataMinValue = 0, Double_t *dataMaxValue = 0){
  int sampleSize = fullTree->Draw(branchName.c_str(), cutString.c_str(), "goff");
  std::vector<Double_t> pointCountVec;
  std::vector<Double_t> graphDataVec;
  TVectorD dataFromBranch(fullTree->GetSelectedRows(), fullTree->GetV1());
  
  for(int iFillData = 0; iFillData < sampleSize; iFillData++) { 
    pointCountVec.push_back(iFillData);
    graphDataVec.push_back(dataFromBranch[iFillData]); 
    if((dataMinValue) && (*dataMinValue > dataFromBranch[iFillData])) { *dataMinValue = dataFromBranch[iFillData]; }
    if((dataMaxValue) && (*dataMaxValue < dataFromBranch[iFillData])) { *dataMaxValue = dataFromBranch[iFillData]; }
  }

  return new TGraph(sampleSize, &pointCountVec[0], &graphDataVec[0]);
}


// the actual plotting and cleaning function (mainly cleaning and not plotting)
void plottingAndCleaning(std::string input_file_name, std::string output_file_name, std::string additional_cuts = "", bool do_cleaning = true, std::string external_anode_1_cuts = "", std::string external_anode_2_cuts = "", std::string external_anode_3_cuts = "", std::string charge_analysis_tree_name = "chargeGainAnalysisTree"){

  // possible function input parameters 
  const bool print_control_plots_to_screen = true;
  const bool print_control_plots_to_pdfs = false;    // currently not implemented (even if set true)
  const bool enable_spark_cuts = true;


  /* open file and get tree */
  TFile* inputFile = new TFile(input_file_name.c_str(), "READ");
  if(!inputFile->IsOpen()){
    std::cout << input_file_name << " could not be opened" << std::endl;
    return;
  }
  else{ std::cout << input_file_name << " open" << std::endl; }

  TTree* chargeAnalysisTree = (TTree*)inputFile->Get(charge_analysis_tree_name.c_str());
  if(!chargeAnalysisTree){
    std::cout << charge_analysis_tree_name << " not found" << std::endl;
    inputFile->Close();
    return;
  }
  else{ std::cout << charge_analysis_tree_name << " loaded" << std::endl; }


  /* in case we do some plotting we should take care of the axis ranges / binning somewhere. e.g. here */
  double tempHisto_baselineMin = -2000;
  double tempHisto_baselineMax = 2000;
  int tempHisto_baselineNbBins = 500;

  double tempHisto_2DAmplitudeHistoMin = -2000;
  double tempHisto_2DAmplitudeHistoMax = 2000;
  int tempHisto_2DAmplitudeHistoBins = 200;

  // some storage
  double allAnodeBaselineMean[3] = { 0, 0, 0};
  double allAnodeBaselineRMS[3] = { 0, 0, 0};


  /* taking care of cuts to be applied to all the data */
  std::string anodeChannelCuts[3] = {"" , "", ""};
  if(additional_cuts != "") {
    anodeChannelCuts[0] = additional_cuts;
    anodeChannelCuts[1] = additional_cuts;
    anodeChannelCuts[2] = additional_cuts;
  }
  if(external_anode_1_cuts != ""){
    if(anodeChannelCuts[0] != ""){ anodeChannelCuts[0] += " && "; }
    anodeChannelCuts[0] += external_anode_1_cuts; 
  }
  if(external_anode_2_cuts != ""){
    if(anodeChannelCuts[1] != ""){ anodeChannelCuts[1] += " && "; }
    anodeChannelCuts[1] += external_anode_2_cuts; 
  }
  if(external_anode_3_cuts != ""){
    if(anodeChannelCuts[2] != ""){ anodeChannelCuts[2] += " && "; }
    anodeChannelCuts[2] += external_anode_3_cuts; 
  }


  /* compile the data cleaning cuts */
  if(do_cleaning){
    for(int iAnode = 1; iAnode <= 3; iAnode++){
      // cleaning cut for the current anode:
      std::string currentAnodeCut = "";


      // baseline cut: are the baseline values of each event in a 5 RMS interval around the mean of all baseline values
      std::stringstream baselineInRMSRangeCut;
      baselineInRMSRangeCut << "BaselineAnode" << iAnode;

      TH1D* tmp_currentBaselineDist = new TH1D((baselineInRMSRangeCut.str()+"_tmpHist").c_str(), (baselineInRMSRangeCut.str()+"_tmpHist").c_str(), tempHisto_baselineNbBins, tempHisto_baselineMin, tempHisto_baselineMax);
      int sampleSize = chargeAnalysisTree->Draw((baselineInRMSRangeCut.str()+" >> "+baselineInRMSRangeCut.str()+"_tmpHist").c_str(), anodeChannelCuts[iAnode-1].c_str(), "goff");
      // fall back solution:
      //int sampleSize = chargeAnalysisTree->Draw(baselineInRMSRangeCut.str().c_str(), anodeChannelCuts[iAnode-1].c_str(), "goff");
      //TVectorD baselineData(chargeAnalysisTree->GetSelectedRows(), chargeAnalysisTree->GetV1());
      //for(int iFillData = 0; iFillData < sampleSize; iFillData++) { tmp_currentBaselineDist->Fill(baselineData[iFillData]); }
      double baselineMean = tmp_currentBaselineDist->GetMean();
      allAnodeBaselineMean[iAnode-1] = baselineMean;
      double baselineRMS = tmp_currentBaselineDist->GetRMS();
      allAnodeBaselineRMS[iAnode-1] = baselineRMS;

      
      //baselineInRMSRangeCut << " < " << baselineMean + 5*baselineRMS << " && BaselineAnode" << iAnode << " > " << baselineMean - 5*baselineRMS;
      baselineInRMSRangeCut << " < " << 30 << " && BaselineAnode" << iAnode << " > " << -30;      
// lets try a more aggressive cut here
      //baselineInRMSRangeCut << " < " << baselineMean + baselineRMS << " && BaselineAnode" << iAnode << " > " << baselineMean - baselineRMS;
      if(sampleSize > 0){ currentAnodeCut += baselineInRMSRangeCut.str(); }


      // baselineRMS cut: are the baselineRMS values of each event in a 5 RMS interval around the mean of all baselineRMS values
      std::stringstream baselineRMSInRMSRangeCut;
      baselineRMSInRMSRangeCut << "BaselineRMSAnode" << iAnode;
      TH1D* tmp_currentBaselineRMSDist = new TH1D((baselineRMSInRMSRangeCut.str()+"_tmpHist").c_str(), (baselineRMSInRMSRangeCut.str()+"_tmpHist").c_str(), tempHisto_baselineNbBins, tempHisto_baselineMin, tempHisto_baselineMax);
      sampleSize = chargeAnalysisTree->Draw((baselineRMSInRMSRangeCut.str()+" >> "+baselineRMSInRMSRangeCut.str()+"_tmpHist").c_str(), anodeChannelCuts[iAnode-1].c_str(), "goff");
      // fall back solution:
      //sampleSize = chargeAnalysisTree->Draw(baselineRMSInRMSRangeCut.str().c_str(), anodeChannelCuts[iAnode-1].c_str(), "goff");
      //TVectorD baselineRMSData(chargeAnalysisTree->GetSelectedRows(), chargeAnalysisTree->GetV1());
      //for(int iFillData = 0; iFillData < sampleSize; iFillData++) { tmp_currentBaselineRMSDist->Fill(baselineRMSData[iFillData]); }
      double baselineRMSMean = tmp_currentBaselineRMSDist->GetMean();
      double baselineRMSRMS = tmp_currentBaselineRMSDist->GetRMS();

      // baselineRMSInRMSRangeCut << " < " << baselineRMSMean + 5*baselineRMSRMS << " && BaselineRMSAnode" << iAnode << " > " << baselineRMSMean - 5*baselineRMSRMS;
      baselineRMSInRMSRangeCut << " < " << 15 << " && BaselineRMSAnode" << iAnode << " > " << 0;
      if(sampleSize > 0){ 
        if(currentAnodeCut != "") { currentAnodeCut += " && "; }
        currentAnodeCut += baselineRMSInRMSRangeCut.str(); 
      }

      
      // baseline below threshold cut: make sure the baseline is smaller than the trigger threshold
      std::stringstream baselineBelowTriggerThresholdCut;
      baselineBelowTriggerThresholdCut << "BaselineAnode" << iAnode << " < Anode" << iAnode << "TriggerThreshold";
      if(currentAnodeCut != "") { currentAnodeCut += " && "; }
      currentAnodeCut += baselineBelowTriggerThresholdCut.str();


      // peak over threshold: make sure the peak height is actually higher than the trigger threshold
      std::stringstream peakOverTriggerThresholdCut;
      //peakOverTriggerThresholdCut << "AmplitudeAnode" << iAnode << " > Anode" << iAnode << "TriggerThreshold";
      // ToDo: Check whether the MaxValueAnode should be used here instead of the AmplitudeAnode cut
      std::cout << " Open question (?): Should the AmplitudeAnode value be used instead of MaxValueAnode - probably not" << std::endl;
      peakOverTriggerThresholdCut << "MaxValueAnode" << iAnode << " > Anode" << iAnode << "TriggerThreshold";
      if(currentAnodeCut != "") { currentAnodeCut += " && "; }
      currentAnodeCut += peakOverTriggerThresholdCut.str();


      // spark flag cut
      std::stringstream cspSparkCheckCut; 
      cspSparkCheckCut << "((GetSparkAnode1 + GetSparkAnode2 + GetSparkAnode3 < 2) && GetSparkAnode" << iAnode << " < 1 )";
      if(currentAnodeCut != "") { currentAnodeCut += " && "; }
      currentAnodeCut += cspSparkCheckCut.str();      

      // collect all cuts, i.e. extend the anode i cut by the currentAnodeCut string
      if(enable_spark_cuts){
        if(anodeChannelCuts[iAnode-1] != "") { anodeChannelCuts[iAnode-1] += " && "; }
        anodeChannelCuts[iAnode-1] += currentAnodeCut;
      }
    } // end of: for(int iAnode ...
      

    // cross talk cuts: the below here does some checks with the ultimate goal to determine the parameters needed to reject cross talk signals
    // First: A linear fit to the X and Y Profile of the amplitude correlation histograms in order to determine whether a cross talk cut is needed
    // Todo: Should this work at some point, this should turn again into functions

    // fit function and other variables (for latter)
    TF1* linearFit = new TF1("linearFit", "[0]+[1]*x", tempHisto_2DAmplitudeHistoMin, tempHisto_2DAmplitudeHistoMax);
    double chiSqNDFXProfile = 0.0;
    double chiSqNDFYProfile = 0.0;
    double slopeXProfile = 0.0;
    double slopeYProfile = 0.0;
    std::stringstream crossTalkAnode12Cut;
    std::stringstream crossTalkAnode23Cut;
    std::stringstream crossTalkAnode31Cut;
      
    // create histogram
    TH2D* tmp_anode1anode2correlations = new TH2D("anode1anode2correlations", "anode1anode2correlations", tempHisto_2DAmplitudeHistoBins, tempHisto_2DAmplitudeHistoMin, tempHisto_2DAmplitudeHistoMax, tempHisto_2DAmplitudeHistoBins, tempHisto_2DAmplitudeHistoMin, tempHisto_2DAmplitudeHistoMax);
    int sampleSize = chargeAnalysisTree->Draw("AmplitudeAnode1:AmplitudeAnode2 >> anode1anode2correlations", "", "goff");
    if(sampleSize > 0){
      // x check
      TH1D* tmp_anode1anode2correlations_profileX = tmp_anode1anode2correlations->ProfileX();
      tmp_anode1anode2correlations_profileX->Fit("linearFit", "Q");
      tmp_anode1anode2correlations_profileX->Fit("linearFit", "Q");

      // check fit
      if(linearFit->GetNDF() == 0) { chiSqNDFXProfile = linearFit->GetChisquare(); }
      else{ chiSqNDFXProfile = linearFit->GetChisquare()/linearFit->GetNDF(); }
      slopeXProfile = linearFit->GetParameter(1);

      // x check
      TH1D* tmp_anode1anode2correlations_profileY = tmp_anode1anode2correlations->ProfileY();
      tmp_anode1anode2correlations_profileY->Fit("linearFit", "Q");
      tmp_anode1anode2correlations_profileY->Fit("linearFit", "Q");

      // check fit
      if(linearFit->GetNDF() == 0) { chiSqNDFYProfile = linearFit->GetChisquare(); }
      else{ chiSqNDFYProfile = linearFit->GetChisquare()/linearFit->GetNDF(); }
      slopeYProfile = linearFit->GetParameter(1);

      // The cross talk cut gets only added in case the linear fit to the 2D distribution turns out bad, suggesting the data points are not sitting all on one row:
      std::cout << "Cross talk checks: Anode 1 / 2: chiSq X: " << chiSqNDFXProfile << " chiSq Y: " << chiSqNDFYProfile << " slope X: " << slopeXProfile << " slope Y: " << slopeYProfile << std::endl;
      if (chiSqNDFXProfile > 50 && chiSqNDFYProfile > 50){
        std::cout <<  "lets consider this a bad fit" << std::endl;
        if (slopeXProfile < slopeYProfile) { crossTalkAnode12Cut << "AmplitudeAnode2/AmplitudeAnode1 > " << slopeYProfile; }
        else{ crossTalkAnode12Cut << "AmplitudeAnode2/AmplitudeAnode1 > " << slopeXProfile; }
      }
    }

    // create histogram
    TH2D* tmp_anode2anode3correlations = new TH2D("anode2anode3correlations", "anode2anode3correlations", tempHisto_2DAmplitudeHistoBins, tempHisto_2DAmplitudeHistoMin, tempHisto_2DAmplitudeHistoMax, tempHisto_2DAmplitudeHistoBins, tempHisto_2DAmplitudeHistoMin, tempHisto_2DAmplitudeHistoMax);
    sampleSize = chargeAnalysisTree->Draw("AmplitudeAnode2:AmplitudeAnode3 >> anode2anode3correlations", "", "goff");
    if(sampleSize > 0){
      // x check
      TH1D* tmp_anode2anode3correlations_profileX = tmp_anode2anode3correlations->ProfileX();
      tmp_anode2anode3correlations_profileX->Fit("linearFit", "Q");
      tmp_anode2anode3correlations_profileX->Fit("linearFit", "Q");

      // check fit
      if(linearFit->GetNDF() == 0) { chiSqNDFXProfile = linearFit->GetChisquare(); }
      else{ chiSqNDFXProfile = linearFit->GetChisquare()/linearFit->GetNDF(); }
      slopeXProfile = linearFit->GetParameter(1);

      // x check
      TH1D* tmp_anode2anode3correlations_profileY = tmp_anode2anode3correlations->ProfileY();
      tmp_anode2anode3correlations_profileY->Fit("linearFit", "Q");
      tmp_anode2anode3correlations_profileY->Fit("linearFit", "Q");

      // check fit
      if(linearFit->GetNDF() == 0) { chiSqNDFYProfile = linearFit->GetChisquare(); }
      else{ chiSqNDFYProfile = linearFit->GetChisquare()/linearFit->GetNDF(); }
      slopeYProfile = linearFit->GetParameter(1);

      // The cross talk cut gets only added in case the linear fit to the 2D distribution turns out bad, suggesting the data points are not sitting all on one row:
      std::cout << "Cross talk checks: Anode 2 / 3: chiSq X: " << chiSqNDFXProfile << " chiSq Y: " << chiSqNDFYProfile << " slope X: " << slopeXProfile << " slope Y: " << slopeYProfile << std::endl;
      if (chiSqNDFXProfile > 50 && chiSqNDFYProfile > 50){
        std::cout <<  "lets consider this a bad fit" << std::endl;
        if (slopeXProfile < slopeYProfile) { crossTalkAnode23Cut << "AmplitudeAnode3/AmplitudeAnode2 > " << slopeYProfile; }
        else{ crossTalkAnode23Cut << "AmplitudeAnode3/AmplitudeAnode2 > " << slopeXProfile; }
      }
    }


    // create histogram
    TH2D* tmp_anode3anode1correlations = new TH2D("anode3anode1correlations", "anode3anode1correlations", tempHisto_2DAmplitudeHistoBins, tempHisto_2DAmplitudeHistoMin, tempHisto_2DAmplitudeHistoMax, tempHisto_2DAmplitudeHistoBins, tempHisto_2DAmplitudeHistoMin, tempHisto_2DAmplitudeHistoMax);
    sampleSize = chargeAnalysisTree->Draw("AmplitudeAnode3:AmplitudeAnode1 >> anode3anode1correlations", "", "goff");
    if(sampleSize > 0){
      // x check
      TH1D* tmp_anode3anode1correlations_profileX = tmp_anode3anode1correlations->ProfileX();
      tmp_anode3anode1correlations_profileX->Fit("linearFit", "Q");
      tmp_anode3anode1correlations_profileX->Fit("linearFit", "Q");

      // check fit
      if(linearFit->GetNDF() == 0) { chiSqNDFXProfile = linearFit->GetChisquare(); }
      else{ chiSqNDFXProfile = linearFit->GetChisquare()/linearFit->GetNDF(); }
      slopeXProfile = linearFit->GetParameter(1);

      // x check
      TH1D* tmp_anode3anode1correlations_profileY = tmp_anode3anode1correlations->ProfileY();
      tmp_anode3anode1correlations_profileY->Fit("linearFit", "Q");
      tmp_anode3anode1correlations_profileY->Fit("linearFit", "Q");

      // check fit
      if(linearFit->GetNDF() == 0) { chiSqNDFYProfile = linearFit->GetChisquare(); }
      else{ chiSqNDFYProfile = linearFit->GetChisquare()/linearFit->GetNDF(); }
      slopeYProfile = linearFit->GetParameter(1);

      // The cross talk cut gets only added in case the linear fit to the 2D distribution turns out bad, suggesting the data points are not sitting all on one row:
      std::cout << "Cross talk checks: Anode 3 / 1: chiSq X: " << chiSqNDFXProfile << " chiSq Y: " << chiSqNDFYProfile << " slope X: " << slopeXProfile << " slope Y: " << slopeYProfile << std::endl;
      if (chiSqNDFXProfile > 50 && chiSqNDFYProfile > 50){
        std::cout <<  "lets consider this a bad fit" << std::endl;
        if (slopeXProfile < slopeYProfile) { crossTalkAnode31Cut << "AmplitudeAnode1/AmplitudeAnode3 < " << slopeYProfile; }
        else{ crossTalkAnode31Cut << "AmplitudeAnode1/AmplitudeAnode3 <" << slopeXProfile; }
      }
    }

    // add the cross talk cuts to the appropriate anode cuts
    std::cout << "cross talk cuts disabled, debug or use the external_anode_i_cuts instead" << std::endl; 
    std::cout << "\t anode 1 cross talk cut:" << std::endl;
    std::cout << "\t " << crossTalkAnode12Cut.str() << " && " << crossTalkAnode31Cut.str() << std::endl;
    std::cout << "\t anode 2 cross talk cut:" << std::endl;
    std::cout << "\t "<< crossTalkAnode12Cut.str() << " && " << crossTalkAnode23Cut.str() << std::endl;
    std::cout << "\t anode 3 cross talk cut:" << std::endl;
    std::cout << "\t " << crossTalkAnode23Cut.str() << " && " << crossTalkAnode31Cut.str() << std::endl;
    /*if(anodeChannelCuts[0] != "") { anodeChannelCuts[0] += " && "; }
    anodeChannelCuts[0] += (crossTalkAnode12.str() + " && " + crossTalkAnode13.str());
    if(anodeChannelCuts[1] != "") { anodeChannelCuts[1] += " && "; }
    anodeChannelCuts[1] += (crossTalkAnode12.str() + " && " + crossTalkAnode23.str());
    if(anodeChannelCuts[2] != "") { anodeChannelCuts[2] += " && "; }
    anodeChannelCuts[2] += (crossTalkAnode23.str() + " && " + crossTalkAnode13.str());*/
  }// end of: if(do_cleaning) ...


  /* From here on the "classical" histograms are created, which we used already for the dmtpc analysis */
  // Create histograms from the tree, using all cuts until here
  // .... this I skipped for the moment - are these histograms still used for any analysis or is only the tree used?
  // .... In case they are still used we can bring the histograms back
  

  /* Write some interesting plots to a pdf */
  // .... currently not implemented 


  /* write output root file */
  // create file
  TFile* outputFile = new TFile(output_file_name.c_str(), "recreate");
  if(!outputFile->IsOpen()){
    std::cout << output_file_name << " could not be opened" << std::endl;
    return;
  }
  else{ std::cout << output_file_name << " open" << std::endl; }  
  outputFile->cd();

  // a good place to write all the "dmtpc analysis" histograms
  // ...


  // lets write four trees: the original one
  //(chargeAnalysisTree->CloneTree())->Write();
  TTree* originalTree = ICanNotBeliveThereIsNoRootToolToDoThis(chargeAnalysisTree, "", "chargeGainAnalysisTree", "chargeGainAnalysisTree");
  
  // create three cleaned trees - each tree for the data after ith (i=1,2,3) anode cleaning cut
  TTree* chargeAnalysisTree_anode1 = ICanNotBeliveThereIsNoRootToolToDoThis(chargeAnalysisTree, anodeChannelCuts[0], "chargeGainAnalysisTree_anode1", "chargeGainAnalysisTree_anode1");
  TTree* chargeAnalysisTree_anode2 = ICanNotBeliveThereIsNoRootToolToDoThis(chargeAnalysisTree, anodeChannelCuts[1], "chargeGainAnalysisTree_anode2", "chargeGainAnalysisTree_anode2");
  TTree* chargeAnalysisTree_anode3 = ICanNotBeliveThereIsNoRootToolToDoThis(chargeAnalysisTree, anodeChannelCuts[2], "chargeGainAnalysisTree_anode3", "chargeGainAnalysisTree_anode3");
  
  outputFile->cd();
  chargeAnalysisTree_anode1->Write();
  chargeAnalysisTree_anode2->Write();
  chargeAnalysisTree_anode3->Write();

  outputFile->Write();
  outputFile->Close();



  /* control plots: baseline checks */
  if(print_control_plots_to_screen){
    TCanvas *allAnodePlots = new TCanvas("allAnodePlots", "allAnodePlots");
    TLegend *legendAllPlots = new TLegend(0.1,0.7,0.48,0.9);
    TCanvas *singleAnodePlots[3] = { new TCanvas("anode1plots", "anode1plots"), new TCanvas("anode2plots", "anode2plots"), new TCanvas("anode3plots", "anode3plots")};
    TGraph *controlGraphBaseline[3] = {NULL, NULL, NULL};
    TGraph *controlGraphMaxValue[3] = {NULL, NULL, NULL};
    TGraph *controlGraphSparkCheck[3] = {NULL, NULL, NULL};
    TGraph *controlGraphTriggerThreshold[3] = {NULL, NULL, NULL};
    TGraph *controlGraphBaselineOffset[3] = {NULL, NULL, NULL};
    TGraph *controlGraphAllBaselineMean[3] = {NULL, NULL, NULL};
    TGraph *controlGraphAllBaselineRMSHigh[3] = {NULL, NULL, NULL};
    TGraph *controlGraphAllBaselineRMSLow[3] = {NULL, NULL, NULL};
    TLegend *legends[3] = {NULL, NULL, NULL};


    // loop over all three anodes and create plots
    for(int iAnode = 1; iAnode <= 3; iAnode++){
      Double_t currentAnodeMin = 1000;
      Double_t currentAnodeMax = -1000;

      std::stringstream controlGraphBaselineName;
      controlGraphBaselineName << "BaselineAnode" << iAnode;
      controlGraphBaseline[iAnode-1] = branchDataAsGraph(chargeAnalysisTree, controlGraphBaselineName.str(), anodeChannelCuts[iAnode-1], &currentAnodeMin, &currentAnodeMax);
      controlGraphBaselineName.str("");
      controlGraphBaselineName << "controlGraphBaselineAnode" << iAnode;
      controlGraphBaseline[iAnode-1]->SetTitle(controlGraphBaselineName.str().c_str());
      controlGraphBaseline[iAnode-1]->SetName(controlGraphBaselineName.str().c_str());

      std::stringstream controlGraphMaxValueName;
      controlGraphMaxValueName << "MaxValueAnode" << iAnode;
      controlGraphMaxValue[iAnode-1] = branchDataAsGraph(chargeAnalysisTree, controlGraphMaxValueName.str(), anodeChannelCuts[iAnode-1], &currentAnodeMin, &currentAnodeMax);
      controlGraphMaxValueName.str("");
      controlGraphMaxValueName << "controlGraphMaxValueAnode" << iAnode;
      controlGraphMaxValue[iAnode-1]->SetTitle(controlGraphMaxValueName.str().c_str());
      controlGraphMaxValue[iAnode-1]->SetName(controlGraphMaxValueName.str().c_str());

      std::stringstream controlGraphSparkCheckName;
      controlGraphSparkCheckName << "GetSparkAnode" << iAnode << "*100";
      controlGraphSparkCheck[iAnode-1] = branchDataAsGraph(chargeAnalysisTree, controlGraphSparkCheckName.str(), anodeChannelCuts[iAnode-1], &currentAnodeMin, &currentAnodeMax);
      controlGraphSparkCheckName.str("");
      controlGraphSparkCheckName << "controlGraphSparkCheckAnode" << iAnode;
      controlGraphSparkCheck[iAnode-1]->SetTitle(controlGraphSparkCheckName.str().c_str());
      controlGraphSparkCheck[iAnode-1]->SetName(controlGraphSparkCheckName.str().c_str());

      std::stringstream controlGraphTriggerThresholdName;
      controlGraphTriggerThresholdName << "Anode" << iAnode << "TriggerThreshold";
      controlGraphTriggerThreshold[iAnode-1] = branchDataAsGraph(chargeAnalysisTree, controlGraphTriggerThresholdName.str(), anodeChannelCuts[iAnode-1], &currentAnodeMin, &currentAnodeMax);
      controlGraphTriggerThresholdName.str("");
      controlGraphTriggerThresholdName << "controlGraphAnode" << iAnode << "TriggerThreshold";
      controlGraphTriggerThreshold[iAnode-1]->SetTitle(controlGraphTriggerThresholdName.str().c_str());
      controlGraphTriggerThreshold[iAnode-1]->SetName(controlGraphTriggerThresholdName.str().c_str());

      std::stringstream controlGraphBaselineOffsetName;
      controlGraphBaselineOffsetName << "Anode" << iAnode << "BaselineOffset";
      controlGraphBaselineOffset[iAnode-1] = branchDataAsGraph(chargeAnalysisTree, controlGraphBaselineOffsetName.str(), anodeChannelCuts[iAnode-1], &currentAnodeMin, &currentAnodeMax);
      controlGraphBaselineOffsetName.str("");
      controlGraphBaselineOffsetName << "controlGraphAnode" << iAnode << "BaselineOffset";
      controlGraphBaselineOffset[iAnode-1]->SetTitle(controlGraphBaselineOffsetName.str().c_str());
      controlGraphBaselineOffset[iAnode-1]->SetName(controlGraphBaselineOffsetName.str().c_str());

      // mean of all baseline means
      std::stringstream controlGraphAllBaselineMeanName;
      controlGraphAllBaselineMeanName << "Anode" << iAnode << "AllBaselineMean";
      double allBaselineMeanAmp[2] = {allAnodeBaselineMean[iAnode-1], allAnodeBaselineMean[iAnode-1]};
      double allBaselineMeanId[2] = {-1000, 1e+6 };
      controlGraphAllBaselineMean[iAnode-1] = new TGraph(2, allBaselineMeanId, allBaselineMeanAmp);
      controlGraphAllBaselineMeanName.str("");
      controlGraphAllBaselineMeanName << "controlGraphAnode" << iAnode << "AllBaselineMean";
      controlGraphAllBaselineMean[iAnode-1]->SetTitle(controlGraphAllBaselineMeanName.str().c_str());
      controlGraphAllBaselineMean[iAnode-1]->SetName(controlGraphAllBaselineMeanName.str().c_str());

      // RMS on the mean of all baselines, upper limit
      std::stringstream controlGraphAllBaselineRMSName;
      controlGraphAllBaselineRMSName << "Anode" << iAnode << "AllBaselineRMS";
      double allBaselineRMSHighAmp[2] = {allAnodeBaselineMean[iAnode-1]+allAnodeBaselineRMS[iAnode-1], allAnodeBaselineMean[iAnode-1]+allAnodeBaselineRMS[iAnode-1]};
      controlGraphAllBaselineRMSHigh[iAnode-1] = new TGraph(2, allBaselineMeanId, allBaselineRMSHighAmp);
      controlGraphAllBaselineRMSName.str("");
      controlGraphAllBaselineRMSName << "controlGraphAnode" << iAnode << "AllBaselineRMS";
      controlGraphAllBaselineRMSHigh[iAnode-1]->SetTitle(controlGraphAllBaselineRMSName.str().c_str());
      controlGraphAllBaselineRMSHigh[iAnode-1]->SetName(controlGraphAllBaselineRMSName.str().c_str());

      // RMS on the mean of all baselines, lower limit
      controlGraphAllBaselineRMSName.str("");
      controlGraphAllBaselineRMSName << "Anode" << iAnode << "AllBaselineRMS";
      double allBaselineRMSLowAmp[2] = {allAnodeBaselineMean[iAnode-1]-allAnodeBaselineRMS[iAnode-1], allAnodeBaselineMean[iAnode-1]-allAnodeBaselineRMS[iAnode-1]};
      controlGraphAllBaselineRMSLow[iAnode-1] = new TGraph(2, allBaselineMeanId, allBaselineRMSLowAmp);
      controlGraphAllBaselineRMSName.str("");
      controlGraphAllBaselineRMSName << "controlGraphAnode" << iAnode << "AllBaselineRMS";
      controlGraphAllBaselineRMSLow[iAnode-1]->SetTitle(controlGraphAllBaselineRMSName.str().c_str());
      controlGraphAllBaselineRMSLow[iAnode-1]->SetName(controlGraphAllBaselineRMSName.str().c_str());


      // plotting
      // single anode canvases
      singleAnodePlots[iAnode-1]->cd();
      controlGraphBaseline[iAnode-1]->GetYaxis()->SetRangeUser(currentAnodeMin, currentAnodeMax);
      controlGraphBaseline[iAnode-1]->SetLineColor(1);
      controlGraphBaseline[iAnode-1]->SetMarkerColor(1);
      controlGraphBaseline[iAnode-1]->SetLineStyle(iAnode);
      controlGraphBaseline[iAnode-1]->SetMarkerStyle(20);
      controlGraphBaseline[iAnode-1]->Draw("AL");
      controlGraphMaxValue[iAnode-1]->SetLineColor(2);
      controlGraphMaxValue[iAnode-1]->SetMarkerColor(2);
      controlGraphMaxValue[iAnode-1]->SetLineStyle(iAnode);
      controlGraphMaxValue[iAnode-1]->SetMarkerStyle(21);
      controlGraphMaxValue[iAnode-1]->Draw("Lsame");
      controlGraphSparkCheck[iAnode-1]->SetLineColor(5);
      controlGraphSparkCheck[iAnode-1]->SetMarkerColor(5);
      controlGraphSparkCheck[iAnode-1]->SetLineStyle(iAnode);
      controlGraphSparkCheck[iAnode-1]->SetMarkerStyle(21);
      controlGraphSparkCheck[iAnode-1]->Draw("Psame");
      controlGraphTriggerThreshold[iAnode-1]->SetLineColor(3);
      controlGraphTriggerThreshold[iAnode-1]->SetMarkerColor(3);
      controlGraphTriggerThreshold[iAnode-1]->SetLineStyle(iAnode);
      controlGraphTriggerThreshold[iAnode-1]->SetMarkerStyle(22);
      controlGraphTriggerThreshold[iAnode-1]->Draw("Lsame");
      controlGraphBaselineOffset[iAnode-1]->SetLineColor(4);
      controlGraphBaselineOffset[iAnode-1]->SetMarkerColor(4);
      controlGraphBaselineOffset[iAnode-1]->SetLineStyle(iAnode);
      controlGraphBaselineOffset[iAnode-1]->SetMarkerStyle(23);
      controlGraphBaselineOffset[iAnode-1]->Draw("Lsame");
      
      controlGraphAllBaselineMean[iAnode-1]->SetLineColor(1);
      controlGraphAllBaselineMean[iAnode-1]->SetMarkerColor(1);
      controlGraphAllBaselineMean[iAnode-1]->SetLineStyle(6);
      controlGraphAllBaselineMean[iAnode-1]->SetMarkerStyle(20);
      controlGraphAllBaselineMean[iAnode-1]->Draw("Lsame");
      controlGraphAllBaselineRMSLow[iAnode-1]->SetLineColor(1);
      controlGraphAllBaselineRMSLow[iAnode-1]->SetMarkerColor(1);
      controlGraphAllBaselineRMSLow[iAnode-1]->SetLineStyle(9);
      controlGraphAllBaselineRMSLow[iAnode-1]->SetMarkerStyle(20);
      controlGraphAllBaselineRMSLow[iAnode-1]->Draw("Lsame");
      controlGraphAllBaselineRMSHigh[iAnode-1]->SetLineColor(1);
      controlGraphAllBaselineRMSHigh[iAnode-1]->SetMarkerColor(1);
      controlGraphAllBaselineRMSHigh[iAnode-1]->SetLineStyle(9);
      controlGraphAllBaselineRMSHigh[iAnode-1]->SetMarkerStyle(20);
      controlGraphAllBaselineRMSHigh[iAnode-1]->Draw("Lsame");


      legends[iAnode-1] = new TLegend(0.1,0.7,0.48,0.9);
      legends[iAnode-1]->AddEntry(controlGraphBaseline[iAnode-1], controlGraphBaselineName.str().c_str(), "lp");
      legends[iAnode-1]->AddEntry(controlGraphMaxValue[iAnode-1], controlGraphMaxValueName.str().c_str(), "lp");
      legends[iAnode-1]->AddEntry(controlGraphSparkCheck[iAnode-1], controlGraphSparkCheckName.str().c_str(), "p");
      legends[iAnode-1]->AddEntry(controlGraphTriggerThreshold[iAnode-1], controlGraphTriggerThresholdName.str().c_str(), "lp");
      legends[iAnode-1]->AddEntry(controlGraphBaselineOffset[iAnode-1], controlGraphBaselineOffsetName.str().c_str(), "lp");
      legends[iAnode-1]->AddEntry(controlGraphAllBaselineMean[iAnode-1], controlGraphAllBaselineMeanName.str().c_str(), "l");
      legends[iAnode-1]->AddEntry(controlGraphAllBaselineRMSLow[iAnode-1], controlGraphAllBaselineRMSName.str().c_str(), "l");
      legends[iAnode-1]->Draw("same");
      //singleAnodePlots[iAnode-1]->Update();

      // all anode canvases
      allAnodePlots->cd();
      // set the first draw option
      if(iAnode == 1) { controlGraphBaseline[iAnode-1]->Draw("AL"); }
      else if(iAnode == 2) { 
        if(controlGraphBaseline[iAnode-2]->GetN() > 1) { 
          controlGraphBaseline[iAnode-1]->Draw("Lsame");
        }
        else{ controlGraphBaseline[iAnode-1]->Draw("AL"); }
      }
      else if(iAnode == 3) { 
        if((controlGraphBaseline[iAnode-2]->GetN() > 1) || (controlGraphBaseline[iAnode-3]->GetN() > 1)) { 
          controlGraphBaseline[iAnode-1]->Draw("Lsame");
        }
        else{ controlGraphBaseline[iAnode-1]->Draw("AL"); }
      }
      // draw the rest
      controlGraphMaxValue[iAnode-1]->Draw("Lsame");
      controlGraphTriggerThreshold[iAnode-1]->Draw("Lsame");
      controlGraphBaselineOffset[iAnode-1]->Draw("Lsame");

      legendAllPlots->AddEntry(controlGraphBaseline[iAnode-1], controlGraphBaselineName.str().c_str(), "lp");
      legendAllPlots->AddEntry(controlGraphMaxValue[iAnode-1], controlGraphMaxValueName.str().c_str(), "lp");
      legendAllPlots->AddEntry(controlGraphSparkCheck[iAnode-1], controlGraphSparkCheckName.str().c_str(), "p");
      legendAllPlots->AddEntry(controlGraphTriggerThreshold[iAnode-1], controlGraphTriggerThresholdName.str().c_str(), "lp");
      legendAllPlots->AddEntry(controlGraphBaselineOffset[iAnode-1], controlGraphBaselineOffsetName.str().c_str(), "lp");
    }

    allAnodePlots->cd();
    legendAllPlots->Draw("same");


    // do the three correlation plots
    TCanvas *anode12correlations = new TCanvas("anode12correlations", "anode12correlations");
    std::string anode12correlationCuts = "";
    if( anodeChannelCuts[0] != "") { 
      anode12correlationCuts += anodeChannelCuts[0]; 
      if( anodeChannelCuts[1] != "" ) { anode12correlationCuts += " && " + anodeChannelCuts[1]; }
    }
    else if( anodeChannelCuts[1] != "" ) { anode12correlationCuts += anodeChannelCuts[1]; }
    anode12correlations->cd();
    chargeAnalysisTree->Draw("AmplitudeAnode1:AmplitudeAnode2", anode12correlationCuts.c_str(), "colz");

    TCanvas *anode23correlations = new TCanvas("anode23correlations", "anode23correlations");
    std::string anode23correlationCuts = "";
    if( anodeChannelCuts[1] != "") { 
      anode23correlationCuts += anodeChannelCuts[1]; 
      if( anodeChannelCuts[2] != "" ) { anode23correlationCuts += " && " + anodeChannelCuts[2]; }
    }
    else if( anodeChannelCuts[2] != "" ) { anode23correlationCuts += anodeChannelCuts[2]; }
    anode23correlations->cd();
    chargeAnalysisTree->Draw("AmplitudeAnode2:AmplitudeAnode3", anode23correlationCuts.c_str(), "colz");

    TCanvas *anode31correlations = new TCanvas("anode31correlations", "anode31correlations");
    std::string anode31correlationCuts = "";
    if( anodeChannelCuts[2] != "") { 
      anode31correlationCuts += anodeChannelCuts[2]; 
      if( anodeChannelCuts[0] != "" ) { anode31correlationCuts += " && " + anodeChannelCuts[0]; }
    }
    else if( anodeChannelCuts[0] != "" ) { anode31correlationCuts += anodeChannelCuts[0]; }
    anode31correlations->cd();
    chargeAnalysisTree->Draw("AmplitudeAnode3:AmplitudeAnode1", anode31correlationCuts.c_str(), "colz");
  }


  // close & clean up
  //inputFile->Close();


  // done
  return;
}

