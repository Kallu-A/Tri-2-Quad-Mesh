![build, test](https://github.com/kallu-A/Tri-2-Quad-Mesh/actions/workflows/continuous.yml/badge.svg)

# Triangle to Quadrilateral Mesh Conversion
![Display](.res/tri-2-quad.png)


## Compile
```shell
cmake -B build -DCMAKE_BUILD_TYPE=Release
cd build
make -j
cd ..
```

or for **linux** & **mac**:
```shell
./compile.sh
```

---

## Execute
```shell
./build/entrypoint_exec
``` 