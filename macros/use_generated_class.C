// ROOT guide for TTree::MakeClass(): https://root.cern.ch/root/htmldoc/guides/users-guide/Trees.html#using-ttreemakeclass
//
// Usage:
//   root 'OfflineAna/macros/use_generated_class.C("../data/AR31DECAY20240708run117.root")'

void use_generated_class(const char *filename, const char *treename = "event0")
{
    // Load the generated code.
    gROOT->ProcessLine(".L MyExperiment_generated_by_ROOT.C");

    auto f = new TFile(filename);
    f->ls();

    if (auto t = dynamic_cast<TTree *>(f->Get(treename)))
    {
        std::cout << "Found tree " << treename << t;
        auto b = new TBrowser("browser", t);
    }
}
