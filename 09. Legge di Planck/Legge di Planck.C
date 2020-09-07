#include <iostream>   
#include <iomanip>  
#include <vector>
//#include <map>
//#include <pair>
#include <cmath>
#include <string>
#include <algorithm>

//ROOT classes
#include <TF1.h>
#include <TCanvas.h>
#include <TGraph.h> 
#include <TGraphAsymmErrors.h>
#include <TGraphErrors.h> 
#include <TMultiGraph.h>     
#include <TAxis.h>                   
#include "TVirtualFitter.h"  
#include <TLegend.h>
#include <TLatex.h>
#include <TColor.h>

using namespace std; 


void vectorPrint(string VECTOR_NAME, vector<float> & VECTOR){
    cout << endl << VECTOR_NAME + ": " << endl;
    for(auto & x: VECTOR){cout << x << endl;}
    cout << endl;
}

float computeVc0(int COLOR, TMultiGraph * MULTIGRAPH, float LAMBDA, vector<float> V_C, vector<float> I, vector<float> S_V_C, vector<float> S_I, int BEGIN_INCL, int END_INCL, int BEGIN_ORIZ, int END_ORIZ){   
    
    
    //gStyle->SetOptStat(1111);
    //gStyle->SetOptTitle(1); 
    //gStyle->SetOptFit(0); mette pcve tutti a 0; non posso scrivere gStyle->SetOptFit(0000) perchè se no pensa che 0000 sia un numero ottale;
    //gStyle->SetOptFit(1); == gStyle->SetOptFit(111); in gStyle->SetOptFit(111) ho che pcve = 0111; lo 0 è implicito perchè se usassi 0111 come argomento di SetOptFit, 0111 verrebbe letto come un numero ottale.
   

    //auto Default = new TStyle("Default","Default Style");
    //gStyle->SetOptFit(0111);

    if(BEGIN_INCL < 0){BEGIN_INCL = 0;};
    if(BEGIN_ORIZ < 0){BEGIN_ORIZ = 0;};
    if(END_INCL > V_C.size() - 1){END_INCL = V_C.size() - 1;};
    if(END_ORIZ > V_C.size() - 1){END_ORIZ = V_C.size() - 1;};

    vector<float> Vc_incl(V_C.cbegin() + BEGIN_INCL, V_C.cbegin() + END_INCL + 1); //range constructor prende [,); l'ultimo elemento è escluso dal range constructor proprio per come è definito
    vector<float> sVc_incl(S_V_C.cbegin() + BEGIN_INCL, S_V_C.cbegin() + END_INCL +1);
    vector<float> i_incl(I.cbegin() + BEGIN_INCL, I.cbegin() + END_INCL + 1);
    vector<float> s_i_incl(S_I.cbegin() + BEGIN_INCL, S_I.cbegin() + END_INCL + 1);

    vector<float> Vc_oriz(V_C.cbegin() + BEGIN_ORIZ , V_C.cbegin() + END_ORIZ + 1);
    vector<float> sVc_oriz(S_V_C.cbegin() + BEGIN_ORIZ , S_V_C.cbegin() + END_ORIZ + 1);
    vector<float> i_oriz(I.cbegin() + BEGIN_ORIZ , I.cbegin() + END_ORIZ + 1);
    vector<float> s_i_oriz(S_I.cbegin() + BEGIN_ORIZ, S_I.cbegin() + END_ORIZ + 1);

    
    TGraphErrors * g_iVc_incl = new TGraphErrors(END_INCL - BEGIN_INCL + 1, Vc_incl.data(), i_incl.data(), sVc_incl.data(), s_i_incl.data());
    g_iVc_incl->SetMarkerSize(0.4);
    g_iVc_incl->SetMarkerStyle(20);

    for(int i = 0; i < Vc_incl.size(); i++){
        string label_str = "#color[" + std::to_string(COLOR) + "]{#scale[0.5]{{}^{ }" + to_string(i + BEGIN_INCL) + "}}";
        char label_char[label_str.size() + 1];
        std::copy(label_str.begin(), label_str.end(), label_char);
        label_char[label_str.size()] = '\0';
        TLatex * tlatex_obj = new TLatex(g_iVc_incl->GetX()[i], g_iVc_incl->GetY()[i],label_char);
        g_iVc_incl->GetListOfFunctions()->Add(tlatex_obj);
    }
           
    TGraphErrors * g_iVc_oriz = new TGraphErrors(END_ORIZ - BEGIN_ORIZ + 1, Vc_oriz.data(), i_oriz.data(), sVc_oriz.data(), s_i_oriz.data());
    g_iVc_oriz->SetMarkerSize(0.4);
    g_iVc_oriz->SetMarkerStyle(20);

    for(int i = 0; i < Vc_oriz.size(); i++){
        string label_str = "#color[" + std::to_string(COLOR) + "]{#scale[0.5]{{}^{ }" + to_string(i + BEGIN_ORIZ) + "}}";
        char label_char[label_str.size() + 1];
        std::copy(label_str.begin(), label_str.end(), label_char);
        label_char[label_str.size()] = '\0';
        TLatex * tlatex_obj = new TLatex(g_iVc_oriz->GetX()[i], g_iVc_oriz->GetY()[i],label_char);
        g_iVc_oriz->GetListOfFunctions()->Add(tlatex_obj);
    }

    TF1 * fz_iVc_incl = new TF1("fz_iVc_incl","[0] + [1] * x", 0, 3000);
    fz_iVc_incl->SetParNames("m (nA mV^{-1})","q (nA)");
    fz_iVc_incl->SetLineColor(COLOR);
    g_iVc_incl->Fit(fz_iVc_incl,"ERM+");

    cout << "Chi^2 :" << fz_iVc_incl->GetChisquare() << ", number of DoF : " << fz_iVc_incl->GetNDF() << " (Probability : " << fz_iVc_incl->GetProb() << ")." << endl;
    cout << "-----------------------------------------------------------------------------------------------------------------------------------------------" << endl;

    TF1 * fz_iVc_oriz = new TF1("fz_iVc_oriz","[0]", 0, 3000);
    fz_iVc_oriz->SetParNames("r(nA)");
    fz_iVc_oriz->SetLineColor(COLOR);
    g_iVc_oriz->Fit(fz_iVc_oriz,"ERM+");
   
    cout << "Chi^2 :" << fz_iVc_oriz->GetChisquare() << ", number of DoF : " << fz_iVc_oriz->GetNDF() << " (Probability : " << fz_iVc_oriz->GetProb() << ")." << endl;
    cout << "-----------------------------------------------------------------------------------------------------------------------------------------------" << endl;

    MULTIGRAPH->Add(g_iVc_incl);
    MULTIGRAPH->Add(g_iVc_oriz);

    int lambda_int = (int) LAMBDA;
    string title_str = "#scale[1]{i vs V_{c} per #lambda = " + to_string(lambda_int) + "(nm)}";
    char title_char[title_str.size() + 1];
    std::copy(title_str.begin(), title_str.end(), title_char);
    title_char[title_str.size()] = '\0';

    MULTIGRAPH->SetTitle(title_char);

    float r = fz_iVc_oriz->GetParameter(0);
    float q = fz_iVc_incl->GetParameter(0);
    float m = fz_iVc_incl->GetParameter(1);
   
    float Vc0 = (r - q) / m;
    
    return Vc0;
}


//map<float,* TCanvas> LAMBDA_TCANVAS_PTR_MAP
vector<float> computeVc0AndErrors(int ORIGIN, float LAMBDA, vector<float> V_C, vector<float> I, int BEGIN_INCL, int END_INCL, int BEGIN_ORIZ, int END_ORIZ){

    gStyle->SetOptFit(0);

    vector<float> sVc;
    for(const float & x: V_C){
        if(x >= 0 && x < 10){sVc.push_back(fabs(x * 0.001 + 5 * 0.001));}
        if(x >= 10 && x < 1e2){sVc.push_back(fabs(x * 0.001 + 5 * 0.01));}
        if(x >= 1e2 && x < 1e3){sVc.push_back(fabs(x * 0.001 + 5 * 0.1));}
        if(x >= 1e3 && x < 1e4){sVc.push_back(fabs(x * 0.001 + 5 * 1));}
    }

    vector<float> s_i;
    for(const float & x: I){s_i.push_back(fabs(x / 100));}

    int lambda_int = (int) LAMBDA;
    string title_canvas_str = "grafico " + to_string(lambda_int) + "(nm)";
    char title_canvas_char[title_canvas_str.size() + 1];
    std::copy(title_canvas_str.begin(), title_canvas_str.end(), title_canvas_char);
    title_canvas_char[title_canvas_str.size()] = '\0';

    TCanvas * cI_V_C = new TCanvas(title_canvas_char, title_canvas_char, 0, 0, 1280, 720); 
    cI_V_C->SetFillColor(0);
    cI_V_C->cd();
    //cI_V_C->SetFrameFillColor(50);
    //cI_V_C->SetFrameLineColor(52);

    TGraphErrors * gI_V_C = new TGraphErrors(V_C.size(), V_C.data(), I.data(), sVc.data(), s_i.data());
    gI_V_C->SetMarkerSize(0.4);
    gI_V_C->SetMarkerStyle(20);

    string title_str = "#scale[1]{i vs V_{C} per #lambda = " + to_string(lambda_int) + "(nm) #color[" + to_string(LAMBDA) + "]{#scale[2]{#bullet}}}";
    char title_char[title_str.size() + 1];
    std::copy(title_str.begin(), title_str.end(), title_char);
    title_char[title_str.size()] = '\0';

    gI_V_C->SetTitle(title_char);
    gI_V_C->GetXaxis()->SetTitle("V_{C} (mV)");
    gI_V_C->GetYaxis()->SetTitle("i (nA)");
    gI_V_C->GetXaxis()->SetLimits(ORIGIN, *std::max_element(V_C.begin(), V_C.end()) + *std::max_element(V_C.begin(), V_C.end()) / 10);
    gI_V_C->GetHistogram()->SetMinimum(*std::min_element(I.begin(), I.end()) + *std::min_element(I.begin(), I.end()) / 20);
    gI_V_C->GetHistogram()->SetMaximum(*std::max_element(I.begin(), I.end()) + *std::max_element(I.begin(), I.end()) / 20);     
    

    for(int i = 0; i < V_C.size(); i++){
        string label_str = "#scale[0.5]{{}^{" + to_string(i) + "}}";
        char label_char[label_str.size() + 1];
        std::copy(label_str.begin(), label_str.end(), label_char);
        label_char[label_str.size()] = '\0';
        TLatex * tlatex_obj = new TLatex(gI_V_C->GetX()[i], gI_V_C->GetY()[i], label_char);
        gI_V_C->GetListOfFunctions()->Add(tlatex_obj);
    }
    
    gI_V_C->Draw("AP");

    string title_str1 = "fits " + to_string(lambda_int);
    char title_char1[title_str1.size() + 1];
    std::copy(title_str1.begin(), title_str1.end(), title_char1);
    title_char1[title_str1.size()] = '\0';

    TCanvas * c_iVc_linearfits = new TCanvas(title_char1, title_char1, 0, 0, 1280, 720);;
    c_iVc_linearfits->SetFillColor(0);
    c_iVc_linearfits->cd();
    //c_iVc_linearfits->SetAstat(0); //non funziona

    TMultiGraph * mg_iVc_linearfits = new TMultiGraph();
    mg_iVc_linearfits->GetXaxis()->SetTitle("V_{C} (mV)");
    mg_iVc_linearfits->GetYaxis()->SetTitle("i (nA)");

    int color = 3;

    float Vc0_left;
    Vc0_left = computeVc0(color - 1 , mg_iVc_linearfits, LAMBDA, V_C, I, sVc, s_i, (BEGIN_INCL - 1), (END_INCL - 1), (BEGIN_ORIZ - 1), (END_ORIZ - 1));
    float Vc0_middle;
    Vc0_middle = computeVc0(color, mg_iVc_linearfits, LAMBDA, V_C, I, sVc, s_i, BEGIN_INCL, END_INCL, BEGIN_ORIZ, END_ORIZ);
    float Vc0_right;
    Vc0_right = computeVc0(color + 1, mg_iVc_linearfits, LAMBDA, V_C, I, sVc, s_i, (BEGIN_INCL + 1), (END_INCL + 1), (BEGIN_ORIZ + 1), (END_ORIZ + 1));


    string file_name_str = "i vs Vc per lambda = " + to_string(lambda_int) + "(nm).png";
    char file_name_char[file_name_str.size() + 1];
    std::copy(file_name_str.begin(), file_name_str.end(), file_name_char);
    file_name_char[file_name_str.size()] = '\0';

    //LAMBDA_TCANVAS_PTR_MAP.insert(pair<LAMBDA, mg_iVc_linearfits>); 
    
    mg_iVc_linearfits->Draw("AP");

    vector<float> pre_return_vec;
    pre_return_vec.push_back(Vc0_left);
    pre_return_vec.push_back(Vc0_middle);
    pre_return_vec.push_back(Vc0_right);

    std::sort(pre_return_vec.begin(), pre_return_vec.end());

    vector<float> return_vec;
    return_vec.push_back(pre_return_vec[1]);
    return_vec.push_back(pre_return_vec[1] - pre_return_vec[0]);
    return_vec.push_back(pre_return_vec[2] - pre_return_vec[1]);

    return return_vec;
}

vector<float> computePlanckConstantAndIonizationPotential(vector<float> LAMBDA, vector<float> S_LEFT_LAMBDA, vector<float> S_RIGHT_LAMBDA, vector<float> V_C_0, vector<float> S_LEFT_V_C_0, vector<float> S_RIGHT_V_C_0){

    gStyle->SetStatBorderSize(0);
    gStyle->SetStatX(0.9); 
    gStyle->SetStatY(0.35); 

    //ms^-1
    //fissata
    float c = 299792458;
    
    //C
    //noto
    float e = 1.602e-19;

    //c (ms^-1) / lambda (nm) * 1e9 = nu (Hz)
    vector<float> nu_Hz;
    vector<float> s_nu_left_Hz;
    vector<float> s_nu_right_Hz;

    for(auto & LAMBDA_i: LAMBDA){nu_Hz.push_back((c / LAMBDA_i) * 1e9);}
    for(int LAMBDA_i = 0; LAMBDA_i < LAMBDA.size(); LAMBDA_i++){s_nu_left_Hz.push_back(((c * pow(1 / LAMBDA[LAMBDA_i], 2) * S_LEFT_LAMBDA[LAMBDA_i])) * 1e9);}
    for(int LAMBDA_i = 0; LAMBDA_i < LAMBDA.size(); LAMBDA_i++){s_nu_right_Hz.push_back(((c * pow(1 / LAMBDA[LAMBDA_i], 2) * S_RIGHT_LAMBDA[LAMBDA_i])) * 1e9);}

    vector<float> eVc0_J;
    vector<float> s_left_eVc0_J;
    vector<float> s_right_eVc0_J;

    float conversion_const_mVC_to_J = 1e-3;

    for(auto & x: V_C_0){eVc0_J.push_back(e * x * conversion_const_mVC_to_J);}
    for(auto & x: S_LEFT_V_C_0){s_left_eVc0_J.push_back(e * x * conversion_const_mVC_to_J);}
    for(auto & x: S_RIGHT_V_C_0){s_right_eVc0_J.push_back(e * x * conversion_const_mVC_to_J);}


    TCanvas * c_eVc0_J_nu_Hz = new TCanvas("c_eVc0_J_nu_Hz", "grafico Planck 1", 0, 0, 1280, 720);
    c_eVc0_J_nu_Hz->SetFillColor(0);
    c_eVc0_J_nu_Hz->cd();

    TGraphAsymmErrors * g_eVc0_J_nu_Hz = new TGraphAsymmErrors(nu_Hz.size(), nu_Hz.data(), eVc0_J.data(), s_nu_left_Hz.data(), s_nu_right_Hz.data(), s_left_eVc0_J.data(), s_right_eVc0_J.data());
    g_eVc0_J_nu_Hz->SetMarkerSize(0.4);
    g_eVc0_J_nu_Hz->SetMarkerStyle(20);
    g_eVc0_J_nu_Hz->SetTitle("#scale[0.8]{Funzione di fit:    eV_{C}_{0} = #hbar#nu - U_{6s^{1}}^{Cs}}");
    g_eVc0_J_nu_Hz->GetYaxis()->SetTitle("e #upoint V_{C}_{0} (J)");//("e #upoint V_{C}_{0} (meV)");
    g_eVc0_J_nu_Hz->GetXaxis()->SetTitle("#nu (Hz)");
    
    float x_end = *std::max_element(std::begin(nu_Hz), std::end(nu_Hz)) + *std::max_element(std::begin(nu_Hz), std::end(nu_Hz)) / 10;

    cout << "x_end: " << x_end << endl;

    TF1 * fz_eVc0_J_nu_Hz = new TF1("fzeVc0nu","[0] * x + [1]", 0, x_end);
    fz_eVc0_J_nu_Hz->SetParNames("#hbar (Js)","U^{Cs}_{6s^{1}} (J)");

    float h_teorico_Js = 6.626e-34;

    float U_Cs6s1_teorico_eV = 3.89;
    float U_Cs6s1_teorico_J = U_Cs6s1_teorico_eV * e;

    fz_eVc0_J_nu_Hz->SetParameter(0, h_teorico_Js);
    fz_eVc0_J_nu_Hz->SetParameter(1, U_Cs6s1_teorico_J);

    //fz_eVc0nu->SetParLimits(0, h_teorico - h_teorico / 10, h_teorico + h_teorico / 10);
    //fz_eVc0nu->SetParLimits(1, U_Cs6s1 - U_Cs6s1 / 10, U_Cs6s1 + U_Cs6s1 / 10); 
    
    fz_eVc0_J_nu_Hz->SetLineColor(2);

    //Hz
    //[405 (THz), 790 (Thz)]
    float visible_light_frequencies_inf = 405e12; 
    float visible_light_frequencies_sup = 790e12;

    g_eVc0_J_nu_Hz->GetXaxis()->SetLimits(visible_light_frequencies_inf - visible_light_frequencies_inf / 10, visible_light_frequencies_sup + visible_light_frequencies_sup / 10);
    g_eVc0_J_nu_Hz->Fit(fz_eVc0_J_nu_Hz,"ERM+");

    cout << "Chi^2:" << fz_eVc0_J_nu_Hz->GetChisquare() << ", number of DoF: " << fz_eVc0_J_nu_Hz->GetNDF() << " (Probability: " << fz_eVc0_J_nu_Hz->GetProb() << ")." << endl;
    cout << "--------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

    g_eVc0_J_nu_Hz->Draw("AP");

    vector<float> nu_THz;
    vector<float> s_nu_left_THz;
    vector<float> s_nu_right_THz;

    for(auto & LAMBDA_i: LAMBDA){nu_THz.push_back((c / LAMBDA_i) / 1e3);}
    for(int LAMBDA_i = 0; LAMBDA_i < LAMBDA.size(); LAMBDA_i++){s_nu_left_THz.push_back(((c * pow(1 / LAMBDA[LAMBDA_i], 2) * S_LEFT_LAMBDA[LAMBDA_i])) / 1e3);}
    for(int LAMBDA_i = 0; LAMBDA_i < LAMBDA.size(); LAMBDA_i++){s_nu_right_THz.push_back(((c * pow(1 / LAMBDA[LAMBDA_i], 2) * S_RIGHT_LAMBDA[LAMBDA_i])) / 1e3);}


    vectorPrint("nu_THz", nu_THz);
    vectorPrint("s_nu_left_THz", s_nu_left_THz);
    vectorPrint("nu_THz", s_nu_right_THz);

    vector<float> eVc0_eV;
    vector<float> s_left_eVc0_eV;
    vector<float> s_right_eVc0_eV;

    float conversion_const_mVC_to_eV = 6.24e15;
    
    for(auto & x: V_C_0){eVc0_eV.push_back(e * x * conversion_const_mVC_to_eV);}
    for(auto & x: S_LEFT_V_C_0){s_left_eVc0_eV.push_back(e * x * conversion_const_mVC_to_eV);}
    for(auto & x: S_RIGHT_V_C_0){s_right_eVc0_eV.push_back(e * x * conversion_const_mVC_to_eV);}


    TCanvas * c_eVc0_eV_nu_THz = new TCanvas("c_eVc0_eV_nu_THz", "grafico Planck 2", 0, 0, 1280, 720);
    c_eVc0_eV_nu_THz->SetFillColor(0);
    c_eVc0_eV_nu_THz->cd();

    TGraphAsymmErrors * g_eVc0_eV_nu_THz = new TGraphAsymmErrors(nu_THz.size(), nu_THz.data(), eVc0_eV.data(), s_nu_left_THz.data(), s_nu_right_THz.data(), s_left_eVc0_eV.data(), s_right_eVc0_eV.data());
    g_eVc0_eV_nu_THz->SetMarkerSize(0.4);
    g_eVc0_eV_nu_THz->SetMarkerStyle(20);
    g_eVc0_eV_nu_THz->SetTitle("#scale[0.8]{Funzione di fit:    eV_{C}_{0} = #hbar#nu - U_{6s^{1}}^{Cs}}");
    g_eVc0_eV_nu_THz->GetYaxis()->SetTitle("e #upoint V_{C}_{0} (eV)");
    g_eVc0_eV_nu_THz->GetXaxis()->SetTitle("#nu (THz)");
    
    float x_end2 = *std::max_element(std::begin(nu_THz), std::end(nu_THz)) + *std::max_element(std::begin(nu_THz), std::end(nu_THz)) / 10;

    cout << "x_end2: " << x_end2 << endl;

    TF1 * fz_eVc0_eV_nu_THz = new TF1("fz_eVc0_eV_nu_THz","[0] * 1e31 / 1.602 * x + [1]", 0, x_end2);
    fz_eVc0_eV_nu_THz->SetParNames("#hbar (Js)","U^{Cs}_{6s^{1}} (eV)");

    fz_eVc0_eV_nu_THz->SetParameter(0,h_teorico_Js);
    fz_eVc0_eV_nu_THz->SetParameter(1,U_Cs6s1_teorico_eV);

    //fz_eVc0_eV_nu_THz->SetParLimits(0, h_teorico - h_teorico / 10, h_teorico + h_teorico / 10);
    //fz_eVc0_eV_nu_THz->SetParLimits(1, U_Cs6s1 - U_Cs6s1 / 10, U_Cs6s1 + U_Cs6s1 / 10); 
    
    fz_eVc0_eV_nu_THz->SetLineColor(2);

    //Hz
    //[405 (THz), 790 (Thz)]
    float visible_light_frequencies_inf_THz = 405; 
    float visible_light_frequencies_sup_THz = 790;

    g_eVc0_eV_nu_THz->GetXaxis()->SetLimits(visible_light_frequencies_inf_THz - visible_light_frequencies_inf_THz / 10, visible_light_frequencies_sup_THz + visible_light_frequencies_sup_THz / 10);
    g_eVc0_eV_nu_THz->Fit(fz_eVc0_eV_nu_THz,"ERM+");

    cout << "Chi^2:" << fz_eVc0_eV_nu_THz->GetChisquare() << ", number of DoF: " << fz_eVc0_eV_nu_THz->GetNDF() << " (Probability: " << fz_eVc0_eV_nu_THz->GetProb() << ")." << endl;
    cout << "--------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

    g_eVc0_eV_nu_THz->Draw("AP");

    vector<float> return_vec;
    
    return_vec.push_back(fz_eVc0_J_nu_Hz->GetParameter(0));
    return_vec.push_back(fz_eVc0_J_nu_Hz->GetParError(0));
    return_vec.push_back(fz_eVc0_J_nu_Hz->GetParameter(1));
    return_vec.push_back(fz_eVc0_J_nu_Hz->GetParError(1));

    return return_vec;
}


void planck(){

    vector<TCanvas *> c_ptr_vec = vector<TCanvas *>();

    cout << "£££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££"<< endl;

    cout << "__________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
    cout << "_______________________________________________________________________________________________RICERCA DI Vc0, LED A LAMBDA = 430 (nm)____________________________________________________________________________________________________" << endl;
    cout << "__________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;

    //nm
    //spettrofotometro e Avantes
    float lambda430 = 430;

    //nA
    //picoamperometro Keithley 6485
    float i430[] = {0.24925, 0.13342, 0.07488, 0.03820, 0.02388, 0.01395, 0.00844, 0.00502, 0.00265, 0.00126, 0.00034, -0.00025, -0.00052, -0.00062, -0.00063, -0.00070, -0.00070, -0.00075, -0.00079, -0.00078, -0.00080, -0.00077, -0.00082, -0.00081, -0.00082, -0.00085, -0.00086, -0.00091};

    //mV
    //Amprobe 37XR-A
    float Vc430[] = {1.3, 100.4, 193.6, 308.1, 398.0, 506.2, 606.1, 702.8, 804.4, 903.7, 1000, 1116, 1203, 1258, 1287, 1320, 1350, 1379, 1406, 1421, 1457, 1483, 1512, 1525, 1575, 1702, 1908, 2293};
    
    vector<float> Vc430_vec(std::begin(Vc430), std::end(Vc430));
    vector<float> i430_vec(std::begin(i430), std::end(i430));

    //TCanvas * c_i430_Vc430 = new TCanvas();

    vector<float> Vc0_430;
    Vc0_430 = computeVc0AndErrors(0, lambda430, Vc430_vec, i430_vec, 13, 19, 25, 27); //ORIGIN = 1e3

    cout << "Vc0_430: " << endl << Vc0_430[0] << endl << Vc0_430[1] << endl << Vc0_430[2] << endl;

    
    cout << "__________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
    cout << "_______________________________________________________________________________________________RICERCA DI Vc0, LED A LAMBDA = 464 (nm)____________________________________________________________________________________________________" << endl;
    cout << "__________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;

    //nm
    //spettrofotometro e Avantes
    float lambda464 = 464;

    //nA
    //picoamperometro Keithley 6485
    float i464[] = {0.32187, 0.24864, 0.14146, 0.05759, 0.01645, -0.00011, -0.00086, -0.0012, -0.00161, -0.00184, -0.00233, -0.00271, -0.00297, -0.00319, -0.00337, -0.00349, -0.00363, -0.00369, -0.00377, -0.00382, -0.0039, -0.00393, -0.00394, -0.00397, -0.00407, -0.00412, -0.00422, -0.00425, -0.00445, -0.00447, -0.00453, -0.00466};

    //mV
    //Amprobe 37XR-A
    float Vc464[] = {1.3, 116, 302.2, 499.8, 701.2, 993, 1028, 1049, 1081, 1102, 1150, 1200, 1250, 1298, 1349, 1397, 1452, 1498, 1555, 1607, 1658, 1700, 1749, 1809, 1898, 1999, 2124, 2195, 2442, 2600, 2786, 3013};
    
    vector<float> Vc464_vec(std::begin(Vc464), std::end(Vc464));
    vector<float> i464_vec(std::begin(i464), std::end(i464));

    int color_rgb_464 [] = {61, 255, 255};

    //TColor * tcolor_464 = new TColor (lambda464, color_rgb_464[0], color_rgb_464[1], color_rgb_464[2]);


    vector<float> Vc0_464;
    Vc0_464 = computeVc0AndErrors(0, lambda464, Vc464_vec, i464_vec, 13, 16, 28, 30); // ORIGIN = 1e3

    cout << "Vc0_464: " << endl << Vc0_464[0] << endl << Vc0_464[1] << endl << Vc0_464[2] << endl;


    cout << "__________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
    cout << "_______________________________________________________________________________________________RICERCA DI Vc0, LED A LAMBDA = 540 (nm)____________________________________________________________________________________________________" << endl;
    cout << "__________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;

    //nm
    //spettrofotometro e Avantes
    float lambda540 = 551;

    //nA
    //picoamperometro Keithley 6485
    float i540[] = {0.00149, 0.00088, 0.00071, 0.00039, 0.00028, 0.00025, 0.00014, 0.00008, 0.00004, 0.00001, -0.00001, -0.00004, -0.00008, -0.00012, -0.00015, -0.00016, -0.00018, -0.00018, -0.00020, -0.00022, -0.00023, -0.00023, -0.00024, -0.00024, -0.00027, -0.00028, -0.00029};

    //mV
    //Amprobe 37XR-A
    float Vc540[] = {1.3, 107.1, 150.0, 251.4, 303.2, 334.9, 401.7, 460.2, 507.6, 552.9, 601.2, 654.2, 751.6, 851.3, 900.4, 951.9, 997.0, 1050, 1156, 1303, 1450, 1602, 1807, 2004, 2412, 2602, 2806};
    
    vector<float> Vc540_vec(std::begin(Vc540), std::end(Vc540));
    vector<float> i540_vec(std::begin(i540), std::end(i540));

    int color_rgb_540 [] = {61, 255, 255};

    //TColor * tcolor_540 = new TColor (lambda540, color_rgb_540[0], color_rgb_540[1], color_rgb_540[2]);

    vector<float> Vc0_540;
    Vc0_540 = computeVc0AndErrors(0, lambda540, Vc540_vec, i540_vec, 13, 16, 25, 30); //ORIGIN = 5e2

    cout << "Vc0_540: " << endl << Vc0_540[0] << endl << Vc0_540[1] << endl << Vc0_540[2] << endl;


    cout << "__________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
    cout << "_______________________________________________________________________________________________RICERCA DI Vc0, LED A LAMBDA = 561(nm)____________________________________________________________________________________________________" << endl;
    cout << "__________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;

    //nm
    //spettrofotometro e Avantes
    float lambda561 = 561;

    //nA
    //picoamperometro Keithley 6485
    

    //mV
    //Amprobe 37XR-A
    
    
    vector<float> Vc561_vec(std::begin(Vc561), std::end(Vc561));
    vector<float> i561_vec(std::begin(i561), std::end(i561));

    //int color_rgb_561 []= {61,50,255};

    vector<float> Vc0_561;
    Vc0_561 = computeVc0AndErrors(0, lambda561, Vc561_vec, i561_vec, 7, 14, 18, 21); //ORIGIN = 3e2

    cout << "Vc0_561: " << endl << Vc0_561[0] << endl << Vc0_561[1] << endl << Vc0_561[2] << endl;


    cout << "__________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
    cout << "_______________________________________________________________________________________________RICERCA DI Vc0, LED A LAMBDA = 590(nm)____________________________________________________________________________________________________" << endl;
    cout << "__________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;

    //nm
    //spettrofotometro e Avantes
    float lambda590 = 590;

    //nA
    //picoamperometro Keithley 6485
    

    //mV
    //Amprobe 37XR-A
    
    
    vector<float> Vc590_vec(std::begin(Vc590), std::end(Vc590));
    vector<float> i590_vec(std::begin(i590), std::end(i590));

    //int color_rgb_590 []= {61,255,255};

 
    vector<float> Vc0_590;
    Vc0_590 = computeVc0AndErrors(0, lambda590, Vc590_vec, i590_vec, 8, 12, 19, 21); //ORIGIN = 2.5e2
    cout << "Vc0_590: " << endl << Vc0_590[0] << endl << Vc0_590[1] << endl << Vc0_590[2] << endl;


    cout << "__________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
    cout << "_______________________________________________________________________________________________RICERCA DI Vc0, LED A LAMBDA = 656(nm)____________________________________________________________________________________________________" << endl;
    cout << "__________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;

    //nm
    //spettrofotometro e Avantes
    float lambda656 = 656;

    //nA
    //picoamperometro Keithley 6485
    

    //mV
    //Amprobe 37XR-A
    
    
    vector<float> Vc656_vec(std::begin(Vc656), std::end(Vc656));
    vector<float> i656_vec(std::begin(i656), std::end(i656));

    //int color_rgb_656 []= {255,0,255};

    vector<float> Vc0_656;
    Vc0_656 = computeVc0AndErrors(0, lambda656, Vc656_vec, i656_vec, 3, 6, 19, 22);//ORIGIN = 1.9e2

    cout << "Vc0_656: " << endl << Vc0_656[0] << endl << Vc0_656[1] << endl << Vc0_656[2] << endl;

    
    
    vector<float> LAMBDA;
    LAMBDA.push_back(lambda430);
    LAMBDA.push_back(lambda464);
    LAMBDA.push_back(lambda540);
    LAMBDA.push_back(lambda561);
    LAMBDA.push_back(lambda590);
    LAMBDA.push_back(lambda656);

    //for(auto & x: LAMBDA){*lambda_TCanvas_ptr_map[x]->Draw("AP");}

    vector<float> Vc0;
    Vc0.push_back(Vc0_430[0]);
    Vc0.push_back(Vc0_464[0]);
    Vc0.push_back(Vc0_540[0]);
    Vc0.push_back(Vc0_561[0]);
    Vc0.push_back(Vc0_590[0]);
    Vc0.push_back(Vc0_656[0]);

    vector<float> s_left_Vc0;
    s_left_Vc0.push_back(Vc0_430[1]);
    s_left_Vc0.push_back(Vc0_464[1]);
    s_left_Vc0.push_back(Vc0_540[1]);
    s_left_Vc0.push_back(Vc0_561[1]);
    s_left_Vc0.push_back(Vc0_590[1]);
    s_left_Vc0.push_back(Vc0_656[1]);

    vector<float> s_right_Vc0;
    s_right_Vc0.push_back(Vc0_430[2]);
    s_right_Vc0.push_back(Vc0_464[2]);
    s_right_Vc0.push_back(Vc0_540[2]);
    s_right_Vc0.push_back(Vc0_561[2]);
    s_right_Vc0.push_back(Vc0_590[2]);
    s_right_Vc0.push_back(Vc0_656[2]);

    //nm
    //HWHM delle code destra e sinistra rispettivamente dello spettro in frequenza di ogni LED,
    // ottenuto con lo spettrofotometro e il software di acquisizione Avantes
    float s_left_LAMBDA [] = {28, 13, 19, 7, 17, 9};
    float s_right_LAMBDA [] = {52, 10, 22, 5, 16, 13};

    vector<float> s_left_LAMBDA_vec(begin(s_left_LAMBDA), end(s_left_LAMBDA));
    vector<float> s_right_LAMBDA_vec(begin(s_left_LAMBDA), end(s_left_LAMBDA));


    vector<float> planck_constant_and_ionization_potential = computePlanckConstantAndIonizationPotential(LAMBDA, s_left_LAMBDA_vec, s_right_LAMBDA_vec, Vc0, s_left_Vc0, s_right_Vc0);
    
    cout << "costante di Planck: (" << planck_constant_and_ionization_potential[0] << " +- " << planck_constant_and_ionization_potential[1] << ") Js" << endl; 
    cout << "potenziale di prima ionizzazione del Cesio: (" << 0 - planck_constant_and_ionization_potential[2] / 1.602e-19 << " +- " << planck_constant_and_ionization_potential[3] / 1.602e-19 << ") meV" << endl; 

    gStyle->SetOptFit(111);
}
