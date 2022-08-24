#include "TCanvas.h"
#include "TGraphErrors.h"
#include<iostream>
#include<iterator>
#include<vector>
#include<string>
#include <TLegend.h>
#include <TAttLine.h>
#include "TMultiGraph.h"
#include "math.h"

using namespace std;
using namespace ROOT;
using namespace Math;

double luminosity = 59.74*1000;
double ZMumu = 0.036;



double *computeFilterEfficiency(double finalEvents[5], double initialCrossSectionN2C1[5]){	
	static double filterEfficiency[5];
	for (int i=0; i<=4; i++){
		filterEfficiency[i] = 100 * finalEvents[i]/((initialCrossSectionN2C1[i])*luminosity);
	}
	return filterEfficiency;
}


double *computeFilterEfficiencyError(double finalEvents[5], double initialCrossSectionN2C1[5], double nTupleEvents[5]){
	static double filterEfficiencyError[5];	
	for (int i=0; i<=4; i++){
		double numEventsExpected = (initialCrossSectionN2C1[i])*luminosity;
		double efficiency = finalEvents[i]/numEventsExpected;
		filterEfficiencyError[i] = 100 * sqrt(efficiency * (1 - efficiency)/nTupleEvents[i]);	
	}
	return filterEfficiencyError;
}


void plotSignalEvents_Wino(std::string overlay, std::string outputDir){
	// data from integrating signal regions, hard coded from results for now
	// organized by lifetime for a given N1 mass

	double massSplittings[5] = {5, 10, 20, 30, 40};
	double massSplittings_err[5] = {0, 0, 0, 0, 0};
	
	//total events after reco-level cuts
	double sample_125N2_1mm[5] = {0.112, 0.480, 0.673, 0.674, 0.400}; 
	double sample_125N2_1mm_err[5] = {0.056, 0.113, 0.135, 0.132, 0.103}; 
	double sample_125N2_10mm[5] = {0.799, 5.528, 5.955, 4.577, 2.270}; 
	double sample_125N2_10mm_err[5] = {0.146, 0.385, 0.379, 0.347, 0.245}; 
	double sample_125N2_100mm[5] = {0.960, 6.089, 6.430, 5.035, 2.977}; 
	double sample_125N2_100mm_err[5] = {0.160, 0.403, 0.396, 0.365, 0.280}; 
	double sample_125N2_1000mm[5] = {0.133, 1.307, 1.422, 0.825, 0.520}; 
	double sample_125N2_1000mm_err[5] = {0.059, 0.187, 0.184, 0.146, 0.116}; 
	double sample_150N2_1mm[5] = {0.016, 0.243, 0.502, 0.405, 0.288}; 
	double sample_150N2_1mm_err[5] = {0.016, 0.061, 0.087, 0.078, 0.066}; 
	double sample_150N2_10mm[5] = {0.549, 3.247, 4.242, 2.683, 2.099}; 
	double sample_150N2_10mm_err[5] = {0.091, 0.223, 0.253, 0.201, 0.178}; 
	double sample_150N2_100mm[5] = {0.581, 3.284, 4.604, 3.132, 3.132}; 
	double sample_150N2_100mm_err[5] = {0.094, 0.224, 0.265, 0.217, 0.217}; 
	double sample_150N2_1000mm[5] = {0.202, 0.702, 0.841, 0.587, 0.423}; 
	double sample_150N2_1000mm_err[5] = {0.056, 0.103, 0.112, 0.094, 0.080};


	// events in signal region (events before 3 GeV are cut)	
        double sampleTotal_125N2_1mm[5] = {0.245, 0.586, 0.673, 0.674, 0.400};
        double sampleTotal_125N2_1mm_err[5] = {0.082, 0.125, 0.135, 0.103};
        double sampleTotal_125N2_10mm[5] = {1.604, 6.162, 6.127, 4.711, 2.296};
        double sampleTotal_125N2_10mm_err[5] = {0.207, 0.405, 0.385, 0.352, 0.246};
        double sampleTotal_125N2_100mm[5] = {1.856, 6.667, 6.651, 5.118, 3.001};
        double sampleTotal_125N2_100mm_err[5] = {0.224, 0.421, 0.403, 0.281};
        double sampleTotal_125N2_1000mm[5] = {0.266, 1.467, 1.472, 0.851, 0.520};
        double sampleTotal_125N2_1000mm_err[5] = {0.084, 0.198, 0.187, 0.148, 0.116};
        double sampleTotal_150N2_1mm[5] = {0.078, 0.351, 0.501, 0.420, 0.288};
        double sampleTotal_150N2_1mm_err[5] = {0.035, 0.073, 0.087, 0.080, 0.066};
        double sampleTotal_150N2_10mm[5] = {1.022, 3.721, 4.441, 2.745, 2.090};
        double sampleTotal_150N2_10mm_err[5] = {0.125, 0.238, 0.259, 0.204, 0.178};
        double sampleTotal_150N2_100mm[5] = {1.041, 3.732, 4.789, 3.194, 2.129};
        double sampleTotal_150N2_100mm_err[5] = {0.126, 0.239, 0.270, 0.219, 0.179};
        double sampleTotal_150N2_1000mm[5] = {0.232, 0.765, 0.899, 0.632, 0.423};
        double sampleTotal_150N2_1000mm_err[5] = {0.060, 0.108, 0.116, 0.098, 0.080};


	// cross-sections/filter efficiency values
	/*
	double theoryCrossSection_125N2_N2C1p[4] = {2.651, 2.081, 1.659, 1.341};
	double theoryCrossSection_125N2_N2C1p_relErr[4] = {0.02327, 0.02425, 0.02520, 0.02615};
	double theoryCrossSection_125N2_N2C1n[4] = {1.644, 1.274, 1.004, 0.802};
	double theoryCrossSection_125N2_N2C1_relErr[4] = {0.04410, 0.04529, 0.04630, 0.04728};
	double theoryCrossSection_125N2_N2N1[4] = {2.726, 2.271, 1.905, 1.608};
	double theoryCrossSection_125N2_N2N1_relErr[4] = {0.02743, 0.02814, 0.02896, 0.02944};

	double theoryCrossSection_150N2_N2C1p[4] = {1.259, 1.035, 0.860, 0.720};
	double theoryCrossSection_150N2_N2C1p_relErr[4] = {0.02644, 0.02736, 0.02827, 0.02917};
	double theoryCrossSection_150N2_N2C1n[4] = {0.750, 0.610, 0.500, 0.415};
	double theoryCrossSection_150N2_N2C1_relErr[4] = {0.04765, 0.04863, 0.04957, 0.05049};
	double theoryCrossSection_150N2_N2N1[4] = {1.220, 1.056, 0.918, 0.800};
	double theoryCrossSection_150N2_N2N1_relErr[4] = {0.03099, 0.03165, 0.03230, 0.03249};
	*/
	double postCutCrossSection_125N2_N2C1_1mm[5] = {0.0190510659857, 0.0186540421757, 0.0175171563932, 0.0148990274419, 0.0154697491688};
	double postCutCrossSection_125N2_N2C1_10mm[5] = {0.0190667646312, 0.0186302410034, 0.0175166499852, 0.0165519428959, 0.01546468508960};
	double postCutCrossSection_125N2_N2C1_100mm[5] = {0.0191548796095, 0.018645433241, 0.017473098904, 0.0163812834265, 0.0153335254381};
	double postCutCrossSection_125N2_N2C1_1000mm[5] = {0.0191361425164, 0.0184909788251, 0.017184952797, 0.0160971885828, 0.0142498124873};

	double postCutCrossSection_150N2_N2C1_1mm[5] = {0.0106946555275, 0.0106450626732, 0.0102184481196, 0.00980836451734, 0.00947049507137};
	double postCutCrossSection_150N2_N2C1_10mm[5] = {0.0107509187657, 0.0106601435412, 0.0101856762334, 0.00980488431703, 0.00902183924912};
	double postCutCrossSection_150N2_N2C1_100mm[5] = {0.0107491786656, 0.0106900152605, 0.0101952467842, 0.00975877166303, 0.00941423183315};
	double postCutCrossSection_150N2_N2C1_1000mm[5] = {0.0107317776641, 0.0106473828068, 0.01010157139280, 0.0095754811138, 0.00923703163445};

	double ntuple_125N2_N2C1_1mm[5] = {37620, 36836, 34591, 29421, 30548};
	double ntuple_125N2_N2C1_10mm[5] = {37651, 36789, 34590, 32685, 30538};
	double ntuple_125N2_N2C1_100mm[5] = {37825, 36819, 34504, 32348, 30279};
	double ntuple_125N2_N2C1_1000mm[5] = {37788, 36514, 33935, 31787, 28139}; 
	
	double ntuple_150N2_N2C1_1mm[5] = {36876, 36705, 35234, 33820, 32655};
	double ntuple_150N2_N2C1_10mm[5] = {37070, 36757, 35121, 33808, 31108};
	double ntuple_150N2_N2C1_100mm[5] = {37064, 36860, 35154, 33649, 32461};
	double ntuple_150N2_N2C1_1000mm[5] = {37004, 36713, 34831, 33017, 31850};
	
	double filterEfficiency_125N2_1mm[5];
	memcpy(filterEfficiency_125N2_1mm, computeFilterEfficiency(sample_125N2_1mm, postCutCrossSection_125N2_N2C1_1mm), sizeof(filterEfficiency_125N2_1mm)); 
	double filterEfficiency_125N2_1mm_err[5];
	memcpy(filterEfficiency_125N2_1mm_err, computeFilterEfficiencyError(sample_125N2_1mm, postCutCrossSection_125N2_N2C1_1mm, ntuple_125N2_N2C1_1mm), sizeof(filterEfficiency_125N2_1mm_err)); 
	double filterEfficiency_125N2_10mm[5];
	memcpy(filterEfficiency_125N2_10mm, computeFilterEfficiency(sample_125N2_10mm, postCutCrossSection_125N2_N2C1_10mm), sizeof(filterEfficiency_125N2_10mm)); 
	double filterEfficiency_125N2_10mm_err[5];
	memcpy(filterEfficiency_125N2_10mm_err, computeFilterEfficiencyError(sample_125N2_10mm, postCutCrossSection_125N2_N2C1_10mm, ntuple_125N2_N2C1_10mm), sizeof(filterEfficiency_125N2_10mm_err)); 
	double filterEfficiency_125N2_100mm[5];
	memcpy(filterEfficiency_125N2_100mm, computeFilterEfficiency(sample_125N2_100mm, postCutCrossSection_125N2_N2C1_100mm), sizeof(filterEfficiency_125N2_100mm)); 
	double filterEfficiency_125N2_100mm_err[5];
	memcpy(filterEfficiency_125N2_100mm_err, computeFilterEfficiencyError(sample_125N2_100mm, postCutCrossSection_125N2_N2C1_100mm, ntuple_125N2_N2C1_100mm), sizeof(filterEfficiency_125N2_100mm_err)); 
	double filterEfficiency_125N2_1000mm[5];
	memcpy(filterEfficiency_125N2_1000mm, computeFilterEfficiency(sample_125N2_1000mm, postCutCrossSection_125N2_N2C1_1000mm), sizeof(filterEfficiency_125N2_1000mm)); 
	double filterEfficiency_125N2_1000mm_err[5];
	memcpy(filterEfficiency_125N2_1000mm_err, computeFilterEfficiencyError(sample_125N2_1000mm, postCutCrossSection_125N2_N2C1_1000mm, ntuple_125N2_N2C1_1000mm), sizeof(filterEfficiency_125N2_1000mm_err)); 
	double filterEfficiency_150N2_1mm[5];
	memcpy(filterEfficiency_150N2_1mm, computeFilterEfficiency(sample_150N2_1mm, postCutCrossSection_150N2_N2C1_1mm), sizeof(filterEfficiency_150N2_1mm)); 
	double filterEfficiency_150N2_1mm_err[5];
	memcpy(filterEfficiency_150N2_1mm_err, computeFilterEfficiencyError(sample_150N2_1mm, postCutCrossSection_150N2_N2C1_1mm, ntuple_150N2_N2C1_1mm), sizeof(filterEfficiency_150N2_1mm_err)); 
	double filterEfficiency_150N2_10mm[5];
	memcpy(filterEfficiency_150N2_10mm, computeFilterEfficiency(sample_150N2_10mm, postCutCrossSection_150N2_N2C1_10mm), sizeof(filterEfficiency_150N2_10mm)); 
	double filterEfficiency_150N2_10mm_err[5];
	memcpy(filterEfficiency_150N2_10mm_err, computeFilterEfficiencyError(sample_150N2_10mm, postCutCrossSection_150N2_N2C1_10mm, ntuple_150N2_N2C1_10mm), sizeof(filterEfficiency_150N2_10mm_err)); 
	double filterEfficiency_150N2_100mm[5];
	memcpy(filterEfficiency_150N2_100mm, computeFilterEfficiency(sample_150N2_100mm, postCutCrossSection_150N2_N2C1_100mm), sizeof(filterEfficiency_150N2_100mm)); 
	double filterEfficiency_150N2_100mm_err[5];
	memcpy(filterEfficiency_150N2_100mm_err, computeFilterEfficiencyError(sample_150N2_100mm, postCutCrossSection_150N2_N2C1_100mm, ntuple_150N2_N2C1_100mm), sizeof(filterEfficiency_150N2_100mm_err)); 
	double filterEfficiency_150N2_1000mm[5];
	memcpy(filterEfficiency_150N2_1000mm, computeFilterEfficiency(sample_150N2_1000mm, postCutCrossSection_150N2_N2C1_1000mm), sizeof(filterEfficiency_150N2_1000mm)); 
	double filterEfficiency_150N2_1000mm_err[5];
	memcpy(filterEfficiency_150N2_1000mm_err, computeFilterEfficiencyError(sample_150N2_1000mm, postCutCrossSection_150N2_N2C1_1000mm, ntuple_150N2_N2C1_1000mm), sizeof(filterEfficiency_125N2_1000mm_err)); 
	
	TCanvas *C = new TCanvas("C", "C");
	C->SetGrid();
	

	TGraphErrors *gr125_1 = new TGraphErrors(5, massSplittings, sample_125N2_1mm, massSplittings_err, sample_125N2_1mm_err); gr125_1->SetMarkerColor(kBlue); gr125_1->SetLineColor(kBlue); gr125_1->SetMarkerStyle(20);  
	TGraphErrors *gr125_10 = new TGraphErrors(5, massSplittings, sample_125N2_10mm, massSplittings_err, sample_125N2_10mm_err); gr125_10->SetMarkerColor(kRed); gr125_10->SetMarkerStyle(20); gr125_10->SetLineColor(kRed);
	TGraphErrors *gr125_100 = new TGraphErrors(5, massSplittings, sample_125N2_100mm, massSplittings_err, sample_125N2_100mm_err); gr125_100->SetMarkerColor(kViolet); gr125_100->SetMarkerStyle(20); gr125_100->SetLineColor(kViolet);
	TGraphErrors *gr125_1000 = new TGraphErrors(5, massSplittings, sample_125N2_1000mm, massSplittings_err, sample_125N2_1000mm_err); gr125_1000->SetMarkerColor(kBlack); gr125_1000->SetMarkerStyle(20); gr125_1000->SetLineColor(kBlack);
	TGraphErrors *gr150_1 = new TGraphErrors(5, massSplittings, sample_150N2_1mm, massSplittings_err, sample_150N2_1mm_err); gr150_1->SetMarkerColor(kBlue); gr150_1->SetMarkerStyle(20); gr150_1->SetLineColor(kBlue);
	TGraphErrors *gr150_10 = new TGraphErrors(5, massSplittings, sample_150N2_10mm, massSplittings_err, sample_150N2_10mm_err); gr150_10->SetMarkerColor(kRed); gr150_10->SetMarkerStyle(20); gr150_10->SetLineColor(kRed);
	TGraphErrors *gr150_100 = new TGraphErrors(5, massSplittings, sample_150N2_100mm, massSplittings_err, sample_150N2_100mm_err); gr150_100->SetMarkerColor(kViolet); gr150_100->SetMarkerStyle(20); gr150_100->SetLineColor(kViolet);
	TGraphErrors *gr150_1000 = new TGraphErrors(5, massSplittings, sample_150N2_1000mm, massSplittings_err, sample_150N2_1000mm_err); gr150_1000->SetMarkerColor(kBlack); gr150_1000->SetMarkerStyle(20); gr150_1000->SetLineColor(kBlack);
	
	TGraphErrors *gr125_1_total = new TGraphErrors(5, massSplittings, sampleTotal_125N2_1mm, massSplittings_err, sampleTotal_125N2_1mm_err); gr125_1_total->SetMarkerColor(kBlue); gr125_1_total->SetMarkerStyle(20); gr125_1_total->SetLineColor(kBlue); 
	TGraphErrors *gr125_10_total = new TGraphErrors(5, massSplittings, sampleTotal_125N2_10mm, massSplittings_err, sampleTotal_125N2_10mm_err); gr125_10_total->SetMarkerColor(kRed); gr125_10_total->SetMarkerStyle(20); gr125_10_total->SetLineColor(kRed);
	TGraphErrors *gr125_100_total = new TGraphErrors(5, massSplittings, sampleTotal_125N2_100mm, massSplittings_err, sampleTotal_125N2_100mm_err); gr125_100_total->SetMarkerColor(kViolet); gr125_100_total->SetMarkerStyle(20); gr125_100_total->SetLineColor(kViolet);
	TGraphErrors *gr125_1000_total = new TGraphErrors(5, massSplittings, sampleTotal_125N2_1000mm, massSplittings_err, sampleTotal_125N2_1000mm_err); gr125_1000_total->SetMarkerColor(kBlack); gr125_1000_total->SetMarkerStyle(20); gr125_1000_total->SetLineColor(kBlack);
	TGraphErrors *gr150_1_total = new TGraphErrors(5, massSplittings, sampleTotal_150N2_1mm, massSplittings_err, sampleTotal_150N2_1mm_err); gr150_1_total->SetMarkerColor(kBlue); gr150_1_total->SetMarkerStyle(20); gr150_1_total->SetLineColor(kBlue);
	TGraphErrors *gr150_10_total = new TGraphErrors(5, massSplittings, sampleTotal_150N2_10mm, massSplittings_err, sampleTotal_150N2_10mm_err); gr150_10_total->SetMarkerColor(kRed); gr150_10_total->SetMarkerStyle(20); gr150_10_total->SetLineColor(kRed);
	TGraphErrors *gr150_100_total = new TGraphErrors(5, massSplittings, sampleTotal_150N2_100mm, massSplittings_err, sampleTotal_150N2_100mm_err); gr150_100_total->SetMarkerColor(kViolet); gr150_100_total->SetMarkerStyle(20); gr150_100_total->SetLineColor(kViolet);
	TGraphErrors *gr150_1000_total = new TGraphErrors(5, massSplittings, sampleTotal_150N2_1000mm, massSplittings_err, sampleTotal_150N2_1000mm_err); gr150_1000_total->SetMarkerColor(kBlack); gr150_1000_total->SetMarkerStyle(20); gr150_1000_total->SetLineColor(kBlack);
	

	TGraphErrors *gr125_1_filtereff = new TGraphErrors(5, massSplittings, filterEfficiency_125N2_1mm, massSplittings_err, filterEfficiency_125N2_1mm_err); gr125_1_filtereff->SetMarkerColor(kBlue); gr125_1_filtereff->SetMarkerStyle(20); gr125_1_filtereff->SetLineColor(kBlue); 
	TGraphErrors *gr125_10_filtereff = new TGraphErrors(5, massSplittings, filterEfficiency_125N2_10mm, massSplittings_err, filterEfficiency_125N2_10mm_err); gr125_10_filtereff->SetMarkerColor(kRed); gr125_10_filtereff->SetMarkerStyle(20); gr125_10_filtereff->SetLineColor(kRed);
	TGraphErrors *gr125_100_filtereff = new TGraphErrors(5, massSplittings, filterEfficiency_125N2_100mm, massSplittings_err, filterEfficiency_125N2_100mm_err); gr125_100_filtereff->SetMarkerColor(kViolet); gr125_100_filtereff->SetMarkerStyle(20); gr125_100_filtereff->SetLineColor(kViolet);
	TGraphErrors *gr125_1000_filtereff = new TGraphErrors(5, massSplittings, filterEfficiency_125N2_1000mm, massSplittings_err, filterEfficiency_125N2_1000mm_err); gr125_1000_filtereff->SetMarkerColor(kBlack); gr125_1000_filtereff->SetMarkerStyle(20); gr125_1000_filtereff->SetLineColor(kBlack);
	TGraphErrors *gr150_1_filtereff = new TGraphErrors(5, massSplittings, filterEfficiency_150N2_1mm, massSplittings_err, filterEfficiency_150N2_1mm_err); gr150_1_filtereff->SetMarkerColor(kBlue); gr150_1_filtereff->SetMarkerStyle(20); gr150_1_filtereff->SetLineColor(kBlue);
	TGraphErrors *gr150_10_filtereff = new TGraphErrors(5, massSplittings, filterEfficiency_150N2_10mm, massSplittings_err, filterEfficiency_150N2_10mm_err); gr150_10_filtereff->SetMarkerColor(kRed); gr150_10_filtereff->SetMarkerStyle(20); gr150_10_filtereff->SetLineColor(kRed);
	TGraphErrors *gr150_100_filtereff = new TGraphErrors(5, massSplittings, filterEfficiency_150N2_100mm, massSplittings_err, filterEfficiency_150N2_100mm_err); gr150_100_filtereff->SetMarkerColor(kViolet); gr150_100_filtereff->SetMarkerStyle(20); gr150_100_filtereff->SetLineColor(kViolet);
	TGraphErrors *gr150_1000_filtereff = new TGraphErrors(5, massSplittings, filterEfficiency_150N2_1000mm, massSplittings_err, filterEfficiency_150N2_1000mm_err); gr150_1000_filtereff->SetMarkerColor(kBlack); gr150_1000_filtereff->SetMarkerStyle(20); gr150_1000_filtereff->SetLineColor(kBlack);
	
	if (overlay=="lifetime" || overlay=="Lifetime" || overlay=="lifetimes" || overlay=="Lifetimes"){
		TMultiGraph *N2_125GeV = new TMultiGraph("N2_125GeV", "N2_125GeV");
                N2_125GeV->Add(gr125_1);
                N2_125GeV->Add(gr125_10);
                N2_125GeV->Add(gr125_100);
                N2_125GeV->Add(gr125_1000);
                N2_125GeV->Draw("AP");
                N2_125GeV->SetTitle("Events in Signal Region vs Mass Splitting for 125 GeV Neutralino 2");
                N2_125GeV->GetXaxis()->SetTitle("N2/N1 Mass Splitting [GeV]");
                N2_125GeV->GetYaxis()->SetTitle("Number of Events in Signal Region");
		TLegend *leg2 = new TLegend(0.75, 0.75, 0.9, 0.9);
                leg2->AddEntry(gr125_1, "1 mm ctau", "lp");
                leg2->AddEntry(gr125_10, "10 mm ctau", "lp");
                leg2->AddEntry(gr125_100, "100 mm ctau", "lp");
                leg2->AddEntry(gr125_1000, "1000 mm ctau", "lp");
                leg2->Draw();
                C->Update();
                C->Print((outputDir+"/N2_125GeV.png").c_str());


		TMultiGraph *N2_150GeV = new TMultiGraph("N2_150GeV", "N2_150GeV");
		N2_150GeV->Add(gr150_1);
		N2_150GeV->Add(gr150_10);
		N2_150GeV->Add(gr150_100);
		N2_150GeV->Add(gr150_1000);
		N2_150GeV->Draw("AP");
		N2_150GeV->SetTitle("Events in Signal Region vs Mass Splitting for 150 GeV Neutralino 2");
		N2_150GeV->GetXaxis()->SetTitle("N2/N1 Mass Splitting [GeV]");
		N2_150GeV->GetYaxis()->SetTitle("Number of Events in Signal Region");
		TLegend *leg = new TLegend(0.75, 0.75, 0.9, 0.9);
		leg->AddEntry(gr150_1, "1 mm ctau", "lp");
		leg->AddEntry(gr150_10, "10 mm ctau", "lp");
		leg->AddEntry(gr150_100, "100 mm ctau", "lp");
		leg->AddEntry(gr150_1000, "1000 mm ctau", "lp");
		leg->Draw();
		C->Update();
		C->Print((outputDir+"/N2_150GeV.png").c_str());


		TMultiGraph *N2_125GeVTot = new TMultiGraph("N2_125GeVTot", "N2_125GeVTot");
                N2_125GeVTot->Add(gr125_1_total);
                N2_125GeVTot->Add(gr125_10_total);
                N2_125GeVTot->Add(gr125_100_total);
                N2_125GeVTot->Add(gr125_1000_total);
                N2_125GeVTot->Draw("AP");
                N2_125GeVTot->SetTitle("Total Events Expected vs Mass Splitting for 125 GeV Neutralino 2");
                N2_125GeVTot->GetXaxis()->SetTitle("N2/N1 Mass Splitting [GeV]");
                N2_125GeVTot->GetYaxis()->SetTitle("Events");
		TLegend *leg2a = new TLegend(0.75, 0.75, 0.9, 0.9);
                leg2a->AddEntry(gr125_1_total, "1 mm ctau", "lp");
                leg2a->AddEntry(gr125_10_total, "10 mm ctau", "lp");
                leg2a->AddEntry(gr125_100_total, "100 mm ctau", "lp");
                leg2a->AddEntry(gr125_1000_total, "1000 mm ctau", "lp");
                leg2a->Draw();
                C->Update();
                C->Print((outputDir+"/N2_125GeVTot.png").c_str());

		TMultiGraph *N2_150GeVTot = new TMultiGraph("N2_150GeVTot", "N2_150GeVTot");
                N2_150GeVTot->Add(gr150_1_total);
                N2_150GeVTot->Add(gr150_10_total);
                N2_150GeVTot->Add(gr150_100_total);
                N2_150GeVTot->Add(gr150_1000_total);
                N2_150GeVTot->Draw("AP");
                N2_150GeVTot->SetTitle("Total Events Expected vs Mass Splitting for 150 GeV Neutralino 2");
                N2_150GeVTot->GetXaxis()->SetTitle("N2/N1 Mass Splitting [GeV]");
                N2_150GeVTot->GetYaxis()->SetTitle("Events");
		TLegend *leg2b = new TLegend(0.75, 0.75, 0.9, 0.9);
                leg2b->AddEntry(gr150_1_total, "1 mm ctau", "lp");
                leg2b->AddEntry(gr150_10_total, "10 mm ctau", "lp");
                leg2b->AddEntry(gr150_100_total, "100 mm ctau", "lp");
                leg2b->AddEntry(gr150_1000_total, "1000 mm ctau", "lp");
                leg2b->Draw();
                C->Update();
                C->Print((outputDir+"/N2_150GeVTot.png").c_str());

		TMultiGraph *N2_125GeVFilter = new TMultiGraph("N2_125GeVFilter", "N2_125GeVFilter");
                N2_125GeVFilter->Add(gr125_1_filtereff);
                N2_125GeVFilter->Add(gr125_10_filtereff);
                N2_125GeVFilter->Add(gr125_100_filtereff);
                N2_125GeVFilter->Add(gr125_1000_filtereff);
                N2_125GeVFilter->Draw("AP");
                N2_125GeVFilter->SetTitle("Reco-Level Filter Efficiency vs Mass Splitting for 125 GeV Neutralino 2");
                N2_125GeVFilter->GetXaxis()->SetTitle("N2/N1 Mass Splitting [GeV]");
                N2_125GeVFilter->GetYaxis()->SetTitle("Filter Efficiency (%)");
		TLegend *leg2c = new TLegend(0.75, 0.75, 0.9, 0.9);
                leg2c->AddEntry(gr125_1_filtereff, "1 mm ctau", "lp");
                leg2c->AddEntry(gr125_10_filtereff, "10 mm ctau", "lp");
                leg2c->AddEntry(gr125_100_filtereff, "100 mm ctau", "lp");
                leg2c->AddEntry(gr125_1000_filtereff, "1000 mm ctau", "lp");
                leg2c->Draw();
                C->Update();
                C->Print((outputDir+"/N2_125GeVFilter.png").c_str());

		TMultiGraph *N2_150GeVFilter = new TMultiGraph("N2_125GeV", "N2_125GeV");
                N2_150GeVFilter->Add(gr150_1_filtereff);
                N2_150GeVFilter->Add(gr150_10_filtereff);
                N2_150GeVFilter->Add(gr150_100_filtereff);
                N2_150GeVFilter->Add(gr150_1000_filtereff);
                N2_150GeVFilter->Draw("AP");
                N2_150GeVFilter->SetTitle("Reco-Level Filter Efficiency vs Mass Splitting for 150 GeV Neutralino 2");
                N2_150GeVFilter->GetXaxis()->SetTitle("N2/N1 Mass Splitting [GeV]");
                N2_150GeVFilter->GetYaxis()->SetTitle("Filter Efficiency (%)");
		TLegend *leg2d = new TLegend(0.75, 0.75, 0.9, 0.9);
                leg2d->AddEntry(gr150_1_filtereff, "1 mm ctau", "lp");
                leg2d->AddEntry(gr150_10_filtereff, "10 mm ctau", "lp");
                leg2d->AddEntry(gr150_100_filtereff, "100 mm ctau", "lp");
                leg2d->AddEntry(gr150_1000_filtereff, "1000 mm ctau", "lp");
                leg2d->Draw();
                C->Update();
                C->Print((outputDir+"/N2_150GeVFilter.png").c_str());


	}

	else{

		//1mm
		TMultiGraph *ctau_1 = new TMultiGraph("ctau_1", "ctau_1");
		gr125_1->SetMarkerColor(kBlue); gr125_1->SetLineColor(kBlue);
		ctau_1->Add(gr125_1);
		gr150_1->SetMarkerColor(kRed); gr150_1->SetLineColor(kRed);
		ctau_1->Add(gr150_1);	
		ctau_1->Draw("AP");
		ctau_1->SetTitle("Events in Signal Region vs Mass Splitting for 1 mm ctau");
		ctau_1->GetXaxis()->SetTitle("N2/N1 Mass Splitting [GeV]");
		ctau_1->GetYaxis()->SetTitle("Number of Events in Signal Region");
		TLegend *legc1 = new TLegend(0.75, 0.75, 0.9, 0.9);
		legc1->AddEntry(gr125_1, "125 GeV N2", "lp");
		legc1->AddEntry(gr150_1, "150 GeV N2", "lp");
		legc1->Draw();
		C->Update();
		C->Print((outputDir+"/ctau_1.png").c_str());
		

		//10mm
		TMultiGraph *ctau_10 = new TMultiGraph("ctau_10", "ctau_10");
		gr125_10->SetMarkerColor(kBlue); gr125_10->SetLineColor(kBlue);
		ctau_10->Add(gr125_10);
		gr150_10->SetMarkerColor(kRed); gr150_10->SetLineColor(kRed);
		ctau_10->Add(gr150_10);	
		ctau_10->Draw("AP");
		ctau_10->SetTitle("Events in Signal Region vs Mass Splitting for 10 mm N2C1 ctau");
		ctau_10->GetXaxis()->SetTitle("N2/N1 Mass Splitting [GeV]");
		ctau_10->GetYaxis()->SetTitle("Number of Events in Signal Region");
		TLegend *legc10 = new TLegend(0.75, 0.75, 0.9, 0.9);
		legc10->AddEntry(gr125_10, "125 GeV N2", "lp");
		legc10->AddEntry(gr150_10, "150 GeV N2", "lp");
		legc10->Draw();
		C->Update();
		C->Print((outputDir+"/ctau_10.png").c_str());
		
		//100mm
		TMultiGraph *ctau_100 = new TMultiGraph("ctau_100", "ctau_100");
		gr125_100->SetMarkerColor(kBlue); gr125_100->SetLineColor(kBlue);
		ctau_100->Add(gr125_100);
		gr150_100->SetMarkerColor(kRed); gr150_100->SetLineColor(kRed);
		ctau_100->Add(gr150_100);	
		ctau_100->Draw("AP");
		ctau_100->SetTitle("Events in Signal Region vs Mass Splitting for 100 mm N2C1 ctau");
		ctau_100->GetXaxis()->SetTitle("N2/N1 Mass Splitting [GeV]");
		ctau_100->GetYaxis()->SetTitle("Number of Events in Signal Region");
		TLegend *legc100 = new TLegend(0.75, 0.75, 0.9, 0.9);
		legc100->AddEntry(gr125_100, "125 GeV N2", "lp");
		legc100->AddEntry(gr150_100, "150 GeV N2", "lp");
		legc100->Draw();
		C->Update();
		C->Print((outputDir+"/ctau_100.png").c_str());
	
		//1000mm	
		TMultiGraph *ctau_1000 = new TMultiGraph("ctau_1000", "ctau_1000");
		gr125_1000->SetMarkerColor(kBlue); gr125_1000->SetLineColor(kBlue);
		ctau_1000->Add(gr125_1000);
		gr150_1000->SetMarkerColor(kRed); gr150_1000->SetLineColor(kRed);
		ctau_1000->Add(gr150_1000);	
		ctau_1000->Draw("AP");
		ctau_1000->SetTitle("Events in Signal Region vs Mass Splitting for 1000 mm N2C1 ctau");
		ctau_1000->GetXaxis()->SetTitle("N2/N1 Mass Splitting [GeV]");
		ctau_1000->GetYaxis()->SetTitle("Number of Events in Signal Region");
		TLegend *legc1000 = new TLegend(0.75, 0.75, 0.9, 0.9);
		legc1000->AddEntry(gr125_1000, "125 GeV N2", "lp");
		legc1000->AddEntry(gr150_1000, "150 GeV N2", "lp");
		legc1000->Draw();
		C->Update();
		C->Print((outputDir+"/ctau_1000.png").c_str());
		
		//1mm
		TMultiGraph *ctau_1_total = new TMultiGraph("ctau_1_total", "ctau_1_total");
		gr125_1_total->SetMarkerColor(kBlue); gr125_1_total->SetLineColor(kBlue);
		ctau_1_total->Add(gr125_1_total);
		gr150_1_total->SetMarkerColor(kRed); gr150_1_total->SetLineColor(kRed);
		ctau_1_total->Add(gr150_1_total);	
		ctau_1_total->Draw("AP");
		ctau_1_total->SetTitle("Total Events Expected vs Mass Splitting for 1 mm ctau");
		ctau_1_total->GetXaxis()->SetTitle("N2/N1 Mass Splitting [GeV]");
		ctau_1_total->GetYaxis()->SetTitle("Events");
		TLegend *legc1a = new TLegend(0.75, 0.75, 0.9, 0.9);
		legc1a->AddEntry(gr125_1_total, "125 GeV N2", "lp");
		legc1a->AddEntry(gr150_1_total, "150 GeV N2", "lp");
		legc1a->Draw();
		C->Update();
		C->Print((outputDir+"/ctau_1_total.png").c_str());
		

		//10mm
		TMultiGraph *ctau_10_total = new TMultiGraph("ctau_10_total", "ctau_10_total");
		gr125_10_total->SetMarkerColor(kBlue); gr125_10_total->SetLineColor(kBlue);
		ctau_10_total->Add(gr125_10_total);
		gr150_10_total->SetMarkerColor(kRed); gr150_10_total->SetLineColor(kRed);
		ctau_10_total->Add(gr150_10_total);	
		ctau_10_total->Draw("AP");
		ctau_10_total->SetTitle("Total Events Expected vs Mass Splitting for 10 mm N2C1 ctau");
		ctau_10_total->GetXaxis()->SetTitle("N2/N1 Mass Splitting [GeV]");
		ctau_10_total->GetYaxis()->SetTitle("Events");
		TLegend *legc10a = new TLegend(0.75, 0.75, 0.9, 0.9);
		legc10a->AddEntry(gr125_10_total, "125 GeV N2", "lp");
		legc10a->AddEntry(gr150_10_total, "150 GeV N2", "lp");
		legc10a->Draw();
		C->Update();
		C->Print((outputDir+"/ctau_10_total.png").c_str());
		
		//100mm
		TMultiGraph *ctau_100_total = new TMultiGraph("ctau_100_total", "ctau_100_total");
		gr125_100_total->SetMarkerColor(kBlue); gr125_100_total->SetLineColor(kBlue);
		ctau_100_total->Add(gr125_100_total);
		gr150_100_total->SetMarkerColor(kRed); gr150_100_total->SetLineColor(kRed);
		ctau_100_total->Add(gr150_100_total);	
		ctau_100_total->Draw("AP");
		ctau_100_total->SetTitle("Total Events Expected vs Mass Splitting for 100 mm N2C1 ctau");
		ctau_100_total->GetXaxis()->SetTitle("N2/N1 Mass Splitting [GeV]");
		ctau_100_total->GetYaxis()->SetTitle("Events");
		TLegend *legc100a = new TLegend(0.75, 0.75, 0.9, 0.9);
		legc100a->AddEntry(gr125_100_total, "125 GeV N2", "lp");
		legc100a->AddEntry(gr150_100_total, "150 GeV N2", "lp");
		legc100a->Draw();
		C->Update();
		C->Print((outputDir+"/ctau_100_total.png").c_str());
	
		//1000mm	
		TMultiGraph *ctau_1000_total = new TMultiGraph("ctau_1000_total", "ctau_1000_total");
		gr125_1000_total->SetMarkerColor(kBlue); gr125_1000_total->SetLineColor(kBlue);
		ctau_1000_total->Add(gr125_1000_total);
		gr150_1000_total->SetMarkerColor(kRed); gr150_1000_total->SetLineColor(kRed);
		ctau_1000_total->Add(gr150_1000_total);	
		ctau_1000_total->Draw("AP");
		ctau_1000_total->SetTitle("Total Events Expected vs Mass Splitting for 1000 mm N2C1 ctau");
		ctau_1000_total->GetXaxis()->SetTitle("N2/N1 Mass Splitting [GeV]");
		ctau_1000_total->GetYaxis()->SetTitle("Events");
		TLegend *legc1000a = new TLegend(0.75, 0.75, 0.9, 0.9);
		legc1000a->AddEntry(gr125_1000_total, "125 GeV N2", "lp");
		legc1000a->AddEntry(gr150_1000_total, "150 GeV N2", "lp");
		legc1000a->Draw();
		C->Update();
		C->Print((outputDir+"/ctau_1000_total.png").c_str());
	
	
		//1mm
		TMultiGraph *ctau_1_filtereff = new TMultiGraph("ctau_1_filtereff", "ctau_1_filtereff");
		gr125_1_filtereff->SetMarkerColor(kBlue); gr125_1_filtereff->SetLineColor(kBlue);
		ctau_1_filtereff->Add(gr125_1_filtereff);
		gr150_1_filtereff->SetMarkerColor(kRed); gr150_1_filtereff->SetLineColor(kRed);
		ctau_1_filtereff->Add(gr150_1_filtereff);	
		ctau_1_filtereff->Draw("AP");
		ctau_1_filtereff->SetTitle("Reco-Level Filter Efficiency vs Mass Splitting for 1 mm ctau");
		ctau_1_filtereff->GetXaxis()->SetTitle("N2/N1 Mass Splitting [GeV]");
		ctau_1_filtereff->GetYaxis()->SetTitle("Filter Efficiency (%)");
		TLegend *legc1b = new TLegend(0.75, 0.75, 0.9, 0.9);
		legc1b->AddEntry(gr125_1_filtereff, "125 GeV N2", "lp");
		legc1b->AddEntry(gr150_1_filtereff, "150 GeV N2", "lp");
		legc1b->Draw();
		C->Update();
		C->Print((outputDir+"/ctau_1_filtereff.png").c_str());
		

		//10mm
		TMultiGraph *ctau_10_filtereff = new TMultiGraph("ctau_1", "ctau_1");
		gr125_10_filtereff->SetMarkerColor(kBlue); gr125_10_filtereff->SetLineColor(kBlue);
		ctau_10_filtereff->Add(gr125_10_filtereff);
		gr150_10_filtereff->SetMarkerColor(kRed); gr150_10_filtereff->SetLineColor(kRed);
		ctau_10_filtereff->Add(gr150_10_filtereff);	
		ctau_10_filtereff->Draw("AP");
		ctau_10_filtereff->SetTitle("Reco-Level Filter Efficiency vs Mass Splitting for 10 mm N2C1 ctau");
		ctau_10_filtereff->GetXaxis()->SetTitle("N2/N1 Mass Splitting [GeV]");
		ctau_10_filtereff->GetYaxis()->SetTitle("Filter Efficiency (%)");
		TLegend *legc10b = new TLegend(0.75, 0.75, 0.9, 0.9);
		legc10b->AddEntry(gr125_10_filtereff, "125 GeV N2", "lp");
		legc10b->AddEntry(gr150_10_filtereff, "150 GeV N2", "lp");
		legc10b->Draw();
		C->Update();
		C->Print((outputDir+"/ctau_10_filtereff.png").c_str());
		
		//100mm
		TMultiGraph *ctau_100_filtereff = new TMultiGraph("ctau_100_filtereff", "ctau_100_filtereff");
		gr125_100_filtereff->SetMarkerColor(kBlue); gr125_100_filtereff->SetLineColor(kBlue);
		ctau_100_filtereff->Add(gr125_100_filtereff);
		gr150_100_filtereff->SetMarkerColor(kRed); gr150_100_filtereff->SetLineColor(kRed);
		ctau_100_filtereff->Add(gr150_100_filtereff);	
		ctau_100_filtereff->Draw("AP");
		ctau_100_filtereff->SetTitle("Reco-Level Filter Efficiency vs Mass Splitting for 100 mm N2C1 ctau");
		ctau_100_filtereff->GetXaxis()->SetTitle("N2/N1 Mass Splitting [GeV]");
		ctau_100_filtereff->GetYaxis()->SetTitle("Filter Efficiency (%)");
		TLegend *legc100b = new TLegend(0.75, 0.75, 0.9, 0.9);
		legc100b->AddEntry(gr125_100_filtereff, "125 GeV N2", "lp");
		legc100b->AddEntry(gr150_100_filtereff, "150 GeV N2", "lp");
		legc100b->Draw();
		C->Update();
		C->Print((outputDir+"/ctau_100_filtereff.png").c_str());
	
		//1000mm	
		TMultiGraph *ctau_1000_filtereff = new TMultiGraph("ctau_1000_filtereff", "ctau_1000_filtereff");
		gr125_1000_filtereff->SetMarkerColor(kBlue); gr125_1000_filtereff->SetLineColor(kBlue);
		ctau_1000_filtereff->Add(gr125_1000_filtereff);
		gr150_1000_filtereff->SetMarkerColor(kRed); gr150_1000_filtereff->SetLineColor(kRed);
		ctau_1000_filtereff->Add(gr150_1000_filtereff);	
		ctau_1000_filtereff->Draw("AP");
		ctau_1000_filtereff->SetTitle("Reco-Level Filter Efficiency vs Mass Splitting for 1000 mm N2C1 ctau");
		ctau_1000_filtereff->GetXaxis()->SetTitle("N2/N1 Mass Splitting [GeV]");
		ctau_1000_filtereff->GetYaxis()->SetTitle("Filter Efficiency (%)");
		TLegend *legc1000b = new TLegend(0.75, 0.75, 0.9, 0.9);
		legc1000b->AddEntry(gr125_1000_filtereff, "125 GeV N2", "lp");
		legc1000b->AddEntry(gr150_1000_filtereff, "150 GeV N2", "lp");
		legc1000b->Draw();
		C->Update();
		C->Print((outputDir+"/ctau_1000_filtereff.png").c_str());
	}


}
