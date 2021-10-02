//#include "TFile"
//#include "TTree"
//#include "TString"
#include "string"
void plot(unsigned int runID=0){
    TFile *f = new TFile(Form("./trainRes_%d.root",runID),"RECREATE");
    TH1F *h1 = new TH1F("h1","x distribution",100,-4,4);
    TTree *T = new TTree("ntuple","data from ascii file");
    Long64_t nlines = T->ReadFile(Form("../result/trainRes_run_%d.csv",runID),"evtID:runID:CutID:SieveRowID:SieveColID:bpmX:bpmY:focal_x:focal_y:focal_th:focal_ph:targCalTh:targCalPh:predicted_theta:predicted_phi");
    printf(" found %lld points\n",nlines);
//    T->Draw("x","z>2");
    T->Write();
    T->Write();

}
