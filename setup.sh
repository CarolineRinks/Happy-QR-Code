#!/bin/bash

wget http://web.eecs.utk.edu/~jplank/plank/jgraph/2017-11-28-Jgraph.tar
tar -xvf 2017-11-28-Jgraph.tar
cd jgraph
make
cd ..
rm 2017-11-28-Jgraph.tar
cd QRcode-generator
make
cd ..
make

