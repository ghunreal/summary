echo off
echo ͼƬ�ϲ�...

%texturePacher ·��%
set cmd=E:\TexturePacker4\bin\TexturePacker.exe 

%��Դ����Ŀ¼��Ҫע�⣺%
% ���� "Cha002/Run/8/0005.png"  ��Cha002��Ҫ��һ��Char002�ĸ�Ŀ¼������Ҫ����Char002Ŀ¼��·���� %
% ����tp�����plist�ļ�����ͼ�����н��������Char002, ���ֽ���Ϊ "Run/8/0005.png"  %
setlocal enabledelayedexpansion

%��Ŀ��Ŀ¼%
set rootPath=..\..\..\..\
cd %rootPath%
set rootPath=%cd%

%plist, png���·��%
set work_path=\code\client\GameSLG\frameworks\runtime-src\proj.win32\Debug.win32
set res_path=\res\custom\
set res_path=%work_path%%res_path%
set out_path= %rootPath%%res_path%

%����֡ͼƬ·��%
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

echo ͼƬ�ϲ����

echo ����json
%Animation.exe·����%
set exe=%work_path%\Animation.exe 

%��ɫ�� ��Ҫ�����Ľ�ɫ�����ļ��е����� all���������н�ɫ%
set role_name= Cha001

%plist png ����Ŀ¼%
set plis_png_path= %out_path%

%json���Ŀ¼%
set json_path= %out_path%

%rootPath%%exe%%frameImg_path%%role_name%%plis_png_path%%json_path%
@pause