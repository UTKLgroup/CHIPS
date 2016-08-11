//--------------------------------------------------
// just execute the main program
// - can pass parameters
int Exe(void)
{

  gROOT->ProcessLine(".L count_file.C");
  gROOT->ProcessLine("string a=\"QQQ\" ");
  gROOT->ProcessLine("count_file(a)");

  exit(0);


}
//--------------------------------------------------
