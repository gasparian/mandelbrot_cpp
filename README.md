# Multi-thread mandelbrot set rendering with C++ &amp; OpenGL  

To create [mandelbrot set](https://en.wikipedia.org/wiki/Mandelbrot_set) we must simply iterate: *z -> z * z + c*, where *z* and *c* are complex numbers. 
For every pixel in the wanted region we calculate mapping to the coordinates on the complex plane and then assign this point to *c*. For every *c* we start iterating from *z = 0*. Iteration stops when |*z*| or number of iterations reaches the boundaries.  

<img src="https://github.com/gasparian/mandelbrot_cpp/blob/master/pics/complex_plane.png" height=250>  

Coloring scheme is a huge field for improvisation. Generally speaking, the intensity is determined by how fast *z* goes to infinity (relative to the maximum iterations).  
Parallel rendering is done by independently processing chunks of window area (I should say, that it's implemented "dirty" - through the global variables).  

<img src="https://github.com/gasparian/mandelbrot_cpp/blob/master/pics/animation.gif" height=500>  

In this implementation, you can zoom-in by double clicking inside the window. And zoom is actually not infinite, due to the limitation of floating point calculations precision. So if you zoom in up to 10e+13 you'll see very discrete image, something like this:  

<img src="https://github.com/gasparian/mandelbrot_cpp/blob/master/pics/mandelbrot_artefacts.png" height=500>  

### Requirements
Install OpenGL libraries and freeGlut:
```
sudo apt-get install mesa-utils
sudo apt-get install freeglut3-dev
```

### Usage
Compile `mandelbrot.cpp`:
```
g++ -O2 -fno-stack-limit -x c++ mandelbrot.cpp -lglut -lGL -lpthread -o mandelbrot
```

Then you can specify arguments and run the code:
- maximum number of iterations (250 by default);  
- number of threads (4 by default);  
- scale factor in range 0..1.0 to use zoom-in (0.5 by default);  
```
./mandelbrot 1000 10 0.5
```  
If you need to change the window size, replace variable `width` inside the code with whatever you want.
