echo $PWD
FILES="./main.c ./modules.c ./keyboard_handler.c ./bbclit.h ./makefile ./boxes.c ./drawing.c"

git add $FILES
git commit -m "$*"
git push
