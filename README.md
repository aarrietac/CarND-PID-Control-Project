# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

## Effects of PID components in implementation

### P: proporcional component

This part use the error of the input variable to compute the control signal. In this project the control signal is the steer angle (hand wheel) and the input is the cross-track-error (cte).
Then, a proper proporcinal gain must be chosen. In the simulator, we can estimate the maximum lateral error (cte) to 5-6 meters approximately. The maximum control signal (steer angle) must be 
between -1 to +1. Therefore, a good approximation for the proporcional gain (Kp) would be 1/5 or 0.2. A important note here, is that this value work pretty well for low speeds (< 30mph), but 
for high speed the CTE change very fast do to the speed of the vehicle. So, it is necessary to scale Kp in order to maintain the control signal in a proper region. Doing test in the simulator, 
a Kp = 0.02 was considered for high speeds.

### I: integral component

The idea of this component is to reduce the steady-state error. Bassically, the interal part compute the error over time. So, generally the integral part is bigger than the CTE or CTE/dt in 
the steady-state condition. In my implementation, a small integral gain (Ki = 0.004) was chosen because the diference in order of magnitude comparing to CTE and CTE/dt.

### D: derivative component

This part of the controller try to stabilize the control signal. In oder words, it compensate the proporcional part if this pass the reference signal. In the example shown by Sebastian in the lessons, this component react to an execive proporcional signal reduing it in order to stabilize the control signal. The derivative gain (Kd) was chosen taking into consideration the difference in order of magnitude obtaned from simulation at high speed. Therefore, the Kd must be from 10 to 100 time bigger than the Kp. Then, doing simulations and change the parameters to make the lap properly, I found the correct derivative gain to Kd = 1.2.

## Final consideration for the PID

The final tunned hyper-parameters for my PID implementation are:

* Kp = 0.02
* Ki = 0.004
* Kd = 1.2

Taking these values for the PID controller the vehicle is able to make the lap properly. Nevertheless, there are some part in the track that the vehicle oscillates. That would be higher values for the CTE/dt due to the fast change in the CTE in a small period (high velocities). In order to compensate this, a emergency braking controller was implemented. This controller take into consideration the spped, CTE and steer angle in order to brake and stabilize the vehicle. In addition, a PID controller was implemented to control the speed of the car taking into consideration a reference speed. In this work a reference speed was set to 60 mph. The PID parameters of the speed controller were chosen properly and then normalized to [0 - 1] values.


---

## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets) == 0.13, but the master branch will probably work just fine
  * Follow the instructions in the [uWebSockets README](https://github.com/uWebSockets/uWebSockets/blob/master/README.md) to get setup for your platform. You can download the zip of the appropriate version from the [releases page](https://github.com/uWebSockets/uWebSockets/releases). Here's a link to the [v0.13 zip](https://github.com/uWebSockets/uWebSockets/archive/v0.13.0.zip).
  * If you run OSX and have homebrew installed you can just run the ./install-mac.sh script to install this
* Simulator. You can download these from the [project intro page](https://github.com/udacity/CarND-PID-Control-Project/releases) in the classroom.

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 

## Editor Settings

We've purposefully kept editor configuration files out of this repo in order to
keep it as simple and environment agnostic as possible. However, we recommend
using the following settings:

* indent using spaces
* set tab width to 2 spaces (keeps the matrices in source code aligned)

## Code Style

Please (do your best to) stick to [Google's C++ style guide](https://google.github.io/styleguide/cppguide.html).

## Project Instructions and Rubric

Note: regardless of the changes you make, your project must be buildable using
cmake and make!

More information is only accessible by people who are already enrolled in Term 2
of CarND. If you are enrolled, see [the project page](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/f1820894-8322-4bb3-81aa-b26b3c6dcbaf/lessons/e8235395-22dd-4b87-88e0-d108c5e5bbf4/concepts/6a4d8d42-6a04-4aa6-b284-1697c0fd6562)
for instructions and the project rubric.

## Hints!

* You don't have to follow this directory structure, but if you do, your work
  will span all of the .cpp files here. Keep an eye out for TODOs.

## Call for IDE Profiles Pull Requests

Help your fellow students!

We decided to create Makefiles with cmake to keep this project as platform
agnostic as possible. Similarly, we omitted IDE profiles in order to we ensure
that students don't feel pressured to use one IDE or another.

However! I'd love to help people get up and running with their IDEs of choice.
If you've created a profile for an IDE that you think other students would
appreciate, we'd love to have you add the requisite profile files and
instructions to ide_profiles/. For example if you wanted to add a VS Code
profile, you'd add:

* /ide_profiles/vscode/.vscode
* /ide_profiles/vscode/README.md

The README should explain what the profile does, how to take advantage of it,
and how to install it.

Frankly, I've never been involved in a project with multiple IDE profiles
before. I believe the best way to handle this would be to keep them out of the
repo root to avoid clutter. My expectation is that most profiles will include
instructions to copy files to a new location to get picked up by the IDE, but
that's just a guess.

One last note here: regardless of the IDE used, every submitted project must
still be compilable with cmake and make./
