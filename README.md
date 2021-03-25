# CPPND: Capstone Snake Game of Babak Oskouei

This is a starter repo for the Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). The code for this repo was inspired by [this](https://codereview.stackexchange.com/questions/212296/snake-game-in-c-with-sdl) excellent StackOverflow post and set of responses.

<img src="snake_game.gif"/>

The Capstone Project gives you a chance to integrate what you've learned throughout this program. This project will become an important part of your portfolio to share with current and future colleagues and employers.

In this project, you can build your own C++ application or extend this Snake game, following the principles you have learned throughout this Nanodegree Program. This project will demonstrate that you can independently create applications using a wide range of C++ features.

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  * Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source.
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions
### Using the Makefile
1. To build and run:
    cd to the root directory.
    make build
2. To debug:
    cd to the root directory.
    make debug

### Using only the CMakeLists.txt:
1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.


# Project Ruberic requirements in the code:
 * Chosen project: Snake Game
 * The project code must compile and run without errors.
 * A variety of control structures are used in the project.
 * The project code is clearly organized into functions.
 * The project reads data from an external file or writes data to a file as part of the necessary operation of the program.  ( see game.cpp lines 150-160)
 * The project accepts input from a user as part of the necessary operation of the program. (See controller.cpp lines 20-40)
 * The project code is organized into classes with class attributes to hold the data, and class methods to perform tasks. (See target.h, .cpp)
 * All class data members are explicitly specified as public, protected, or private. (See target.h, .cpp)
 * All class members that are set to argument values are initialized through member initialization lists. (See target.cpp)
 * All class member functions document their effects, either through function names, comments, or formal documentation. Member functions do not change program state in undocumented ways.
 * Appropriate data and functions are grouped into classes. Member data that is subject to an invariant is hidden from the user. State is accessed via member functions. (See target.h, target.cpp)
 * Inheritance hierarchies are logical. Composition is used instead of inheritance when appropriate. Abstract classes are composed of pure virtual functions. Override functions are specified. (See target.h, target.cpp)
 * One function is overloaded with different signatures for the same function name. (See target.h lines 41, 42)
 * One member function in an inherited class overrides a virtual base class member function. (See target.h lines 30, 49)
 * One function is declared with a template that allows it to accept a generic parameter. (See msgqueue.h line 10, 26, 33)
 * At least two variables are defined as references, or two functions use pass-by-reference in the project code. (See game.cpp line 20)
 * At least one class that uses unmanaged dynamically allocated memory, along with any class that otherwise needs to modify state upon the termination of an object, uses a destructor. (See game.cpp line 10)
 * The project follows the Resource Acquisition Is Initialization pattern where appropriate, by allocating objects at compile-time, initializing objects when they are declared, and utilizing scope to ensure their automatic destruction. (See Game::ReplaceFood line 96)
 * For all classes, if any one of the copy constructor, copy assignment operator, move constructor, move assignment operator, and destructor are defined, then all of these functions are defined. (See target.cpp)
 * For classes with move constructors, the project returns objects of that class by value, and relies on the move constructor, instead of copying the object. (See game.cpp Game::ReplaceFood and target.cpp)
 * The project uses at least one smart pointer: unique_ptr, shared_ptr, or weak_ptr. The project does not use raw pointers. (See game.cpp Game::ReplaceFood line 110)
 * The project uses multiple threads in the execution. (See game.cpp)
 * A promise and future is used to pass data from a worker thread to a parent thread in the project code. (See game.cpp Game::ReplaceFood line 88)
 * A mutex or lock (e.g. std::lock_guard or `std::unique_lock) is used to protect data that is shared across multiple threads in the project code. (See game.cpp Game::ReplaceFood)
 * A std::condition_variable is used in the project code to synchronize thread execution. (See msgqueue.h line 22)