=====================
Gonga: Ngaro VM in Go
=====================

--------
Building
--------

To build:

::

  export GOPATH=`pwd`
  go install main


---------------------
Non-Standard Features
---------------------

In addition to normal ngaro features, this Go version
adds concurrency support: port 13 is used to launch
new cores, and the ports 1 and 2 can be used to
send to/receive from synchronous channels.

::

   chain: concurrency'
      : :go ( a- ) 1 13 out wait ;              ( start new core with ip set to a )
      : go ( "-  ) ' :go ;                      ( parse a word and run it on a new core )
      : ->c ( xy- ) 2 out wait ;                ( send x to channel y )
      : c-> ( x-y ) 1 out wait 1 in ;           ( receive y from channel x )
      : delchan ( x- ) dup 1 out 2 out wait ;   ( delete channel )
   ;chain

