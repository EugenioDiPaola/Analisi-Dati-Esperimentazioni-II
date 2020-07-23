#include <iostream>           
#include <TGraphErrors.h>     
#include <TAxis.h>            
#include <TCanvas.h>          
#include <TF1.h>              
#include <iomanip>           
#include <cmath>
#include <math.h>

using namespace std; 


void malus(){

    cout << "£££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££"<< endl;

    cout << fixed;

    gStyle->SetOptFit(111);
    gStyle->SetOptTitle(1); 
    gStyle->SetOptStat(0);
    gStyle->SetStatBorderSize(0);
    gStyle->SetStatX(0.895); 
    gStyle->SetStatY(0.87);
    gStyle->SetStatW(0.12); 
    gStyle->SetStatH(0.12);
    

cout << "__________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
cout << "__________________________________________________________________________________________________LEGGE DI MALUS SENZA LAMINA QUARTO D'ONDA_______________________________________________________________________________________________" << endl;
cout << "__________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;

 
    const int nmisure_malus1 = 36;

    const float PI = 3.1415926535;

    //°
    //scala graduata del filtro analizzatore
    float theta1 [nmisure_malus1];
    for (int i = 0; i < nmisure_malus1; ++i){theta1 [i] = 10 * i;}

    //errore di sensibilità
    float s_theta1 [nmisure_malus1];
    for(int i = 0 ; i < nmisure_malus1; i++){s_theta1 [i] = 1;}

    //μA
    //Amprobe 37XR-A HP 34401A
    //float i_theta1[] = {4.89, 2.48, 0.76, 0.09, 0.59, 2.11, 4.61, 7.63, 10.68, 13.56, 15.85, 17.38, 17.96, 17.54, 16.14, 13.9, 11.11, 7.95, 4.94, 2.36, 0.73, 0.09, 0.58, 2.16, 4.59, 7.58, 10.67, 13.66, 16.03, 17.56, 18.06, 17.6, 16.08, 13.83, 10.98, 7.87};    //errore sull'ultima cifra che non oscilla immediatamente
    float i_theta1 [] = {4.9, 2.5, 0.8, 0.1, 0.6, 2.1, 4.6, 7.6, 10.7, 13.6, 15.8, 17.4, 18.00, 17.5, 16.1, 14.0, 11.1, 7.9, 4.9, 2.4, 0.7, 0.1, 0.6, 2.2, 5.0, 7.6, 10.7, 13.7, 16.0, 17.6, 18, 17.6, 16.1, 13.8, 11, 7.9};
    //errore sull'ultima cifra non oscillante
    float s_i_theta1 [nmisure_malus1];
    for (int i = 0; i < nmisure_malus1; ++i){s_i_theta1 [i] = 0.1;}
    
    TCanvas * c_theta_i_theta1 = new TCanvas("c_theta_i_theta1", "grafico legge Malus 1", 0, 0 ,1280, 720);
    c_theta_i_theta1->SetFillColor(0);
    c_theta_i_theta1->cd();  

    TGraphErrors * g_theta_i_theta1 = new TGraphErrors(nmisure_malus1, theta1, i_theta1, s_theta1, s_i_theta1);
    g_theta_i_theta1->SetMarkerSize(0.4);
    g_theta_i_theta1->SetMarkerStyle(20);
    g_theta_i_theta1->SetTitle("#scale[0.85]{Funzione di fit:    i_{#theta} = i_{#theta}_{0}cos^{2}(#theta + #phi) + i_{#theta}_{offset}}");
    g_theta_i_theta1->GetXaxis()->SetTitle("#theta (#circ)");
    g_theta_i_theta1->GetYaxis()->SetTitle("i_{#theta} (#muA)");
    g_theta_i_theta1->GetXaxis()->SetLimits(-10,355);
    //0, *std::max_element(std::begin(theta1), std::end(theta1)) + *std::max_element(std::begin(theta1), std::end(theta1)) / 10);
    
    g_theta_i_theta1->Draw("AP");

    cout << "\n\n --- Ipotesi  [0] * cos((x - [1]) * 3.1415926535 / 180) * cos((x - [1]) * 3.1415926535 / 180) + [2] --- \n" << endl;
    TF1 * fz_theta_i_theta1 = new TF1("fz_theta_i_theta1","[0] * cos((x - [1]) * 3.1415926535 / 180) * cos((x - [1]) * 3.1415926535 / 180) + [2]", *std::min_element(std::begin(theta1),std::end(theta1)), *std::max_element(std::begin(theta1),std::end(theta1)));
    fz_theta_i_theta1->SetParNames("i_{#theta}_{0} (#muA)","#phi (#circ)","i_{#theta}_{offset} (#muA)");

    float guess_par0_malus1 = ((18.06 + 17.96) / 2) - ((0.09 + 0.09) / 2); //media tra i due massimi meno media tra i due minimi
    float guess_par1_malus1 = 130; //130° è il valore di theta per cui ho il primo massimo (ho un cos^2() nella formula dunque teoricamente senza sfasamento dovrei partire con il massimo in 0°)
    float guess_par2_malus1 = (0.09 + 0.09) / 2;

    cout << "guess_par0_malus1: " << guess_par0_malus1 << endl;
    cout << "guess_par1_malus1: " << guess_par1_malus1 << endl;
    cout << "guess_par2_malus1: " << guess_par2_malus1 << endl; 

    fz_theta_i_theta1->SetParameter(0, guess_par0_malus1);
    fz_theta_i_theta1->SetParameter(1, guess_par1_malus1); 
    fz_theta_i_theta1->SetParameter(2, guess_par2_malus1);

    fz_theta_i_theta1->SetParLimits(0, guess_par0_malus1 - guess_par0_malus1 / 10, guess_par0_malus1 + guess_par0_malus1 / 10);            
    fz_theta_i_theta1->SetParLimits(1, guess_par1_malus1 - guess_par1_malus1 / 10, guess_par1_malus1 + guess_par1_malus1 / 10); 
    fz_theta_i_theta1->SetParLimits(2, guess_par2_malus1 - guess_par1_malus1 / 10, guess_par2_malus1 + guess_par2_malus1 / 10);

    fz_theta_i_theta1->SetLineColor(38);
    g_theta_i_theta1->Fit(fz_theta_i_theta1," ", " ", 120, 130);

    cout << "Chi^2:" << fz_theta_i_theta1->GetChisquare() << ", number of DoF: " << fz_theta_i_theta1->GetNDF() << " (Probability: " << fz_theta_i_theta1->GetProb() << ")." << endl;
    cout << "--------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;


    TCanvas * c_theta_i_theta11 = new TCanvas("c_theta_i_theta11", "grafico legge Malus 11", 0, 0 ,1280, 720);
    c_theta_i_theta11->SetFillColor(0);
    c_theta_i_theta11->cd();  

    TGraphErrors * g_theta_i_theta11 = new TGraphErrors(nmisure_malus1, theta1, i_theta1, s_theta1, s_i_theta1);
    g_theta_i_theta11->SetMarkerSize(0.4);
    g_theta_i_theta11->SetMarkerStyle(20);
    g_theta_i_theta11->SetTitle("#scale[0.85]{Funzione di fit:    i_{#theta} = i_{#theta}_{0}cos^{2}(#theta + #phi)}");
    g_theta_i_theta11->GetXaxis()->SetTitle("#theta (#circ)");
    g_theta_i_theta11->GetYaxis()->SetTitle("i_{#theta} (#muA)");
    g_theta_i_theta11->GetXaxis()->SetLimits(-10,355);
    //0, *std::max_element(std::begin(theta1), std::end(theta1)) + *std::max_element(std::begin(theta1), std::end(theta1)) / 10);
    
    cout << "\n\n --- Ipotesi  [0] * cos((x - [1]) * 3.1415926535 / 180) * cos((x - [1]) * 3.1415926535 / 180) --- \n" << endl;
    TF1 * fz_theta_i_theta11 = new TF1("fz_theta_i_theta11","[0] * cos((x - [1]) * 3.1415926535 / 180) * cos((x - [1]) * 3.1415926535 / 180) ", *std::min_element(std::begin(theta1),std::end(theta1)), *std::max_element(std::begin(theta1),std::end(theta1)));
    fz_theta_i_theta11->SetParNames("i_{#theta}_{0} (#muA)","#phi (#circ)");

    float guess_par0_malus11 = ((18.06 + 17.96) / 2) - ((0.09 + 0.09) / 2); //media tra i due massimi meno media tra i due minimi
    float guess_par1_malus11 = 130; //130° è il valore di theta per cui ho il primo massimo (ho un cos^2() nella formula dunque teoricamente senza sfasamento dovrei partire con il massimo in 0°)
    

    cout << "guess_par0_malus11: " << guess_par0_malus11 << endl;
    cout << "guess_par1_malus11: " << guess_par1_malus11 << endl;
    
    fz_theta_i_theta11->SetParameter(0, guess_par0_malus11);
    fz_theta_i_theta11->SetParameter(1, guess_par1_malus11); 

    fz_theta_i_theta11->SetParLimits(0, guess_par0_malus11 - guess_par0_malus11 / 10, guess_par0_malus11 + guess_par0_malus11 / 10);            
    fz_theta_i_theta11->SetParLimits(1, guess_par1_malus11 - guess_par1_malus11 / 10, guess_par1_malus11 + guess_par1_malus11 / 10); 

    fz_theta_i_theta11->SetLineColor(38);
    g_theta_i_theta11->Fit(fz_theta_i_theta11,"ERM+");

    g_theta_i_theta11->Draw("AP");

    cout << "Chi^2:" << fz_theta_i_theta11->GetChisquare() << ", number of DoF: " << fz_theta_i_theta11->GetNDF() << " (Probability: " << fz_theta_i_theta11->GetProb() << ")." << endl;
    cout << "-----------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;


    cout << "__________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
    cout << "____________________________________________________________________________________________________LEGGE DI MALUS CON LAMINA QUARTO D'ONDA_______________________________________________________________________________________________" << endl;
    cout << "__________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;

    const int nmisure_malus2 = 36;

    //°
    //scala graduata del filtro analizzatore
    float theta2 [nmisure_malus2];
    for (int i = 0; i < nmisure_malus2; ++i){theta2 [i] = 10 * i;}

    //errore di sensibilità
    float s_theta2 [nmisure_malus2];
    for(int i = 0 ; i < nmisure_malus2; i++){s_theta2 [i] = 1;}

    //μA
    //Amprobe 37XR-A HP 34401A
    //float i_theta2 [] = {13.26, 13.14, 12.42, 11.12, 9.65, 7.54, 5.76, 4.46, 3.3, 2.81, 2.94, 3.62,4.6, 6.48, 8.23, 9.83, 11.54, 12.78, 13.22, 13.12, 12.48, 11.32, 9.64, 7.85, 6.02, 4.56, 3.44, 2.93, 3.03, 3.8, 5.11, 6.76, 8.52, 10.32, 11.71, 12.74};    //errore sull'ultima cifra che non oscilla immediatamente
    float i_theta2 [] = {13.3, 13.1, 12.4, 11.1, 9.6, 7.5, 5.8, 4.5, 3, 2.8, 2.9, 3.6, 4.6, 6.5, 8.2, 9.8, 11.5, 12.8, 13.2, 13.1, 12.5, 11.3, 9.6, 7.8, 6.0, 4.56, 3.4, 2.9, 3, 3.8, 5.1, 6.76, 8.5, 10.3, 11.7, 12.7};
    //errore sull'ultima cifra non oscillante
    float s_i_theta2 [nmisure_malus2];
    for (int i = 0; i < nmisure_malus2; ++i){s_i_theta2 [i] = 0.1;}
    
    TCanvas * c_theta_i_theta2 = new TCanvas("c_theta_i_theta2", "grafico legge Malus 2", 0, 0 ,1280, 720);
    c_theta_i_theta2->SetFillColor(0);
    c_theta_i_theta2->cd();  

    TGraphErrors * g_theta_i_theta2 = new TGraphErrors(nmisure_malus2, theta2, i_theta2, s_theta2, s_i_theta2);
    g_theta_i_theta2->SetMarkerSize(0.4);
    g_theta_i_theta2->SetMarkerStyle(20);
    g_theta_i_theta2->SetTitle("#scale[0.85]{Funzione di fit:    i_{#theta} = i_{#theta}_{0}cos^{2}(#theta + #phi)} + i_{#theta}_{offset}");
    g_theta_i_theta2->GetXaxis()->SetTitle("#theta (#circ)");
    g_theta_i_theta2->GetYaxis()->SetTitle("i_{#theta} (#muA)");
    g_theta_i_theta2->GetXaxis()->SetLimits(-10,355); 
    //*std::max_element(std::begin(theta2), std::end(theta2)) + *std::max_element(std::begin(theta2), std::end(theta2)) / 10);
    
    cout << "\n\n --- Ipotesi  [0] * cos((x - [1]) * 3.1415926535 / 180) * cos((x - [1]) * 3.1415926535 / 180) --- \n" << endl;
    TF1 * fz_theta_i_theta2 = new TF1("fz_theta_i_theta2","[0] * cos((x - [1]) * 3.1415926535 / 180) * cos((x - [1]) * 3.1415926535 / 180) + [2]", *std::min_element(std::begin(theta2),std::end(theta2)), *std::max_element(std::begin(theta2),std::end(theta2)));
    fz_theta_i_theta2->SetParNames("i_{#theta}_{0} (#muA)","#phi (#circ)", "i_{#theta}_{offset} (#muA)");

    float guess_par0_malus2 = ((13.26 + 13.22) / 2) - ((2.81 + 2.93) / 2); //media tra i due massimi meno media tra i due minimi
    float guess_par1_malus2 = 0; //ora dovrebbe essere rifasato 
    float guess_par2_malus2 = *std::min_element(std::begin(i_theta2), std::end(i_theta2)); //ora dovrebbe essere rifasato

    cout << "guess_par0_malus2: " << guess_par0_malus2 << endl;
    cout << "guess_par1_malus2: " << guess_par1_malus2 << endl;
    cout << "guess_par2_malus2: " << guess_par2_malus2 << endl;

    fz_theta_i_theta2->SetParameter(0, guess_par0_malus2);
    fz_theta_i_theta2->SetParameter(1, guess_par1_malus2); 

    fz_theta_i_theta2->SetParLimits(0, guess_par0_malus2 - guess_par0_malus2 / 10, guess_par0_malus2 + guess_par0_malus2 / 10);            
    fz_theta_i_theta2->SetParLimits(1, guess_par1_malus2 - guess_par1_malus2 / 10, guess_par1_malus2 + guess_par1_malus2 / 10); 
    fz_theta_i_theta2->SetParLimits(2, guess_par2_malus2 - guess_par2_malus2 / 10, guess_par2_malus2 + guess_par2_malus2 / 10); 

    fz_theta_i_theta2->SetLineColor(38);
    g_theta_i_theta2->Fit(fz_theta_i_theta2,"ERM+");

    cout << "Chi^2:" << fz_theta_i_theta2->GetChisquare() << ", number of DoF: " << fz_theta_i_theta2->GetNDF() << " (Probability: " << fz_theta_i_theta2->GetProb() << ")." << endl;
    cout << "--------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

    g_theta_i_theta2->Draw("AP");




    TCanvas * c_theta_i_theta22 = new TCanvas("c_theta_i_theta22", "grafico legge Malus 22", 0, 0 ,1280, 720);
    c_theta_i_theta22->SetFillColor(0);
    c_theta_i_theta22->cd();  

    TGraphErrors * g_theta_i_theta22 = new TGraphErrors(nmisure_malus2, theta2, i_theta2, s_theta2, s_i_theta2);
    g_theta_i_theta22->SetMarkerSize(0.4);
    g_theta_i_theta22->SetMarkerStyle(20);
    g_theta_i_theta22->SetTitle("#scale[0.85]{Funzione di fit:    i_{#theta} = i_{#theta}_{0}#left[cos^{2}(#theta + #phi) + #epsilon sin^{2}(#theta + #phi)#right]}");
    g_theta_i_theta22->GetXaxis()->SetTitle("#theta (#circ)");
    g_theta_i_theta22->GetYaxis()->SetTitle("i_{#theta} (#muA)");
    g_theta_i_theta22->GetXaxis()->SetLimits(-10,355); 
    //*std::max_element(std::begin(theta2), std::end(theta2)) + *std::max_element(std::begin(theta2), std::end(theta2)) / 10);
    
    cout << "\n\n --- Ipotesi  [0] * ( cos((x - [1]) * 3.1415926535 / 180) * cos((x - [1]) * 3.1415926535 / 180) + [2] * sin((x - [1]) * 3.1415926535 / 180) * sin((x - [1]) * 3.1415926535 / 180) ) --- \n" << endl;
    TF1 * fz_theta_i_theta22 = new TF1("fz_theta_i_theta22","[0] * ( cos((x - [1]) * 3.1415926535 / 180) * cos((x - [1]) * 3.1415926535 / 180) + [2] * sin((x - [1]) * 3.1415926535 / 180) * sin((x - [1]) * 3.1415926535 / 180) )", *std::min_element(std::begin(theta2),std::end(theta2)), *std::max_element(std::begin(theta2),std::end(theta2)));
    fz_theta_i_theta22->SetParNames("i_{#theta}_{0} (#muA)","#phi (#circ)", "#epsilon (adim)");

    float guess_par0_malus22 = (13.26 + 13.22) / 2; //media tra i due massimi meno media tra i due minimi
    float guess_par1_malus22 = 0.1; //ora dovrebbe essere rifasato
    float guess_par2_malus22 = *std::min_element(std::begin(i_theta2),std::end(i_theta2)) / *std::max_element(std::begin(i_theta2),std::end(i_theta2));

    cout << "guess_par0_malus22: " << guess_par0_malus22 << endl;
    cout << "guess_par1_malus22: " << guess_par1_malus22 << endl;
    cout << "guess_par2_malus22: " << guess_par2_malus22 << endl;

    fz_theta_i_theta22->SetParameter(0, guess_par0_malus22);
    fz_theta_i_theta22->SetParameter(1, guess_par1_malus22); 
    fz_theta_i_theta22->SetParameter(2, guess_par2_malus22);

    fz_theta_i_theta22->SetParLimits(0, guess_par0_malus22 - guess_par0_malus22 / 10, guess_par0_malus22 + guess_par0_malus22 / 10);            
    fz_theta_i_theta22->SetParLimits(1, guess_par1_malus22 - guess_par1_malus22 / 10, guess_par1_malus22 + guess_par1_malus22 / 10); 
    fz_theta_i_theta22->SetParLimits(2, guess_par2_malus22 - guess_par2_malus22 / 10, guess_par2_malus22 + guess_par2_malus22 / 10); 

    fz_theta_i_theta22->SetLineColor(38);
    g_theta_i_theta22->Fit(fz_theta_i_theta22,"ERM+");

    cout << "Chi^2:" << fz_theta_i_theta22->GetChisquare() << ", number of DoF: " << fz_theta_i_theta22->GetNDF() << " (Probability: " << fz_theta_i_theta22->GetProb() << ")." << endl;
    cout << "-----------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

    g_theta_i_theta22->Draw("AP");
}