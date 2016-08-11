#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>    // std::max

void divide(){

  std::string base="/scratch/01230/fnova/output/";
  //std::string direc1="2014_05_04_gps__CHIPS__refl_0__yes_looking_down__no__bottom__depth_60";
  //std::string direc2="2014_05_04_cosmics____CHIPS__refl_0__yes_looking_down__no__bottom__depth_60_b";
  std::string direc2="2014_05_03_gps__CHIPS__refl_0__no_looking_down__yes__bottom__depth_60";
  std::string direc1="2014_05_03_cosmics____CHIPS__refl_0__no_looking_down__yes__bottom__depth_60_b";
  //std::string direc2="2014_05_06_gps__CHIPS__refl_0__at_42__depth_60";
  //std::string direc1="2014_05_06_cosmics__CHIPS__refl_0__at_42__depth_60";

  std::string volume;
  double time, energy, length, x0_ID, y0_ID, z0_ID, x0_VETO, y0_VETO, z0_VETO, x0, y0, z0;

  TFile input_file1((base+direc1+"/coordinates2.root").c_str());
  TFile input_file2((base+direc2+"/coordinates2.root").c_str());
  TFile output_file((base+direc1+"/divide.root").c_str(),"RECREATE");

  string veto_volume="veto_pmt";
  string ID_volume="pmt";

  TH2F * h_y0_ID_vs_x0_ID_top_1 = (TH2F*)input_file1.Get("h_y0_ID_vs_x0_ID_top");
  TH2F * h_y0_ID_vs_x0_ID_top_2 = (TH2F*)input_file2.Get("h_y0_ID_vs_x0_ID_top");
  TH2F * h_y0_ID_vs_x0_ID_top_ratio = (TH2F*)h_y0_ID_vs_x0_ID_top_2->Clone("h_y0_ID_vs_x0_ID_top_ratio");
  h_y0_ID_vs_x0_ID_top_ratio->Divide(h_y0_ID_vs_x0_ID_top_1, h_y0_ID_vs_x0_ID_top_2);
  h_y0_ID_vs_x0_ID_top_ratio->Write();


  TH2F * h_y0_ID_vs_x0_ID_bottom_1 = (TH2F*)input_file1.Get("h_y0_ID_vs_x0_ID_bottom");
  TH2F * h_y0_ID_vs_x0_ID_bottom_2 = (TH2F*)input_file2.Get("h_y0_ID_vs_x0_ID_bottom");
  TH2F * h_y0_ID_vs_x0_ID_bottom_ratio = (TH2F*)h_y0_ID_vs_x0_ID_bottom_2->Clone("h_y0_ID_vs_x0_ID_bottom_ratio");
  h_y0_ID_vs_x0_ID_bottom_ratio->Divide(h_y0_ID_vs_x0_ID_bottom_1, h_y0_ID_vs_x0_ID_bottom_2);
  h_y0_ID_vs_x0_ID_bottom_ratio->Write();



  TH2F * h_z0_ID_vs_phi_ID_1 = (TH2F*)input_file1.Get("h_z0_ID_vs_phi_ID");
  TH2F * h_z0_ID_vs_phi_ID_2 = (TH2F*)input_file2.Get("h_z0_ID_vs_phi_ID");
  TH2F * h_z0_ID_vs_phi_ID_ratio = (TH2F*)h_z0_ID_vs_phi_ID_2->Clone("h_z0_ID_vs_phi_ID_ratio");
  h_z0_ID_vs_phi_ID_ratio->Divide(h_z0_ID_vs_phi_ID_1, h_z0_ID_vs_phi_ID_2);
  h_z0_ID_vs_phi_ID_ratio->Write();


  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1,0);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  double BottomMargin = .17;   // fraction of pad height
  double LeftMargin = .1;
  double RightMargin = .18;

  TCanvas c_display("c_display","c_display",600,600);
  c_display.SetBottomMargin(BottomMargin);
  c_display.SetLeftMargin(LeftMargin);
  c_display.SetRightMargin(RightMargin);

  TPad * topPad;
  TPad * sidePad;
  TPad * bottomPad;

  double zmax_top = h_y0_ID_vs_x0_ID_top_ratio->GetMaximum();
  double zmax_side = h_z0_ID_vs_phi_ID_ratio->GetMaximum();
  double zmax_bottom = h_y0_ID_vs_x0_ID_bottom_ratio->GetMaximum();
  std::clog << " zmax top " << zmax_top << " side " << zmax_side << " bottom " << zmax_bottom << std::endl;
  double the_max = std::max(zmax_top, zmax_side);
  the_max = std::max(the_max, zmax_bottom);
  h_y0_ID_vs_x0_ID_top_ratio->GetZaxis()->SetRangeUser(0, the_max);
  h_z0_ID_vs_phi_ID_ratio->GetZaxis()->SetRangeUser(0, the_max);
  h_y0_ID_vs_x0_ID_bottom_ratio->GetZaxis()->SetRangeUser(0, the_max);
  double xmin=0.33;
  double xmax=0.67;
  double Xmin=0.005;
  double Xmax=0.995;
  double ymaxtop=0.995;
  double ymintop=0.66;
  double ymaxside=0.66;
  double yminside=0.33;
  double ymaxbottom=0.33;
  double yminbottom=0.005;
  c_display.Divide(1,3);
  topPad =  (TPad*)c_display.GetListOfPrimitives()->FindObject("c_display_1");
  topPad->SetPad(xmin, ymintop, xmax, ymaxtop);
  sidePad =  (TPad*)c_display.GetListOfPrimitives()->FindObject("c_display_2");
  sidePad->SetPad(Xmin, yminside, Xmax, ymaxside);
  bottomPad =  (TPad*)c_display.GetListOfPrimitives()->FindObject("c_display_3");
  bottomPad->SetPad(xmin, yminbottom, xmax, ymaxbottom);

  topPad->cd();
  h_y0_ID_vs_x0_ID_top_ratio->Draw("col");
  sidePad->cd();
  h_z0_ID_vs_phi_ID_ratio->Draw("colz");
  bottomPad->cd();
  h_y0_ID_vs_x0_ID_bottom_ratio->Draw("col");
  TPaletteAxis *palette = (TPaletteAxis*)h_z0_ID_vs_phi_ID_1->GetListOfFunctions()->FindObject("palette");
  palette->SetX1NDC(0.9);
  palette->SetX2NDC(0.95);
  palette->SetY1NDC(0.2);
  palette->SetY2NDC(0.8);
  c_display.Modified();
  c_display.Update();

  string figname="figure/display_"+direc1+"__over__"+direc2+".eps";
  c_display.Print(figname.c_str());


  exit(0);


}

