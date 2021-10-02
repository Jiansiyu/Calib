#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "string"
#include "TCanvas.h"

void plot(unsigned int runID=0){
    TFile *f = new TFile(Form("./trainRes_%d.root",runID),"RECREATE");
    TH1F *h1 = new TH1F("h1","x distribution",100,-4,4);
    TTree *T = new TTree("ntuple","data from ascii file");
    Long64_t nlines = T->ReadFile(Form("../result/trainRes_run_%d.csv",runID),"evtID:runID:CutID:SieveRowID:SieveColID:bpmX:bpmY:focal_x:focal_y:focal_th:focal_ph:targCalTh:targCalPh:predicted_theta:predicted_phi");
    printf(" found %lld points\n",nlines);


    TCanvas *canv_2d = new TCanvas("Target_theta_phi","Target_theta_phi",1960,1080);
    canv_2d->cd();
    // plot 2-D histogram
    TH2F *theta_phihh = new TH2F(Form("theta_phi_hh",runID),
                                 Form("Target Theta vs. Phi plot / run.%d",runID),
                                 1000,-0.045,0.045,
                                 1000,-0.045,0.045);

    T->Project(theta_phihh->GetName(),"predicted_theta:predicted_phi");
    theta_phihh->Draw("zcol");
    canv_2d->Update();
    canv_2d->Write();
    theta_phihh->Write();

    // save the file into histogram and get the residual
    auto CutIDMax = T->GetMaximum("CutID");
    auto CutIDMin = T->GetMinimum("CutID");



    T->Write();
    f->Write();
}
