# mandelbrot_cpp
multi-thread fractals rendering with C++ &amp; OpenGL  

<img src="https://github.com/gasparian/fractals_cpp/blob/master/pics/mandelbrot_animation.gif" height=500>  

Compile `mandelbrot.cpp`:
```
g++ -std=c++11 mandelbrot.cpp -lglut -lGL -lpthread -o mandelbrot
```

And start exploring:
```
./mandelbrot 1000 10 0.5
```
