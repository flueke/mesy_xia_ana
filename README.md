# mesytec-mvlc listfile to TTree

This is example code showing how to use mesytec-mvlc to read a listfile, extract
contained data and write data to a ROOT TTree. Docker is used to make it easy to
test with different ubuntu and/or ROOT versions.

### Changes from the original code

* GUI code is disabled for now, only the runCLI tool remains. I had issues
  correctly building the ROOT dictionary so I removed everything that's not
  immediately needed.

* The ROOT dictionary is built as a shared lib (.so) instead of an object file
  (.o). This way the generated code exists only once instead of being present in
  each executable.

* Added a Dockerfile to be able to test this without having to install
  ubuntu-20.04.


### Using the example

  git clone https://github.com/flueke/mesy_xia_ana
  cd mesy_xia_ana
  wget https://root.cern/download/root_v6.26.16.Linux-ubuntu20-x86_64-gcc9.4.tar.gz
  docker build -f Dockerfile.ubuntu-20.04 --progress plain -t xia-offline-ana .
  mkdir ~/data
  wget https://www.mesytec.com/kundendaten/Xiaokang.Xia/AR31DECAY20240708run117.zip ~/data
  docker run -v /data/:~/data:rw -it --rm xia-offline-ana /sources/runCLI /data/AR31DECAY20240708run117.zip

With the above commands you get a docker image called 'xia-offline-ana'. This
image contains the downloaded ROOT installation and the executable
'/sources/runCLI' which does the same as the runGUI tool did, just without the
GUI.

Example data you sent us (AR31DECAY20240708run117.zip) is then downloaded to a
local directory. In the 'docker run' line this directory is made available to
the container for reading and writing. runCLI processes the whole file, prints
out a lot of debug information and write sthe output .root file.

Look at the Dockerfile for detailed commands needed to get the same to work on a
ubuntu-20.04 machine.
