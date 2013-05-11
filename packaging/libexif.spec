Name:           libexif
Version:        0.6.21
Release:        0
License:        LGPL-2.1+
Summary:        An EXIF Tag Parsing Library for Digital Cameras
Url:            http://libexif.sourceforge.net
Group:          System/Libraries
Source:         %{name}-%{version}.tar.bz2
Source1:        baselibs.conf
BuildRequires:  doxygen
BuildRequires:  pkg-config

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

%build
%configure --with-pic \
	--disable-static \
	--with-doc-dir=%{_docdir}/%{name}
make %{?_smp_mflags}

%check
make check

%install
%make_install
%find_lang %{name}-12

%post  -p /sbin/ldconfig

%postun  -p /sbin/ldconfig

%files -f %{name}-12.lang
%license COPYING
%defattr(-,root,root)
%{_libdir}/*.so.*

%files devel
%defattr(-,root,root)
%doc %{_docdir}/%{name}
%{_libdir}/*.so
%{_libdir}/pkgconfig/*.pc
%{_includedir}/*

%changelog
