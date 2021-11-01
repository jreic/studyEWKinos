#include "TCanvas.h"
#include "TROOT.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TLegend.h"
#include "TArrow.h"
#include "TLatex.h"
#include "TSystem.h"
#include "TTree.h"
#include "TFile.h"
#include "TBranch.h"
#include <vector>
#include <iostream>
using namespace std;
void GenvsRecoSameFile(){
	cout<<"Hello"<<endl;
	/*
	gSystem->cd("ntuples_gbm");
	//TH1D *h1= (TH1D*)f1->Get("reco_gm_pt");
	GetObject("reco_gm_pt", f1);
	reco_gm_pt->DrawNormalized("hist")
	//h1->SetLineWidth(3);
	//h1->SetLineColor(2);
*/

	std::vector<int> genEventNum;
	std::vector<int> recoEventNum;	
	std::vector<int> genLumiSec;
	std::vector<int> recoLumiSec;
	std::vector<int> genRunNum;
	std::vector<int> recoRunNum;

	// must modify script as there is no lumi sec and run num branches for genT
	
	TFile *f1 = TFile::Open("/nfs/cms/mc1/ama288/EWKino/hats_2020_gen/CMSSW_10_6_12/src/GEN4HATS/genproductions/bin/MadGraph5_aMCatNLO/lxplusData/200_190_180/reco/200_190_180_1mm_numEvent100000.root");

	TCanvas* c = new TCanvas();	
	TDirectoryFile* dir = (TDirectoryFile*)f1->Get("ntuples_gbm");
	TH1D *h1 = new TH1D("Pt", "event_num",1000,0,1000);
	TH1D *h2 = new TH1D();
	TTree* recoT = (TTree*) dir->Get("recoT");
	TTree* genT = (TTree*) dir->Get("genT");
	TBranch *eventnum = (TBranch*) genT->GetBranch("event_num");
	static Long_t address;
	eventnum->SetAddress(&address);

	TBranch *lumisec = (TBranch*) genT->GetBranch("lumi_sec");
	static Long_t address2;
	lumisec->SetAddress(&address2);

	TBranch *runnum = (TBranch*) genT->GetBranch("run_num");
	static Long_t address3;
	runnum->SetAddress(&address3);

	Int_t numEntries = (Int_t)genT->GetEntries();	
	for (int i=0; i<numEntries; i++){
		eventnum->GetEntry(i);
		lumisec->GetEntry(i);
		runnum->GetEntry(i);
		genEventNum.push_back(address);
		genLumiSec.push_back(address2);
		genRunNum.push_back(address3);
}
	//h1->Draw();


	TBranch *eventnumreco = (TBranch*) recoT->GetBranch("event_num");
	static Long_t addressreco;
	eventnumreco->SetAddress(&addressreco);

	TBranch *lumisecreco = (TBranch*) recoT->GetBranch("lumi_sec");
	static Long_t address2reco;
	lumisecreco->SetAddress(&address2reco);

	TBranch *runnumreco = (TBranch*) recoT->GetBranch("run_num");
	static Long_t address3reco;
	runnumreco->SetAddress(&address3reco);
	Int_t numEntries2 = (Int_t)recoT->GetEntries();	
	
	for (int i=0; i<numEntries2; i++){
		eventnumreco->GetEntry(i);
		lumisecreco->GetEntry(i);
		runnumreco->GetEntry(i);
		recoEventNum.push_back(addressreco);
		recoLumiSec.push_back(address2reco);
		recoRunNum.push_back(address3reco);
}

	//for(std::vector<int>::const_iterator i = recoEventNum.begin(); i != recoEventNum.end(); ++i){
	//	cout<<*i<<endl;
//}


	bool Event = (genEventNum==recoEventNum);	
	cout<<Event<<endl;
	/*
	cout<"truth value event num: "<<Event<<endl;
	cout<"truth value lumi sec: "<<(*genLumiSec==*recoLumiSec)<<endl;
	cout<"truth value run num: "<<(*genRunNum==*recoRunNum)<<endl;
	*/	





	//cout<<(genEventNum == recoEventNum)<<endl;
/*
	recoT->Draw("reco_gm_pt>>hist1");
	TCanvas* c = new TCanvas(); c->cd();
	h1->SetLineWidth(2);
	h1->SetLineColor(4);
	h1->DrawNormalized();	
	hist->DrawNormalized("same");

*/	
	//c->SaveAs("test123.pdf");

//	TH1D* h2 = (TH1D*)h2->Clone();
//	hist->Reset();
//	gen->Draw("gen_pt>>hist2");
//	hist->SetNameTitle("hist","hist");
//	hist->SetLineColor(2);
		
}

