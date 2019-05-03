### udf_pkg_ver_cmp
An UDF for MySQL/MariaDB, that does version comparsion between DPKG or RPM packages.

----

### 1. Intro
This is a MySQL UDF written in C/C++ to compare package versions, supporting both DPKG(deb) and RPM(rpm).

### 2. Prerequisites and Compilation

* Some libs

	apt install libmariadbclient-dev librpm-dev libapt-pkg-dev

* Compile

	g++ ver_cmp.cpp -o ver_cmp.so -lapt-pkg -lrpm -shared -fPIC


### 3. Load and Run

* Load UDF in MySQL
	
	CREATE FUNCTION ver_cmp RETURNS INT SONAME "ver_cmp.so";

* Run RPM version comparsion

	select ver_cmp("rpm", "1.2", "1.3");	# shall return -1

* Run DPKG version comparsion

	select ver_cmp("deb", "1.2", "1:1.1");	# shall return -1


### 4. References

* [http://ftp.rpm.org/api/4.14.0/rpmlib_8h.html#a83273f9ece6bf5608622e495dc15ee1b](http://ftp.rpm.org/api/4.14.0/rpmlib_8h.html#a83273f9ece6bf5608622e495dc15ee1b)
* [://github.com/excid3/python-apt/blob/master/python/apt_pkgmodule.cc](https://github.com/excid3/python-apt/blob/master/python/apt_pkgmodule.cc)
* [https://github.com/Debian/apt/tree/master/apt-pkg](https://github.com/Debian/apt/tree/master/apt-pkg)
