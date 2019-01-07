# mandelbrot_cpp
Multi-thread fractals rendering with C++ &amp; OpenGL  

<img src="https://github.com/gasparian/mandelbrot_cpp/blob/master/pics/animation.gif" height=500>  

Parallel rendering is done by independently processing the chunks of window area.

### Usage
Compile `mandelbrot.cpp`:
```
g++ -std=c++11 mandelbrot.cpp -lglut -lGL -lpthread -o mandelbrot
```

Then you can specify arguments and run the code:
- maximum number of iterations (250 by default);  
- number of threads (4 by default);  
- scale factor in range 0..1.0 to use zoom-in (0.5 by default);  
```
./mandelbrot 1000 10 0.5
```  
Zoom-in is available by double clicking inside the window.
