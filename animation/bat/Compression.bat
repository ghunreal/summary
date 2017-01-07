%将该bat和pngquant.exe 拷贝至需要压缩的png目录%
%注意 压缩后的文件将覆盖原始文件%

echo "开始压缩png..."
@echo off

for /R %%i in (*.png) do (
  pngquant -f --ext .png --quality 50-80 "%%i"
)

pause