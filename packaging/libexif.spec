Name:           libexif
Version:        0.6.21
Release:        0
License:        LGPL-2.1+
Summary:        An EXIF Tag Parsing Library for Digital Cameras
Url:            http://libexif.sourceforge.net
Group:          System/Libraries
Source:         %{name}-%{version}.tar.bz2
Source1:        baselibs.conf
Source1001: 	libexif.manifest
BuildRequires:  doxygen
BuildRequires:  pkg-config
BuildRequires:  gettext-tools

%define debug_package_requires %{name} = %{version}-%{release}

%description
This library is used to parse EXIF information from JPEGs created by
digital cameras.

%package devel
Summary:        An EXIF Tag Parsing Library for Digital Cameras (Development files)
Group:          Development/Libraries/C and C++
Requires:       %{name} = %{version}
Requires:       glibc-devel

%description devel
This library is used to parse EXIF information from JPEGs created by
digital cameras.

%prep
%setup -q
cp %{SOURCE1001} .

%build
%reconfigure --with-pic \
	--disable-static \
	--with-doc-dir=%{_docdir}/%{name}
make 
### %{?_smp_mflags}

%check
make check

%install
%make_install
%find_lang %{name}-12

%post  -p /sbin/ldconfig

%postun  -p /sbin/ldconfig

%files -f %{name}-12.lang
%manifest %{name}.manifest
%license COPYING
%defattr(-,root,root)
%{_libdir}/*.so.*

%files devel
%manifest %{name}.manifest
%defattr(-,root,root)
%doc %{_docdir}/%{name}
%{_libdir}/*.so
%{_libdir}/pkgconfig/*.pc
%{_includedir}/*

%changelog
