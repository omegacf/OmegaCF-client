[![Build status](https://ci.appveyor.com/api/projects/status/4jbml8q4uu8udri1/branch/dev?svg=true)](https://ci.appveyor.com/project/Simple-codinger/omegacf-client/branch/dev)

## OmegaCF-client
This is the client for our Connect-Four project. 
The client uses machine learning to evaluate the possible moves.

## Installation
The application uses pyTorch for the deep learning component. 
### Download pytorch
````
cd ./lib
wget https://download.pytorch.org/libtorch/nightly/cpu/libtorch-shared-with-deps-latest.zip
unzip libtorch-shared-with-deps-latest.zip 
rm libtorch-shared-with-deps-latest.zip 
````
## Build project
````
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH=/absolute/path/to/libtorch ..
cmake --build . --config Release
````

## Run Application
````
./bin/OmegaCF-client
````

## License
The source code is licensed under the Lesser General Public License (LGPL) v3
