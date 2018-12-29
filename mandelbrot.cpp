#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <thread>
using namespace std;

//g++ -std=c++14 mandelbrot.cpp -lglut -lGL -lpthread -o mandelbrot

const int width = 800;
const int height = width * 0.77;
double matrix[width][height][3] = {};

int MAX_ITERATIONS = 250;
int NUM_THREADS = 4;
float left = -2.125, right = 0.75, top = 1.125, bottom = -1.125;

// Arguments to specify the region on the complex plane to plot
void compute_mandelbrot(vector<int> rows) {

    for( auto y : rows ) {
        for( int x = 0; x < ::width; ++x ) {
            // Convert current pixel point to the coordinates on complex plane
            complex<double> c( ::left + ( x * ( ::right - ::left ) / ::width ),
                ::top + ( y * ( ::bottom - ::top ) / ::height ) );

            complex<double> z( 0.0, 0.0 );
            
            int n = 0;
            double smooth1 = exp(-1.0 * abs( z ));
            double smooth2, smooth3;

            while( abs( z ) < 2.0 && n < ::MAX_ITERATIONS ) {
                z = ( z * z ) + c;
                smooth1 += exp(-1.0 * abs( z ));
                smooth2 = smooth1 / 2.0;
                smooth3 = smooth1 / 4.0;
                ++n;
            }

            if (n != MAX_ITERATIONS) {
                 ::matrix[x][y][0] = smooth1 / (sqrt(::MAX_ITERATIONS));
                 ::matrix[x][y][1] = smooth2 / (sqrt(::MAX_ITERATIONS));
                 ::matrix[x][y][2] = smooth3 / (sqrt(::MAX_ITERATIONS));
            }
        }
    }
}

void draw() {
    thread t[::NUM_THREADS];

    int rc, N = 0, n_rows = ::height / ::NUM_THREADS;
    vector< vector<int>> buckets( ::NUM_THREADS ); 
    for (int y = 0; y < ::height; ++y) {
	 buckets[N].push_back(y);
         if ( (buckets[N].size() == n_rows) && (N < (::NUM_THREADS - 1)) ) {
             N += 1;
         }
    }

    for ( int n = 0; n < ::NUM_THREADS; ++n ) {
        cout << buckets[n].size() << endl;
    }

    for( int i = 0; i < ::NUM_THREADS; ++i ) {
        cout << "creating thread, " << i << endl;
        t[i] = thread(compute_mandelbrot, buckets[i]);
    }

    for( int i = 0; i < ::NUM_THREADS; ++i ) {
        t[i].join();
    }
} 

void display()
{
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glClear( GL_COLOR_BUFFER_BIT );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0, ::width, 0, ::height, -1, 1 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    draw();
    glBegin( GL_POINTS ); // Single pixel mode
    for (int y = 0; y < ::height; ++y) {
        for (int x = 0; x < ::width; ++x) { 
            glColor3f(::matrix[x][y][0], ::matrix[x][y][1], ::matrix[x][y][2]);
            glVertex2i( x, y );
        }
    }
    glEnd();
    glutSwapBuffers();
}

int main( int argc, char** argv )
{
    if (argc > 1) {
        ::MAX_ITERATIONS = atoi(argv[1]);
    } 
    if (argc > 2) {
        ::NUM_THREADS = atoi(argv[2]);
    }
 
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA );
    glutInitWindowSize( ::width, ::height );
    glutCreateWindow( "Mandelbrot" );
    glutDisplayFunc( display );
    glutMainLoop();
    return 0;
}
