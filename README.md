![build, test](https://github.com/kallu-A/Tri-2-Quad-Mesh/actions/workflows/continuous.yml/badge.svg)

# Triangle to Quadrilateral Mesh Conversion
![Display](.res/tri-2-quad.png)
Result with `triToQuadSimple.cpp`

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
or 
./build/entrypoint_exec path=mesh_path n_region=number_region p_area=percent_area_region
# each parameter can be added when needed you can have only path or path and n_area
``` 
All parameters: **gif**, **path**, **n_region**, **p_area**
