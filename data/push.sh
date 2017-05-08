#!/bin/sh

echo -n "Pls input push dir(such as /sdcard or /dev/sample):>"
read dir
echo "input dir: $dir"

echo mkdir files ...
adb shell run-as com.forget3d.demo mkdir files

echo adb push tris.md2 ...
adb push tris.md2 $dir/tris.md2
adb shell run-as com.forget3d.demo cp $dir/tris.md2 files/tris.md2

echo adb push tris.bmp ...
adb push tris.bmp $dir/
adb shell run-as com.forget3d.demo cp $dir/tris.bmp files/tris.bmp

echo adb push weapon.md2 ...
adb push weapon.md2 $dir/
adb shell run-as com.forget3d.demo cp $dir/weapon.md2 files/weapon.md2

echo adb push weapon.bmp ...
adb push weapon.bmp $dir/
adb shell run-as com.forget3d.demo cp $dir/weapon.bmp files/weapon.bmp

echo adb push font.bmp ...
adb push font.bmp $dir/
adb shell run-as com.forget3d.demo cp $dir/font.bmp files/font.bmp

echo adb push floor.bmp ...
adb push floor.bmp $dir/
adb shell run-as com.forget3d.demo cp $dir/floor.bmp files/floor.bmp

echo adb push clouds.bmp ...
adb push clouds.bmp $dir/
adb shell run-as com.forget3d.demo cp $dir/clouds.bmp files/clouds.bmp

# echo adb push guard.g3d ...
# adb push guard.g3d $dir/

# echo adb push guard.bmp ...
# adb push guard.bmp $dir/

# echo adb push run.ms3d ...
# adb push run.ms3d $dir/

# echo adb push DM_Base.bmp ...
# adb push DM_Base.bmp $dir/

# echo adb push DM_Face.bmp ...
# adb push DM_Face.bmp $dir/

echo adb push star.bmp ...
adb push star.bmp $dir/
adb shell run-as com.forget3d.demo cp $dir/star.bmp files/star.bmp

# echo adb push monster.bmp ...
# adb push monster.bmp $dir/

# echo adb push monster.ms3d ...
# adb push monster.ms3d $dir/

echo adb push f3d_logo.bmp ...
adb push f3d_logo.bmp $dir/
adb shell run-as com.forget3d.demo cp $dir/f3d_logo.bmp files/f3d_logo.bmp
