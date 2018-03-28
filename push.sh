git add -A
git commit -m "made changes"
git push origin master
if [ ! -z $? ]; then
    git pull origin master
    git add -A
    git commit -m "push error, then pull and push"
    git push origin master
fi