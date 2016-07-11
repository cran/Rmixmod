#!/bin/bash
##############################################
# define function to make symbolic link of files from a directory
function make_symbolic_link(){
# loop over header files
for file in $1/*.h
do 
if [ -f $file ];
then
	link=$(basename $file)
	ln -s $file $link
fi
#echo $link
done

# loop over source files
for file in $1/*.cpp
do 
if [ -f $file ];
then
	link=$(basename $file)
	ln -s $file $link
fi
#echo $link
done
}

######## NEWMAT sources
cd NEWMAT
make_symbolic_link ../../../../mixmodLib/trunk/SRC/NEWMAT
cd ..

######## mixmod sources
cd mixmod/Utilities
make_symbolic_link ../../../../../mixmodLib/trunk/SRC/mixmod/Utilities
cd exceptions
make_symbolic_link ../../../../../../mixmodLib/trunk/SRC/mixmod/Utilities/exceptions
cd ..
cd maths
make_symbolic_link ../../../../../../mixmodLib/trunk/SRC/mixmod/Utilities/maths
cd ../..
cd Matrix
make_symbolic_link ../../../../../mixmodLib/trunk/SRC/mixmod/Matrix
cd ..
cd Kernel/Algo
make_symbolic_link ../../../../../../mixmodLib/trunk/SRC/mixmod/Kernel/Algo
cd ..
cd Model
make_symbolic_link ../../../../../../mixmodLib/trunk/SRC/mixmod/Kernel/Model
cd ..
cd IO
make_symbolic_link ../../../../../../mixmodLib/trunk/SRC/mixmod/Kernel/IO
cd ..
cd Criterion
make_symbolic_link ../../../../../../mixmodLib/trunk/SRC/mixmod/Kernel/Criterion
cd ..
cd Parameter
make_symbolic_link ../../../../../../mixmodLib/trunk/SRC/mixmod/Kernel/Parameter
cd ../..
cd Clustering
make_symbolic_link ../../../../../mixmodLib/trunk/SRC/mixmod/Clustering
cd ..
cd DiscriminantAnalysis/Learn
make_symbolic_link ../../../../../../mixmodLib/trunk/SRC/mixmod/DiscriminantAnalysis/Learn
cd ..
cd Predict
make_symbolic_link ../../../../../../mixmodLib/trunk/SRC/mixmod/DiscriminantAnalysis/Predict
cd ../../..
