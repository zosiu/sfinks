# sfinks

## How to compile
`git clone git@github.com:zosiu/sfinks.git --recursive`
`rm -rf bird_lady/vendor/z3/.git` see (here)[https://github.com/Z3Prover/z3/issues/1551]

`cmake -B build -DCMAKE_BUILD_TYPE=Release`

`cmake --build build`

the executable will be generated to `bin/[your_platform]_release`
