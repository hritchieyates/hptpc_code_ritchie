{
  gROOT->ProcessLine(".L plotPeakPositionWithBigAxisLabels.C");
  gROOT->ProcessLine("plotPeakPositionNewAm241(\"SchemeBMod4\", true, \"Anode1\")");
  gROOT->ProcessLine("plotPeakPositionNewAm241(\"SchemeBMod4\", true, \"Anode2\")");
  gROOT->ProcessLine("plotPeakPositionNewAm241(\"SchemeBMod4\", true, \"\")");
  gROOT->ProcessLine("plotPeakPositionNewAm241(\"SchemeBMod4\", false, \"Anode1\")");
  gROOT->ProcessLine("plotPeakPositionNewAm241(\"SchemeBMod4\", false, \"Anode2\")");
  gROOT->ProcessLine("plotPeakPositionNewAm241(\"SchemeBMod4\", false, \"\")");
  gROOT->ProcessLine("plotPeakPositionNewAm241(\"SchemeCMod4\", true, \"Anode1\")");
  gROOT->ProcessLine("plotPeakPositionNewAm241(\"SchemeCMod4\", true, \"Anode2\")");
  gROOT->ProcessLine("plotPeakPositionNewAm241(\"SchemeCMod4\", true, \"\")");
  gROOT->ProcessLine("plotPeakPositionNewAm241(\"SchemeCMod4\", false, \"Anode1\")");
  gROOT->ProcessLine("plotPeakPositionNewAm241(\"SchemeCMod4\", false, \"Anode2\")");
  gROOT->ProcessLine("plotPeakPositionNewAm241(\"SchemeCMod4\", false, \"\")");
  gROOT->ProcessLine("plotPeakPositionNewAm241(\"SchemeA\", true, \"Anode1\")");
  gROOT->ProcessLine("plotPeakPositionNewAm241(\"SchemeA\", true, \"Anode2\")");
  gROOT->ProcessLine("plotPeakPositionNewAm241(\"SchemeA\", true, \"\")");
  gROOT->ProcessLine("plotPeakPositionNewAm241(\"SchemeA\", false, \"Anode1\")");
  gROOT->ProcessLine("plotPeakPositionNewAm241(\"SchemeA\", false, \"Anode2\")");
  gROOT->ProcessLine("plotPeakPositionNewAm241(\"SchemeA\", false, \"\")");
}
