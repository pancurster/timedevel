Timedevel application
=====================

Introduction
------------
Timedevel is a small task/time manager and user activity tracker. The main
funcion is to capture active window and add this app to some group who represent
a task. 
This project is made primarily for training and educational reasons.

Right now application offers the following features:

  *  Capturing information about the active program/window, and log time
     spent on task.
  *  Adding, removing, renaming, adding to groups, reparenting tasks.

In future I plan to add the following features:

  *  Filtering tasks with regular expression
  *  Add support for the database to hold data
  *  Add the CLI and Web user interfaces
  *  Currently application works only on Linux/X11, so support for other 
     operating systems and devices (where Qt works) is planned.


Installation
~~~~~~~~~~~~

Actually timedevel has no simple instalator/package. To try it You need to
download source code and compile by yourself.

Downloading timedevel
---------------------

You can download source code directly from git respository::

    http://github.com/pancurster/timedevel.git

by clonign it::

    $ git clone http://github.com/pancurster/timedevel.git

Building
--------

To build application You need following libraries::

.. _Qt Library: http://www.qt.nokia.com/products/
.. _Qxt Library: http://www.dev.libqxt.org/libqxt/wiki/Home
.. _Boost Library: http://www.boost.org/

You need also X11 system headers

If u have everythig just type::

    $ qmake
    $ make

and::

    $ ./timedevel

If you are lucky program will start :)
    
Contact
-------

In case you have any questions or would like to make feature request, you can
contact me through email

    Lukasz Panek (pancurster@gmail.com)

or by filing issues at github project pages.

