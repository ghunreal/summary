%����bat��pngquant.exe ��������Ҫѹ����pngĿ¼%
%ע�� ѹ������ļ�������ԭʼ�ļ�%

echo "��ʼѹ��png..."
@echo off

for /R %%i in (*.png) do (
  pngquant -f --ext .png --quality 50-80 "%%i"
)

pause