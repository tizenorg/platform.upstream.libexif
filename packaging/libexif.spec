Name:           libexif
Version:        0.6.21
Release:        1
License:        LGPL-2.1+
Summary:        An EXIF Tag Parsing Library for Digital Cameras
Url:            http://libexif.sourceforge.net
Group:          System/Libraries
Source:         %{name}-%{version}.tar.bz2
Source1:        baselibs.conf
Source1001: 	   libexif.manifest
Requires(post):  /sbin/ldconfig
Requires(postun):  /sbin/ldconfig

%define debug_package_requires %{name} = %{version}-%{release}

%description
This library is used to parse EXIF information from JPEGs created by
digital cameras.

%package devel
Summary:        An EXIF Tag Parsing Library for Digital Cameras (Development files)
Group:          Development/Libraries/C and C++
Requires:       %{name} = %{version}

%description devel
This library is used to parse EXIF information from JPEGs created by
digital cameras.

%prep
%setup -q
cp %{SOURCE1001} .

%build
sh ./autogen.sh
%configure --with-pic \
	--disable-static \
	--disable-docs

make %{?jobs:-j%jobs}

%install
mkdir -p %{buildroot}/usr/share/license
cp COPYING %{buildroot}/usr/share/license/%{name}
%make_install
#%find_lang %{name}-12

%post  -p /sbin/ldconfig

%postun  -p /sbin/ldconfig

#%files -f %{name}-12.lang
%files
%manifest %{name}.manifest
%defattr(-,root,root)
%{_libdir}/*.so.*
%{_datadir}/license/%{name}

%files devel
%defattr(-,root,root)
%{_includedir}/libexif
%{_libdir}/*.so
%{_libdir}/pkgconfig/libexif.pc

