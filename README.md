# Vtux 0.0.2

Vtux is a program that allows a computer to loan and lend a graphics card from/to another computer.

## TODO for 0.1.0
    * Get Client to work on VM (currenly only works on real hardware)
    * Build Communication from server and client to complete the pipeline (driver-server exist)
    * Extend Client to send ioctl messages to driver

## Current state
    * Working drm and char driver to pass ioctl the graphics card receives to userspace.
    * Working client to lease graphics card from X server

### Install
Clone the repo:
`git clone https://github.com/IdoGuzi/vtux.git`.

Go to a directory of a module and simply run:
`make`
to compile.
The name of the directory should also be the name of the executable (driver should use `insmod`).

To remove compilation products:
`make clean`.

### Contribution
See TODO list here and look at the issues, open a new issue for new ideas.
