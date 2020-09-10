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

int planck2()
{

	float lambda430 = 430;

	float i430[] = {0.24925, 0.13342, 0.07488, 0.03820, 0.02388, 0.01395, 0.00844, 0.00502, 0.00265, 0.00126, 0.00034, -0.00025, -0.00052, -0.00062, -0.00063, -0.00070, -0.00070, -0.00075, -0.00079, -0.00078, -0.00080, -0.00077, -0.00082, -0.00081, -0.00082, -0.00085, -0.00086, -0.00091};

	float Vc430[] = {1.3, 100.4, 193.6, 308.1, 398.0, 506.2, 606.1, 702.8, 804.4, 903.7, 1000, 1116, 1203, 1258, 1287, 1320, 1350, 1379, 1406, 1421, 1457, 1483, 1512, 1525, 1575, 1702, 1908, 2293};

	vector<float> Vc430_vec(std::begin(Vc430), std::end(Vc430));
	vector<float> i430_vec(std::begin(i430), std::end(i430));

	vector<float> s_i430_vec = vector<float>();
	for(int i = 0; i < i430_vec.size(); i++)
	{
		s_i430_vec.push_back(0.00001);
	}

	vector<float> s_Vc430_vec = vector<float>();
	for(auto & element : Vc430_vec)
	{
		if(element < 10)
		{
			s_i430_vec.push_back(0.1);
		}
		if(element > 10 && element < 100)
		{
			s_i430_vec.push_back(0.1);
		}
		if(element >= 100 && element < 1000)
		{
			s_i430_vec.push_back(0.1);
		}
		if(element >= 10000 && element <= 10000)
		{
			s_i430_vec.push_back(1);
		}
	}


	//------------------------------
	float lambda464 = 464;

	float i464[] = {0.32187, 0.24864, 0.14146, 0.05759, 0.01645, -0.00011, -0.00086, -0.0012, -0.00161, -0.00184, -0.00233, -0.00271, -0.00297, -0.00319, -0.00337, -0.00349, -0.00363, -0.00369, -0.00377, -0.00382, -0.0039, -0.00393, -0.00394, -0.00397, -0.00407, -0.00412, -0.00422, -0.00425, -0.00445, -0.00447, -0.00453, -0.00466};

	float Vc464[] = {1.3, 116, 302.2, 499.8, 701.2, 993, 1028, 1049, 1081, 1102, 1150, 1200, 1250, 1298, 1349, 1397, 1452, 1498, 1555, 1607, 1658, 1700, 1749, 1809, 1898, 1999, 2124, 2195, 2442, 2600, 2786, 3013};

	vector<float> Vc464_vec(std::begin(Vc464), std::end(Vc464));
	vector<float> i464_vec(std::begin(i464), std::end(i464));

	vector<float> s_i464_vec = vector<float>();
	for(int i = 0; i < i464_vec.size(); i++)
	{
		s_i464_vec.push_back(0.00001);
	}

	vector<float> s_Vc464_vec = vector<float>();
	for(auto & element : Vc464_vec)
	{
		if(element < 10)
		{
			s_i464_vec.push_back(0.1);
		}
		if(element > 10 && element < 100)
		{
			s_i464_vec.push_back(0.1);
		}
		if(element >= 100 && element < 1000)
		{
			s_i464_vec.push_back(0.1);
		}
		if(element >= 10000 && element <= 10000)
		{
			s_i464_vec.push_back(1);
		}
	}


	float lambda551 = 551;

	float i551[] = {0.00149, 0.00088, 0.00071, 0.00039, 0.00028, 0.00025, 0.00014, 0.00008, 0.00004, 0.00001, -0.00001, -0.00004, -0.00008, -0.00012, -0.00015, -0.00016, -0.00018, -0.00018, -0.00020, -0.00022, -0.00023, -0.00023, -0.00024, -0.00024, -0.00027, -0.00028, -0.00029};

	float Vc551[] = {1.3, 107.1, 150.0, 251.4, 303.2, 334.9, 401.7, 460.2, 507.6, 552.9, 601.2, 654.2, 751.6, 851.3, 900.4, 951.9, 997.0, 1050, 1156, 1303, 1450, 1602, 1807, 2004, 2412, 2602, 2806};

	//for(float * it = begin(i551); it < end(i551); ++it){(*it) *= 100;}


	vector<float> Vc551_vec(std::begin(Vc551), std::end(Vc551));
	vector<float> i551_vec(std::begin(i551), std::end(i551));

	vector<float> s_i551_vec = vector<float>();
	for(int i = 0; i < i551_vec.size(); i++)
	{
		s_i551_vec.push_back(0.00001);
	}

	vector<float> s_Vc551_vec = vector<float>();
	for(auto & element : Vc551_vec)
	{
		if(element < 10)
		{
			s_i551_vec.push_back(0.1);
		}
		if(element > 10 && element < 100)
		{
			s_i551_vec.push_back(0.1);
		}
		if(element >= 100 && element < 1000)
		{
			s_i551_vec.push_back(0.1);
		}
		if(element >= 10000 && element <= 10000)
		{
			s_i551_vec.push_back(1);
		}
	}


	float lambda561 = 561;

	float i561[] = {0.00113, 0.00053, 0.00015, 0.00011, -0.00002, -0.00003, -0.00005, -0.00010, -0.00010, -0.00012, -0.00015, -0.00014, -0.00017, -0.00016, -0.00019, -0.00020, -0.00023, -0.00021, -0.00024, -0.00023, -0.00023, -0.00024};

	float Vc561[] = {1.3, 150.9, 266.6, 299.8, 379.8, 427.1, 471.5, 504.2, 553.2, 604.1, 654.1, 700.7, 750.3, 800.0, 905.5, 999.0, 1149, 1301, 1459, 1562, 1751, 2004};

	//for(float * it = begin(i561); it < end(i561); ++it){(*it) *= 100;}

	vector<float> Vc561_vec(std::begin(Vc561), std::end(Vc561));
	vector<float> i561_vec(std::begin(i561), std::end(i561));

	vector<float> s_i561_vec = vector<float>();
	for(int i = 0; i < i561_vec.size(); i++)
	{
		s_i561_vec.push_back(0.00001);
	}

	vector<float> s_Vc561_vec = vector<float>();
	for(auto & element : Vc561_vec)
	{
		if(element < 10)
		{
			s_i561_vec.push_back(0.1);
		}
		if(element > 10 && element < 100)
		{
			s_i561_vec.push_back(0.1);
		}
		if(element >= 100 && element < 1000)
		{
			s_i561_vec.push_back(0.1);
		}
		if(element >= 10000 && element <= 10000)
		{
			s_i561_vec.push_back(1);
		}
	}


	float lambda590 = 590;

	float i590[] = {0.00175, 0.00064, 0.00050, 0.00026, 0.00004, -0.00005, -0.00010, -0.00015, -0.00019, -0.00020, -0.00021, -0.00022, -0.00023, -0.00023, -0.00023, -0.00025, -0.00026, -0.00027, -0.00028, -0.00029, -0.00032, -0.00032};

	float Vc590[] = {0.9, 129.5, 150.0, 199.2, 267.0, 303.9, 353.2, 402.2, 472.4, 505.9, 550.3, 603.0, 650.2, 699.0, 754.6, 1001, 1242, 1513, 1752, 2005, 2606, 2959};

	//for(float * it = begin(i590); it < end(i590); ++it){(*it) *= 100;}

	vector<float> Vc590_vec(std::begin(Vc590), std::end(Vc590));
	vector<float> i590_vec(std::begin(i590), std::end(i590));

	vector<float> s_i590_vec = vector<float>();
	for(int i = 0; i < i590_vec.size(); i++)
	{
		s_i590_vec.push_back(0.00001);
	}

	vector<float> s_Vc590_vec = vector<float>();
	for(auto & element : Vc590_vec)
	{
		if(element < 10)
		{
			s_i590_vec.push_back(0.1);
		}
		if(element > 10 && element < 100)
		{
			s_i590_vec.push_back(0.1);
		}
		if(element >= 100 && element < 1000)
		{
			s_i590_vec.push_back(0.1);
		}
		if(element >= 10000 && element <= 10000)
		{
			s_i590_vec.push_back(1);
		}
	}


	float lambda656 = 656;

	float i656[] = {0.0028, -0.00031, -0.00055, -0.00074, -0.00083, -0.00088, -0.00095, -0.00099, -0.00103, -0.00104, -0.00107, -0.00111, -0.00111, -0.00111, -0.00114, -0.00119, -0.00119, -0.0012, -0.0012, -0.00122, -0.00122, -0.00125, -0.00126};

	float Vc656[] = {1.3, 205.7, 251.5, 301.8, 325.2, 349.3, 373.8, 404.8, 425, 451.4, 476, 498.1, 525, 549.3, 600.4, 801.5, 850.1, 900, 997, 1100, 1196, 1300, 1501};

	//for(float * it = begin(i656); it < end(i656); ++it){(*it) *= 100;}

	vector<float> Vc656_vec(std::begin(Vc656), std::end(Vc656));
	vector<float> i656_vec(std::begin(i656), std::end(i656));

	vector<float> s_i656_vec = vector<float>();
	for(int i = 0; i < i656_vec.size(); i++)
	{
		s_i656_vec.push_back(0.00001);
	}

	vector<float> s_Vc656_vec = vector<float>();
	for(auto & element : Vc656_vec)
	{
		if(element < 10)
		{
			s_i656_vec.push_back(0.1);
		}
		if(element > 10 && element < 100)
		{
			s_i656_vec.push_back(0.1);
		}
		if(element >= 100 && element < 1000)
		{
			s_i656_vec.push_back(0.1);
		}
		if(element >= 10000 && element <= 10000)
		{
			s_i656_vec.push_back(1);
		}
	}


	TGraphErrors * g_i430_Vc430 = new TGraphErrors(Vc430_vec.size(), i430_vec.data(), Vc430_vec.data(), s_i430_vec.data(), s_Vc430_vec.data());
	g_i430_Vc430->SetMarkerColor(4);
	g_i430_Vc430->SetMarkerStyle(20);
	g_i430_Vc430->SetMarkerSize(0.4);
	g_i430_Vc430->SetMarkerStyle(20);
	g_i430_Vc430->GetXaxis()->SetTitle("V_{C} (mV)");
	g_i430_Vc430->GetYaxis()->SetTitle("i (nA)");

	TGraphErrors * g_i464_Vc464 = new TGraphErrors(Vc464_vec.size(), i464_vec.data(), Vc464_vec.data(), s_i464_vec.data(), s_Vc464_vec.data());
	g_i464_Vc464->SetMarkerColor(7);
	g_i464_Vc464->SetMarkerStyle(20);
	g_i464_Vc464->SetMarkerSize(0.4);
	g_i464_Vc464->SetMarkerStyle(20);
	g_i464_Vc464->GetXaxis()->SetTitle("V_{C} (mV)");
	g_i464_Vc464->GetYaxis()->SetTitle("i (nA)");

	TGraphErrors * g_i551_Vc551 = new TGraphErrors(Vc551_vec.size(), i551_vec.data(), Vc551_vec.data(), s_i551_vec.data(), s_Vc551_vec.data());
	g_i551_Vc551->SetMarkerColor(8);
	g_i551_Vc551->SetMarkerStyle(20);
	g_i551_Vc551->SetMarkerSize(0.4);
	g_i551_Vc551->SetMarkerStyle(20);
	g_i551_Vc551->GetXaxis()->SetTitle("V_{C} (mV)");
	g_i551_Vc551->GetYaxis()->SetTitle("i (nA)");

	TGraphErrors * g_i561_Vc561 = new TGraphErrors(Vc561_vec.size(), i561_vec.data(), Vc561_vec.data(), s_i561_vec.data(), s_Vc561_vec.data());
	g_i561_Vc561->SetMarkerColor(3);
	g_i561_Vc561->SetMarkerStyle(20);
	g_i561_Vc561->SetMarkerSize(0.4);
	g_i561_Vc561->SetMarkerStyle(20);
	g_i561_Vc561->GetXaxis()->SetTitle("V_{C} (mV)");
	g_i561_Vc561->GetYaxis()->SetTitle("i (nA)");

	TGraphErrors * g_i590_Vc590 = new TGraphErrors(Vc590_vec.size(), i590_vec.data(), Vc590_vec.data(), s_i590_vec.data(), s_Vc590_vec.data());
	g_i590_Vc590->SetMarkerColor(5);
	g_i590_Vc590->SetMarkerStyle(20);
	g_i590_Vc590->SetMarkerSize(0.4);
	g_i590_Vc590->SetMarkerStyle(20);
	g_i590_Vc590->GetXaxis()->SetTitle("V_{C} (mV)");
	g_i590_Vc590->GetYaxis()->SetTitle("i (nA)");

	TGraphErrors * g_i656_Vc656 = new TGraphErrors(Vc656_vec.size(), i656_vec.data(), Vc656_vec.data(), s_i656_vec.data(), s_Vc656_vec.data());
	g_i656_Vc656->SetMarkerColor(2);
	g_i656_Vc656->SetMarkerStyle(20);
	g_i656_Vc656->SetMarkerSize(0.4);
	g_i656_Vc656->SetMarkerStyle(20);
	g_i656_Vc656->GetXaxis()->SetTitle("V_{C} (mV)");
	g_i656_Vc656->GetYaxis()->SetTitle("i (nA)");

	TCanvas * c_i_Vc = new TCanvas("c_i_Vc", "grafico1", 0, 0, 1280, 720);
	c_i_Vc->cd();


	g_i430_Vc430->Draw("AP");
	g_i464_Vc464->Draw("AP");
	g_i551_Vc551->Draw("AP");
	g_i561_Vc561->Draw("AP");
	g_i590_Vc590->Draw("AP");
	g_i656_Vc656->Draw("AP");


	TMultiGraph * mg_i_Vc = new TMultiGraph("mg_i_Vc", "#scale[0.6]{V_{C} vs i, tutti i LED} #scale[0.4]{#splitline{#splitline{#color[4]{#bullet #lambda_{p} = 430 (nm)} #color[7]{#bullet #lambda_{p} = 464 (nm)}}{#color[8]{#bullet #lambda_{p} = 551 (nm)} #color[3]{#bullet #lambda_{p} = 561 (nm)}}}{#color[5]{#bullet #lambda_{p} = 590 (nm)} #color[2]{#bullet #lambda_{p} = 656 (nm)}}}");
	mg_i_Vc->GetXaxis()->SetTitle("i (nA)");
	mg_i_Vc->GetYaxis()->SetTitle("V_{C} (mV)");
	mg_i_Vc->Add(g_i430_Vc430);
	mg_i_Vc->Add(g_i464_Vc464);
	mg_i_Vc->Add(g_i551_Vc551);
	mg_i_Vc->Add(g_i561_Vc561);
	mg_i_Vc->Add(g_i590_Vc590);
	mg_i_Vc->Add(g_i656_Vc656);
	mg_i_Vc->Draw("AP");

	return 0;
}
//V_{C} vs i, tutti i LED #color[4]{#bullet #lambda_{p} = 430 (nm)} #color[7]{#bullet #lambda_{p} = 464 (nm)}
					//															#color[8]{#bullet #lambda_{p} = 551 (nm)} #color[3]{#bullet #lambda_{p} = 561 (nm)}
					//															#color[5]{#bullet #lambda_{p} = 590 (nm)} #color[2]{#bullet #lambda_{p} = 656 (nm)}
