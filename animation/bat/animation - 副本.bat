echo off
echo 图片合并...

%texturePacher 路径%
set cmd=E:\TexturePacker4\bin\TexturePacker.exe 

%资源所在目录需要注意：%
% 例： "Cha002/Run/8/0005.png"  中Cha002需要有一个Char002的父目录，即需要两个Char002目录在路径中 %
% 否则tp输出的plist文件中碎图名字中将不会出现Char002, 名字将变为 "Run/8/0005.png"  %
setlocal enabledelayedexpansion

%项目根目录%
set rootPath=..\..\..\..\
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
for /d %%i in (*) do (
echo %%i
%cmd%%%i%param%%sheet%%out_path%%%i%png%%data%%out_path%%%i%plist%
)

echo 图片合并完成

echo 导出json
%Animation.exe路径名%
set exe=%work_path%\Animation.exe 

%角色名 需要导出的角色所在文件夹的名字 all代表导出所有角色%
set role_name= Cha001

%plist png 所在目录%
set plis_png_path= %out_path%

%json输出目录%
set json_path= %out_path%

%rootPath%%exe%%frameImg_path%%role_name%%plis_png_path%%json_path%
@pause