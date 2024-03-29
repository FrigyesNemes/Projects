#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <sstream>
#include "math.h"
#include "math.h"

using namespace std;

#include "stdlib.h"

#include "TMath.h"

#include "TF1.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TFile.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TLatex.h"
#include "TLine.h"
#include "TMinuit.h"
#include "TStyle.h"

#include "TVector.h"
#include "TMatrix.h"
#include "TMarker.h"
#include "TFitResult.h"

#include "TRandom3.h"
#include "TMultiGraph.h"

#include <iomanip>

Double_t log_like_function(Double_t *x, Double_t *par)
{
        double f1 = par[0] * pow(log(x[0]), 0) ;
        double f2 = par[1] * pow(log(x[0]), 1) ;
        double f3 = par[2] * pow(log(x[0]), 2) ;

        double f = par[3] * (f1 + f2 + f3) ;
      
        cout << "energy: " << x[0] << " " << f << endl ;

        return f ;
}

const int scenario_prelim_1 = 1 ;
const int scenario_prelim_2 = 2 ;

void mydraw()
{
   TLatex *latex = new TLatex() ;

   latex->SetNDC() ;
   latex->SetTextFont(132) ;
   latex->SetTextColor(kBlack) ;
	
	latex->DrawLatex(.7, .14, "TOTEM-D0") ;
}

const double axis_title_size = (0.035*1.2) ;

int plot_sigtot(int scenario)
{

	string myfilename = "data/TOTEM_D0_14_PRL_preliminary_1_cross_section.txt" ;
	string myplotname = "fig/TOTEM_D0_14_PRL_preliminary_1_cross_section.pdf" ;
	
	double cp = 73.75607 ;
	double bp = 8.41508 ;
	double ap = 2.29419 ;
	
	string axis_title = "#sqrt{s} (TeV)" ;

   double epsilon = 0.01 ;
	double TeV_to_GeV = 1.0 ;
	
	if(scenario ==  scenario_prelim_2)
	{
		myfilename = "data/TOTEM_D0_14_PRL_preliminary_2_cross_section.txt" ;
		myplotname = "fig/TOTEM_D0_14_PRL_preliminary_2_cross_section.pdf" ;

		cp = 125.098837177212 ;
		bp = -23.2803355035864 ;
		ap = 2.29419079175536 ;

   	TeV_to_GeV = 1.0e3 ;

		axis_title = "#sqrt{s} (GeV)" ;

	}

   TH2D *hist_2d = new TH2D("hist_2d", "hist_2d", 100, 1.0 * TeV_to_GeV, 14.0 * TeV_to_GeV, 100, 75, 115) ;	

   TF1 *func = new TF1("func", log_like_function, epsilon * TeV_to_GeV, 14.0 *  TeV_to_GeV, 4) ;	
   TF1 *func_p = new TF1("func_p", log_like_function, epsilon * TeV_to_GeV, 14.0 *  TeV_to_GeV, 4) ;	
   TF1 *func_m = new TF1("func_m", log_like_function, epsilon * TeV_to_GeV, 14.0 *  TeV_to_GeV, 4) ;	

   gStyle->SetLineScalePS(.3) ;
	gStyle->SetOptFit(1111);
	gStyle->SetOptStat("");

	TGraphErrors *graph = new TGraphErrors ;
	TGraphErrors *graph_1p96 = new TGraphErrors ;
	
	TCanvas c ;
	
	hist_2d->SetTitle("") ;

	graph->SetMarkerStyle(20) ;
	graph_1p96->SetMarkerStyle(29) ;
	graph_1p96->SetMarkerSize(1.4) ;

	graph_1p96->SetMarkerColor(kBlue) ;

	ifstream data(myfilename.c_str()) ;
	
	string word ;
	
	double energy, sigtot, sigtotunc ;

	int i = 0 ;
	
	while(data >> energy >> sigtot >> sigtotunc)
	{
		cout << energy << " " << sigtot << " " << sigtotunc << endl ;

		if(energy == 1.96 * TeV_to_GeV)
		{
			graph_1p96->SetPoint(0, energy, sigtot) ;
			graph_1p96->SetPointError(0, 0, sigtotunc) ;
		}
		else
		{
			graph->SetPoint(i, energy, sigtot) ;
			graph->SetPointError(i, 0, sigtotunc) ;
			++i ;
		}
	}
	
	double uncertainy_factor = 1.01 ;

   func->SetParameters(cp, bp, ap, 1.0) ;
   func_p->SetParameters(cp, bp, ap, uncertainy_factor) ;
   func_m->SetParameters(cp, bp, ap, 1.0 / uncertainy_factor) ;

   func->SetNpx(100) ;	
   func_p->SetNpx(100) ;	
   func_m->SetNpx(100) ;	

   func->SetLineColor(kRed) ;	
   func_p->SetLineColor(kMagenta) ;	
   func_m->SetLineColor(kMagenta) ;	

   func_p->SetLineStyle(kDashed) ;
   func_m->SetLineStyle(kDashed) ;

	hist_2d->Draw() ;
	hist_2d->GetXaxis()->SetTitle(axis_title.c_str()) ;
	hist_2d->GetYaxis()->SetTitle("#sigma_{tot} (mb)") ;

	hist_2d->GetXaxis()->SetTitleSize(axis_title_size) ;
	hist_2d->GetYaxis()->SetTitleSize(axis_title_size) ;

	graph->Draw("same p") ;
	graph_1p96->Draw("same p") ;
	func->SetRange(1.5 * TeV_to_GeV, 15 * TeV_to_GeV) ;
	func->Draw("same l") ;
	// func_p->Draw("same l") ;
	// func_m->Draw("same l") ;
	
	TLegend *legend = new TLegend(0.12, 0.65, 0.5, 0.88) ;
	
	legend->AddEntry(graph, "TOTEM measurements", "pe") ;
	legend->AddEntry(graph_1p96, "extrapolation", "pe") ;
	// legend->AddEntry(func, "fit, prelim. uncert!", "l") ;
	
	legend->Draw("same") ;

	mydraw() ;	

	c.SaveAs(myplotname.c_str()) ;

	delete hist_2d ;
	delete graph ;
	delete graph_1p96 ;
	delete func ;
	delete legend ;
}

int plot_dsdt()
{
	TCanvas c ;
	c.SetLogy() ;

   gStyle->SetLineScalePS(.3) ;
	gStyle->SetOptFit(1111);
	gStyle->SetOptStat("");

   TH2D *hist_2d = new TH2D("hist_2d", "hist_2d", 100, 0.45, 1.0, 100, 0.005, 0.1) ;	

	hist_2d->SetTitle("") ;
	TGraphErrors *graph = new TGraphErrors ;
	TGraphErrors *graph2 = new TGraphErrors ;
	TGraphErrors *graph_band_up = new TGraphErrors ;
	TGraphErrors *graph_band_down = new TGraphErrors ;
	TGraphAsymmErrors *graph_center = new TGraphAsymmErrors ;
	TGraphAsymmErrors *graph_center2 = new TGraphAsymmErrors ;

	graph->SetMarkerStyle(20) ;
	graph2->SetMarkerStyle(22) ;
	graph2->SetMarkerSize(1.1) ;

	graph->SetMarkerColor(kBlue) ;
	graph2->SetMarkerColor(kRed) ;
	
	graph_center->SetMarkerStyle(22) ;
	graph_center->SetMarkerSize(1.0) ;
	graph_center->SetMarkerColor(kRed) ;

	graph_center2->SetMarkerStyle(22) ;
	graph_center2->SetMarkerSize(1.0) ;
	graph_center2->SetMarkerColor(kRed) ;

 	ifstream data("data/TOTEM_D0_14_PRL_preliminary_1_dsdt.txt") ;

	double t_value , t_unc , dsdt , dsdt_unc , norm_plus , norm_minus ;

	int i = 0 ;

	while(data >> t_value >> t_unc >> dsdt >> dsdt_unc >> norm_plus >> norm_minus)
	{
		graph->SetPoint(i, t_value, dsdt) ;
		graph->SetPointError(i, 0, dsdt_unc) ;
		
		++i ;
	}
	
	data.close() ;

	ifstream data2("data/TOTEM_D0_14_PRL_preliminary_1_dsdt_extrapolated.txt") ;

	i = 0 ;
	
	double chi2start ;

	while(data2 >> t_value >> dsdt >> dsdt_unc >> chi2start)
	{
		graph2->SetPoint(i, t_value, dsdt) ;
		graph2->SetPointError(i, 0, dsdt_unc) ;
		
		++i ;
	}
	
	data2.close() ;

	ifstream data4("data/TOTEM_D0_14_PRL_preliminary_1_dsdt_extrapolated_uncertainty_center.txt") ;

	i = 0 ;


	double dsdt_down, dsdt_up, f_norm_down, f_norm_up ;
	double dummy ;

	while(data4 >> t_value >> dsdt >> dsdt_up >> dsdt_down >> dummy >> dummy)
	{
		graph_center->SetPoint(i, t_value, dsdt) ;

		++i ;
	}

	data4.close() ;
	
	ifstream data3("data/TOTEM_D0_14_PRL_preliminary_1_dsdt_extrapolated_uncertainty_band.txt") ;

	i = 0 ;
	int j = 0 ;

	while(data3 >> t_value >> dsdt_down >> dsdt_up >> f_norm_down >> f_norm_up)
	{
		graph_band_up->SetPoint(i, t_value, dsdt_up) ;
		graph_band_down->SetPoint(i, t_value, dsdt_down) ;


		double myx, myy ;

		graph_center->GetPoint(i, myx, myy) ;
		graph_center->SetPointError(i, 0, 0, myy - f_norm_down, f_norm_up - myy) ;

		double myx2, myy2 ;
		graph2->GetPoint(j, myx2, myy2) ;

		if(myx == myx2)
		{
			graph_center2->SetPoint(j, myx, myy) ;
			graph_center2->SetPointError(j, 0, 0, myy - f_norm_down, f_norm_up - myy) ;
			++j ;
		}

		++i ;
	}

	data3.close() ;
	


	hist_2d->Draw() ;
	hist_2d->GetXaxis()->SetTitle("|t| (GeV^{2})") ;
	hist_2d->GetYaxis()->SetTitle("d#sigma/dt (mb/GeV^{2})") ;

	hist_2d->GetXaxis()->SetTitleSize(axis_title_size) ;
	hist_2d->GetYaxis()->SetTitleSize(axis_title_size) ;
	// cout << "titlesize " << hist_2d->GetXaxis()->GetTitleSize() ;

	graph->Draw("same p") ;
	// graph2->Draw("same p") ;

	graph_band_up->Draw("same l") ;
	graph_band_down->Draw("same l") ;
	
	graph_center2->Draw("same p") ;
	
	graph_band_up->SetLineColor(kRed) ;
	graph_band_down->SetLineColor(kRed) ;
	graph_band_up->SetLineStyle(kDashed) ;
	graph_band_down->SetLineStyle(kDashed) ;

	TLegend *legend = new TLegend(0.4, 0.65, 0.88, 0.88) ;
	
	legend->AddEntry(graph, "p#bar{p} measurement by D0 at #sqrt{s}=1.96 TeV", "pe") ;
	legend->AddEntry(graph2, "pp extrapolated", "pe") ;
	legend->AddEntry(graph_band_up, "band width (#pm 1 #sigma)", "l") ;
	
	legend->Draw("same") ;

	mydraw() ;

   c.SaveAs("fig/TOTEM_D0_14_PRL_preliminary_1_dsdt.pdf") ;
}

int main(int argc, char *argv[])
{
	plot_sigtot(scenario_prelim_1) ;
	plot_sigtot(scenario_prelim_2) ;
	plot_dsdt() ;
}
