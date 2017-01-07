echo off
echo 图片合并...
setlocal enabledelayedexpansion

%texturePacher 路径%
set cmd=E:\TexturePacker4\bin\TexturePacker.exe 
set role_name=all

echo %cd%

%项目根目录%
set rootPath=..\..\..\..\..\
cd %rootPath%
set rootPath=%cd%


%plist, png输出路径%
set work_path=\code\client\GameSLG\frameworks\runtime-src\proj.win32\Debug.win32
set res_path=\res\custom\
set res_path=%work_path%%res_path%
set out_path= %rootPath%%res_path%

%序列帧图片路径%
set frameImg_path=\content\frameAnimations\
set frameImg_path=%rootPath%%frameImg_path%

set sheet= --sheet 
set data= --data 
set png=-{n}.png
set plist=-{n}.plist
 
set param= --allow-free-size --shape-padding 0 --border-padding 0 --inner-padding 0 --trim-mode Trim --opt RGBA8888 --algorithm MaxRects --trim-threshold 1 --enable-rotation  --size-constraints AnySize --scale-mode Smooth --dither-fs-alpha --pack-mode Best --disable-auto-alias --multipack 

cd %frameImg_path%
if " all"=="%role_name%" (
for /d %%i in (*) do (
echo %%i
%cmd%%%i%param%%sheet%%out_path%%%i%png%%data%%out_path%%%i%plist%
)
) else (echo %role_name%
%cmd%%role_name%%param%%sheet%%out_path%%role_name%%png%%data%%out_path%%role_name%%plist%)

echo 图片合并完成