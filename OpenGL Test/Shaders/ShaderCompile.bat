
@echo off
echo Validating shaders...

glslangValidator -V --target-env opengl -o Bin/Triangle.vert.spv Triangle.vert
glslangValidator -V --target-env opengl -o Bin/Triangle.frag.spv Triangle.frag

echo done.
