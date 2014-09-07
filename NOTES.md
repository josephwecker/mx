* Standard installation:
  - tar - xvf ...
  - cd ...
  - ./configure        (or ./configure --prefix ~/usr ) (or run from build dir)
  - make
  - make check
  - sudo make install  (or sudo make install-strip)  ( == make install-exec + make install-data)
  - make installcheck

* make distcheck - creates tar.gz for the app

* standard hierarchy:
  |prefix            [/usr/local]
    |exec-prefix     [$prefix]
      |bindir        [$exec-prefix/bin]
      |libdir        [$exec-prefix/lib]
      |...
    |includedir      [$prefix/include]
    |datarootdir     [$prefix/share]
      |datadir       [$datarootdir]
      |mandir        [$datarootdir/man]
      |infodir       [$datarootdir/info]

* Because ./configure outputs to current working directory- could be very easy
  to have separate build directories for separate contexts.

* autotools: autoconfiscate subpackages == git submodules ?

* use ./configure -C to cache results and speed up reconfigurations




configure.ac + Makefile.am + src/Makefile.am... -> autoreconf -> configure & makefiles...


