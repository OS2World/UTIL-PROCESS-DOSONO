So, there I was, running Scott Dudley's nifty Maximus OS/2 BBS with
FidoNet access, open 24 hours, under Binkley EE, which has fax and a very
nice scheduler built-in. Lots of message areas, lots of files, free access,
the works. Big phone bills, of course.

No other data-only lines available. Now, along comes this great new network
that can only be connected to via a proprietary DOS program--that runs as a
TSR using 280 kilobytes of RAM, no less--and I want to use the program to
connect to the new network. But this TSR DOS program hog doesn't know how
to quit when it's done and let my OS/2 BBS session pick up the line again.

So, what we need is a program Binkley can call under OS/2 that will run
that balky proprietary DOS program for an hour or so in its own session
and then kill it, see?

I look but cannot find one. So I fire up my nifty IBM C/Set compiler
and pick up my copy of the _OS/2 Technical Library Programming Guide,
Volume I_, and look up the DosStartSession and DosKillSession API routines.

Sure enough, can be done, I suppose. And the program seems quite versatile
for many such uses, looking at it, especially for two hours' work.

So anyhow, here you are. Freeware.

Hope you don't have to re-invent this wheel anymore.

Sorry, no docs. No time. For instructions, run the program with no arguments.

Send comments, bugs, etc to a null pipe or barring that to:

William Herrera
Lookout Mountain Software
611 Alma Pueblo, CO 81004-1607

...or to Cuerna Verde BBS at:
1:307/18.0@fidonet.org
81:300/5@os2net.org
307.n18.z1.fidonet.org

