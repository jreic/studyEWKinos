#include "TFile.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "THStack.h"
#include<iostream>
#include<iterator>
#include<vector>
#include<string>
using namespace std;
using namespace ROOT;

void integrateSignal(std::string filename, double lowerBound, double upperBound){
	const char *myFile = filename.c_str();
	TFile *f1 = TFile::Open(myFile);
	THStack *s1 = (THStack*)f1->Get("matched_muon_Mmumu_cut29-SIGNAL");
	TList *histList = (TList*)s1->GetHists();
	TH1F *hist1mm = (TH1F*) histList->At(0);
	TH1F *hist1000mm = (TH1F*) histList->At(1);
	TH1F *hist10mm = (TH1F*) histList->At(2);
	TH1F *hist100mm = (TH1F*) histList->At(3);
	
	int lowerBin = hist1mm->FindBin(lowerBound);
	int upperBin = hist1mm->FindBin(upperBound) - 1;
	Double_t error = 0;

	cout<<"Integral Values for "<<filename<<" between "<<lowerBound<<" and "<<upperBound<<" GeV."<<endl;
	
	double mm1Integral = hist1mm->IntegralAndError(lowerBin, upperBin, error);
	cout<<"Events in Region for 1 mm Lifetime: "<<mm1Integral<<" +- "<<error<<endl;

	double mm10Integral = hist10mm->IntegralAndError(lowerBin, upperBin, error);
	cout<<"Events in Region for 10 mm Lifetime: "<<mm10Integral<<" +- "<<error<<endl;

	double mm100Integral = hist100mm->IntegralAndError(lowerBin, upperBin, error);
	cout<<"Events in Region for 100 mm Lifetime: "<<mm100Integral<<" +- "<<error<<endl;

	double mm1000Integral = hist1000mm->IntegralAndError(lowerBin, upperBin, error);
	cout<<"Events in Region for 1000 mm Lifetime: "<<mm1000Integral<<" +- "<<error<<endl;
	cout<<"\n"<<endl;
}
