//eliminare librerie inutili

#include <iostream>           
#include <TGraphErrors.h>     
#include <TAxis.h>            
#include <TCanvas.h>          
#include <TF1.h>              
#include <iomanip>           
#include <cmath>
#include <math.h>

using namespace std; 


void estinzione(){

    cout << "£££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££"<< endl;

    cout << fixed;

    gStyle->SetOptFit(111);
    gStyle->SetOptTitle(1); 
    gStyle->SetOptStat(0);
    gStyle->SetStatBorderSize(0);
    gStyle->SetStatX(0.895); 
    gStyle->SetStatY(0.85);

    //possibile errore di parallasse
    //altro operatore controlla che la squadra, appaggiata al bordo del becher sia ortogonale al piano del tavolo

    //cm
    //a sensazione
    float delta_becher = 0.17 + 0.12 / 2;

    //DIN EN ISO 3819:2015-12 defines the following types and sizes:
    //https://en.wikipedia.org/wiki/Beaker_(glassware)

    //float s_delta_becher = delta_becher / 20; // stimiamo un 10% di errore
    //float s_delta_becher = delta_becher / 10;
    //float s_delta_becher = 0.000001;
    float s_delta_becher = 0.17 - 0.12 / 2;

    //μA
    //HP 34401A a lampada spenta e con becher in posizione;
    float fondo = 0.005;
    float s_fondo = 0.001;

    //TCanvas * c_all = new TCanvas("c_all", "grafico 0", 0, 0, 1280, 720);
    //c_all->Divide(3,1,0,0);

    cout << "_______________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
    cout << "_________________________________________________________________________________________________________________COEFFICIENTE DI ESTINZIONE SUCCO DI FRUTTA____________________________________________________________________________________________________________" << endl;
    cout << "_______________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;


    const int nmisure_succo = 13;

    //cm
    //squadra
    float d_succo [] = {3.4, 4.5, 4.9, 5.9, 6.8, 7.1, 7.9, 8.4, 8.8, 9.1, 9.7, 10.3, 12.3};
    float s_d_succo [nmisure_succo];
    for (int i = 0; i < nmisure_succo; ++i){s_d_succo [i] = 0.1;}

    //cm
    //con correzioni varie
    float D_succo[nmisure_succo]; 
    for (int i = 0; i < nmisure_succo; ++i){D_succo [i] = d_succo[i] - delta_becher + 0.5;} //0.5 è la parte non graduata della squadra
    float s_D_succo[nmisure_succo];
    for (int i = 0; i < nmisure_succo; ++i){s_D_succo[i] = sqrt(pow(s_d_succo[i],2) + pow(s_delta_becher,2));}   
    //for (int i = 0; i < nmisure_succo; ++i){s_D_succo[i] = 0.000001;}   
    

    //μA
    //HP 34401A
    float i_D_succo[] = {18.6, 14.7, 11.8, 8, 5.8, 4.8, 3.8, 3.3, 2.8, 2.3, 2.1, 1.8, 0.9};
    float s_i_D_succo[nmisure_succo];
    //sottraggo il fondo 
    for(int i = 0 ; i < nmisure_succo; i++){i_D_succo [i] -= fondo;}
    for (int i = 0; i < nmisure_succo; ++i){s_i_D_succo[i] = sqrt(pow(0.1, 2) + pow(s_fondo, 2));}
    //for (int i = 0; i < nmisure_succo; ++i){s_i_D_succo[i] = 0.0001;}
  
    TCanvas * c_i_D_succo_D_succo = new TCanvas("c_i_d_succo_d_succo", "grafico succo", 0, 0 ,1280, 720);
    c_i_D_succo_D_succo->SetFillColor(0);
    c_i_D_succo_D_succo->cd();  

    //c_all->cd(1);

    TGraphErrors * g_i_D_succo_D_succo = new TGraphErrors(nmisure_succo, D_succo, i_D_succo, s_D_succo, s_i_D_succo);
    g_i_D_succo_D_succo->SetMarkerSize(0.4);
    g_i_D_succo_D_succo->SetMarkerStyle(20);
    g_i_D_succo_D_succo->SetTitle("#scale[0.9]{Funzione di fit:    I_{D} = I_{D}_{0}e^{-KD}    Liquido: succo mix di frutta }");
    g_i_D_succo_D_succo->GetXaxis()->SetTitle("D (cm)");
    g_i_D_succo_D_succo->GetYaxis()->SetTitle("I_{d} (#muA)");
    g_i_D_succo_D_succo->Draw("AP");

    cout << "\n\n --- Ipotesi  [0] * pow(e, 0 - [1] * x) --- \n" << endl;

    float inf_range_fz_succo = *std::min_element(std::begin(D_succo), std::end(D_succo));
    float sup_range_fz_succo = *std::max_element(std::begin(D_succo), std::end(D_succo));
    TF1 * fz_i_D_succo_D_succo = new TF1("fz_i_d_succo_D_succo","[0] * pow(e, 0 - [1] * x)", inf_range_fz_succo - inf_range_fz_succo / 20, sup_range_fz_succo + sup_range_fz_succo / 20);
    fz_i_D_succo_D_succo->SetParNames("I_{D}_{0} (#muA)","K (cm^{-1})");

    float guess_par0 = 58.23;
    float guess_par1 = 0.34;

    fz_i_D_succo_D_succo->SetParameter(0, guess_par0);
    fz_i_D_succo_D_succo->SetParameter(1, guess_par1); 

    //fz_i_D_succo_D_succo->SetParLimits(0, guess_par0 - guess_par0 / 10, guess_par0 + guess_par0 / 10);            
    //fz_i_D_succo_D_succo->SetParLimits(1, guess_par1 - guess_par1 / 10, guess_par1 + guess_par1 / 10); 

    fz_i_D_succo_D_succo->SetLineColor(42);
    g_i_D_succo_D_succo->Fit(fz_i_D_succo_D_succo,"ERM+");

    cout << "Chi^2:" << fz_i_D_succo_D_succo->GetChisquare() << ", number of DoF: " << fz_i_D_succo_D_succo->GetNDF() << " (Probability: " << fz_i_D_succo_D_succo->GetProb() << ")." << endl;
    cout << "-----------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
  

    cout << "_______________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
    cout << "_________________________________________________________________________________________________________________COEFFICIENTE DI ESTINZIONE GINGER____________________________________________________________________________________________________________" << endl;
    cout << "_______________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;

   
    const int nmisure_ginger = 14;

    //cm
    //squadra
    float d_ginger [] = {2.2, 3.4, 4.1, 4.9, 5.8, 6.3, 7, 7.9, 8.7, 9.5, 10.1, 10.7, 11.6, 12.7};
    float s_d_ginger [nmisure_ginger];
    for (int i = 0; i < nmisure_ginger; ++i){s_d_ginger [i] = 0.1;}

    //cm
    //con correzioni varie
    float D_ginger[nmisure_ginger]; 
    for (int i = 0; i < nmisure_ginger; ++i){D_ginger [i] = d_ginger[i] - delta_becher + 0.5;} //0.5 è la parte non graduata della squadra
    float s_D_ginger[nmisure_ginger];
    for (int i = 0; i < nmisure_ginger; ++i){s_D_ginger[i] = sqrt(pow(s_d_ginger[i],2) + pow(s_delta_becher,2));}   
    //for (int i = 0; i < nmisure_ginger; ++i){s_D_ginger[i] = 0.000001;}   
      

    //μA
    //HP 34401A
    float i_D_ginger [] = {34, 24.8, 19.6, 15.6, 12, 10.1, 8.4, 6.6, 5.2, 4, 3.3, 2.7, 2.2, 1.6};
    float s_i_D_ginger[nmisure_ginger];
    //sottraggo il fondo 
    for(int i = 0 ; i < nmisure_ginger; i++){i_D_ginger [i] -= fondo;}
    for (int i = 0; i < nmisure_ginger; ++i){s_i_D_ginger[i] = sqrt(pow(0.1, 2) + pow(s_fondo, 2));}
    //for (int i = 0; i < nmisure_ginger; ++i){s_i_D_ginger[i] = 0.0001;}

  
    TCanvas * c_i_d_ginger_D_ginger = new TCanvas("c_i_d_ginger_d_ginger", "grafico ginger", 0, 0 ,1280, 720);
    c_i_d_ginger_D_ginger->SetFillColor(0);
    c_i_d_ginger_D_ginger->cd();  

    //c_all->cd(2);

    TGraphErrors * g_i_D_ginger_D_ginger = new TGraphErrors(nmisure_ginger, D_ginger, i_D_ginger, s_D_ginger, s_i_D_ginger);
    g_i_D_ginger_D_ginger->SetMarkerSize(0.4);
    g_i_D_ginger_D_ginger->SetMarkerStyle(20);
    g_i_D_ginger_D_ginger->SetTitle("#scale[0.9]{Funzione di fit:    I_{D} = I_{D}_{0}e^{-KD}    Liquido: ginger}");
    g_i_D_ginger_D_ginger->GetXaxis()->SetTitle("D (cm)");
    g_i_D_ginger_D_ginger->GetYaxis()->SetTitle("I_{d} (#muA)");
    g_i_D_ginger_D_ginger->Draw("AP");

    cout << "\n\n --- Ipotesi  [0] * pow(e, 0 - [1] * x) --- \n" << endl;

    float inf_range_fz_ginger = *std::min_element(std::begin(D_ginger),std::end(D_ginger));
    float sup_range_fz_ginger = *std::max_element(std::begin(D_ginger),std::end(D_ginger));
    TF1 * fz_i_D_ginger_D_ginger = new TF1("fz_i_D_ginger_D_ginger","[0] * pow(e, 0 - [1] * x)", inf_range_fz_ginger - inf_range_fz_ginger / 20, sup_range_fz_ginger + sup_range_fz_ginger / 20);
    fz_i_D_ginger_D_ginger->SetParNames("I_{D}_{0} (#muA)","K (cm^{-1})");

    float guess_par0_ginger = 63.69;
    float guess_par1_ginger = 0.293;

    fz_i_D_ginger_D_ginger->SetParameter(0, guess_par0_ginger);
    fz_i_D_ginger_D_ginger->SetParameter(1, guess_par1_ginger); 

    //fz_i_D_ginger_D_ginger->SetParLimits(0, guess_par0_ginger - guess_par0_ginger / 10, guess_par0_ginger + guess_par0_ginger / 10);            
    //fz_i_D_ginger_D_ginger->SetParLimits(1, guess_par1_ginger - guess_par1_ginger / 10, guess_par1_ginger + guess_par1_ginger / 10); 

    fz_i_D_ginger_D_ginger->SetLineColor(46);
    g_i_D_ginger_D_ginger->Fit(fz_i_D_ginger_D_ginger,"ERM+");

    cout << "Chi^2:" << fz_i_D_ginger_D_ginger->GetChisquare() << ", number of DoF: " << fz_i_D_ginger_D_ginger->GetNDF() << " (Probability: " << fz_i_D_ginger_D_ginger->GetProb() << ")." << endl;
    cout << "-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
      
    cout << "_______________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
    cout << "_________________________________________________________________________________________________________________COEFFICIENTE DI ESTINZIONE COLA____________________________________________________________________________________________________________" << endl;
    cout << "_______________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;

   
    const int nmisure_cola = 12;

    //cm
    //squadra
    float d_cola [] = {1.8, 2.9, 3.4, 3.9, 4.2, 4.6, 4.9, 5.2, 5.6, 6, 6.5, 9.2};
    float s_d_cola [nmisure_cola];
    for (int i = 0; i < nmisure_cola; ++i){s_d_cola [i] = 0.1;}

    //cm
    //con correzioni varie
    float D_cola[nmisure_cola]; 
    for (int i = 0; i < nmisure_cola; ++i){D_cola [i] = d_cola[i] - delta_becher + 0.5;} //0.5 è la parte non graduata della squadra
    float s_D_cola[nmisure_cola];
    for (int i = 0; i < nmisure_cola; ++i){s_D_cola[i] = sqrt(pow(s_d_cola[i],2) + pow(s_delta_becher,2));}   
    //for (int i = 0; i < nmisure_cola; ++i){s_D_cola[i] = 0.000001;}   

    //μA
    //HP 34401A
    float i_D_cola[] = {15.6, 6.9, 5, 3.5, 2.7, 2.2, 1.8, 1.4, 1.1, 0.9, 0.6, 0.2};
    float s_i_D_cola[nmisure_cola];
    //sottraggo il fondo 
    for(int i = 0 ; i < nmisure_cola; i++){i_D_cola [i] -= fondo;}
    for (int i = 0; i < nmisure_cola; ++i){s_i_D_cola[i] = sqrt(pow(0.1, 2) + pow(s_fondo, 2));}
    //for(int i = 0; i < nmisure_cola; ++i){s_i_D_cola[i] = 0.0001;}

  
    TCanvas * c_i_d_cola_D_cola = new TCanvas("c_i_d_cola_d_cola", "grafico cola", 0, 0, 1280, 720);
    c_i_d_cola_D_cola->SetFillColor(0);
    c_i_d_cola_D_cola->cd();  

    //c_all->cd(3);

    TGraphErrors * g_i_D_cola_D_cola = new TGraphErrors(nmisure_cola, D_cola, i_D_cola, s_D_cola, s_i_D_cola);
    g_i_D_cola_D_cola->SetMarkerSize(0.4);
    g_i_D_cola_D_cola->SetMarkerStyle(20);
    g_i_D_cola_D_cola->SetTitle("#scale[0.9]{Funzione di fit:    I_{D} = I_{D}_{0}e^{-KD}    Liquido: cola}");
    g_i_D_cola_D_cola->GetXaxis()->SetTitle("D (cm)");
    g_i_D_cola_D_cola->GetYaxis()->SetTitle("I_{d} (#muA)");
    g_i_D_cola_D_cola->Draw("AP");

    cout << "\n\n --- Ipotesi  [0] * pow(e, 0 - [1] * x) --- \n" << endl;

    float inf_range_fz_cola = *std::min_element(std::begin(D_cola), std::end(D_cola));
    float sup_range_fz_cola = *std::max_element(std::begin(D_cola), std::end(D_cola));
    TF1 * fz_i_D_cola_D_cola = new TF1("fz_i_d_cola_D_cola","[0] * pow(e, 0 - [1] * x)", inf_range_fz_cola - inf_range_fz_cola / 20, sup_range_fz_cola + sup_range_fz_cola / 20);
    fz_i_D_cola_D_cola->SetParNames("I_{D}_{0} (#muA)","K (cm^{-1})");

    float guess_par0_cola = 49.51;
    float guess_par1_cola = 0.679;

    fz_i_D_cola_D_cola->SetParameter(0, guess_par0_cola);
    fz_i_D_cola_D_cola->SetParameter(1, guess_par1_cola); 

    //fz_i_d_cola_D_cola->SetParLimits(0, guess_par0_cola - guess_par0_cola / 10, guess_par0_cola + guess_par0_cola / 10);            
    //fz_i_d_cola_D_cola->SetParLimits(1, guess_par1_cola - guess_par1_cola / 10, guess_par1_cola + guess_par1_cola / 10); 

    fz_i_D_cola_D_cola->SetLineColor(28);
    g_i_D_cola_D_cola->Fit(fz_i_D_cola_D_cola,"ERM+");

    cout << "Chi^2:" << fz_i_D_cola_D_cola->GetChisquare() << ", number of DoF: " << fz_i_D_cola_D_cola->GetNDF() << " (Probability: " << fz_i_D_cola_D_cola->GetProb() << ")." << endl;
    cout << "-----------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

}
  
