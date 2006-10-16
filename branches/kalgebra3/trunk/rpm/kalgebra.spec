%define name kalgebra
%define version 20050830
%define release 1

Name:           %{name}
Version:        %{version}
Release:        %mkrel %release
Summary:        KAlgebra is a mathematical calculator based content markup MathML language.
Group:          Graphical desktop/KDE
License:        GPL
Source0:        http://www.proli.net/kalgebra/%{name}-%{version}.tar.bz2
Source1:        %{name}-%{version}.tar.bz2
Source2:        cr16-app-kalgebra.png
Source3:        cr32-app-kalgebra.png
Source4:        cr48-app-kalgebra.png
URL:            http://kalgebra.sourceforge.net/
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-buildroot

BuildRequires:  libkdecore4-devel, qt3-devel
BuildRequires:  jpeg-devel, png-devel
BuildRequires:  libxorg-x11-devel
Requires:       libkdecore4

%description
This is the KAlgebra homepage. KAlgebra is a mathematical calculator based
content markup MathML language. Nowadays it is capable to make simple matml
operations (arithmetic, logical) and representate 2D and 3D graphs.

%prep
%setup -q -a 1

%build
%configure
%make


%install
rm -rf $RPM_BUILD_ROOT
%makeinstall

install -m644 %{SOURCE2} -D $RPM_BUILD_ROOT%{_miconsdir}/%{name}.png
install -m644 %{SOURCE3} -D $RPM_BUILD_ROOT%{_iconsdir}/%{name}.png
install -m644 %{SOURCE4} -D $RPM_BUILD_ROOT%{_liconsdir}/%{name}.png

install -d $RPM_BUILD_ROOT%{_menudir}
kdedesktop2mdkmenu.pl %{name} "More Applications" $RPM_BUILD_ROOT%{_datadir}/applnk/Utilities/kalgebra.desktop $RPM_BUILD_ROOT%{_menudir}/%{name}

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(0755,root,root)
%doc README NEWS COPYING AUTHORS
%{_bindir}/kalgebra
%{_datadir}/applnk/Utilities/kalgebra.desktop
%{_datadir}/apps/kalgebra/kalgebraui.rc
%{_datadir}/doc/HTML/en/kalgebra/common
%{_datadir}/doc/HTML/en/kalgebra/index.cache.bz2
%{_datadir}/doc/HTML/en/kalgebra/index.docbook
%{_menudir}/%{name}
%{_miconsdir}/*.png
%{_iconsdir}/*.png
%{_liconsdir}/*.png
%{_iconsdir}/crystalsvg/64x64/apps/*.png

%changelog

* Thu Sep  1 2005 Frank Mertens <frank@cyglogic.de> 20050830-1mdk
- 20050830 snapshot
- fix required
