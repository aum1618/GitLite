branches=("master" "umer" "zain" "adnan")

git add .
git commit -m "Bash script added"
git push origin durrani

for branch in "${branches[@]}"
do
    git checkout "$branch"
    git merge durrani
    git push origin "$branch"
done

git checkout durrani
