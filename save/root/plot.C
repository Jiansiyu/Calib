#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "string"
#include "TCanvas.h"
#include "map"
#include "TLine.h"
#include "iostream"
const UInt_t NSieveRow = 7;
const UInt_t NSieveCol = 13;

void plot(unsigned int runID=0){
    TFile *f = new TFile(Form("./result/trainRes_%d.root",runID),"RECREATE");
    TTree *T = new TTree("ntuple","data from ascii file");
    Long64_t nlines = T->ReadFile(Form("../result/trainRes_run_%d.csv",runID),"evtID:runID:CutID:SieveRowID:SieveColID:bpmX:bpmY:focal_x:focal_y:focal_th:focal_ph:targCalTh:targCalPh:predicted_theta:predicted_phi");
    printf(" found %lld points\n",nlines);


    // save the file into histogram and get the residual
    std::map<int, TH1F *> predict_theta_list;
    std::map<int, TH1F *> predict_phi_list;

    std::map<int, TH1F *> theory_theta_list;
    std::map<int, TH1F *> theory_phi_list;

    // loop over the sieve col and row

    for (auto rowID = 0; rowID < NSieveRow; rowID++){
        for(auto colID = 0; colID < NSieveCol; colID++){
            int sieveUID = colID * NSieveRow + rowID;

            predict_phi_list[sieveUID] = new TH1F(Form("predict_phi_histo%d",sieveUID),Form("predict_phi_histo%d",sieveUID),1000,-0.045,0.045);
            predict_theta_list[sieveUID] = new TH1F(Form("predict_theta_histo%d",sieveUID),Form("predict_theta_histo%d",sieveUID),1000,-0.045,0.045);

            theory_phi_list[sieveUID] = new TH1F(Form("theory_phi_histo%d",sieveUID),Form("theory_phi_histo%d",sieveUID),1000,-0.045,0.045);
            theory_theta_list[sieveUID] = new TH1F(Form("theory_theta_histo%d",sieveUID),Form("theory_theta_histo%d",sieveUID),1000,-0.045,0.045);

            // project the values
            T->Project(predict_theta_list[sieveUID]->GetName(),"predicted_theta", Form("(SieveRowID==%d)&&(SieveColID==%d)",rowID,colID));
            T->Project(predict_phi_list[sieveUID]->GetName(),"predicted_phi", Form("(SieveRowID==%d)&&(SieveColID==%d)",rowID,colID));
            T->Project(theory_phi_list[sieveUID]->GetName(),"targCalPh", Form("(SieveRowID==%d)&&(SieveColID==%d)",rowID,colID));
            T->Project(theory_theta_list[sieveUID]->GetName(),"targCalTh", Form("(SieveRowID==%d)&&(SieveColID==%d)",rowID,colID));
        }
    }

//    TCanvas *canv_residual = new TCanvas("residual_plot","residual_plot",1960,1080);
//    canv_residual->Divide(2,1);



    TCanvas *canv_2d = new TCanvas("Target_theta_phi","Target_theta_phi",1960,1080);
    canv_2d->cd();
    // plot 2-D histogram
    TH2F *theta_phihh = new TH2F(Form("theta_phi_hh",runID),
                                 Form("Target Theta vs. Phi plot / run.%d",runID),
                                 1000,-0.045,0.045,
                                 1000,-0.045,0.045);
    theta_phihh->GetXaxis()->SetTitle(Form("#Phi"));
    theta_phihh->GetYaxis()->SetTitle(Form("#Theta"));




    T->Project(theta_phihh->GetName(),"predicted_theta:predicted_phi");
    theta_phihh->Draw("zcol");

    // plot the theoretical line

    for (auto mapiter = theory_theta_list.begin(); mapiter!=theory_theta_list.end();mapiter++){
        if (mapiter->second->GetEntries() > 100){
            float theo_theta = mapiter->second->GetMean();
            // plot the theoretical value in the canvas
            TLine *line = new TLine((theta_phihh->GetXaxis()->GetXmin()),theo_theta,(theta_phihh->GetXaxis()->GetXmax()),theo_theta);
            line->SetLineWidth(2);
            line->SetLineColor(3);
            line->Draw("same");
        }
    }

    for(auto  mapIter = theory_phi_list.begin(); mapIter != theory_phi_list.end(); mapIter++){
        if(mapIter->second->GetEntries()>100){
            float theo_phi = mapIter->second->GetMean();
            TLine *line = new TLine(theo_phi,(theta_phihh->GetYaxis()->GetXmin()),theo_phi,(theta_phihh->GetYaxis()->GetXmax()));
            line->SetLineWidth(2);
            line->SetLineColor(3);
            line->Draw("same");
        }
    }

    canv_2d->Update();
    canv_2d->Write();
    theta_phihh->Write();


    TCanvas *canv_residual = new TCanvas("residual_plot","residual_plot",1960,1080);
    canv_residual->Divide(1,2);


    TH1F *sieveThetaResidualDistri=new TH1F("SieveThetaResiduals","Sieve #Theta' Residuals",NSieveCol*NSieveRow+10,0,NSieveCol*NSieveRow+10);
    sieveThetaResidualDistri->GetYaxis()->SetRangeUser(-0.003,0.003);
    sieveThetaResidualDistri->GetXaxis()->SetTitle("SieveHoleID");
    sieveThetaResidualDistri->GetYaxis()->SetTitle("#Theta' Residuals");

    for (auto mapiter = predict_theta_list.begin(); mapiter!=predict_theta_list.end(); mapiter++){
        if ((mapiter->second->GetEntries())>200){
            int sieveUID = mapiter->first;
            float residual_mean = mapiter->second->GetMean() - theory_theta_list[mapiter->first]->GetMean();
            float residual_std = mapiter->second->GetRMS();

//            std::cout<<"runID : "<<sieveUID<<"  mean: "<<residual_mean<<std::endl;

            sieveThetaResidualDistri->Fill(sieveUID,residual_mean);
            sieveThetaResidualDistri->SetBinError(sieveUID+1,residual_std);

        }
    }

    TH1F *sievePhiResidualDistri=new TH1F("SievePhiResiduals","Sieve #Phi' Residuals",NSieveCol*NSieveRow+10,0,NSieveCol*NSieveRow+10);
    sievePhiResidualDistri->GetYaxis()->SetRangeUser(-0.003,0.003);
    sievePhiResidualDistri->GetXaxis()->SetTitle("SieveHoleID");
    sievePhiResidualDistri->GetYaxis()->SetTitle("#Phi' Residuals");

    for(auto mapiter = predict_phi_list.begin(); mapiter!=predict_phi_list.end(); mapiter++){
        if ((mapiter->second->GetEntries())>200){
            int sieveUID = mapiter->first;
            float residual_mean = mapiter->second->GetMean() - theory_phi_list[mapiter->first]->GetMean();
            float residual_std = mapiter->second->GetRMS();

            sievePhiResidualDistri->Fill(sieveUID,residual_mean);
            sievePhiResidualDistri->SetBinError(sieveUID+1,residual_std);
        }
    }

    canv_residual->cd(1);
    sieveThetaResidualDistri->SetLineWidth(2);
    sieveThetaResidualDistri->SetMarkerStyle(20);

    sieveThetaResidualDistri->Draw("E1");

    TLine *line1=new TLine(0,0,100,0);
    line1->SetLineColor(3);
    line1->SetLineWidth(2);
    line1->Draw("same");

    TLine *line2=new TLine(0,0.001,100,0.001);
    line2->SetLineColor(42);
    line2->SetLineWidth(2);
    line2->Draw("same");

    TLine *line3=new TLine(0,-0.001,100,-0.001);
    line3->SetLineColor(42);
    line3->SetLineWidth(2);
    line3->Draw("same");


    canv_residual->cd(2);
    sievePhiResidualDistri->SetLineWidth(2);
    sievePhiResidualDistri->SetMarkerStyle(20);
    sievePhiResidualDistri->Draw("E1");
    line1->Draw("same");
    line2->Draw("same");
    line3->Draw("same");
    canv_residual->Update();
    canv_residual->Write();


    canv_residual->SaveAs(Form("./result/run%d_residual.jpg",runID));
    canv_2d->SaveAs(Form("./result/run%d_2d.jpg",runID));
    T->Write();
    f->Write();
}
