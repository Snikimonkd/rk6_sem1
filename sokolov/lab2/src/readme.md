# Как собрать
```bash
mkdir build
cd build
cmake ../
make
```
# Как запустить
```bash
./main rk4 out.csv
./main ab2 out.csv
```
# Графики
```bash
gnuplot
plot 'build/ab2.csv' u 1:2 w l, 'build/ab2.csv' u 1:3 w l, 'build/ab2.csv' u 1:4 w l
plot 'build/rk4.csv' u 1:2 w l, 'build/rk4.csv' u 1:3 w l, 'build/rk4.csv' u 1:4 w l
plot 'build/ab2.csv' u 1:2 w l, 'build/ab2.csv' u 1:3 w l, 'build/ab2.csv' u 1:4 w l, 'build/rk4.csv' u 1:2 w l, 'build/rk4.csv' u 1:3 w l, 'build/rk4.csv' u 1:4 w l
```