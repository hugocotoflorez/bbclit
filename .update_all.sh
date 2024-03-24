echo $PWD
FILES="./main.c ./bbclit.c ./keyboard_handler.c ./bbclit.h ./makefile ./boxes.c ./drawing.c"

git add $FILES
git commit -m "AutoUpdate"
git push
