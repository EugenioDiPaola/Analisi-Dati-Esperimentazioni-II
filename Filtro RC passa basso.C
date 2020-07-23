#include <iostream>
#include <TGraphErrors.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TF1.h>
#include <iomanip>
#include "math.h"

using namespace std;

void rcpassabasso(){

    cout << "£££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££"<< endl;

    cout << fixed;

    gStyle->SetOptFit(1111);
    gStyle->SetOptTitle(1); 
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(1111); 
    gStyle->SetStatBorderSize(0);
    gStyle->SetStatX(0.9); 
    gStyle->SetStatY(0.880);

    const float PI = 3.141592;
    float z_critico = 1.96; //5% significatività

    const int nmisure = 37;

    //Ohm
    //Amprobe 37XR-A
    float R = 61.36e+3;

    //Ohm
    //Amprobe 37XR-A 1000 Ohm to 1000 kOhm ranges: ± (0.5 % rdg + 8 dgts)
    float sR = R * 0.005 + 8 * 0.001;

    //F
    //ponte RLC
    float C = 2.7e-9;

    //F
    //sensibilità del ponte RLC
    float sC = 0.05 * C;

    //Hz
    //letti sul generatore di funzioni
    float f[] = {101,201,301,401,501,601,702,800,848,869,888,900,913,922,930,945,953,960,970,981,994,1001,1101,1220,1237,1254,1300,1400,1500,1600,1700,1800,1900,2000,2200,2400,2600};

    //Hz
    //stima dell'1% perchè abbiamo fatto casino nel leggere sto errore; avremmo dovuto leggere quello sui tempi e poi trasformarlo in quello sulle frequenze
    float sf[nmisure];
    for(int j = 0; j < nmisure; j++){sf[j] = f[j] / 100;}

    //V
    //misurati sul menu misure dell'oscilloscopio
    float Vo[] = {5.6,5.44,5.32,5.12,4.96,4.72,4.48,4.24,4.16,4.08,4.08,4.04,4.00,3.96,3.92,3.90,3.88,3.88,3.84,3.80,3.80,3.78,3.60,3.40,3.36,3.32,3.26,3.08,2.96,2.80,2.68,2.56,2.48,2.36,2.18,2.04,1.88};

    //V
    //misurati sul menu misure dell'oscilloscopio
    float Vi[] = {5.9, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9, 5.9};

    //V
    //errore di sensibilità schermo dell'oscilloscopio
    float sVi[] = {0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2};

    //V
    //errore di sensibilità schermo dell'oscilloscopio
    float sVo[] = {0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1};

    //adim
    //Vo / vi
    float A[nmisure];

    //adim
    //errore propagato
    float sA[nmisure];
    for(int j = 0; j < nmisure; j++){
        A[j] = Vo[j] / Vi[j];
        sA[j] = sqrt((pow(Vo[j],2) / pow(Vi[j],4)) * pow(sVi[j],2) + (1 / pow(Vi[j],2)) * pow(sVo[j],2));
    };

    //Hz
    float f_T_teorica = 1 / (2 * PI * R * C);

    //Hz
    float sf_T_teorica = 1 / (2 * PI) * sqrt(1/(pow(R,4) * pow(C,2)) * pow(sR,2) + 1 / (pow(R,2) * pow(C,4)) * pow(sC,2));

    for(int j = 0 ; j < nmisure ; j++){
        cout << "Measurement number " << j << ":" 
        << " \t f = ("   << setprecision(5) << f[j] << " +- " << setprecision(4) << sf[j]  << ") Hz," 
        << " \t Vo = ("  << setprecision(5) << Vo[j] << " +- " << setprecision(4) << sVo[j] << ") V," 
        << " \t Vi = ("  << setprecision(5) << Vi[j] << " +- " << setprecision(4) << sVi[j] << ") V," 
        << " \t A = ("   << setprecision(5) << A[j] << " +- " << setprecision(4) << sA[j]  << ") adim." 
        << " \t sf/f "   << setprecision(4) << sf[j] / f[j] 
        << " \t Vo/sVo "  << setprecision(4) << sVo[j] / Vo[j] 
        << " \t Vi/sVi "  << setprecision(4) << sVi[j] / Vi[j]  
        << " \t sf/f " << setprecision(4) << sf[j] / f[j] 
        << " \t sA/A " << setprecision(4) << sA[j] / A[j] 
        << endl;   
    }

    cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
    cout << "____________________________________________________________________________________________________________FIT CON ERRORI 0.01*f SU f_____________________________________________________________________________________________________" << endl;
    cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;

    TCanvas * cAf1 = new TCanvas ("cAf1","grafico1",0,0,1280,720);
    cAf1->SetFillColor(0);
    cAf1->SetLogx();
    cAf1->cd();

    TGraphErrors * gAf1 = new TGraphErrors(nmisure,f,A,sf,sA);
    gAf1->SetMarkerSize(0.4);
    gAf1->SetMarkerStyle(20);
    gAf1->SetTitle("#scale[0.85]{Funzione di fit:    A^{ }=^{ }1 / #sqrt{1 + f^{2} / f_{t}^{2}}}");
    gAf1->GetXaxis()->SetTitle("f^{ }(Hz)");
    gAf1->GetYaxis()->SetTitle("A^{ }(adim)");
    gAf1->Draw("AP");

    cout << "\n\n --- Ipotesi passa basso 1 / sqrt(1 + pow(x,2) / [0])  ---  \n" << endl; 
    TF1 * funz1 = new TF1("funz1","1 / sqrt(1 + pow(x,2) / [0])",0,30000);
    funz1->SetParNames("f_{T}^{2} (Hz^{2})");
    funz1->SetParameter(0,pow(f_T_teorica,2));
    funz1->SetLineColor(2);
    gAf1->Fit(funz1,"ERM+");

    cout << "Chi^2:" << funz1->GetChisquare() << ", number of DoF: " << funz1->GetNDF() << " (Probability: " << funz1->GetProb() << ")." << endl;
    cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
    cAf1->SaveAs("FILTRORCPASSABASSO 1.png");
  
    cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
    cout << "_________________________________________________________________________________________________________FIT CON ERRORI PIU' PICCOLI SU f__________________________________________________________________________________________________" << endl;
    cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;

    //dato che il chi quadro del fit precedente è molto piccolo potremmo aver sovrastimato gli errori; 
    //dato che quello su f lo abbiamo scelto noi proviamo a rifare il fit con un errore su f dieci volte più piccolo
    //anche con errori più piccoli dell'1% su f il chiquadro non cambia

    //Hz
    float sf2[nmisure];
    for(int j = 0; j < nmisure; j++){sf2[j] = f[j] / 10000;}
  
    //con errori più piccoli su f
    TCanvas * cAf2 = new TCanvas ("cA2","grafico2",0,0,1280,720);
    cAf2->SetFillColor(0);
    cAf2->SetLogx();
    cAf2->cd();

    TGraphErrors * gAf2 = new TGraphErrors(nmisure,f,A,sf2,sA);
    gAf2->SetMarkerSize(0.4);
    gAf2->SetMarkerStyle(20);
    gAf2->SetTitle("#scale[0.85]{Funzione di fit:    A^{ }=^{ }1 / #sqrt{1 + f^{2} / f_{T}^{2}}}");
    gAf2->GetXaxis()->SetTitle("f^{ }(Hz)");
    gAf2->GetYaxis()->SetTitle("A^{ }(adim)");
    gAf2->Draw("AP");

    cout << " \n\n ----- Ipotesi 1 / sqrt(1 + pow(x,2) / [0]) ------ \n " <<endl;
    TF1 * funz2 = new TF1 ("funz2","1 / sqrt(1 + pow(x,2) / [0])",0,400000);
    funz2->SetParNames("f_{T}^{2} (Hz^{2})");
    funz2->SetParameter(0,pow(f_T_teorica,2));
    funz2->SetLineColor(3);
    gAf2->Fit(funz2,"ERM+");

    cout << "Chi^2:" << funz2->GetChisquare() << ", number of DoF: " << funz2->GetNDF() << " (Probability: " << funz2->GetProb() << ")." << endl;
    cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
    cAf2->SaveAs("FILTRORCPASSABASSO 2.png");


    cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
    cout << "_____________________________________________________________________________________________________________________TEST Z________________________________________________________________________________________________________________" << endl;
    cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;

    float p0 = funz1->GetParameter(0);// il quadrato di [0]=f_t ottenuta dal fit
    float sp0 = funz1->GetParError(0);
    float f_T_sperimentale = sqrt(p0);
    float sf_T_sperimentale = sp0 / (2 * f_T_sperimentale);//errore propagato; p0=f_T_sperimentale^2

 
    cout << "R: (" << R << "+-" << sR << ")" << endl;
    cout << "C: (" << setprecision(13) << C << "+-" << sC << ")" << endl;
    cout << "frequenza di taglio teorica: (" << f_T_teorica << "+-" << sf_T_teorica << ")" << endl;
    cout << "frequenza di taglio sperimentale: (" << f_T_sperimentale << "+-" << sf_T_sperimentale << ")" << endl;

    float z = (f_T_teorica - f_T_sperimentale) / sqrt(pow(sf_T_sperimentale,2) + pow(sf_T_teorica,2));
    if(z <= z_critico) cout << "z: " << z << " < " << z_critico << endl;
    if(z > z_critico) cout << "z: " << z << " > " << z_critico << endl;

    //__________________________________________________________________________________________________________________________________________________________________________________________________________________________
    std::ofstream out("FILTRORCPASSABASSO_CSV.csv");
    out << "f (kHz)" << endl;
    for (auto & value : f){out << fixed << showpoint << setprecision(7) << value / 1000 << endl;}
    out << endl;

    out << "sf (kHz)" << endl;
    for (auto & value : sf){out << fixed << showpoint << setprecision(7) << value / 1000 << endl;}
    out << endl;

    out << "Vo (V)" << endl;
    for (auto & value : Vo){out << fixed << showpoint << setprecision(7) << value << endl;}
    out << endl;

    out << "sVo (V)" << endl;
    for (auto & value : sVo){out << fixed << showpoint << setprecision(7) << value << endl;}   
    out << endl;

    out << "Vi (V)" << endl;
    for (auto & value : Vi){out << fixed << showpoint << setprecision(7) << value << endl;}
    out << endl;

    out << "sVi (V)" << endl;
    for (auto & value : sVi){out << fixed << showpoint << setprecision(7) << value << endl;} 
    out << endl;

    out << "A (adim)" << endl;
    for (auto & value : A){out << fixed << showpoint << setprecision(7) << value << endl;} 
    out << endl;

    out << "sA (adim)" << endl;
    for (auto & value : sA){out << fixed << showpoint << setprecision(7) << value << endl;}
    out << endl;
}

 