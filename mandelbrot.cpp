#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <ctime>
#include <chrono>
#include <thread>
using namespace std;
using namespace std::chrono;

// g++ -std=c++14 mandelbrot.cpp -lglut -lGL -lpthread -o mandelbrot
// ./mandelbrot 1000 10 0.5

// declare window size and initialize pixels matrix
const int width = 800;
const int height = width * 0.77;
double matrix[width][height][3] = {};

int MAX_ITERATIONS = 250; // max. number iterations for every pixel
int NUM_THREADS = 4; // number of chunks for parallel processing
int N_CLICKS = 0; 
float SCALE_FACTOR = .6; // scale factor for zoom-in
auto START = steady_clock::now();
bool FLAG = false;
float DIVIDER;

// Arguments to specify the region on the complex plane to plot
long double left = -2.125, right = 0.75, top = 1.125, bottom = -1.125;

void compute_mandelbrot(vector<int> rows) {

    for( auto y : rows ) {
        for( int x = 0; x < ::width; ++x ) {
            // Convert current pixel point to the coordinates on complex plane
            complex<double> c( ::left + ( x * ( ::right - ::left ) / ::width ),
                               ::top + ( y * ( ::bottom - ::top ) / ::height ) );

            complex<double> z( 0.0, 0.0 );
            
            int n = 0;
            double smooth1 = 0.0, smooth2 = 0.0, smooth3 = 0.0;

            while( abs( z ) < 2.0 && n < ::MAX_ITERATIONS ) {
                z = ( z * z ) + c;
                // adjusting color
                smooth1 += exp(-1.0 * abs( z ));
                ++n;
            }

            smooth2 = sin(smooth1)*smooth1;
            smooth3 = cos(smooth1)*smooth1;

            if (n != MAX_ITERATIONS) {
                 ::matrix[x][y][0] = smooth3 / ::DIVIDER;
                 ::matrix[x][y][1] = smooth2 / ::DIVIDER;
                 ::matrix[x][y][2] = smooth1 / ::DIVIDER;
            } else { 
                 ::matrix[x][y][0] = 0.0;
                 ::matrix[x][y][1] = 0.0;
                 ::matrix[x][y][2] = 0.0;
            }
        }
    }
}

void draw() {
    thread t[::NUM_THREADS];
    // Split window by height 
    int rc, N = 0, n_rows = ::height / ::NUM_THREADS;
    vector< vector<int>> buckets( ::NUM_THREADS ); 
    for (int y = 0; y < ::height; ++y) {
	 buckets[N].push_back(y);
         if ( (buckets[N].size() == n_rows) && (N < (::NUM_THREADS - 1)) ) {
             N++;
         }
    }
    // compute mandelbrot set on each chunk
    for( int i = 0; i < ::NUM_THREADS; ++i ) {
        t[i] = thread(compute_mandelbrot, buckets[i]);
    }

    cout << " [INFO] Threads created: " << ::NUM_THREADS << endl;

    for( int i = 0; i < ::NUM_THREADS; ++i ) {
        t[i].join();
    }
} 

void display() {
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
    ::FLAG = false;
    glutSwapBuffers();    
}

void mouseButton(int button, int state, int x, int y) {

        if ( state == GLUT_DOWN) {
            if ( button == GLUT_LEFT_BUTTON ) {
                ::N_CLICKS++;
            }

            if ( ::N_CLICKS == 1 ) {
                ::START = steady_clock::now();
            }
            
            if ( ::N_CLICKS >= 2 ) {
                if ( ::N_CLICKS == 2 ) { 
                    auto finish = steady_clock::now();
                    int duration = duration_cast<milliseconds>(finish - ::START).count();
                    // recompute mandelbrot set in smaller region around the mouse position
                    if ( (duration <= 500) && (not ::FLAG) ) {

                        ::FLAG = true;

                        long double dx = ::right - ::left;
                        long double dy = ::top - ::bottom;

                        long double new_center_x = ::left + ( x * dx / ::width );
                        long double new_center_y = ::bottom + ( y * dy / ::height );

                        cout << " [DEBUG] New center: " << new_center_x << " " << new_center_y << endl;
                   
                        long double new_delta_x = abs( dx * ::SCALE_FACTOR );
                        long double new_delta_y = abs( dy * ::SCALE_FACTOR );

                        cout << " [DEBUG] New deltas: " << new_delta_x << " " << new_delta_y << endl;

                        ::left = new_center_x - ( new_delta_x / 2.0 );
                        ::right = new_center_x + ( new_delta_x / 2.0 );
                        ::bottom = new_center_y - ( new_delta_y / 2.0 );
                        ::top = new_center_y + ( new_delta_y / 2.0 ); 

                        cout << " [DEBUG] New borders: " << ::left << " " << ::right << " " << ::bottom << " " << ::top << endl;

                        glutPostRedisplay();
                    }
                }
                ::N_CLICKS = 0;
            }
        }       
}

int main( int argc, char** argv ) {
    // read the arguments
    if (argc > 1) {
        ::MAX_ITERATIONS = atoi(argv[1]);
    } 
    if (argc > 2) {
        ::NUM_THREADS = atoi(argv[2]);
    }

    if (argc > 3) {
        ::SCALE_FACTOR = atof(argv[3]);
    }
 
    ::DIVIDER = sqrt(::MAX_ITERATIONS);
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA );
    glutInitWindowSize( ::width, ::height );
    glutCreateWindow( "Mandelbrot" );
    glutDisplayFunc( display );

    // make zoom-in by mouse-click
    glutMouseFunc(mouseButton);

    glutMainLoop();
    return 0;
}
