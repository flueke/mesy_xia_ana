// ROOT guide for TTree::MakeClass(): https://root.cern.ch/root/htmldoc/guides/users-guide/Trees.html#using-ttreemakeclass
//
// Usage:
//   root 'OfflineAna/macros/make_class_for_tree.C("../data/AR31DECAY20240708run117.root", "event0")'
// This will create MyExperiment_generated_by_ROOT.h/C in the current directory.
// See macros/make_class_for_tree.C on how to use the generated code.

void make_class_for_tree(const char *filename, const char *treename = "event0")
{
    auto f = new TFile(filename);
    f->ls();

    if (auto t = dynamic_cast<TTree *>(f->Get(treename)))
    {
        std::cout << "Found tree " << treename << t;
        //t->Print();
        t->MakeClass("MyExperiment_generated_by_ROOT");
    }
    else
    {
        std::cout << "Error: TTree named '" << treename << "' not present in input file '" << filename << "'. Nothing to do.\n";
    }
}
