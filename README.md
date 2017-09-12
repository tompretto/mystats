# mystats
c program to display System stats from Linux command line

To use this program, at the command line type:
  ./mytools
  
 Options:
  -s  Show extended information

mystats is a program which uses low-level Unix I/O routines (open, read, write) to report on the behavior of the Linux kernel by inspecting the kernel state and displaying the following values on stdout.  
  • CPU type, model and number of processors
  • Kernel version
  • Amount of time since the system was last booted, in the form dd:hh:mm:ss 
    (forexample, 3 days 13 hours 46 minutes 32 seconds would be formatted as03:13:46:32)
    
 
 
 The program uses getopts() to retrieve the command line arguments.  It accepts a command-line argument of '-s' (e.g.:  ‘mystats –s’) and displays the following additional statistics only when this flag has been passed

   
  • The amount of time that the CPU has spent in user mode, in system mode and idle.
  • The number of disk requests made on the system.
  • The number of context switches that the kernel has performed.
  • The time when the system was last booted.
  • The number of processes that have been created since the system was booted.
