set curl_dir="curl-7.72.0"

call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars32.bat"

cd /D %~dp0
cd %curl_dir%\winbuild
nmake /f Makefile.vc mode=static VC=15 RTLIBCFG=static MACHINE=x86
cd /D %~dp0

xcopy /s /i /y %curl_dir%\builds\libcurl-vc15-x86-release-static-ipv6-sspi-schannel sys
del sys\lib\libcurl.lib
ren sys\lib\libcurl_a.lib libcurl.lib
