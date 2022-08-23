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
#include <TLorentzVector.h>
#include<string>
#include <TLegend.h>
#include <TAttLine.h>
using namespace std;
using namespace ROOT;
using namespace Math;
using namespace GenVector;



float findDeltaR (ROOT::Math::PtEtaPhiM4D<float> muon1, ROOT::Math::PtEtaPhiM4D<float> muon2){
	float eta1 = muon1.Eta();
	float eta2 = muon2.Eta();
	float phi1 = muon1.Phi();
	float phi2 = muon2.Phi();
	/*
	float deltaEta = abs(eta1-eta2);
	float deltaPhi = abs(phi1-phi2);
	float deltaR = sqrt(pow(deltaEta,2)+pow(deltaPhi,2));
	*/
	TLorentzVector muon1new; TLorentzVector muon2new;
	muon1new.SetPtEtaPhiM(muon1.Pt(), eta1, phi1, muon1.M());
	muon2new.SetPtEtaPhiM(muon2.Pt(), eta2, phi2, muon2.M());
	float deltaR = muon1new.DeltaR(muon2new);
	return deltaR;
}


void GentoRecoMatchingv2(std::string masses, int lifetime){
	int a = lifetime;
	cout<<masses<<endl;
	cout<<a<<endl;
	std::string a2 = to_string(a);	
	std::string fileString = "/nfs/cms/mc1/ama288/EWKino/hats_2020_gen/CMSSW_10_6_12/src/GEN4HATS/genproductions/bin/MadGraph5_aMCatNLO/lxplusData/" + masses + "/reco/" + masses + "_N2N1_v6_" + a2 + "mm.root";
	cout<<fileString<<endl;
	const char *c = fileString.c_str();
	TFile *f1 = TFile::Open(c);	

	TDirectoryFile* dir = (TDirectoryFile*)f1->Get("ntuples_gbm");
	TH2F *muonPt = new TH2F("muonPt", "Reco/Gen Muon Pt",50, 0, 50, 50, 0,50); 
	TCanvas* C = new TCanvas("C","Reco/Gen Muon Pt"); 
	TH1F *deltaRHistGM =  new TH1F("deltaRHist", "delta R Distribution", 50, 0, 1);
	TH1F *deltaRHistDSA =  new TH1F("deltaRHist", "delta R Distribution", 50, 0, 1);
	TH2F *muonPtGM = new TH2F("muonPtGM", "GM Reco/Gen Muon Pt", 50, 0, 50, 50, 0, 50);
	TH2F *muonPtDSA = new TH2F("muonPtDSA", "DSA Reco/Gen Muon Pt", 50, 0, 50, 50, 0, 50);
	TH2F *muonEtaGM = new TH2F("muonEtaGM", "GM Leading Reco/Gen Muon Pseudorapidity", 24, -6, 6, 24, -6, 6);
	TH2F *muonEtaDSA = new TH2F("muonEtaDSA", "DSA Leading Reco/Gen Muon Pseudorapidity", 24, -6, 6, 24, -6, 6);
	TH2F *muonPhiGM = new TH2F("muonPhiGM", "GM Leading Reco/Gen Muon Phi", 16, -4, 4, 16, -4, 4);
	TH2F *muonPhiDSA = new TH2F("muonPhiDSA", "DSA Leading Reco/Gen Muon Phi", 16, -4, 4, 16, -4, 4);
	TH1F *muondxyGM = new TH1F("muondxyGM", "GM Impact Parameter for Leading Reco Muon", 400, -a, a);
	TH1F *muondxyDSA = new TH1F("muondxyDSA", "DSA Impact Parameter for Leading Reco Muon", 400, -400, 400);

	TH2F *MET = new TH2F("MET", "Reco/Gen Missing Transverse Energy", 300, 0, 300, 300, 0, 300);
	TH1F *genJetPtPlot = new TH1F("genJetPtPlot", "Leading Jet Gen Pt Plot", 200, 0, 400);
	TH1F *recoJetPtPlot = new TH1F("recoJetPtPlot", "Leading Jet Reco Pt Plot", 200, 0, 400);

	// changes added 8/23 - new histograms
	TH1F *numGenMuons = new TH1F("numGenMuons", "Number of Gen Muons Per Event", 5,0,5);
	TH1F *numGMRecoMuons = new TH1F("numGMRecoMuons", "Number of GM Reco Muons Per Event", 8,0,8);
	TH1F *numDSARecoMuons = new TH1F("numDSARecoMuons", "Number of DSA Reco Muons Per Event", 8,0,8);
	TH1F *deltaRGenPairGM = new TH1F("deltaRGenPair", "Delta R between Gen Pair", 100, 0, 10);
	TH1F *deltaRRecoPairGM = new TH1F("deltaRRecoPair", "Delta R between Reco Pair", 100, 0, 10);
	TH1F *deltaRGenPairDSA = new TH1F("deltaRGenPair", "Delta R between Gen Pair", 100, 0, 10);
	TH1F *deltaRRecoPairDSA = new TH1F("deltaRRecoPair", "Delta R between Reco Pair", 100, 0, 10);
	TH1F *N2C1dPhi = new TH1F("N2C1dPhi", "Delta Phi between Neutralino 2 and Chargino 1", 80, -4, 4);
	TH1F *N2JdPhi = new TH1F("N2JdPhi", "Delta Phi between Neutralino 2 and Leading Jet", 80, -4, 4);
	TH1F *C1JdPhi =  new TH1F("C1JdPhi", "Delta Phi between Chargino 1 and Leading Jet", 80, -4, 4);
	TH1F *numJetsPerEvent = new TH1F("numJetsPerEvent", "Number of Jets Per Event", 10, 0, 10); 	
	TH2F *C1J_N2C1 = new TH2F("C1J_N2C1","Delta Phi between Chargino 1 and Leading Jet vs Neutralino 2 and Chargino 1", 20, -4, 4, 20, -4, 4);
	TH2F *C1J_N2J = new TH2F("C1J_N2J","Delta Phi between Chargino 1 and Leading Jet vs Neutralino 2 and Leading Jet", 20, -4, 4, 20, -4, 4);
	TH2F *N2C1_N2J = new TH2F("N2C1_N2J", "Delta Phi between Neutralino 2 and Chargino 1 vs  Neutralino 2 and Leading Jet", 20, -4, 4, 20, -4, 4);
	TH2F *C1PtvsN2C1dPhi =new TH2F("C1PtvsN2C1dPhi", "C1 Pt vs N2/C1 Delta Phi", 20, -4, 4, 50, 0, 500);
	TH2F *N2PtvsN2C1dPhi =new TH2F("N2PtvsN2C1dPhi", "N2 Pt vs N2/C1 Delta Phi", 20, -4, 4, 50, 0, 500);
	TH2F *C1PtvsC1JdPhi =new TH2F("C1PtvsC1JdPhi", "C1 Pt vs C1/J Delta Phi", 20, -4, 4, 50, 0, 500);
	TH2F *N2PtvsN2JdPhi =new TH2F("N2PtvsN2JdPhi", "N2 Pt vs N2/J Delta Phi", 20, -4, 4, 50, 0, 500);
	TH1F *dPhiRecoMuonsDSA = new TH1F("dPhiRecoMuonsDSA", "Delta Phi Between Reco Muons", 40, -4, 4);
	TH1F *dPhiGenMuonsDSA = new TH1F("dPhiGenMuonsDSA", "Delta Phi Between Gen Muons", 40, -4, 4);
	TH1F *dPhiRecoMuonsGM = new TH1F("dPhiRecoMuonsGM", "Delta Phi Between Reco Muons",40, -4, 4);
	TH1F *dPhiGenMuonsGM = new TH1F("dPhiGenMuonsGM", "Delta Phi Between Gen Muons", 40, -4, 4);
	TH2F *N2PtvsC1Pt = new TH2F("N2PtvsC1Pt", "Neutralino 2 vs Chargino 1 Pt", 50, 0, 500, 50, 0, 500);	
	TH1F *GenMuonMass = new TH1F("GenMuonMass", "Invariant Mass of Gen Muon Pair", 50, 0, 100);
	TH1F *GMRecoMuonMass = new TH1F("GMRecoMuonMass", "Invariant Mass of GMReco Muon Pair", 50, 0, 100);
	TH1F *DSARecoMuonMass = new TH1F("DSARecoMuonMass", "Invariant Mass of DSAReco Muon Pair", 50, 0, 100);


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
	TTreeReaderValue<std::vector<float>> recoJetPhi(myReaderReco, "reco_PF_jet_phi");
	TTreeReaderValue<std::vector<float>> recoJetEta(myReaderReco, "reco_PF_jet_eta");
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
	std::vector<ROOT::Math::PtEtaPhiM4D<float>> bestGenPair;
	std::vector<ROOT::Math::PtEtaPhiM4D<float>> bestRecoPair;
	std::vector<TLorentzVector> bestGenPair2;
	std::vector<TLorentzVector> bestRecoPair2;
	ROOT::Math::PtEtaPhiM4D<float> muonGen;
	ROOT::Math::PtEtaPhiM4D<float> muonReco;
	ROOT::Math::PtEtaPhiM4D<float> leadPt;
	ROOT::Math::PtEtaPhiM4D<float> subleadPt;
	TLorentzVector N2;
	TLorentzVector C1;
	TLorentzVector leadingJet;
	
	bool TwoGenMuons = true;
	int eventnum=0;
	while (myReaderGen.Next() && myReaderReco.Next()){
		eventnum++;
		TwoGenMuons = true;
		int counter = 0;
		int counter2= 0;
		int counter3= 0;
		float N2Phi = 0;
		float C1Phi = 0;
		float LeadingJetPhi =  0;
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
		numGMRecoMuons->Fill(ptListReco->size());
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
			//muonGen.clear();
			if (*intIterator == 1000023){
				N2.SetPtEtaPhiM(ptListGen->at(counter), etaListGen->at(counter), phiListGen->at(counter), massListGen->at(counter));	
			}
			if (*intIterator == 1000024){
				C1.SetPtEtaPhiM(ptListGen->at(counter), etaListGen->at(counter), phiListGen->at(counter), massListGen->at(counter));	
			}
			counter++;

		}
		
		//if (eventMuonsGenPos.empty()) {cout<<"GenPos is Empty "<<eventnum<<endl;}
		//if (eventMuonsGenNeg.empty()) {cout<<"GenNeg is Empty "<<eventnum<<endl;}
		numGenMuons->Fill(eventMuonsGenNeg.size() + eventMuonsGenPos.size());
		if (eventMuonsGenNeg.size() + eventMuonsGenPos.size() != 2){
			TwoGenMuons = false;	
		}

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
		numJetsPerEvent->Fill(recoJetPt->size());
		
		float leadingPtReco = 0.; float leadingPhiReco = 0;
		int jetCounter = 0; float bestJetPhi = 0;
		for (floatIterator = recoJetPt->begin(); floatIterator < recoJetPt->end(); floatIterator++){
			if(*floatIterator>leadingPtReco){
				leadingPtReco = *floatIterator;
				leadingJet.SetPtEtaPhiM(recoJetPt->at(jetCounter), recoJetEta->at(jetCounter), recoJetPhi->at(jetCounter), 5); // 5 is arbitrary number	
			}
			jetCounter++;		
		}


		recoJetPtPlot->Fill(leadingPtReco);
		N2C1dPhi->Fill(N2.DeltaPhi(C1));
		N2JdPhi->Fill(N2.DeltaPhi(leadingJet));
		C1JdPhi->Fill(C1.DeltaPhi(leadingJet));		
		N2C1_N2J->Fill(N2.DeltaPhi(C1), N2.DeltaPhi(leadingJet));
		C1J_N2C1->Fill(C1.DeltaPhi(leadingJet), N2.DeltaPhi(C1));
		C1J_N2J->Fill(C1.DeltaPhi(leadingJet), N2.DeltaPhi(leadingJet));
		C1PtvsN2C1dPhi->Fill(N2.DeltaPhi(C1), C1.Pt());
		N2PtvsN2C1dPhi->Fill(N2.DeltaPhi(C1), N2.Pt());
		C1PtvsC1JdPhi->Fill(C1.DeltaPhi(leadingJet), C1.Pt());
		N2PtvsN2JdPhi->Fill(N2.DeltaPhi(leadingJet), N2.Pt());
		N2PtvsC1Pt->Fill(N2.Pt(), C1.Pt());
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
		//cout<<"next part again"<<endl;
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
		//cout<<"next part again again"<<endl;
		int muonNum1 = 0;
		int muonNum2 = 0;
		TLorentzVector tempGen; TLorentzVector tempReco;
		if (!(eventMuonsGenPos.empty())&& !(eventMuonsRecPos.empty())){
		while  (muonNum1 < eventMuonsGenPosOrdered.size()) {
			if (muonNum1 >= eventMuonsGenPosOrdered.size()){
				//cout<<"ERROR POS"<<endl;
			}
			leadPt = eventMuonsGenPosOrdered.at(muonNum1);	
			for (muonIterator = eventMuonsRecPos.begin(); muonIterator < eventMuonsRecPos.end(); muonIterator++){
				deltaR.push_back(findDeltaR(leadPt,*muonIterator));	
			}
			if (*min_element(deltaR.begin(), deltaR.end()) > float(0.4)){
				muonNum1++;
				continue;
			}
			//solidify matches, fill histograms, etc
			//findIterator = find(deltaR.begin(), deltaR.end(), *min_element(deltaR.begin(), deltaR.end()));
			//cout<<"position calc"<<endl;
			int position = distance(deltaR.begin(),find(deltaR.begin(), deltaR.end(), *min_element(deltaR.begin(), deltaR.end())));
				if (position < eventMuonsRecPos.size() && position < muonTypePos.size()){
					deltaRHistGM->Fill(*min_element(deltaR.begin(), deltaR.end()));			
					if (muonTypePos.size()>position && eventMuonsRecPos.size()>position){
					muonPhiGM->Fill(eventMuonsGenPosOrdered.at(muonNum1).Phi(),eventMuonsRecPos.at(position).Phi());
					muonEtaGM->Fill(eventMuonsGenPosOrdered.at(muonNum1).Eta(),eventMuonsRecPos.at(position).Eta());
					muondxyGM->Fill(recoMuonPosdxy.at(position));
					muonPtGM->Fill(eventMuonsGenPosOrdered.at(muonNum1).Pt(),eventMuonsRecPos.at(position).Pt());		
					bestGenPair.push_back(eventMuonsGenPosOrdered.at(muonNum1));
					bestRecoPair.push_back(eventMuonsRecPos.at(position));
					tempGen.SetPtEtaPhiM(eventMuonsGenPosOrdered.at(muonNum1).Pt(), eventMuonsGenPosOrdered.at(muonNum1).Eta(), eventMuonsGenPosOrdered.at(muonNum1).Phi(), eventMuonsGenPosOrdered.at(muonNum1).M());
					tempReco.SetPtEtaPhiM(eventMuonsRecPos.at(position).Pt(), eventMuonsRecPos.at(position).Eta(), eventMuonsRecPos.at(position).Phi(), eventMuonsRecPos.at(position).M());
					bestGenPair2.push_back(tempGen);
					bestRecoPair2.push_back(tempReco);

			}	}
				else{
					muonNum1++;
					continue;
				}	
			break;

			}}
		deltaR.clear();
		tempGen = TLorentzVector();
		tempReco = TLorentzVector();
		//cout<<"Negative muons"<<endl;
		if  (!(eventMuonsGenNeg.empty() && !(eventMuonsRecNeg.empty()))){
		//cout<<eventMuonsGenNegOrdered.size()<<endl;
		//cout<<"passes if statment"<<endl;

		TLorentzVector tempGen; TLorentzVector tempReco;
		while  (muonNum2 < eventMuonsGenNegOrdered.size()) {
			//cout<<"enters while loop"<<endl;
			if (muonNum2 >= eventMuonsGenNegOrdered.size()){
				//cout<<"ERROR NEg"<<endl;
			}
			//cout<<"neg test 1"<<endl;
			leadPt = eventMuonsGenNegOrdered.at(muonNum2);
			//cout<<"neg test 2"<<endl;	
			for (muonIterator = eventMuonsRecNeg.begin(); muonIterator < eventMuonsRecNeg.end(); muonIterator++){
				deltaR.push_back(findDeltaR(leadPt,*muonIterator));	
			}	
			//cout<<"neg test 3"<<endl;
			if (*min_element(deltaR.begin(), deltaR.end()) > float(0.4)){
				muonNum2++;
				continue;
			}
			//solidify matches, fill histograms, etc			
			//findIterator = find(deltaR.begin(), deltaR.end(), *min_element(deltaR.begin(), deltaR.end()));
			//cout<<"position calc neg"<<endl;
			int position2 = distance(deltaR.begin(),find(deltaR.begin(), deltaR.end(), *min_element(deltaR.begin(), deltaR.end())));
				if (position2 < eventMuonsRecNeg.size() && position2 < muonTypeNeg.size()){
					deltaRHistGM->Fill(*min_element(deltaR.begin(), deltaR.end()));			
					if (muonTypeNeg.size()>position2 && eventMuonsRecNeg.size()>position2){
					muonPhiGM->Fill(eventMuonsGenNegOrdered.at(muonNum2).Phi(),eventMuonsRecNeg.at(position2).Phi());
					muonEtaGM->Fill(eventMuonsGenNegOrdered.at(muonNum2).Eta(),eventMuonsRecNeg.at(position2).Eta());
					muondxyGM->Fill(recoMuonNegdxy.at(position2));
					muonPtGM->Fill(eventMuonsGenNegOrdered.at(muonNum2).Pt(),eventMuonsRecNeg.at(position2).Pt());				
					bestGenPair.push_back(eventMuonsGenNegOrdered.at(muonNum2));
					bestRecoPair.push_back(eventMuonsRecNeg.at(position2));
					TLorentzVector tempGen; TLorentzVector tempReco;
					tempGen.SetPtEtaPhiM(eventMuonsGenNegOrdered.at(muonNum2).Pt(), eventMuonsGenNegOrdered.at(muonNum2).Eta(), eventMuonsGenNegOrdered.at(muonNum2).Phi(), eventMuonsGenNegOrdered.at(muonNum2).M());
					tempReco.SetPtEtaPhiM(eventMuonsRecNeg.at(position2).Pt(), eventMuonsRecNeg.at(position2).Eta(), eventMuonsRecNeg.at(position2).Phi(), eventMuonsRecNeg.at(position2).M());
					bestGenPair2.push_back(tempGen);
					bestRecoPair2.push_back(tempReco);

				}}
				else{
					muonNum2++;
					continue;
				}	


			break;

			}}
		//cout<<"down here"<<endl;
		if (bestGenPair.size()==2 && bestRecoPair.size()==2){
		deltaRGenPairGM->Fill(findDeltaR(bestGenPair[0], bestGenPair[1]));
		deltaRRecoPairGM->Fill(findDeltaR(bestRecoPair[0], bestRecoPair[1]));
		dPhiGenMuonsGM->Fill(bestGenPair2[0].DeltaPhi(bestGenPair2[1]));
		dPhiRecoMuonsGM->Fill(bestRecoPair2[0].DeltaPhi(bestRecoPair2[1]));
		}
		if (TwoGenMuons && bestRecoPair.size()==2){
			TLorentzVector GenSum = bestGenPair2[0] + bestGenPair2[1]; TLorentzVector recoSum = bestRecoPair2[0] + bestRecoPair2[1];
			GenMuonMass->Fill(GenSum.M());
			GMRecoMuonMass->Fill(recoSum.M());
		}
		//cout<<"down here 2"<<endl;
		tempGen = TLorentzVector();
		tempReco = TLorentzVector();
		bestGenPair.clear();
		bestRecoPair.clear();
		bestGenPair2.clear();
		bestRecoPair2.clear();
		deltaR.clear();
		//////cout<<"event num"<<eventnum<<endl;		

	}
	
	/////////////////////////////////////////////////////////////////
	myReaderGen.Restart(); myReaderReco.Restart();
	//cout<<"DSA"<<endl;	
	while (myReaderGen.Next() && myReaderReco.Next()){
		TwoGenMuons = true;
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
		numDSARecoMuons->Fill(ptListDSAReco->size());
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
		float leadingPt = 0.;
		for (floatIterator = genJetPt->begin(); floatIterator < genJetPt->end(); floatIterator++){
			if(*floatIterator>leadingPt){
				leadingPt = *floatIterator;
			}		
		}

		if (eventMuonsGenNeg.size() + eventMuonsGenPos.size() != 2){
			TwoGenMuons = false;	
		}
		//iterate through reco info
		for (floatIterator = ptListDSAReco->begin(); floatIterator < ptListDSAReco->end(); floatIterator++){
			if (*floatIterator == 0){
				continue;
			}
			if (counter2<ptListDSAReco->size() && counter3<etaListDSAReco->size() && counter3<phiListDSAReco->size()){
			muonReco.SetCoordinates(ptListDSAReco->at(counter2),etaListDSAReco->at(counter2),phiListDSAReco->at(counter2),float(.10566));
			if (chargeListDSAReco->at(counter2)==1){
				//cout<<"here 2"<<endl;
				eventMuonsRecPos.push_back(muonReco);
				muonTypePos.push_back(1);
				recoMuonPosdxy.push_back(dxyListDSAReco->at(counter2));
			}
			if (chargeListDSAReco->at(counter2)==-1){
				//cout<<"here 2a"<<endl;
				eventMuonsRecNeg.push_back(muonReco);
				muonTypeNeg.push_back(1);
				recoMuonNegdxy.push_back(dxyListDSAReco->at(counter2));
			}}
			counter2++;
			//muonReco.clear();	
		}
		

		float leadingPtDSAReco = 0.;
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
		TLorentzVector tempGen; TLorentzVector tempReco;
		if (!(eventMuonsGenPos.empty())&& !(eventMuonsRecPos.empty())){
		while  (muonNum1 < eventMuonsGenPosOrdered.size()) {
			if (muonNum1 >= eventMuonsGenPosOrdered.size()){
				cout<<"ERROR POS"<<endl;
			}
			leadPt = eventMuonsGenPosOrdered.at(muonNum1);	
			for (muonIterator = eventMuonsRecPos.begin(); muonIterator < eventMuonsRecPos.end(); muonIterator++){
				deltaR.push_back(findDeltaR(leadPt,*muonIterator));	
			}
			
			if (*min_element(deltaR.begin(), deltaR.end()) > float(0.4)){
				muonNum1++;
				continue;
			}
			//solidify matches, fill histograms, etc
			//findIterator = find(deltaR.begin(), deltaR.end(), *min_element(deltaR.begin(), deltaR.end()));
			int position = distance(deltaR.begin(),find(deltaR.begin(), deltaR.end(), *min_element(deltaR.begin(), deltaR.end())));
				if (position < eventMuonsRecPos.size() && position < muonTypePos.size()){
					deltaRHistDSA->Fill(*min_element(deltaR.begin(), deltaR.end()));			
					if (muonTypePos.size()>position && eventMuonsRecPos.size()>position){
					muonPhiDSA->Fill(eventMuonsGenPosOrdered.at(muonNum1).Phi(),eventMuonsRecPos.at(position).Phi());
					muonEtaDSA->Fill(eventMuonsGenPosOrdered.at(muonNum1).Eta(),eventMuonsRecPos.at(position).Eta());
					muondxyDSA->Fill(recoMuonPosdxy.at(position));
					muonPtDSA->Fill(eventMuonsGenPosOrdered.at(muonNum1).Pt(),eventMuonsRecPos.at(position).Pt());		
					bestGenPair.push_back(eventMuonsGenPosOrdered.at(muonNum1));
					bestRecoPair.push_back(eventMuonsRecPos.at(position));
					tempGen.SetPtEtaPhiM(eventMuonsGenPosOrdered.at(muonNum1).Pt(), eventMuonsGenPosOrdered.at(muonNum1).Eta(), eventMuonsGenPosOrdered.at(muonNum1).Phi(), eventMuonsGenPosOrdered.at(muonNum1).M());
					tempReco.SetPtEtaPhiM(eventMuonsRecPos.at(position).Pt(), eventMuonsRecPos.at(position).Eta(), eventMuonsRecPos.at(position).Phi(), eventMuonsRecPos.at(position).M());
					bestGenPair2.push_back(tempGen);
					bestRecoPair2.push_back(tempReco);
				}}
				else{
					muonNum1++;
					continue;
				}	
			break;

			}}
		deltaR.clear();
		tempGen = TLorentzVector(); tempReco = TLorentzVector();
		if  (!(eventMuonsGenNeg.empty() && !(eventMuonsRecNeg.empty()))){
		while  (muonNum2 < eventMuonsGenNegOrdered.size()) {
			if (muonNum2 >= eventMuonsGenNegOrdered.size()){
				cout<<"ERROR NEg"<<endl;
			}
			leadPt = eventMuonsGenNegOrdered.at(muonNum2);	
			for (muonIterator = eventMuonsRecNeg.begin(); muonIterator < eventMuonsRecNeg.end(); muonIterator++){
				deltaR.push_back(findDeltaR(leadPt,*muonIterator));	
			}	
					
			if (*min_element(deltaR.begin(), deltaR.end()) > float(0.4)){
				muonNum2++;
				continue;
			}
			//solidify matches, fill histograms, etc			
			//findIterator = find(deltaR.begin(), deltaR.end(), *min_element(deltaR.begin(), deltaR.end()));
			int position2 = distance(deltaR.begin(),find(deltaR.begin(), deltaR.end(), *min_element(deltaR.begin(), deltaR.end())));
				if (position2 < eventMuonsRecNeg.size() && position2 < muonTypeNeg.size()){
					deltaRHistDSA->Fill(*min_element(deltaR.begin(), deltaR.end()));			
					if (muonTypeNeg.size()>position2 && eventMuonsRecNeg.size()>position2){
					muonPhiDSA->Fill(eventMuonsGenNegOrdered.at(muonNum2).Phi(),eventMuonsRecNeg.at(position2).Phi());
					muonEtaDSA->Fill(eventMuonsGenNegOrdered.at(muonNum2).Eta(),eventMuonsRecNeg.at(position2).Eta());
					muondxyDSA->Fill(recoMuonNegdxy.at(position2));
					muonPtDSA->Fill(eventMuonsGenNegOrdered.at(muonNum2).Pt(),eventMuonsRecNeg.at(position2).Pt());		
					bestGenPair.push_back(eventMuonsGenNegOrdered.at(muonNum2));
					bestRecoPair.push_back(eventMuonsRecNeg.at(position2));
					TLorentzVector tempGen; TLorentzVector tempReco;
					tempGen.SetPtEtaPhiM(eventMuonsGenNegOrdered.at(muonNum2).Pt(), eventMuonsGenNegOrdered.at(muonNum2).Eta(), eventMuonsGenNegOrdered.at(muonNum2).Phi(), eventMuonsGenNegOrdered.at(muonNum2).M());
					tempReco.SetPtEtaPhiM(eventMuonsRecNeg.at(position2).Pt(), eventMuonsRecNeg.at(position2).Eta(), eventMuonsRecNeg.at(position2).Phi(), eventMuonsRecNeg.at(position2).M());
					bestGenPair2.push_back(tempGen);
					bestRecoPair2.push_back(tempReco);
				
				}}
				else{
					muonNum2++;
					continue;
				}	


			break;

			}}
		if (bestGenPair.size()==2 && bestRecoPair.size()==2){
		deltaRGenPairDSA->Fill(findDeltaR(bestGenPair[0], bestGenPair[1]));
		deltaRRecoPairDSA->Fill(findDeltaR(bestRecoPair[0], bestRecoPair[1]));
		dPhiGenMuonsDSA->Fill(bestGenPair2[0].DeltaPhi(bestGenPair2[1]));
		dPhiRecoMuonsDSA->Fill(bestRecoPair2[0].DeltaPhi(bestRecoPair2[1]));
		}
		
		if (TwoGenMuons && bestRecoPair.size()==2){
			TLorentzVector GenSum = bestGenPair2[0] + bestGenPair2[1]; TLorentzVector recoSum = bestRecoPair2[0] + bestRecoPair2[1];
			DSARecoMuonMass->Fill(recoSum.M());
		}
		tempGen = TLorentzVector(); tempReco = TLorentzVector();
		bestGenPair.clear();
		bestRecoPair.clear();
		bestGenPair2.clear();
		bestRecoPair2.clear();
		deltaR.clear();
		//cout<<"event num"<<eventnum<<endl;		
	}
		
	/////////////////////////////////////////////////////////
	cout<<"completed iterations"<<endl;

	std::string outputdir = "~/public_html/EWKino/GentoRecoMatching/" + masses + "_N2N1" +"/" + a2 + "mm/";
	std::string outputRoot = outputdir + ".root";
	const char *d = outputRoot.c_str();
	TFile* outfile = new TFile(d,"RECREATE");
	system(("mkdir " + outputdir).c_str());


	TAxis* xAxis = new TAxis();	

	muonPtGM->GetXaxis()->SetTitle("Gen Pt [GeV]");
	muonPtGM->GetYaxis()->SetTitle("Reco Pt [GeV]");
	muonPtGM->Draw("COLZ");
	C->Print((outputdir+"MuonPt2DGM.png").c_str());
	
	muonPtDSA->GetXaxis()->SetTitle("Gen Pt [GeV]");
	muonPtDSA->GetYaxis()->SetTitle("Reco Pt [GeV]");
	muonPtDSA->Draw("COLZ");
	C->Print((outputdir+"MuonPt2DDSA.png").c_str());

	TH1D* muonPtGenGM = muonPtGM->ProjectionX();
	muonPtGenGM->SetTitle(" Gen Muon Pt");
	muonPtGenGM->GetXaxis()->SetTitle("Pt [GeV]");
	muonPtGenGM->DrawNormalized();
	C->Print((outputdir+"MuonPtGenGM.png").c_str());

	TH1D* muonPtGenDSA = muonPtDSA->ProjectionX();
	muonPtGenDSA->SetTitle(" Gen Muon Pt");
	muonPtGenDSA->GetXaxis()->SetTitle("Pt [GeV]");
	muonPtGenDSA->DrawNormalized();

	TH1D* muonPtReco = muonPtGM->ProjectionY();
	muonPtReco->SetTitle("Reco Muon Pt");
	muonPtReco->GetXaxis()->SetTitle("Pt [GeV]");
	muonPtReco->DrawNormalized();
	C->Print((outputdir+"MuonPtRecoGM.png").c_str());
	
	TH1D* muonPtRecoDSA = muonPtDSA->ProjectionY();
	muonPtRecoDSA->SetTitle("Reco Muon Pt");
	muonPtRecoDSA->GetXaxis()->SetTitle("Pt [GeV]");
	muonPtRecoDSA->DrawNormalized();
	C->Print((outputdir+"MuonPtRecoDSA.png").c_str());
	
	deltaRHistGM->GetXaxis()->SetTitle("delta R");
	deltaRHistGM->DrawNormalized();
	C->Print((outputdir+"deltaRHisGMt.png").c_str());

	deltaRHistDSA->GetXaxis()->SetTitle("delta R");
	deltaRHistDSA->DrawNormalized();
	C->Print((outputdir+"deltaRHistDSA.png").c_str());

	deltaRGenPairGM->GetXaxis()->SetTitle("delta R");
	deltaRGenPairGM->DrawNormalized();
	C->Print((outputdir+"deltaRGenPairGM.png").c_str());

	deltaRRecoPairGM->GetXaxis()->SetTitle("delta R");
	deltaRRecoPairGM->DrawNormalized();
	C->Print((outputdir+"deltaRRecoPairGM.png").c_str());

	deltaRGenPairDSA->GetXaxis()->SetTitle("delta R");
	deltaRGenPairDSA->DrawNormalized();
	C->Print((outputdir+"deltaRGenPairDSA.png").c_str());
	
	deltaRRecoPairDSA->GetXaxis()->SetTitle("delta R");
	deltaRRecoPairDSA->DrawNormalized();
	C->Print((outputdir+"deltaRRecoPairDSA.png").c_str());

	muonPhiGM->GetXaxis()->SetTitle("Gen");
	muonPhiGM->GetYaxis()->SetTitle("Reco");
	muonPhiGM->Draw("COLZ");
	C->Print((outputdir+"LeadingMuonPhi2DGM.png").c_str());

	muonPhiDSA->GetXaxis()->SetTitle("Gen");
	muonPhiDSA->GetYaxis()->SetTitle("Reco");
	muonPhiDSA->Draw("COLZ");
	C->Print((outputdir+"LeadingMuonPhi2DDSA.png").c_str());
/*
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
*/	

	muonEtaGM->GetXaxis()->SetTitle("Gen");
	muonEtaGM->GetYaxis()->SetTitle("Reco");
	muonEtaGM->Draw("COLZ");
	C->Print((outputdir+"LeadingMuonEta2DGM.png").c_str());

	muonEtaDSA->GetXaxis()->SetTitle("Gen");
	muonEtaDSA->GetYaxis()->SetTitle("Reco");
	muonEtaDSA->Draw("COLZ");
	C->Print((outputdir+"LeadingMuonEta2DGMDSA.png").c_str());
/*
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
*/

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

	numGenMuons->GetXaxis()->SetTitle("Number of Gen Level Muons");
	numGenMuons->DrawNormalized();
	C->Print((outputdir+"numGenMuons.png").c_str());

	numGMRecoMuons->GetXaxis()->SetTitle("Number of GM Reco LevelMuons");
	numGMRecoMuons->DrawNormalized();
	C->Print((outputdir+"numGMRecoMuons.png").c_str());
	
	numDSARecoMuons->GetXaxis()->SetTitle("Number of DSA Reco Level Muons");
	numDSARecoMuons->DrawNormalized();
	C->Print((outputdir+"numDSARecoMuons.png").c_str());

	N2C1dPhi->GetXaxis()->SetTitle("delta Phi");
	N2C1dPhi->DrawNormalized();
	C->Print((outputdir+"N2C1dPhi.png").c_str());

	N2JdPhi->GetXaxis()->SetTitle("delta Phi");
	N2JdPhi->DrawNormalized();
	C->Print((outputdir+"N2JdPhi.png").c_str());

	C1JdPhi->GetXaxis()->SetTitle("delta Phi");
	C1JdPhi->DrawNormalized();
	C->Print((outputdir+"C1JdPhi.png").c_str());
	
	numJetsPerEvent->GetXaxis()->SetTitle("Number of Jets Per Event");
	numJetsPerEvent->DrawNormalized();
	C->Print((outputdir+"numJetsPerEvent.png").c_str());

	C1J_N2C1->GetXaxis()->SetTitle("C1/Jet Delta Phi");
	C1J_N2C1->GetYaxis()->SetTitle("N2/C1 Delta Phi");
	C1J_N2C1->Draw("COLZ");
	C->Print((outputdir+"C1J_N2C1.png").c_str());

	C1J_N2J->GetXaxis()->SetTitle("C1/Jet Delta Phi");
	C1J_N2J->GetYaxis()->SetTitle("N2/Jet Delta Phi");
	C1J_N2J->Draw("COLZ");
	C->Print((outputdir+"C1J_N2J.png").c_str());
	
	N2C1_N2J->GetXaxis()->SetTitle("N2/C1 Delta Phi");
	N2C1_N2J->GetYaxis()->SetTitle("N2/Jet Delta Phi");
	N2C1_N2J->Draw("COLZ");
	C->Print((outputdir+"N2C1_N2J.png").c_str());

	C1PtvsN2C1dPhi->GetXaxis()->SetTitle("N2/C1 Delta Phi");
	C1PtvsN2C1dPhi->GetYaxis()->SetTitle("C1 Pt");
	C1PtvsN2C1dPhi->Draw("COLZ");
	C->Print((outputdir+"C1PtvsN2C1dPhi.png").c_str());

	N2PtvsN2C1dPhi->GetXaxis()->SetTitle("N2/C1 Delta Phi");
	N2PtvsN2C1dPhi->GetYaxis()->SetTitle("N2 Pt");
	N2PtvsN2C1dPhi->Draw("COLZ");
	C->Print((outputdir+"N2PtvsN2C1dPhi.png").c_str());

	C1PtvsC1JdPhi->GetXaxis()->SetTitle("C1/Jet Delta Phi");
	C1PtvsC1JdPhi->GetYaxis()->SetTitle("C1 Pt");
	C1PtvsC1JdPhi->Draw("COLZ");
	C->Print((outputdir+"C1PtvsC1JdPhi.png").c_str());

	N2PtvsN2JdPhi->GetXaxis()->SetTitle("N2/Jet Delta Phi");
	N2PtvsN2JdPhi->GetYaxis()->SetTitle("N2 Pt");
	N2PtvsN2JdPhi->Draw("COLZ");
	C->Print((outputdir+"N2PtvsN2JdPhi.png").c_str());

	dPhiGenMuonsDSA->GetXaxis()->SetTitle("delta Phi");
	dPhiGenMuonsDSA->DrawNormalized();
	C->Print((outputdir+"dPhiGenMuonsDSA.png").c_str());
	
	dPhiRecoMuonsDSA->GetXaxis()->SetTitle("delta Phi");
	dPhiRecoMuonsDSA->DrawNormalized();
	C->Print((outputdir+"dPhiRecoMuonsDSA.png").c_str());
	
	dPhiGenMuonsGM->GetXaxis()->SetTitle("delta Phi");
	dPhiGenMuonsGM->DrawNormalized();
	C->Print((outputdir+"dPhiGenMuonsGM.png").c_str());
	
	dPhiRecoMuonsGM->GetXaxis()->SetTitle("delta Phi");
	dPhiRecoMuonsGM->DrawNormalized();
	C->Print((outputdir+"dPhiRecoMuonsGM.png").c_str());

	N2PtvsC1Pt->GetXaxis()->SetTitle("N2 Pt");
	N2PtvsC1Pt->GetYaxis()->SetTitle("C1 Pt");
	N2PtvsC1Pt->Draw("COLZ");
	C->Print((outputdir+"N2PtvsC1Pt.png").c_str());

	GenMuonMass->SetStats(0);
	GenMuonMass->SetLineColor(2); 
	GenMuonMass->GetXaxis()->SetTitle("Mass [GeV]");
	GenMuonMass->SetTitle("Invariant Mass Distributions");
	GenMuonMass->DrawNormalized();
	DSARecoMuonMass->SetStats(0); DSARecoMuonMass->SetLineColor(3); DSARecoMuonMass->DrawNormalized("Same");
	GMRecoMuonMass->SetStats(0); GMRecoMuonMass->SetLineColor(4); GMRecoMuonMass->DrawNormalized("Same");
	GenMuonMass->GetXaxis()->SetTitle("Mass [GeV]");
	GenMuonMass->SetTitle("Invariant Mass Distributions");
	TLegend *legend = new TLegend(0.6,0.7, 0.9,0.9);
	legend->AddEntry("GenMuonMass", "Gen Muon Pair", "l"); legend->AddEntry("GMRecoMuonMass", "GM Reco Muon Pair", "l"); legend->AddEntry("DSARecoMuonMass", "DSA Reco Muon Pair", "l");
	legend->Draw();
	C->Print((outputdir+"InvariantMassPlot.png").c_str());

	outfile->Write();


}
