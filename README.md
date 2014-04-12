container-init
==============

Status: WIP
-----------

TODO: Actually use it

The problem
-----------

Docker and other container systems start the main process as PID 0 but most
programs are not designed to handle the special role of that process.

In POSIX systems, a parent process is supposed to wait(2) on child processes.
If it doesn't do that the processes stay around as a zombie processes and are
never unloaded from memory. The PID 0 is special in that any process whom
parent disappears will have PID 0 re-assigned as it's parent process. It's
thus important that PID 0 waits on all these childs appearing.

container-init is a really simple piece of software just designed to do that.
Run a single program and wait for child processes. That way your main program
will be at PID 1 and not have to worry about these child processes. That's
all.

