set terminal gif animate
set output 'param.3.png'
set xrange [-7:15]
set yrange [-1:15]
set xzeroaxis lt -1
set yzeroaxis lt -1
set grid
set object 1 ellipse center -3, 6 size 5, 10 fs empty bo 3
set object 2 ellipse center 6, 3 size 10, 5 fs empty bo 3
plot '-' with points palette
-5.400002 4.699998 1
-2.400002 5.699998 1
e
pause 1.0
plot '-' with points palette
-5.400002 4.699998 0
-2.400002 5.699998 0