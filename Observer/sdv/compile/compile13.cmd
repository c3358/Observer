set PATH=C:\Program Files (x86)\Windows Kits\10\TOOLS\SDV\bin\engine;%PATH%
set PATH=C:\Program Files (x86)\Microsoft Visual Studio 14.0\\VC\bin\x86_amd64;C:\Program Files (x86)\Microsoft Visual Studio 14.0\\VC\bin;C:\Program Files (x86)\Microsoft Visual Studio 14.0\\common7\ide\;%PATH%

set INCLUDE=C:\Users\Raffi\documents\visual studio 2015\Projects\Observer\Observer\;C:\Program Files (x86)\Windows Kits\10\Include\10.0.10586.0\km\crt;C:\Program Files (x86)\Windows Kits\10\Include\10.0.10586.0\km;C:\Program Files (x86)\Windows Kits\10\Include\10.0.10586.0\shared

slamcl  --update sdv\compile\slamdrv.obj  /D_SDV_ "-IC:\Users\Raffi\documents\visual studio 2015\Projects\Observer\Observer" "-IC:\Users\Raffi\documents\visual studio 2015\Projects\Observer\Observer\SDV\scan" "-IC:\Program Files (x86)\Windows Kits\10\TOOLS\SDV\osmodel\wdm" "/FIC:\Program Files (x86)\Windows Kits\10\TOOLS\SDV\osmodel\wdm\SDV_cl_frontend.h"  @sdv\compile\sdv_cl13.rsp  "C:\Users\Raffi\documents\visual studio 2015\Projects\Observer\Observer\Registry\RegistryFilterCallback.c"