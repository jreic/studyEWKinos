#include "TFile.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TAxis.h"
#include "TGraph.h"
#include<iostream>
#include<iterator>
#include<vector>
#include "Math/GenVector/LorentzVector.h"
#include "Math/Math.h"
#include "Math/GenVector/Boost.h"
#include <algorithm>
#include <cmath>
#include <bits/stdc++.h>
#include <string>
#include "TPad.h"
#include <stdlib.h>
using namespace std;
using namespace ROOT;
using namespace Math;
using namespace GenVector;



float findDeltaR (ROOT::Math::PtEtaPhiM4D<float> muon1, ROOT::Math::PtEtaPhiM4D<float> muon2){
	float eta1 = muon1.Eta();
	float eta2 = muon2.Eta();
	float phi1 = muon1.Phi();
	float phi2 = muon2.Phi();
	float deltaEta = abs(eta1-eta2);
	float deltaPhi = abs(phi1-phi2);
	float deltaR = sqrt(pow(deltaEta,2)+pow(deltaPhi,2));
	deltaR = DeltaR(muon1, muon2);
	return deltaR;
}


void GentoRecoMatching(){

	TFile *f1 = TFile::Open("/nfs/cms/mc1/ama288/EWKino/hats_2020_gen/CMSSW_10_6_12/src/GEN4HATS/genproductions/bin/MadGraph5_aMCatNLO/lxplusData/200_190_180/reco/200_190_180_v6_1mm.root");	

	TDirectoryFile* dir = (TDirectoryFile*)f1->Get("ntuples_gbm");
	TH2F *muonPt = new TH2F("muonPt", "Reco/Gen Muon Pt",15, 0, 30, 15, 0,30); 
	TCanvas* C = new TCanvas("C","Reco/Gen Muon Pt"); 
	TH1F *deltaRHist =  new TH1F("deltaRHist", "delta R Distribution", 50, 0, 1);
	TH2F *muonPhi = new TH2F("muonPhi", "Leading Reco/Gen Muon Phi", 16, -4, 4, 16, -4, 4);	
	TH2F *muonEta = new TH2F("muonEta", "Leading Reco/Gen Muon Pseudorapidity", 24, -6, 6, 24, -6, 6);
	TH2F *muonPtGM = new TH2F("muonPtGM", "GM Reco/Gen Muon Pt", 15, 0, 30, 15, 0, 30);
	TH2F *muonPtDSA = new TH2F("muonPtDSA", "DSA Reco/Gen Muon Pt", 15, 0, 30, 15, 0, 30);
	TH2F *muonEtaGM = new TH2F("muonEtaGM", "GM Leading Reco/Gen Muon Pseudorapidity", 24, -6, 6, 24, -6, 6);
	TH2F *muonEtaDSA = new TH2F("muonEtaDSA", "DSA Leading Reco/Gen Muon Pseudorapidity", 24, -6, 6, 24, -6, 6);
	TH2F *muonPhiGM = new TH2F("muonPhiGM", "GM Leading Reco/Gen Muon Phi", 16, -4, 4, 16, -4, 4);
	TH2F *muonPhiDSA = new TH2F("muonPhiDSA", "DSA Leading Reco/Gen Muon Phi", 16, -4, 4, 16, -4, 4);
	TH1F *muondxyGM = new TH1F("muondxyGM", "GM Impact Parameter for Leading Reco Muon", 400, -400, 400);
	TH1F *muondxyDSA = new TH1F("muondxyDSA", "DSA Impact Parameter for Leading Reco Muon", 400, -400, 400);

	TH2F *MET = new TH2F("MET", "Reco/Gen Missing Transverse Energy", 300, 0, 300, 300, 0, 300);
	TH1F *genJetPtPlot = new TH1F("genJetPtPlot", "Leading Jet Gen Pt Plot", 200, 0, 400);
	TH1F *recoJetPtPlot = new TH1F("recoJetPtPlot", "Leading Jet Reco Pt Plot", 200, 0, 400);

	//gen branches and readers
	TTreeReader myReaderGen("genT", dir);
        TTreeReaderValue<std::vector<float>> ptListGen(myReaderGen, "gen_pt");
        TTreeReaderValue<std::vector<int>> pdgListGen(myReaderGen, "gen_ID");
	TTreeReaderValue<std::vector<float>> phiListGen(myReaderGen, "gen_phi");
	TTreeReaderValue<std::vector<float>> etaListGen(myReaderGen, "gen_eta");
	TTreeReaderValue<std::vector<float>> massListGen(myReaderGen, "gen_mass");
	TTreeReaderValue<std::vector<int>> chargeListGen(myReaderGen, "gen_charge");
	//TTreeReaderValue<std::vector<int>> dxyListGen(myReaderGen, "gen_dxy");

	TTreeReaderValue<Float_t> METGen(myReaderGen, "gen_MET_pt");
	TTreeReaderValue<std::vector<float>> genJetPt(myReaderGen, "gen_jet_pt");
	//reco branches and readers
	TTreeReader myReaderReco("recoT", dir);
        TTreeReaderValue<std::vector<float>> ptListReco(myReaderReco, "reco_gm_pt");
	TTreeReaderValue<std::vector<float>> phiListReco(myReaderReco, "reco_gm_phi");
	TTreeReaderValue<std::vector<float>> etaListReco(myReaderReco, "reco_gm_eta");
	TTreeReaderValue<std::vector<int>> chargeListReco(myReaderReco, "reco_gm_charge");
	TTreeReaderValue<std::vector<float>> dxyListReco(myReaderReco, "reco_gm_dxy");

	TTreeReaderValue<std::vector<float>> ptListDSAReco(myReaderReco, "reco_dsa_pt");	
	TTreeReaderValue<std::vector<float>> phiListDSAReco(myReaderReco, "reco_dsa_phi");
	TTreeReaderValue<std::vector<float>> etaListDSAReco(myReaderReco, "reco_dsa_eta");
	TTreeReaderValue<std::vector<int>> chargeListDSAReco (myReaderReco, "reco_dsa_charge");
	TTreeReaderValue<std::vector<float>> dxyListDSAReco (myReaderReco, "reco_dsa_dxy");

	TTreeReaderValue<Float_t> METReco(myReaderReco, "reco_PF_MET_pt");
	TTreeReaderValue<std::vector<float>> recoJetPt(myReaderReco, "reco_PF_jet_pt");
	vector<int>::iterator intIterator;
	vector<float>::iterator floatIterator;
	//vector<int>::iterator findIterator;
	vector<ROOT::Math::PtEtaPhiM4D<float>>::iterator muonIterator;

	std::vector<float> ptOrdered;
	std::vector<float> deltaR;		
	std::vector<float> deltaR2;	
	std::vector<int> muonTypePos;
	std::vector<int> muonTypeNeg;
	std::vector<float> recoMuonPosdxy;
	std::vector<float> recoMuonNegdxy;	
	std::vector<float> genMuonsPosdxy;
	std::vector<float> genMuonsNegdxy;
	std::vector<float> genMuonsPosOrdereddxy;
	std::vector<float> genMuonsNegOrdereddxy;


	//match muons event by event
	std::vector<ROOT::Math::PtEtaPhiM4D<float>> eventMuonsGenPos;
	std::vector<ROOT::Math::PtEtaPhiM4D<float>> eventMuonsRecPos;
	std::vector<ROOT::Math::PtEtaPhiM4D<float>> eventMuonsGenNeg;
	std::vector<ROOT::Math::PtEtaPhiM4D<float>> eventMuonsRecNeg;
	std::vector<ROOT::Math::PtEtaPhiM4D<float>> eventMuonsGenPosOrdered;
	std::vector<ROOT::Math::PtEtaPhiM4D<float>> eventMuonsGenNegOrdered;
	ROOT::Math::PtEtaPhiM4D<float> muonGen;
	ROOT::Math::PtEtaPhiM4D<float> muonReco;
	ROOT::Math::PtEtaPhiM4D<float> leadPt;
	ROOT::Math::PtEtaPhiM4D<float> subleadPt;

	int eventnum=0;
	while (myReaderGen.Next() && myReaderReco.Next()){
		eventnum++;
		int counter =0;
		int counter2=0;
		int counter3=0;
		eventMuonsGenPos.clear();
		eventMuonsRecPos.clear();
		eventMuonsGenNeg.clear();
		eventMuonsRecNeg.clear();
		eventMuonsGenPosOrdered.clear();
		eventMuonsGenNegOrdered.clear();
		muonTypePos.clear();
		muonTypeNeg.clear();
		recoMuonPosdxy.clear();
		recoMuonNegdxy.clear();
		//iterate through geninfo
		for (intIterator = pdgListGen->begin(); intIterator < pdgListGen->end(); intIterator++){
			
			if (*intIterator == 13 && counter<ptListGen->size() && counter<etaListGen->size() && counter<phiListGen->size() && counter<massListGen->size()){
				//cout<<"here 1"<<endl;
				muonGen.SetCoordinates(ptListGen->at(counter),etaListGen->at(counter),phiListGen->at(counter),massListGen->at(counter));
				eventMuonsGenNeg.push_back(muonGen);
				//genMuonsPosdxy.push_back(dxyListGen->at(counter));
			}
			if (*intIterator == -13 && counter<ptListGen->size() && counter<etaListGen->size() && counter<phiListGen->size() && counter<massListGen->size()){
				//cout<<"here 1a"<<endl;
				muonGen.SetCoordinates(ptListGen->at(counter),etaListGen->at(counter),phiListGen->at(counter),massListGen->at(counter));
				eventMuonsGenPos.push_back(muonGen);
				//genMuonsNegdxy.push_back(dxyListGen->at(counter));
			}
			counter++;
			//muonGen.clear();
		}
		//if (eventMuonsGenPos.empty()) {cout<<"GenPos is Empty "<<eventnum<<endl;}
		//if (eventMuonsGenNeg.empty()) {cout<<"GenNeg is Empty "<<eventnum<<endl;}

		//getMET 
		MET->Fill(*METGen,*METReco);
		float leadingPt = 0.;
		for (floatIterator = genJetPt->begin(); floatIterator < genJetPt->end(); floatIterator++){
			if(*floatIterator>leadingPt){
				leadingPt = *floatIterator;
			}		
		}

		genJetPtPlot->Fill(leadingPt);
		//iterate through reco info
		for (floatIterator = ptListReco->begin(); floatIterator < ptListReco->end(); floatIterator++){
			if (*floatIterator == 0){
				continue;
			}
			if (counter2<ptListReco->size() && counter3<etaListReco->size() && counter3<phiListDSAReco->size()){
			muonReco.SetCoordinates(ptListReco->at(counter2),etaListReco->at(counter2),phiListReco->at(counter2),float(.10566));
			if (chargeListReco->at(counter2)==1){
				//cout<<"here 2"<<endl;
				eventMuonsRecPos.push_back(muonReco);
				muonTypePos.push_back(1);
				recoMuonPosdxy.push_back(dxyListReco->at(counter2));
			}
			if (chargeListReco->at(counter2)==-1){
				//cout<<"here 2a"<<endl;
				eventMuonsRecNeg.push_back(muonReco);
				muonTypeNeg.push_back(1);
				recoMuonNegdxy.push_back(dxyListReco->at(counter2));
			}}
			counter2++;
			//muonReco.clear();	
		}
		
		for (floatIterator = ptListDSAReco->begin(); floatIterator < ptListDSAReco->end(); floatIterator++){
			if (*floatIterator == 0){
				continue;
			}
			if (counter3<ptListDSAReco->size() && counter3<etaListDSAReco->size() && counter3<phiListDSAReco->size()){
			muonReco.SetCoordinates(ptListDSAReco->at(counter3),etaListDSAReco->at(counter3),phiListDSAReco->at(counter3),float(.10566));
			if (chargeListDSAReco->at(counter3)==1){
				//cout<<"here 3"<<endl;
				eventMuonsRecPos.push_back(muonReco);
				muonTypePos.push_back(-1);
				recoMuonPosdxy.push_back(dxyListDSAReco->at(counter3));
			}
			if (chargeListDSAReco->at(counter3)==-1){
				//cout<<"here 3a"<<endl;
				eventMuonsRecNeg.push_back(muonReco);
				muonTypeNeg.push_back(-1);
				recoMuonNegdxy.push_back(dxyListDSAReco->at(counter3));
			}}
			counter3++;
			//muonReco.clear();
			}

		float leadingPtReco = 0.;
		for (floatIterator = recoJetPt->begin(); floatIterator < recoJetPt->end(); floatIterator++){
			if(*floatIterator>leadingPtReco){
				leadingPtReco = *floatIterator;
			}		
		}

		recoJetPtPlot->Fill(leadingPtReco);
		//cout<<"next part"<<endl;
		if (!(eventMuonsGenPos.empty()) && !(eventMuonsRecPos.empty())){
		for (muonIterator = eventMuonsGenPos.begin(); muonIterator < eventMuonsGenPos.end(); muonIterator++){
				ptOrdered.push_back((*muonIterator).Pt());			
		}
		sort(ptOrdered.begin(), ptOrdered.end(),greater<float>());
		
		int dxyCounterPos = 0;
		for (floatIterator = ptOrdered.begin(); floatIterator < ptOrdered.end(); floatIterator++){
			for (muonIterator = eventMuonsGenPos.begin(); muonIterator < eventMuonsGenPos.end(); muonIterator++){
				if (*floatIterator==(*muonIterator).Pt()){
					eventMuonsGenPosOrdered.push_back(*muonIterator);
					//genMuonsPosOrdereddxy.push_back(genMuonsPosdxy.at(dxyCounterPos));
				}
				dxyCounterPos++;
			}
		}
}
		if (!(eventMuonsGenNeg.empty()) && !(eventMuonsRecNeg.empty())){
		ptOrdered.clear();
                for (muonIterator = eventMuonsGenNeg.begin(); muonIterator < eventMuonsGenNeg.end(); muonIterator++){
                                ptOrdered.push_back((*muonIterator).Pt());
                }
                sort(ptOrdered.begin(), ptOrdered.end(),greater<float>());

		int dxyCounterNeg = 0; 
                for (floatIterator = ptOrdered.begin(); floatIterator < ptOrdered.end(); floatIterator++){
                        for (muonIterator = eventMuonsGenNeg.begin(); muonIterator < eventMuonsGenNeg.end(); muonIterator++){
                                if (*floatIterator==(*muonIterator).Pt()){
                                        eventMuonsGenNegOrdered.push_back(*muonIterator);
					//genMuonsNegOrdereddxy.push_back(genMuonsNegdxy.at(dxyCounterNeg));
                                }
				dxyCounterNeg++;
                        }
                }
}
		//cout<<"next part 2"<<endl;
		int muonNum1 = 0;
		int muonNum2 = 0;
		if (!(eventMuonsGenPos.empty())&& !(eventMuonsRecPos.empty())){
		while  (muonNum1 < eventMuonsGenPosOrdered.size()) {
			if (muonNum1 >= eventMuonsGenPosOrdered.size()){
				cout<<"ERROR POS"<<endl;
			}
			leadPt = eventMuonsGenPosOrdered.at(muonNum1);	
			for (muonIterator = eventMuonsRecPos.begin(); muonIterator < eventMuonsRecPos.end(); muonIterator++){
				deltaR.push_back(findDeltaR(leadPt,*muonIterator));	
			}
			if (muonNum1==0){
				//cout<<"checkpoint 1"<<endl;
				deltaRHist->Fill(*min_element(deltaR.begin(), deltaR.end()));			
				int position = distance(deltaR.begin(),find(deltaR.begin(), deltaR.end(), *min_element(deltaR.begin(), deltaR.end())));
				if (muonTypePos.size()>position && eventMuonsRecPos.size()>position){
				muonPhi->Fill(eventMuonsGenPosOrdered.at(muonNum1).Phi(),eventMuonsRecPos.at(position).Phi());			
				muonEta->Fill(eventMuonsGenPosOrdered.at(muonNum1).Eta(),eventMuonsRecPos.at(position).Eta());
				if (muonTypePos.at(position)==1){
					muonPhiGM->Fill(eventMuonsGenPosOrdered.at(muonNum1).Phi(),eventMuonsRecPos.at(position).Phi());
					muonEtaGM->Fill(eventMuonsGenPosOrdered.at(muonNum1).Eta(),eventMuonsRecPos.at(position).Eta());
					muondxyGM->Fill(recoMuonPosdxy.at(position));
				}else{
					muonPhiDSA->Fill(eventMuonsGenPosOrdered.at(muonNum1).Phi(),eventMuonsRecPos.at(position).Phi());
                                        muonEtaDSA->Fill(eventMuonsGenPosOrdered.at(muonNum1).Eta(),eventMuonsRecPos.at(position).Eta());
					muondxyDSA->Fill(recoMuonPosdxy.at(position));				
				}		
			}}
			if (*min_element(deltaR.begin(), deltaR.end()) > float(0.4)){
				muonNum1++;
				continue;
			}
			//solidify matches, fill histograms, etc
			//findIterator = find(deltaR.begin(), deltaR.end(), *min_element(deltaR.begin(), deltaR.end()));
			int position = distance(deltaR.begin(),find(deltaR.begin(), deltaR.end(), *min_element(deltaR.begin(), deltaR.end())));
				if (position < eventMuonsRecPos.size() && position < muonTypePos.size()){
					//cout<<"checkpoint 2"<<endl;
					muonPt->Fill(eventMuonsGenPosOrdered.at(muonNum1).Pt(),eventMuonsRecPos.at(position).Pt());		
					//muonPhi->Fill(eventMuonsGenPosOrdered.at(muonNum1).Phi(),eventMuonsRecPos.at(position).Phi());			
					//muonEta->Fill(eventMuonsGenPosOrdered.at(muonNum1).Eta(),eventMuonsRecPos.at(position).Eta());		
					if (muonTypePos.at(position)==1){
						muonPtGM->Fill(eventMuonsGenPosOrdered.at(muonNum1).Pt(),eventMuonsRecPos.at(position).Pt());		
					}else{
						muonPtDSA->Fill(eventMuonsGenPosOrdered.at(muonNum1).Pt(),eventMuonsRecPos.at(position).Pt());		
					}		
				}
				else{
					muonNum1++;
					continue;
				}	
			break;

			}}
		deltaR.clear();
		if  (!(eventMuonsGenNeg.empty() && !(eventMuonsRecNeg.empty()))){
		while  (muonNum2 < eventMuonsGenNegOrdered.size()) {
			if (muonNum2 >= eventMuonsGenNegOrdered.size()){
				cout<<"ERROR NEg"<<endl;
			}
			leadPt = eventMuonsGenNegOrdered.at(muonNum2);	
			for (muonIterator = eventMuonsRecNeg.begin(); muonIterator < eventMuonsRecNeg.end(); muonIterator++){
				deltaR.push_back(findDeltaR(leadPt,*muonIterator));	
			}	
			if (muonNum2==0){
				//cout<<"checkpoint 3"<<endl;
				deltaRHist->Fill(*min_element(deltaR.begin(), deltaR.end()));			
				int position2 = distance(deltaR.begin(),find(deltaR.begin(), deltaR.end(), *min_element(deltaR.begin(), deltaR.end())));
				if (muonTypeNeg.size()>position2 && eventMuonsRecNeg.size()>position2){	
				muonPhi->Fill(eventMuonsGenNegOrdered.at(muonNum2).Phi(),eventMuonsRecNeg.at(position2).Phi());			
				muonEta->Fill(eventMuonsGenNegOrdered.at(muonNum2).Eta(),eventMuonsRecNeg.at(position2).Eta());		
				if (muonTypeNeg.at(position2)==1){
					muonPhiGM->Fill(eventMuonsGenNegOrdered.at(muonNum2).Phi(),eventMuonsRecNeg.at(position2).Phi());
					muonEtaGM->Fill(eventMuonsGenNegOrdered.at(muonNum2).Eta(),eventMuonsRecNeg.at(position2).Eta());
					muondxyGM->Fill(recoMuonNegdxy.at(position2));
				}else{
					muonPhiDSA->Fill(eventMuonsGenNegOrdered.at(muonNum2).Phi(),eventMuonsRecNeg.at(position2).Phi());
                                        muonEtaDSA->Fill(eventMuonsGenNegOrdered.at(muonNum2).Eta(),eventMuonsRecNeg.at(position2).Eta());				
					muondxyDSA->Fill(recoMuonNegdxy.at(position2));
				}
			}}		
			if (*min_element(deltaR.begin(), deltaR.end()) > float(0.4)){
				muonNum2++;
				continue;
			}
			//solidify matches, fill histograms, etc			
			//findIterator = find(deltaR.begin(), deltaR.end(), *min_element(deltaR.begin(), deltaR.end()));
			int position2 = distance(deltaR.begin(),find(deltaR.begin(), deltaR.end(), *min_element(deltaR.begin(), deltaR.end())));
				if (position2 < eventMuonsRecNeg.size() && position2 < muonTypeNeg.size()){
					//cout<<"checkpoint 4"<<endl;
					muonPt->Fill(eventMuonsGenNegOrdered.at(muonNum2).Pt(),eventMuonsRecNeg.at(position2).Pt());			
					//muonPhi->Fill(eventMuonsGenNegOrdered.at(muonNum2).Phi(),eventMuonsRecNeg.at(position2).Phi());			
					//muonEta->Fill(eventMuonsGenNegOrdered.at(muonNum2).Eta(),eventMuonsRecNeg.at(position2).Eta());		
					if (muonTypeNeg.at(position2)==1){
						muonPtGM->Fill(eventMuonsGenNegOrdered.at(muonNum2).Pt(),eventMuonsRecNeg.at(position2).Pt());		
					}else{
						muonPtDSA->Fill(eventMuonsGenNegOrdered.at(muonNum2).Pt(),eventMuonsRecNeg.at(position2).Pt());		
					}		
				}
				else{
					muonNum2++;
					continue;
				}	


			break;

			}}

		deltaR.clear();
		cout<<"event num"<<eventnum<<endl;		
		

	}
	

	cout<<"completed iterations"<<endl;

	string outputdir = "~/public_html/EWKino/GentoRecoMatching/200_190_180/newCuts_v6/1mm/";
	TFile* outfile = new TFile((outputdir+"out.root").c_str(),"RECREATE");
	system(("mkdir " + outputdir).c_str());


	TAxis* xAxis = new TAxis();	
	muonPt->GetXaxis()->SetTitle("Gen Pt [GeV]");
	muonPt->GetYaxis()->SetTitle("Reco Pt [GeV]");
	muonPt->Draw("COLZ");
	C->Print((outputdir+"MuonPt2D.png").c_str());

	muonPtGM->GetXaxis()->SetTitle("Gen Pt [GeV]");
	muonPtGM->GetYaxis()->SetTitle("Reco Pt [GeV]");
	muonPtGM->Draw("COLZ");
	C->Print((outputdir+"MuonPt2DGM.png").c_str());
	
	muonPtDSA->GetXaxis()->SetTitle("Gen Pt [GeV]");
	muonPtDSA->GetYaxis()->SetTitle("Reco Pt [GeV]");
	muonPtDSA->Draw("COLZ");
	C->Print((outputdir+"MuonPt2DDSA.png").c_str());

	TH1D* muonPtGen = muonPt->ProjectionX();
	muonPtGen->SetTitle(" Gen Muon Pt");
	muonPtGen->GetXaxis()->SetTitle("Pt [GeV]");
	muonPtGen->DrawNormalized();
	C->Print((outputdir+"MuonPtGen.png").c_str());

	TH1D* muonPtReco = muonPt->ProjectionY();
	muonPtReco->SetTitle("Reco Muon Pt");
	muonPtReco->GetXaxis()->SetTitle("Pt [GeV]");
	muonPtReco->DrawNormalized();
	C->Print((outputdir+"MuonPtReco.png").c_str());
	
	deltaRHist->GetXaxis()->SetTitle("delta R");
	deltaRHist->DrawNormalized();
	C->Print((outputdir+"deltaRHist.png").c_str());

	muonPhi->GetXaxis()->SetTitle("Gen");
	muonPhi->GetYaxis()->SetTitle("Reco");
	muonPhi->Draw("COLZ");
	C->Print((outputdir+"LeadingMuonPhi2D.png").c_str());

	muonPhiGM->GetXaxis()->SetTitle("Gen");
	muonPhiGM->GetYaxis()->SetTitle("Reco");
	muonPhiGM->Draw("COLZ");
	C->Print((outputdir+"LeadingMuonPhi2DGM.png").c_str());

	muonPhiDSA->GetXaxis()->SetTitle("Gen");
	muonPhiDSA->GetYaxis()->SetTitle("Reco");
	muonPhiDSA->Draw("COLZ");
	C->Print((outputdir+"LeadingMuonPhi2DDSA.png").c_str());

	TH1D* muonPhiGen = muonPhi->ProjectionX();
	muonPhiGen->SetTitle("Leading Gen Muon Phi");
	muonPhiGen->GetXaxis()->SetTitle("Phi");
	muonPhiGen->DrawNormalized();
	C->Print((outputdir+"MuonPhiGen.png").c_str());
	
	TH1D* muonPhiReco = muonPhi->ProjectionY();
	muonPhiReco->SetTitle("Leading Reco Muon Phi");
	muonPhiReco->GetXaxis()->SetTitle("Phi");
	muonPhiReco->DrawNormalized();
	C->Print((outputdir+"MuonPhiReco.png").c_str());
	
	muonEta->GetXaxis()->SetTitle("Gen");
	muonEta->GetYaxis()->SetTitle("Reco");
	muonEta->Draw("COLZ");
	C->Print((outputdir+"LeadingMuonEta2D.png").c_str());

	muonEtaGM->GetXaxis()->SetTitle("Gen");
	muonEtaGM->GetYaxis()->SetTitle("Reco");
	muonEtaGM->Draw("COLZ");
	C->Print((outputdir+"LeadingMuonEta2DGM.png").c_str());

	muonEtaDSA->GetXaxis()->SetTitle("Gen");
	muonEtaDSA->GetYaxis()->SetTitle("Reco");
	muonEtaDSA->Draw("COLZ");
	C->Print((outputdir+"LeadingMuonEta2DGMDSA.png").c_str());

	TH1D* muonEtaGen = muonEta->ProjectionX();
	muonEtaGen->SetTitle("Leading Gen Muon Eta");
	muonEtaGen->GetXaxis()->SetTitle("Eta");
	muonEtaGen->DrawNormalized();
	C->Print((outputdir+"MuonEtaGen.png").c_str());

	TH1D* muonEtaReco = muonEta->ProjectionY();
	muonEtaReco->SetTitle("Leading Reco Muon Eta");
	muonEtaReco->GetXaxis()->SetTitle("Eta");
	muonEtaReco->DrawNormalized();
	C->Print((outputdir+"MuonEtaReco.png").c_str());


	MET->GetXaxis()->SetTitle("Gen [GeV]");
	MET->GetYaxis()->SetTitle("Reco [GeV]");
	MET->Draw("COLZ");
	C->Print((outputdir+"MET2D.png").c_str());

	TH1D* MetGen = MET->ProjectionX();
	MetGen->SetTitle("Gen Level MET");
	MetGen->GetXaxis()->SetTitle("MET [GeV]");
	MetGen->DrawNormalized();
	C->Print((outputdir+"METGen.png").c_str());

	TH1D* MetReco = MET->ProjectionY();
	MetReco->SetTitle("Reco Level MET");
	MetReco->GetXaxis()->SetTitle("MET [GeV]");
	MetReco->DrawNormalized();
	C->Print((outputdir+"METReco.png").c_str());

	muondxyGM->GetXaxis()->SetTitle("Displacement [mm]");
	muondxyGM->DrawNormalized();
	C->Print((outputdir+"muondxyGM.png").c_str());

	muondxyDSA->GetXaxis()->SetTitle("Displacement [mm]");
	muondxyDSA->DrawNormalized();
	C->Print((outputdir+"muondxyDSA.png").c_str());

	genJetPtPlot->GetXaxis()->SetTitle("Momentum [GeV]");
	genJetPtPlot->DrawNormalized();
	C->Print((outputdir+"LeadingGenJetPt.png").c_str());
	
	recoJetPtPlot->GetXaxis()->SetTitle("Momentum [GeV]");
	recoJetPtPlot->DrawNormalized();
	C->Print((outputdir+"LeadingRecoJetPt.png").c_str());
	//muonPtGM->DrawNormalized("COL");
	//muonPtDSA->DrawNormalized("COLSAME");
	//C->Print((outputdir+"PtComparison.png").c_str());
	
	outfile->Write();

}

