#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"
#include "TCut.h"
#include <TMath.h>
#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include "TGClient.h"
#include "TGButton.h"
#include "TRootEmbeddedCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TApplication.h"
#include "TGStatusBar.h"
#include "GuiTypes.h"
#include "TGFont.h"
#include "MyMainFrame.h"
#include "TGFileDialog.h"
#include "TObjString.h"
#include "TGFrame.h"
using namespace std;

int main(int argc, char** argv) {
	TApplication theApp("App", &argc, argv);
	MyMainFrame* fMainFrame = new MyMainFrame(gClient->GetRoot(), 550, 600); // Increased height for vertical arrangement
	theApp.Run();
	return 0;
}
