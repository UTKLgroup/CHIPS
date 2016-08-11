//--------------------------------------------------
// just execute the main program
// - can pass parameters
int Exe(void)
{

  gROOT->ProcessLine(".L count_file.C");
  gROOT->ProcessLine("string a=\"/scratch/01230/fnova/output/2014_06_30_cosmics__CHIPS__refl_10__at_42__depth_60__2_mirrors/2014_06_30_cosmics__CHIPS__refl_10__at_42__depth_60__2_mirrors_data.root\" ");
  gROOT->ProcessLine("count_file(a)");

  exit(0);


}
//--------------------------------------------------
